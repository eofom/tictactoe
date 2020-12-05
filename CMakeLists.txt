cmake_minimum_required(VERSION 3.5)

project(tic-tac-toe-game LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(tic-tac-toe-game main.cpp board.h board.cpp marksline.h marksline.cpp
  game.h game.cpp)

# tell CMake to use CTest extension
enable_testing()
add_subdirectory(googletest EXCLUDE_FROM_ALL)

# create an executable, which instantiates a runner from
# GoogleTest, Boost.Test, QtTest or whatever framework you use
add_executable(my_test
               test_main.cpp board.cpp marksline.cpp game.cpp)
target_compile_features(my_test PRIVATE cxx_std_17)

# depending on the framework, you need to link to it
target_link_libraries(my_test
                      gtest_main)

# now register the executable with CTest
add_test(NAME my_test COMMAND my_test)
