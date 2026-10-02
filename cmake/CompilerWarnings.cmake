# Compiler warnings for AMMC
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  add_compile_options(
    -Wall -Wextra -Wpedantic
    -Wconversion -Wsign-conversion
    -Wshadow -Wnon-virtual-dtor
    -Wold-style-cast -Wcast-align
    -Wunused -Woverloaded-virtual
    -Wnull-dereference
  )
elseif(MSVC)
  add_compile_options(/W4 /permissive-)
endif()
