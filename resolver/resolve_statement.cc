#include "resolver.hh"

#include "ast/expression.hh"
#include "ast/statement.hh"

#include "meta/scope.hh"

#include "utils/overloaded.hh"

namespace Cougar::Resolver {

using namespace Ast;
using namespace Meta;
using namespace Utils;

Resolver::ControlFlowInfo
Resolver::resolveStatement(StGroup &grp, const StatementContext &ctx) {

  assert(!grp.scope);

  StatementContext groupCtx = ctx;
  groupCtx.scope = Zone::make<Scope>("", ctx.scope);
  return resolveGroup(grp, groupCtx);
}

Resolver::ControlFlowInfo
Resolver::resolveStatement(StReturn &ret, const StatementContext &ctx) {

  if (ret.expression) {
    resolveExpression(ret.expression, ctx.scope);
    // check if return type matches function type
    TypeInfo *returnedType = ret.expression->type();
    TypeInfo *expectedReturnType = ctx.function->returnType;
    if (returnedType && expectedReturnType) {
      if (returnedType != expectedReturnType) {
        mDiag.error(
            ctx.location,
            "Returning expression of type '{}' from function returning '{}'",
            returnedType->prettyName(), expectedReturnType->prettyName());
      }
    }

  } else {
    // TODO support void functions
    mDiag.error(ctx.location, "Function must return value");
  }

  ControlFlowInfo cf;
  cf.returns = true;
  return cf;
}

Resolver::ControlFlowInfo Resolver::resolveGroup(Ast::StGroup &group,
                                                 const StatementContext &ctx) {

  StatementContext statementCtx = ctx;

  bool unreachable = false;
  bool unreachableReported = false;

  ControlFlowInfo controlFlow;
  ControlFlowInfo lastStatementCf;
  for (Ast::Statement *stmt : group.statements) {
    statementCtx.location = stmt->token()->location;

    // is this statement unreachable?
    if (unreachable && !unreachableReported) {
      unreachableReported = true;
      mDiag.error(statementCtx.location, "Statement(s) unreachable");
    }

    lastStatementCf = resolveStatement(stmt, statementCtx);

    if (!lastStatementCf.fallsTrough)
      unreachable = true;

    controlFlow.returns |= lastStatementCf.returns;
  }

  controlFlow.fallsTrough = !unreachable;

  return controlFlow;
}

Meta::FunctionInfo *Resolver::resolveNamedFunction(std::string_view name,
                                                   Meta::Scope *scope) {
  FunctionInfo *fi = mState.mBuildInScope->findFunction(name);
  if (fi)
    return fi;

  // traverse local scopes
  for (; scope->parent(); scope = scope->parent()) {
    FunctionInfo *fi = scope->findFunction(name);
    if (fi)
      return fi;
  }
  return nullptr;
}

Resolver::ControlFlowInfo
Resolver::resolveStatement(StFunctionCall &stmt, const StatementContext &ctx) {

  Resolver::ControlFlowInfo cf;
  cf.fallsTrough = true;

  // - resolve arguments
  for (Expression *e : stmt.params->params()) {
    resolveExpression(e, ctx.scope);
  }

  // - find function by name
  FunctionInfo *fi = resolveNamedFunction(stmt.name, ctx.scope);
  if (!fi) {
    mDiag.error(ctx.location, "Unknown function '{}'", stmt.name);
    return cf;
  }

  // TODO
  // - TODO - perform overload resolution
  // - inject cast expression if required

  // for now, just check if param types match
  auto it1 = fi->args.begin();
  auto it2 = stmt.params->params().begin();
  int idx = 0;
  while (it1 != fi->args.end() && it2 != stmt.params->params().end()) {
    idx++;
    if (it1->type != (*it2)->type()) {
      mDiag.error(ctx.location, "Function call parameter type mismatch");
      mDiag.error(ctx.location, "Parameter #{} expects type '{}', got '{}'",
                  it1->type->prettyName(), (*it2)->type()->prettyName());
      return cf;
    }
    ++it1;
    ++it2;
  }

  if (it1 != fi->args.end()) {
    mDiag.error(ctx.location, "Too few paramters passed to function call");
    return cf;
  } else if (it2 != stmt.params->params().end()) {
    mDiag.error(ctx.location, "Too many paramters passed to function call");
    return cf;
  }

  stmt.info = fi;

  return cf;
}

Resolver::ControlFlowInfo
Resolver::resolveStatement(Ast::Statement *stmt, const StatementContext &ctx) {

  return stmt->visit([&](auto &stmt) { return resolveStatement(stmt, ctx); });
}

} // namespace Cougar::Resolver