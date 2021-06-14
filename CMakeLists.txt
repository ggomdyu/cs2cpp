cmake_minimum_required(VERSION 3.8)

set(CMAKE_TOOLCHAIN_FILE vcpkg/scripts/buildsystems/vcpkg.cmake)

project(cs2cpp VERSION 1.0.0)

include(CheckIncludeFile)
include(CheckIncludeFiles)
include(CMakePackageConfigHelpers)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}")
set(CMAKE_MACOSX_RPATH YES)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/")

if(WIN32)
    set(CS2CPP_PLATFORM "Windows" CACHE STRING "")
elseif(APPLE)
    set(CS2CPP_PLATFORM "MacOS" CACHE STRING "")
    set(CMAKE_CXX_FLAGS "-fobjc-abi-version=2 -fobjc-arc")
else()
    message(FATAL_ERROR "Your platform is not supported.")
endif()

if(NOT CS2CPP_PLATFORM STREQUAL "Windows")
    set(CS2CPP_SUPPORT_POSIX TRUE)
else()
    set(CS2CPP_SUPPORT_POSIX FALSE)
endif()
set(CS2CPP_NAMESPACE_NAME "tg" CACHE STRING "")

check_include_file(emmintrin.h CS2CPP_SUPPORT_SSE2)
check_include_file(arm_neon.h CS2CPP_SUPPORT_NEON)
if(CS2CPP_SUPPORT_SSE2 OR CS2CPP_SUPPORT_NEON)
    set(CS2CPP_SUPPORT_SIMD TRUE)
else()
    set(CS2CPP_SUPPORT_SIMD FALSE)
endif()
option(CS2CPP_ENABLE_SIMD "" ${CS2CPP_SUPPORT_SIMD})
option(CS2CPP_ENABLE_NAMESPACE "" TRUE)
option(CS2CPP_ENABLE_TEST "" TRUE)
option(CS2CPP_ENABLE_INSTALL "" TRUE)

function(init_find_package)
    find_package(Microsoft.GSL CONFIG REQUIRED)
    find_package(FMT REQUIRED)
    find_package(ICU 67.1 COMPONENTS uc io i18n data REQUIRED)
    find_path(STB_INCLUDE_DIRS "stb.h")
endfunction()

function(init_subdirectories)
    add_library(cs2cpp cs2cpp.natvis)
    add_subdirectory(src)
    add_subdirectory(include)

    if (CS2CPP_ENABLE_TEST)
        enable_testing()
        add_subdirectory(test)
    endif ()
endfunction()

function(init_properties)
    set_target_properties(cs2cpp PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
    )
endfunction()

function(init_include_directories)
    target_include_directories(cs2cpp PUBLIC
        include
        ${STB_INCLUDE_DIRS}
        ${GSL_INCLUDE_DIRS}
        ${ICU_INCLUDE_DIRS}
    )
endfunction()

function(init_compile_definitions)
    target_compile_definitions(cs2cpp PUBLIC
        # Disable exception on fmt
        FMT_EXCEPTIONS=0

        # Add UNICODE, _UNICODE if the target platform is Windows
        $<$<STREQUAL:$<UPPER_CASE:"${CS2CPP_PLATFORM}">,"WINDOWS">:
            -DUNICODE
            -D_UNICODE
            -DWIN32_LEAN_AND_MEAN
            -DNOMINMAX
        >

        # Add namespace support macro
        $<IF:$<BOOL:${CS2CPP_ENABLE_NAMESPACE}>, CS2CPP_NAMESPACE=${CS2CPP_NAMESPACE_NAME},>
        $<IF:$<BOOL:${CS2CPP_ENABLE_NAMESPACE}>, CS2CPP_NAMESPACE_BEGIN=namespace\t${CS2CPP_NAMESPACE_NAME}{,>
        $<IF:$<BOOL:${CS2CPP_ENABLE_NAMESPACE}>, CS2CPP_NAMESPACE_END=},>

        # Add platform identifier macro
        $<UPPER_CASE:CS2CPP_PLATFORM_${CS2CPP_PLATFORM}>=1

        # Add SIMD support macro
        CS2CPP_ENABLE_SIMD=$<BOOL:${CS2CPP_ENABLE_SIMD}>
        CS2CPP_SUPPORT_SIMD=$<BOOL:${CS2CPP_SUPPORT_SIMD}>
        $<IF:$<BOOL:${CS2CPP_SUPPORT_SSE2}>, CS2CPP_SIMD_SSE2=1,
        $<IF:$<BOOL:${CS2CPP_SUPPORT_NEON}>, CS2CPP_SIMD_NEON=1,>>

        # Add POSIX support macro
        CS2CPP_SUPPORT_POSIX=$<BOOL:${CS2CPP_SUPPORT_POSIX}>
    )
endfunction()

function(init_link_libraries)
    target_link_libraries(cs2cpp PUBLIC
        fmt::fmt-header-only
        ICU::uc
        ICU::i18n
        ICU::data
        ICU::io
        Microsoft.GSL::GSL
    )
endfunction()

function(init_install_package)
    set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include/)
    set(LIBRARIES ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
    configure_package_config_file(
        cmake/${PROJECT_NAME}-config.cmake.in
        ${PROJECT_NAME}-config.cmake
        INSTALL_DESTINATION ${CMAKE_CURRENT_BINARY_DIR}
        PATH_VARS INCLUDE_DIR LIBRARIES
    )

    write_basic_package_version_file(
        ${PROJECT_NAME}-config-version.cmake
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY SameMajorVersion
    )

    install(
        FILES ${PROJECT_NAME}-config.cmake ${PROJECT_NAME}-config-version.cmake
        DESTINATION lib/${PROJECT_NAME}/cmake
    )

    export(
        TARGETS cs2cpp ${ICU_LIBRARIES}
        FILE ${PROJECT_NAME}-targets.cmake
    )
endfunction()

function(init_cs2cpp)
    init_find_package()
    init_subdirectories()
    init_properties()
    init_include_directories()
    init_compile_definitions()
    init_link_libraries()

    if (CS2CPP_ENABLE_INSTALL)
        init_install_package()
    endif()
endfunction()

init_cs2cpp()
