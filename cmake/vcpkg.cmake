if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    if(DEFINED ENV{VCPKG_ROOT})
        set(CS2CPP_VCPKG_ROOT $ENV{VCPKG_ROOT})
    else()
        set(CS2CPP_VCPKG_ROOT external/vcpkg)
    endif()

    set(CMAKE_TOOLCHAIN_FILE "${CS2CPP_VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
endif()