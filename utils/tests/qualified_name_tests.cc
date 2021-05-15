#include <gtest/gtest.h>

#include "utils/qualified_name.hh"

namespace Cougar::Utils::Tests {

TEST(QualifiedNameTests, Formatting) {

  ZoneAllocator zone;

  QualifiedName qn1("abc");

  EXPECT_EQ("abc", fmt::format("{}", qn1));

  Qualification q2;
  q2.appendScope("s1");

  QualifiedName qn2("abc", q2);

  EXPECT_EQ("s1::abc", fmt::format("{}", qn2));

  Qualification q3;
  q3.appendScope("s1");
  q3.appendScope("s2");
  QualifiedName qn3("abc", q3);

  EXPECT_EQ("s1::s2::abc", fmt::format("{}", qn3));
}

} // namespace Cougar::Utils::Tests
