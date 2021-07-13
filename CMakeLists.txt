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
    set(CS2CPP_SUPPORT_POSIX FALSE)
elseif(APPLE)
    set(CS2CPP_PLATFORM "MacOS" CACHE STRING "")
    set(CMAKE_CXX_FLAGS "-fobjc-abi-version=2 -fobjc-arc")
    set(CS2CPP_SUPPORT_POSIX TRUE)
else()
    message(FATAL_ERROR "Your platform is not supported.")
endif()

set(CS2CPP_NAMESPACE_NAME "tg" CACHE STRING "")

check_include_file(emmintrin.h CS2CPP_SUPPORT_SSE2)
check_include_file(arm_neon.h CS2CPP_SUPPORT_NEON)
option(CS2CPP_ENABLE_NAMESPACE "" TRUE)
option(CS2CPP_ENABLE_TEST "" TRUE)
option(CS2CPP_ENABLE_INSTALL "" TRUE)

function(find_packages)
    find_package(Microsoft.GSL CONFIG REQUIRED)
    find_package(FMT REQUIRED)
    find_package(ICU 67.1 COMPONENTS uc io i18n data REQUIRED)
    find_path(STB_INCLUDE_DIRS "stb.h")
endfunction()

function(export_package)
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
        # Namespaces
        "CS2CPP_NAMESPACE_NAME=${CS2CPP_NAMESPACE_NAME}"
        "CS2CPP_NAMESPACE_BEGIN=$<$<BOOL:${CS2CPP_ENABLE_NAMESPACE}>:namespace\t${CS2CPP_NAMESPACE_NAME}{>"
        "CS2CPP_NAMESPACE_END=$<$<BOOL:${CS2CPP_ENABLE_NAMESPACE}>:}>"

        # SIMD settings
        "CS2CPP_SUPPORT_SIMD=$<OR:$<BOOL:${CS2CPP_SUPPORT_SSE2}>,$<BOOL:${CS2CPP_SUPPORT_NEON}>>"
        "CS2CPP_SUPPORT_SSE2=$<BOOL:${CS2CPP_SUPPORT_SSE2}>"
        "CS2CPP_SUPPORT_NEON=$<BOOL:${CS2CPP_SUPPORT_NEON}>"

        # Platform settings
        "CS2CPP_PLATFORM_$<UPPER_CASE:${CS2CPP_PLATFORM}>=1"
        "CS2CPP_SUPPORT_POSIX=$<NOT:$<PLATFORM_ID:Windows>>"

        # Compiler attributes
        "CS2CPP_NOINLINE=$<IF:$<CXX_COMPILER_ID:MSVC>,__declspec(noinline),__attribute__((noinline))>"
        "CS2CPP_NOVTABLE=$<$<CXX_COMPILER_ID:MSVC>:__declspec(novtable)>"

        # Etc
        $<$<PLATFORM_ID:Windows>:UNICODE _UNICODE WIN32_LEAN_AND_MEAN NOMINMAX>
        "FMT_EXCEPTIONS=0"
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

function(init_cs2cpp)
    find_packages()

    init_subdirectories()
    init_properties()
    init_include_directories()
    init_compile_definitions()
    init_link_libraries()

    if (CS2CPP_ENABLE_INSTALL)
        export_package()
    endif()
endfunction()

init_cs2cpp()
