cmake_minimum_required(VERSION 3.5)

project(tray LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB headers "*.h")
file(GLOB sources "*.cpp")
add_executable(sh-tray ${sources} ${headers})

install(TARGETS sh-tray
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
