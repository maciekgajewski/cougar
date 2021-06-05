#include "file_system.hh"

#include <boost/filesystem.hpp>

#include <fmt/core.h>

namespace Cougar::Utils::FileSystem {

namespace bfs = boost::filesystem;

std::string generateTmpPath(std::string_view prefix, std::string_view suffix) {

  std::string model = fmt::format("{}-%%%%-%%%%-%%%%-%%%%{}", prefix, suffix);
  return (bfs::temp_directory_path() / bfs::unique_path(model)).string();
}

std::string getBaseName(std::string_view path) {
  bfs::path p(path.begin(), path.end());
  return p.stem().string();
}

void deleteFile(std::string_view path) {
  bfs::path p(path.begin(), path.end());
  bfs::remove(p);
}

} // namespace Cougar::Utils::FileSystem