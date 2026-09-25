cmake_minimum_required(VERSION 3.21)
project(WinDiskImager VERSION 2.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Svg is not called directly: linking it makes both deploy scripts ship the
# iconengines/qsvgicon plugin the button icons need at runtime.
find_package(Qt6 REQUIRED COMPONENTS Widgets Svg)

# gzip/xz/bzip2/zstd image support in imagesource.cpp. zstd has no
# find module in CMake; its own package config is used, from MSYS2 and Fedora
# alike, with the shared library so both builds ship the same libzstd.dll.
find_package(ZLIB REQUIRED)
find_package(LibLZMA REQUIRED)
find_package(BZip2 REQUIRED)
find_package(zstd REQUIRED CONFIG)

# When cross-compiling, Qt6::lrelease resolves to a Windows .exe that cannot run
# on the build host. Point LRELEASE_EXECUTABLE at the host's native lrelease
# instead (e.g. -DLRELEASE_EXECUTABLE=/usr/bin/lrelease-qt6).
if(NOT LRELEASE_EXECUTABLE)
    find_package(Qt6 REQUIRED COMPONENTS LinguistTools)
    set(LRELEASE_EXECUTABLE Qt6::lrelease)
endif()

set(LANGUAGES es it pl nl de fr zh_CN zh_TW ta_IN ko ja)

# translations.qrc references lang/*.qm relative to this dir, so build them in-tree.
set(QM_FILES "")
foreach(lang ${LANGUAGES})
    set(ts "${CMAKE_CURRENT_SOURCE_DIR}/lang/diskimager_${lang}.ts")
    set(qm "${CMAKE_CURRENT_SOURCE_DIR}/lang/diskimager_${lang}.qm")
    add_custom_command(OUTPUT ${qm}
        COMMAND ${LRELEASE_EXECUTABLE} ${ts} -qm ${qm}
        DEPENDS ${ts} VERBATIM)
    list(APPEND QM_FILES ${qm})
endforeach()
add_custom_target(translations DEPENDS ${QM_FILES})

# The .ico is rendered from the SVG at build time by tools/mkicon, which must
# run on the build host, so it is configured separately with the host's
# native compiler and Qt rather than the cross toolchain.
set(ICON_SVG ${CMAKE_CURRENT_SOURCE_DIR}/images/WinDiskImager.svg)
# Build tree, not source tree: the 128 and 256 entries are PNG, and different
# zlib versions compress the same pixels to different bytes, so a committed
# .ico would be dirtied by every build on another host.
set(ICON_ICO ${CMAKE_CURRENT_BINARY_DIR}/images/WinDiskImager.ico)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/images)
set(MKICON_BUILD ${CMAKE_BINARY_DIR}/mkicon-host)
if(CMAKE_HOST_WIN32)
    set(MKICON_EXE ${MKICON_BUILD}/mkicon.exe)
else()
    set(MKICON_EXE ${MKICON_BUILD}/mkicon)
endif()

# --unset=CC --unset=CXX: on a first configure cmake puts the compiler it is
# detecting into its own environment, so a child would inherit the MinGW cross
# compiler and fail to find a native Qt. Reconfigures do not, which makes the
# failure intermittent. CMAKE_TOOLCHAIN_FILE is cleared too because cmake 3.21+
# also reads it from the environment.
if(EXISTS ${MKICON_BUILD}/CMakeCache.txt)
    file(READ ${MKICON_BUILD}/CMakeCache.txt MKICON_CACHE)
    if(MKICON_CACHE MATCHES "CMAKE_CXX_COMPILER:[^=]*=[^
]*mingw")
        # Left by a run that inherited the cross compiler; it would keep failing.
        file(REMOVE_RECURSE ${MKICON_BUILD})
    endif()
endif()

execute_process(
    COMMAND ${CMAKE_COMMAND} -E env --unset=CC --unset=CXX
            ${CMAKE_COMMAND} -S ${CMAKE_CURRENT_SOURCE_DIR}/../tools/mkicon
                             -B ${MKICON_BUILD} -G ${CMAKE_GENERATOR}
                             -DCMAKE_BUILD_TYPE=Release
                             -DCMAKE_TOOLCHAIN_FILE=
    RESULT_VARIABLE MKICON_CONFIGURED
    OUTPUT_VARIABLE MKICON_OUTPUT
    ERROR_VARIABLE  MKICON_OUTPUT)
