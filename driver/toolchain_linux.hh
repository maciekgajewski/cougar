#pragma once

#include "toolchain_interface.hh"

namespace Cougar {

class ToolchainLinux final : public IToolchain {
public:
  std::string_view objectFileSuffix() const override { return ".o"; }
  std::string_view executableSuffix() const override { return {}; }

  void linkExecutable(std::vector<std::string> objectFiles,
                      std::string outputName) const override;
};

} // namespace Cougar