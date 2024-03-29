include(CheckIncludeFile)
include(CheckIncludeFiles)

set(CS2CPP_NAMESPACE_NAME "cs" CACHE STRING "")

if(WIN32)
    set(CS2CPP_PLATFORM "Windows" CACHE STRING "")
    set(CS2CPP_SUPPORT_POSIX FALSE)
else()
    if(APPLE)
        set(CS2CPP_PLATFORM "Darwin" CACHE STRING "")
        set(CMAKE_CXX_FLAGS "-fobjc-abi-version=2 -fobjc-arc")
    elseif(UNIX)
        set(CS2CPP_PLATFORM "Linux" CACHE STRING "")
    endif()
    set(CS2CPP_SUPPORT_POSIX TRUE)
endif()

option(CS2CPP_ENABLE_NAMESPACE "" TRUE)
option(CS2CPP_ENABLE_TEST "" TRUE)
