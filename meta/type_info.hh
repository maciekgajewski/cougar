#pragma once

#include "cookie.hh"

#include "utils/qualified_name.hh"
#include "utils/zone_allocator.hh"

#include <string_view>
#include <variant>

namespace Cougar::Meta {

class Scope;

class TypeInfo {
public:
  enum Flags { COMPILE_TIME = 1, NUMERIC = 2 };

  struct Pointer {
    TypeInfo *pointed = nullptr;
  };

  struct Simple {
    std::string_view name;
  };

  TypeInfo(Simple s, std::uint64_t flags, Utils::Qualification q)
      : mData(s), mFlags(flags), mName(s.name, q) {}

  TypeInfo(Pointer p) : mData(p) {
    mName.setName(Utils::Zone::format("{}*", p.pointed->prettyName()));
  }

  void dump(int indent) const;

  TypeInfo *pointerType() { return mPointerType; }
  void setPointerType(TypeInfo *ptrType);

  bool isPointer() const { return std::holds_alternative<Pointer>(mData); }

  const Utils::QualifiedName &prettyName() const { return mName; }

  Cookie codegenData;

  template <typename F> auto visit(F f) { return std::visit(f, mData); }

  bool isCompiletime() const { return mFlags & COMPILE_TIME; }
  bool isRuntime() const { return !(mFlags & COMPILE_TIME); }

  void addImpicitCastTo(TypeInfo *other) {
    mImplicitlyCastsTo.emplace_back(other);
  }

  bool canImplicitlyCastTo(TypeInfo *other) const {
    auto it =
        std::find(mImplicitlyCastsTo.begin(), mImplicitlyCastsTo.end(), other);
    return it != mImplicitlyCastsTo.end();
  }

private:
  std::variant<Pointer, Simple> mData;
  TypeInfo *mPointerType = nullptr;
  std::uint64_t mFlags = 0;
  Utils::QualifiedName mName;

  // TODO implement Utils::Set
  Utils::List<TypeInfo *> mImplicitlyCastsTo;
};

} // namespace Cougar::Meta
