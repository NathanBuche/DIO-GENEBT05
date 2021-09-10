cmake_minimum_required(VERSION 3.10)

project(superio)
add_executable(superio superio.c libaaeon.c aaeon_f81866d.c aaeon_superio.c)

install(TARGETS superio DESTINATION bin)