#include "toolchain_linux.hh"

#include <fmt/format.h>

#include <sys/wait.h>
#include <unistd.h>

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

  std::string linker = "/usr/bin/ld";

  std::vector<std::string> args;
  args.push_back(linker);

  args.push_back("-dynamic-linker");
  args.push_back("/lib64/ld-linux-x86-64.so.2");

  args.push_back("-o");
  args.push_back(outputName);

  args.insert(args.end(), objectFiles.begin(), objectFiles.end());

  args.push_back("/usr/lib/x86_64-linux-gnu/crti.o");
  args.push_back("/usr/lib/x86_64-linux-gnu/crt1.o");
  args.push_back("/usr/lib/gcc/x86_64-linux-gnu/9/crtbegin.o");
  args.push_back("/usr/lib/gcc/x86_64-linux-gnu/9/crtend.o");
  args.push_back("/usr/lib/x86_64-linux-gnu/crtn.o");

  args.push_back("-lc");

  // TODO move to utils maybe if used again
  std::vector<const char *> execArgs;
  execArgs.reserve(args.size() + 1);
  for (const std::string &arg : args)
    execArgs.push_back(arg.c_str());
  execArgs.push_back(nullptr);

  ::pid_t pid = ::fork();
  if (pid == 0) {

    ::execv(linker.c_str(), const_cast<char *const *>(execArgs.data()));
  } else {
    int s;
    ::waitpid(pid, &s, 0);
  }
}

} // namespace Cougar