#include "text_sink.hh"

#include <cerrno>
#include <cstdio>
#include <cstring>
#include <stdexcept>

namespace Cougar::Utils {

TextSink::TextSink(std::string_view outFile) {

  std::string of(outFile);
  mStream = std::fopen(of.c_str(), "w");
  if (!mStream)
    throw std::runtime_error(fmt::format(
        "Error opening file '{}' for writing: {}", of, std::strerror(errno)));
}

TextSink::~TextSink() {
  if (mStream)
    std::fclose(mStream);
}

} // namespace Cougar::Utils