cmake_minimum_required(VERSION 3.10)

project(tape_sort CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(include)

add_executable(tape_sort 
    src/main.cpp 
    src/file_tape.cpp 
    src/config.cpp
    src/file_tape_supplier.cpp
    src/sorter.cpp
    src/utils.cpp)

add_executable(tests 
    src/file_tape.cpp
    src/sorter.cpp
    src/file_tape_supplier.cpp
    src/utils.cpp
    test/doctest_main.cpp  
    test/tape_test.cpp
    test/sort_test.cpp   
    )

target_include_directories(tests PRIVATE doctest)

if(MSVC)
  target_compile_options(tape_sort PRIVATE /W4 /WX)
  target_compile_options(tests PRIVATE /W4 /WX)

else()
  target_compile_options(tape_sort PRIVATE -Wall -Wextra -Wpedantic -Werror)
  target_compile_options(tests PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()



