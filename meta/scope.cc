#include "scope.hh"

#include "utils/iprint.hh"

namespace Cougar::Meta {

using namespace Utils;

void Scope::dump(int indent) const {

  if (mIsBuiltIn) {
    iprint(indent, "Scope (<built in>)");
  } else {
    iprint(indent, "Scope ({})", mName);
  }
}

} // namespace Cougar::Meta