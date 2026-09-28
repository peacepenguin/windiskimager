# Standalone build of the combine harness, not part of the application build.
# Usually run via tools/combinetest.sh; by hand:
#
#   cmake -S tools/combinetest -B build-combinetest -G Ninja
#   cmake --build build-combinetest
#   ./build-combinetest/combinetest.exe
cmake_minimum_required(VERSION 3.21)
project(combinetest LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# combinedialog.h declares a Q_OBJECT class.
set(CMAKE_AUTOMOC ON)

set(APPSRC "${CMAKE_CURRENT_SOURCE_DIR}/../../src" CACHE PATH "application sources")

find_package(Qt6 REQUIRED COMPONENTS Widgets)
# The dialog reads its images through ImageSource, compressed ones included.
find_package(ZLIB REQUIRED)
find_package(LibLZMA REQUIRED)
find_package(BZip2 REQUIRED)
find_package(zstd REQUIRED CONFIG)

# disk.cpp includes mainwindow.h, which includes the generated ui header; see
# tools/gpttest/CMakeLists.txt.
qt6_wrap_ui(COMBINETEST_UI ${APPSRC}/mainwindow.ui)

add_executable(combinetest
    main.cpp
    ${APPSRC}/combine.cpp
    ${APPSRC}/combinedialog.cpp
    ${APPSRC}/combinereader.cpp
    ${APPSRC}/imagesource.cpp
    ${APPSRC}/disk.cpp
    ${COMBINETEST_UI}
)

target_include_directories(combinetest PRIVATE ${APPSRC})
target_compile_definitions(combinetest PRIVATE
    VER="combinetest"
    WINVER=0x0601
    _WIN32_WINNT=0x0601
)
target_link_libraries(combinetest PRIVATE Qt6::Widgets setupapi ZLIB::ZLIB LibLZMA::LibLZMA
                      BZip2::BZip2 zstd::libzstd_shared)

# disk.cpp uses the ANSI Win32 API; Qt6 turns UNICODE on by default.
qt6_disable_unicode_defines(combinetest)
