#include "function.hh"

#include "statement.hh"
#include "type.hh"

#include "meta/function_info.hh"

namespace Cougar::Ast {

void FunctionDeclaration::doDump(int indent) const {
  iprint(indent, "FunctionDeclaration(access={}, name={})", mAccess, mName);
  if (mInfo) {
    iprint(indent + 2, "[resolved as:]");
    mInfo->dump(indent + 6);
  }
  iprint(indent + 2, "- return type:");
  mReturnType->dump(indent + 6);
  iprint(indent + 2, "- args:");
  for (const FunctionArg &arg : mArgs) {
    arg.dump(indent + 6);
  }
  if (mBody) {
    iprint(indent + 2, "- body:");
    mBody->dump(indent + 6);
  }
}

void FunctionDeclaration::addArg(TypeNode *argType, std::string_view argName) {
  mArgs.emplace_back(argType, argName);
}

void FunctionDeclaration::setBody(Statement *body) {
  assert(body);
  if (mBody)
    throw std::runtime_error("Function already has a body");
  mBody = body;
}

void FunctionArg::dump(int indent) const {
  iprint(indent, "FunctionArg(name={})", name);
  type->dump(indent + 2);
}

} // namespace Cougar::Ast