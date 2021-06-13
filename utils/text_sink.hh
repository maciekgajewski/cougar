#pragma once

#include <fmt/format.h>

namespace Cougar::Utils {

// Sink for formatted text data
class TextSink {
public:
  TextSink(std::string_view outFile);
  ~TextSink();
  TextSink(const TextSink &) = delete;

  template <typename... Args>
  void print(std::string_view fmt, const Args &...args) {
    fmt::print(mStream, fmt, args...);
  }

private:
  std::FILE *mStream = nullptr;
};

} // namespace Cougar::Utils