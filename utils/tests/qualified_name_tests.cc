#include <gtest/gtest.h>

#include "utils/qualified_name.hh"

namespace Cougar::Utils::Tests {

TEST(QualifiedNameTests, Formatting) {

  ZoneAllocator zone;

  QualifiedName qn("abc");

  EXPECT_EQ("abc", fmt::format("{}", qn));
}

} // namespace Cougar::Utils::Tests
