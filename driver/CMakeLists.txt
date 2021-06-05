add_executable(cougar
  driver.cc

  toolchain_interface.hh
  toolchain_linux.cc toolchain_linux.hh
)

target_link_libraries(cougar resolver_lib)
target_link_libraries(cougar parser_lib)
target_link_libraries(cougar lexer_lib)
target_link_libraries(cougar llvm_codegen_lib)
