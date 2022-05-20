cmake_minimum_required(VERSION 3.20)

project(TestTasklist 
    VERSION 0.1
    DESCRIPTION "Very nice project"
    LANGUAGES CXX)

add_executable(test_tasklist
    src/main.cpp)

target_include_directories(test_tasklist PUBLIC
    inc)

target_compile_options(test_tasklist PUBLIC
    "-fconcepts")

set_property(TARGET test_tasklist PROPERTY CXX_STANDARD 20)