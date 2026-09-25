# Standalone build of the GPT harness, not part of the application build.
# Usually run via tools/gpttest.sh; by hand:
#
#   cmake -S tools/gpttest -B build-gpttest -G Ninja
#   cmake --build build-gpttest
#   ./build-gpttest/gpttest.exe
cmake_minimum_required(VERSION 3.21)
project(gpttest LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(APPSRC "${CMAKE_CURRENT_SOURCE_DIR}/../../src" CACHE PATH "application sources")

find_package(Qt6 REQUIRED COMPONENTS Widgets)

# disk.cpp includes mainwindow.h, which includes the generated ui header.
# AUTOUIC only scans a target's own sources for that include and would not see
# it in a header next door, so run uic directly.
qt6_wrap_ui(GPTTEST_UI ${APPSRC}/mainwindow.ui)

add_executable(gpttest
    main.cpp
    ${APPSRC}/disk.cpp
    ${GPTTEST_UI}
)

target_include_directories(gpttest PRIVATE ${APPSRC})
target_compile_definitions(gpttest PRIVATE
    VER="gpttest"
    WINVER=0x0601
    _WIN32_WINNT=0x0601
)
# setupapi: disk.cpp's device enumeration, linked though no case calls it.
target_link_libraries(gpttest PRIVATE Qt6::Widgets setupapi)

# disk.cpp uses the ANSI Win32 API; Qt6 turns UNICODE on by default.
qt6_disable_unicode_defines(gpttest)
