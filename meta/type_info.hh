#pragma once

#include "cookie.hh"

#include "utils/zone_allocator.hh"

#include <string_view>
#include <variant>

namespace Cougar::Meta {

class Scope;

class TypeInfo {
public:
  enum Flags { COMPILE_TIME = 1 };

  struct Pointer {
    TypeInfo *pointed = nullptr;
  };

  struct Simple {
    std::string_view name;
  };

  TypeInfo(Simple s, std::uint64_t flags = 0) : mData(s), mFlags(flags) {
    mPrettyName = s.name;
  }
  TypeInfo(Pointer p) : mData(p) {
    mPrettyName = Utils::Zone::format("{}*", p.pointed->prettyName());
  }

  void dump(int indent) const;

  TypeInfo *pointerType() { return mPointerType; }
  void setPointerType(TypeInfo *ptrType);

  bool isPointer() const { return std::holds_alternative<Pointer>(mData); }

  std::string_view prettyName() const { return mPrettyName; }

  Cookie codegenData;

  template <typename F> auto visit(F f) { return std::visit(f, mData); }

  bool isCompileTime() const { return mFlags & COMPILE_TIME; }

private:
  std::variant<Pointer, Simple> mData;
  TypeInfo *mPointerType = nullptr;
  std::string_view mPrettyName;
  std::uint64_t mFlags = 0;
};

} // namespace Cougar::Meta
