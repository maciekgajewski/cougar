#include "function.hh"

#include "type.hh"

namespace Cougar::Ast {

void FunctionDeclaration::doDump(int indent) const {
  iprint(indent, "FunctionDeclaration(access={}, name={})", mAccess, mName);
  iprint(indent + 2, "- return type:");
  mReturnType->dump(indent + 6);
  iprint(indent + 2, "- args:");
  for (const FunctionArg &arg : mArgs) {
    arg.dump(indent + 6);
  }
}

void FunctionDeclaration::addArg(TypeNode *argType, std::string_view argName) {
  mArgs.emplace_back(argType, argName);
}

void FunctionArg::doDump(int indent) const {
  iprint(indent, "FunctionArg(type={}, name={})", mType->name(), mName);
}

} // namespace Cougar::Ast