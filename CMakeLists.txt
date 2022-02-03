cmake_minimum_required(VERSION 3.8)

include(cmake/options.cmake)

set(CMAKE_TOOLCHAIN_FILE vcpkg/scripts/buildsystems/vcpkg.cmake)
set(CMAKE_MACOSX_RPATH ON)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
cmake_policy(SET CMP0079 NEW)

project(cs2cpp)

function(find_packages)
    find_package(FMT REQUIRED)
    find_package(ICU COMPONENTS i18n uc io data)
endfunction()

function(init_subdirectories)
    add_library(cs2cpp cs2cpp.natvis)
    add_subdirectory(src)
    add_subdirectory(include)
    if (CS2CPP_ENABLE_TEST)
        enable_testing()
        add_subdirectory(test)
    endif()
endfunction()

function(init_properties)
    set_target_properties(cs2cpp PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS OFF
    )
endfunction()

function(init_include_directories)
    target_include_directories(cs2cpp PUBLIC
        include
        ${ICU_INCLUDE_DIRS}
    )
endfunction()

function(init_compile_definitions)
    target_compile_definitions(cs2cpp PUBLIC
        # Namespaces
        "CS2CPP_NAMESPACE_NAME=${CS2CPP_NAMESPACE_NAME}"
        "CS2CPP_NAMESPACE_BEGIN=$<$<BOOL:${CS2CPP_ENABLE_NAMESPACE}>:namespace\t${CS2CPP_NAMESPACE_NAME}{>"
        "CS2CPP_NAMESPACE_END=$<$<BOOL:${CS2CPP_ENABLE_NAMESPACE}>:}>"

        # Platform settings
        "CS2CPP_PLATFORM_$<UPPER_CASE:${CS2CPP_PLATFORM}>=1"
        "CS2CPP_SUPPORT_POSIX=$<NOT:$<PLATFORM_ID:Windows>>"

        # Compiler attributes
        "CS2CPP_NOINLINE=$<IF:$<CXX_COMPILER_ID:MSVC>,__declspec(noinline),__attribute__((noinline))>"
        "CS2CPP_NOVTABLE=$<$<CXX_COMPILER_ID:MSVC>:__declspec(novtable)>"
        "CS2CPP_SPRINTF=$<IF:$<CXX_COMPILER_ID:MSVC>,sprintf_s,sprintf>"

        # Etc
        $<$<PLATFORM_ID:Windows>:UNICODE _UNICODE WIN32_LEAN_AND_MEAN NOMINMAX>
        "FMT_EXCEPTIONS=0"
    )
endfunction()

function(init_link_libraries)
    target_link_libraries(cs2cpp PUBLIC
        fmt::fmt-header-only
        ICU::i18n
        ICU::uc
        ICU::io
        ICU::data
    )
endfunction()

function(init_cs2cpp)
    find_packages()

    init_subdirectories()
    init_properties()
    init_include_directories()
    init_compile_definitions()
    init_link_libraries()
endfunction()

init_cs2cpp()
