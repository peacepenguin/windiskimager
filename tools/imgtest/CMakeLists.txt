# Standalone build of the image-decoder harness, not part of the application
# build. Usually run via tools/imgtest.sh; by hand:
#
#   cmake -S tools/imgtest -B build-imgtest -G Ninja
#   cmake --build build-imgtest
#   ./build-imgtest/imgtest.exe
cmake_minimum_required(VERSION 3.21)
project(imgtest LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(APPSRC "${CMAKE_CURRENT_SOURCE_DIR}/../../src" CACHE PATH "application sources")

# Core only: ImageSource needs no widgets, and the fixtures are built with the
# same zlib, liblzma, libbz2 and libzstd the application decodes with.
find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(ZLIB REQUIRED)
find_package(LibLZMA REQUIRED)
find_package(BZip2 REQUIRED)
find_package(zstd REQUIRED CONFIG)

add_executable(imgtest
    main.cpp
    imgtest.rc
    ${APPSRC}/imagesource.cpp
    ${APPSRC}/transferpipe.cpp
)
# windres reads the manifest while compiling the resource script.
set_property(SOURCE imgtest.rc APPEND PROPERTY OBJECT_DEPENDS
             ${CMAKE_CURRENT_SOURCE_DIR}/imgtest.manifest)

target_include_directories(imgtest PRIVATE ${APPSRC})
target_compile_definitions(imgtest PRIVATE
    WINVER=0x0601
    _WIN32_WINNT=0x0601
)
target_link_libraries(imgtest PRIVATE Qt6::Core ZLIB::ZLIB LibLZMA::LibLZMA
                      BZip2::BZip2 zstd::libzstd_shared)

# Match the application build, which turns off Qt6's default UNICODE defines.
qt6_disable_unicode_defines(imgtest)
