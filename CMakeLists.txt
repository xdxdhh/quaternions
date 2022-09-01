cmake_minimum_required(VERSION 3.15)
project(quaternions)

# GoogleTest requires at least C++14
# We try to stay on the latest stable version
set(CMAKE_CXX_STANDARD 20)

include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.12.1
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

# build the test executable with the tests.cpp file
add_executable(
    test_quaternions
    tests.cpp
)

# use the gtest_main instead of defining the main() in tests.cpp
target_link_libraries(
    test_quaternions
    gtest_main
)

include(GoogleTest)
gtest_discover_tests(test_quaternions)


