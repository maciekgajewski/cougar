#pragma once

#include <string>

namespace Cougar::Utils::FileSystem {

// Generates unique temproary file with name starting with prefix
std::string generateTmpPath(std::string_view prefix, std::string_view suffix);

} // namespace Cougar::Utils::FileSystem