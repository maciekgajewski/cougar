#pragma once

#include <string_view>

#include "function_info.hh"
#include "type_info.hh"
#include "variable_info.hh"

#include "utils/map.hh"

namespace Cougar::Meta {

class _built_in_tag;

class Scope {
public:
  Scope(std::string_view name, Scope *parent) : mName(name), mParent(parent) {
    if (!name.empty() && parent) {
      mQualification = parent->mQualification;
      mQualification.appendScope(name);
    }
  }

  Scope(const _built_in_tag &) { mIsBuiltIn = true; }

  void dump(int indent = 0) const;

  TypeInfo *addType(TypeInfo::Simple s, std::uint64_t flags) {
    return mTypes.emplace(s.name, s, flags, mQualification);
  }

  TypeInfo *findType(std::string_view name) { return mTypes.find(name); }

  Scope *parent() { return mParent; }

  Scope *addNamedChild(std::string_view name) {
    return mNamedChildren.emplace(name, name, this);
  }

  FunctionInfo *addFunction(std::string_view name) {
    return mFunctions.emplace(name, name);
  }

  FunctionInfo *findFunction(std::string_view name) {
    return mFunctions.find(name);
  }

  VariableInfo *addVariable(std::string_view name) {
    return mVariables.emplace(name, name);
  }

  VariableInfo *findVariable(std::string_view name) {
    return mVariables.find(name);
  }

private:
  std::string_view mName;
  Scope *mParent = nullptr;
  bool mIsBuiltIn = false;

  Utils::Map<std::string_view, TypeInfo> mTypes;
  Utils::Map<std::string_view, FunctionInfo> mFunctions;
  Utils::Map<std::string_view, Scope> mNamedChildren;
  Utils::Map<std::string_view, VariableInfo> mVariables;
  Utils::Qualification mQualification;
};

} // namespace Cougar::Meta
