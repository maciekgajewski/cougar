#include "toolchain_linux.hh"

#include <fmt/format.h>

namespace Cougar {

void ToolchainLinux::linkExecutable(std::vector<std::string> objectFiles,
                                    std::string outputName) const {

  fmt::print("Linking {} into executable {}\n", fmt::join(objectFiles, ","),
             outputName);

  // TODO link
  //
  // Workign invocation:
  //
  // ld -z relro --eh-frame-hdr -m elf_x86_64  -dynamic-linker
  // /lib64/ld-linux-x86-64.so.2 -o hwld output.o
  // /usr/lib/x86_64-linux-gnu/crti.o /usr/lib/x86_64-linux-gnu/crt1.o
  // /usr/lib/gcc/x86_64-linux-gnu/9/crtbegin.o -lc
  // /usr/bin/../lib/gcc/x86_64-linux-gnu/9/crtend.o
  // /usr/bin/../lib/gcc/x86_64-linux-gnu/9/../../../x86_64-linux-gnu/crtn.o
}

} // namespace Cougar