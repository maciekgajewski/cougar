#pragma once

#include "utils/diagnostics.hh"

namespace Cougar::Ast {
class Module;
class FunctionDeclaration;
class TypeNode;
class TypeName;

class Statement;
struct StGroup;
struct StFunctionCall;
struct StReturn;

class Expression;
} // namespace Cougar::Ast

namespace Cougar::Meta {
class Scope;
class TypeInfo;
class FunctionInfo;
} // namespace Cougar::Meta

namespace Cougar::Lexer {
class Token;
}

namespace Cougar::Resolver {

// TODO move somwhere else
struct CompilationState {
  Meta::Scope *mBuildInScope = nullptr;
  Meta::Scope *mRootScope = nullptr;
};

class Resolver {
public:
  Resolver(Utils::Diagnostics &diag, CompilationState &state);

  void resolveModule(Ast::Module *module);

private:
  struct ControlFlowInfo {
    bool returns = false;
    bool fallsTrough = false;
  };

  struct StatementContext {
    Meta::Scope *scope = nullptr;
    Meta::FunctionInfo *function = nullptr;
    Utils::SourceLocation location;
  };

  // functions
  void resolveFunctionDelcaration(Ast::FunctionDeclaration *,
                                  Meta::Scope *scope);
  void resolveFunctionBody(Ast::FunctionDeclaration *, Meta::Scope *scope);

  // types
  Meta::TypeInfo *resolveType(Ast::TypeNode *tn, Meta::Scope *scope);
  Meta::TypeInfo *resolveNamedType(std::string_view name, Meta::Scope *scope);

  // statements
  ControlFlowInfo resolveStatement(Ast::Statement *stmt,
                                   const StatementContext &ctx);

  ControlFlowInfo resolveStatement(Ast::StGroup &stmt,
                                   const StatementContext &ctx);
  ControlFlowInfo resolveStatement(Ast::StFunctionCall &stmt,
                                   const StatementContext &ctx);
  ControlFlowInfo resolveStatement(Ast::StReturn &stmt,
                                   const StatementContext &ctx);

  // statement helpers
  ControlFlowInfo resolveGroup(Ast::StGroup &stmt, const StatementContext &ctx);
  Meta::FunctionInfo *resolveNamedFunction(std::string_view name,
                                           Meta::Scope *scope);

  // expressions
  void resolveExpression(Ast::Expression *e, Meta::Scope *scope);

  Ast::Expression *createImplicitCast(Ast::Expression *from,
                                      Meta::TypeInfo *to);

  Utils::Diagnostics &mDiag;
  CompilationState &mState;
};

} // namespace Cougar::Resolver