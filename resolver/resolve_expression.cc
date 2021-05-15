#include "resolver.hh"

#include "ast/expression.hh"

#include "meta/built_in.hh"
#include "meta/scope.hh"

#include "utils/overloaded.hh"

namespace Cougar::Resolver {

using namespace Ast;
using namespace Meta;
using namespace Utils;

namespace {
// TODO this will be an enitire compile-time evaluation machinery, one day

double evaluate(Expression *ex) {
  return ex->visit(overloaded{[](const ExNumberLiteral &num) {
                                return std::stod(std::string(num.content));
                              },
                              [](const auto &) -> double {
                                throw std::logic_error(
                                    "Unable to evaluate expression");
                              }});
}
} // namespace

void Resolver::resolveExpression(Ast::Expression *e, Meta::Scope *scope) {

  (void)scope;
  assert(e);

  e->visit(overloaded{
      [&](const ExStringLiteral &) { e->setType(BuiltIn::typeCStr); },
      [&](const ExNumberLiteral &) { e->setType(BuiltIn::typeCtNumber); },
      [&](const ExImplicitCast &) {}, [&](const ExConstant &) {}});
}

Ast::Expression *Resolver::createImplicitCast(Ast::Expression *from,
                                              Meta::TypeInfo *to) {

  assert(from->type());
  assert(to);
  assert(from->type() != to);

  if (from->type()->isCompiletime() && to->isRuntime()) {
    // from compile time to runtime -> generate const
    ExConstant cnst;
    cnst.source = from;
    cnst.targetType = to;
    cnst.value = evaluate(from);

    return Zone::make<Expression>(cnst);

  } else if (from->type()->isRuntime() && to->isCompiletime()) {
    // from runtime to compile time -> not sure what to do?
    throw std::logic_error(
        "Not sure how to generate cast from runtime to compile-time type");
    // TODO probably should attempt to evaluate expression at compile-time
  } else {
    // simple runtime->runtime implicit cast
    ExImplicitCast c;
    c.source = from;
    c.targetType = to;
    return Zone::make<Expression>(c);
  }
}

} // namespace Cougar::Resolver
