#include "file_system.hh"

#include <fmt/core.h>

#include <cerrno>
#include <cstring>

#include <stdlib.h>
#include <unistd.h>

namespace Cougar::Utils::FileSystem {

std::string generateTmpPath(std::string_view prefix, std::string_view suffix) {

  std::string tmp = fmt::format("{}XXXXXX{}", prefix, suffix);

  int r = ::mkstemps(tmp.data(), suffix.size());
  if (r < 0)
    throw std::runtime_error(
        fmt::format("mkstemps failed: {}", std::strerror(errno)));

  ::close(r);

  return tmp;
}

} // namespace Cougar::Utils::FileSystem