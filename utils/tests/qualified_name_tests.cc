#include <gtest/gtest.h>

#include "utils/qualified_name.hh"

namespace Cougar::Utils::Tests {

TEST(QualifiedNameTests, Formatting) {

  ZoneAllocator zone;

  QualifiedName qn("abc");

  EXPECT_EQ("abc", fmt::format("{}", qn));

  qn.appendScope("s1");

  EXPECT_EQ("s1::abc", fmt::format("{}", qn));

  qn.appendScope("s2");

  EXPECT_EQ("s1::s2::abc", fmt::format("{}", qn));
}

} // namespace Cougar::Utils::Tests
