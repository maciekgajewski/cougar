#include <gtest/gtest.h>

#include "utils/utf8_decoder.hh"

namespace Cougar::Utils::Tests {

TEST(Utf8DecoderTests, UnicodeDecoding) {

  // the test sequence contains all:
  // - ascii chars
  // 2-byte chars (Eastenr European)
  // 3-byte chars (Traditional Chineese)
  // 4-byte chars (Emoji)
  const char *input =
      "Założę się o pięć stów że 驢 Carl 😉 próbuje to przetłumaczyć";

  std::vector<rune_t> expectedOutput = {
      {U'Z', U'a', U'ł',  U'o', U'ż', U'ę', U' ', U's', U'i', U'ę', U' ', U'o',
       U' ', U'p', U'i',  U'ę', U'ć', U' ', U's', U't', U'ó', U'w', U' ', U'ż',
       U'e', U' ', U'驢', U' ', U'C', U'a', U'r', U'l', U' ', U'😉', U' ', U'p',
       U'r', U'ó', U'b',  U'u', U'j', U'e', U' ', U't', U'o', U' ', U'p', U'r',
       U'z', U'e', U't',  U'ł', U'u', U'm', U'a', U'c', U'z', U'y', U'ć'}};

  std::vector<rune_t> output;

  Utf8Decoder decoder(input);
  while (rune_t rune = decoder.next()) {
    output.push_back(rune);
  }

  EXPECT_EQ(expectedOutput, output);
}

} // namespace Cougar::Utils::Tests