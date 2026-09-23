# Builds mkicon, which renders an SVG into a multi-size Windows .ico.
#
#   mkicon <in.svg> <out.ico>
#
# src/CMakeLists.txt configures and builds this separately, for the build host,
# since it runs during the build. Not meant to be invoked by hand. See BUILD.md.
cmake_minimum_required(VERSION 3.21)
project(mkicon LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
find_package(Qt6 REQUIRED COMPONENTS Gui Svg)
add_executable(mkicon main.cpp)
target_link_libraries(mkicon PRIVATE Qt6::Gui Qt6::Svg)
