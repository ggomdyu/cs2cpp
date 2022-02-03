cmake_minimum_required(VERSION 3.8)

set(GLOB_ARGUMENTS *.cpp)
file(GLOB_RECURSE SRC_PATH_LIST ${GLOB_ARGUMENTS})
add_executable(test ${SRC_PATH_LIST})

find_package(GTest CONFIG REQUIRED)

target_link_libraries(test PRIVATE
    cs2cpp
    GTest::gmock
    GTest::gtest
)

set_target_properties(test PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

target_include_directories(test PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)