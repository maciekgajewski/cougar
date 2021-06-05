#pragma once

#include <string>
#include <string_view>

namespace Cougar::Utils::FileSystem {

// Generates unique temproary file with name starting with prefix
std::string generateTmpPath(std::string_view prefix, std::string_view suffix);

// Returns base name, ie 'dir/file.ext' -> 'file'
std::string getBaseName(std::string_view path);

} // namespace Cougar::Utils::FileSystem