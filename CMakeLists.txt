cmake_minimum_required(VERSION 3.20)

project(badlang)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_definitions(-Wall -O2)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
add_executable(badlang src/main.cpp src/errors.cpp src/token.cpp src/keywords.cpp src/tokenizer.cpp src/grammar.cpp src/lr1_automaton.cpp src/parser.cpp src/ast.cpp)