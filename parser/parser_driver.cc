#include <fmt/core.h>

#include "parser.hh"

#include "lexer/lexer.hh"
#include "lexer/token.hh"

#include "ast/module.hh"

#include "utils/file_loader.hh"
#include "utils/zone_allocator.hh"

int main(int argc, char **argv) {
  fmt::print("Parser!\n");

  if (argc < 2)
    throw std::runtime_error("Argument missing: file path");

  std::string path = argv[1];
  fmt::print("Parsing file '{}'...\n", path);

  using namespace Cougar;

  Utils::ZoneAllocator zone;

  auto tokens = Lexer::lexFile(path);

  Utils::Diagnostics diag;
  diag.setPath(path);

  Parser::Parser parser(diag);

  auto module = parser.parseModule(tokens);

  diag.print();

  if (!diag.hasErrors()) {
    module->dump();
  }
}