if(NOT MKICON_CONFIGURED EQUAL 0)
    message(FATAL_ERROR
        "cannot configure tools/mkicon, which renders the application icon.
"
        "It is built with the host's own compiler and Qt, not the cross ones, "
        "because it has to run here rather than on the machine being built for. "
        "On Fedora that means gcc-c++, qt6-qtbase-devel and qt6-qtsvg-devel; "
        "MSYS2 UCRT64 already has them.

"
        "cmake said:
${MKICON_OUTPUT}")
endif()

# QT_QPA_PLATFORM: mkicon is a QGuiApplication and the build host may have no display.
add_custom_command(
    OUTPUT ${ICON_ICO}
    COMMAND ${CMAKE_COMMAND} --build ${MKICON_BUILD}
    COMMAND ${CMAKE_COMMAND} -E env QT_QPA_PLATFORM=offscreen
            ${MKICON_EXE} ${ICON_SVG} ${ICON_ICO}
    DEPENDS ${ICON_SVG}
    COMMENT "Rendering the application icon from images/WinDiskImager.svg"
    VERBATIM)
add_custom_target(appicon DEPENDS ${ICON_ICO})

qt_add_executable(WinDiskImager WIN32
    disk.cpp
    imagesource.cpp
    main.cpp
    mainwindow.cpp
    devicecombobox.cpp
    elapsedtimer.cpp
    mainwindow.ui
    gui_icons.qrc
    translations.qrc
    WinDiskImager.rc
)
add_dependencies(WinDiskImager translations appicon)
# windres finds the generated .ico (and the TEST_NO_ADMIN manifest) along its
# include path.
target_include_directories(WinDiskImager PRIVATE "${CMAKE_CURRENT_BINARY_DIR}")
# windres reads the .ico while compiling the resource script.
set_property(SOURCE WinDiskImager.rc APPEND PROPERTY OBJECT_DEPENDS ${ICON_ICO})

# -DTEST_NO_ADMIN=ON: asInvoker manifest, so the GUI starts without a UAC
# prompt. Such a build cannot open a device for writing; GUI work only.
option(TEST_NO_ADMIN "Build without the requireAdministrator manifest (GUI testing only)" OFF)
if(TEST_NO_ADMIN)
    message(WARNING
        "TEST_NO_ADMIN is on: this build requests no elevation and cannot write "
        "to a device. Do not ship it.")
    file(READ "${CMAKE_CURRENT_SOURCE_DIR}/WinDiskImager.manifest" _manifest)
    string(REPLACE "requireAdministrator" "asInvoker" _manifest "${_manifest}")
    file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/WinDiskImager-asinvoker.manifest" "${_manifest}")
    set_property(SOURCE WinDiskImager.rc APPEND PROPERTY COMPILE_DEFINITIONS TEST_NO_ADMIN)
    target_compile_definitions(WinDiskImager PRIVATE TEST_NO_ADMIN)
endif()

target_compile_definitions(WinDiskImager PRIVATE
    VER="${PROJECT_VERSION}"
    # The version's only source. These definitions also reach WinDiskImager.rc
    # through windres's preprocessor, which needs two more forms:
    #   VER_TUPLE  comma-separated, for VERSIONINFO's FILEVERSION.
    #   VER_RAW    dotted and unquoted; windres strips the quotes from -D
    #              values (VER arrives as a bare X.Y.Z), so the .rc applies #.
    VER_TUPLE=${PROJECT_VERSION_MAJOR},${PROJECT_VERSION_MINOR},${PROJECT_VERSION_PATCH},0
    VER_RAW=${PROJECT_VERSION}
    WINVER=0x0601
    _WIN32_WINNT=0x0601
)

# setupapi: enumeratePhysicalDevices() lists disks through it.
target_link_libraries(WinDiskImager PRIVATE Qt6::Widgets Qt6::Svg ZLIB::ZLIB LibLZMA::LibLZMA
                      BZip2::BZip2 zstd::libzstd_shared setupapi)

# Some Win32 calls use the ANSI API (e.g. CreateFile with toLatin1()); Qt6
# turns UNICODE on by default.
qt6_disable_unicode_defines(WinDiskImager)

set_target_properties(WinDiskImager PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
)
