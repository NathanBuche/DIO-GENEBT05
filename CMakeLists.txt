cmake_minimum_required(VERSION 3.10)

project(superio)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -I/user/src/linux/include/ -O2")
add_executable(superio superio.c libaaeon.c aaeon_f81866d.c aaeon_superio.c)

install(TARGETS superio DESTINATION bin)