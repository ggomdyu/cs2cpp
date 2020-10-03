set(GLOB_AGRUMENTS *.cpp *.c *.mm)
file(GLOB_RECURSE SRC_PATH_LIST ${GLOB_AGRUMENTS})

string(TOUPPER ${CS2CPP_PLATFORM} UPPERCASE_PLATFORM)

foreach(SRC_PATH ${SRC_PATH_LIST})
    string(TOUPPER ${SRC_PATH} UPPERCASE_SRC_PATH)

    # Ignore the source which is not specific to current platform.
    if(${UPPERCASE_SRC_PATH} MATCHES "((\\.WINDOWS)|(\\.DARWIN)|(\\.ANDROID))\\.")
        # If the platform prefix doesn't matched from current platform
        if(NOT ${UPPERCASE_SRC_PATH} MATCHES ${UPPERCASE_PLATFORM})
            continue()
        endif()
    # Ignore the source if the current platform doesn't support POSIX.
    elseif(${UPPERCASE_SRC_PATH} MATCHES "\\.POSIX\\.")
        if(NOT ${CS2CPP_SUPPORT_POSIX})
            continue()
        endif()
    endif()

    list(APPEND CS2CPP_SOURCES ${SRC_PATH})
endforeach()

target_sources(cs2cpp PUBLIC ${CS2CPP_SOURCES})