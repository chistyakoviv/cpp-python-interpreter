cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

set(CMAKE_CXX_STANDARD 17)

project(cpp-python-interpeter)

set(sources
    main.cpp
    lexer.cpp
    token.cpp
)

set(headers
    lexer.h
    token.h
)

add_executable(main ${sources} ${headers})
