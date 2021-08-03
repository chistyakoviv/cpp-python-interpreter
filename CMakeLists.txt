cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

set(CMAKE_CXX_STANDARD 20)

project(cpp-python-interpeter)

set(sources
    main.cpp
    lexer.cpp
    token.cpp
    object.cpp
    object_holder.cpp
    parser.cpp
    comparators.cpp
    ast.cpp
)

set(headers
    lexer.h
    token.h
    object.h
    object_holder.h
    parser.h
    comparators.h
    ast.h
)

add_executable(main ${sources} ${headers})
