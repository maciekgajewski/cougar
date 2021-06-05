#pragma once

#include <string_view>
#include <vector>

namespace Cougar {

class IToolchain {
public:
  virtual ~IToolchain() = default;

  virtual std::string_view objectFileSuffix() const = 0;
  virtual std::string_view executableSuffix() const = 0;

  virtual void linkExecutable(std::vector<std::string> objectFiles,
                              std::string outputName) const = 0;
};

} // namespace Cougar