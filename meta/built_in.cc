#include "built_in.hh"

#include "scope.hh"

#include "utils/zone_allocator.hh"

namespace Cougar::Meta {

namespace BuiltIn {

TypeInfo *typeCStr = nullptr;
TypeInfo *typeInt8 = nullptr;
TypeInfo *typeInt16 = nullptr;
TypeInfo *typeInt32 = nullptr;
TypeInfo *typeInt64 = nullptr;
TypeInfo *typeCtNumber = nullptr;

} // namespace BuiltIn

using namespace Utils;

class _built_in_tag {};

Scope *createBuiltInScope() {

  Scope *builtIn = Zone::make<Scope>(_built_in_tag{});

  BuiltIn::typeInt8 =
      builtIn->addType(TypeInfo::Simple{"int8"}, TypeInfo::NUMERIC);
  BuiltIn::typeInt16 =
      builtIn->addType(TypeInfo::Simple{"int16"}, TypeInfo::NUMERIC);
  BuiltIn::typeInt32 =
      builtIn->addType(TypeInfo::Simple{"int32"}, TypeInfo::NUMERIC);
  BuiltIn::typeInt64 =
      builtIn->addType(TypeInfo::Simple{"int64"}, TypeInfo::NUMERIC);

  BuiltIn::typeCStr = builtIn->addType(TypeInfo::Simple{"cstr"}, 0);

  // compile-time

  Scope *ct = builtIn->addNamedChild("ct");

  // ct::number
  BuiltIn::typeCtNumber = ct->addType(
      TypeInfo::Simple{"number"}, TypeInfo::NUMERIC | TypeInfo::COMPILE_TIME);

  BuiltIn::typeCtNumber->addImpicitCastTo(BuiltIn::typeInt8);
  BuiltIn::typeCtNumber->addImpicitCastTo(BuiltIn::typeInt16);
  BuiltIn::typeCtNumber->addImpicitCastTo(BuiltIn::typeInt32);
  BuiltIn::typeCtNumber->addImpicitCastTo(BuiltIn::typeInt64);

  return builtIn;
}

} // namespace Cougar::Meta