cmake_minimum_required(VERSION 3.10)

project(MiniCompiler)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(src)

set(SOURCES
    src/parser/Parser.cpp
    src/parser/ExpressionParser.cpp
    src/semantic/SemanticAnalyzer.cpp
    src/semantic/SymbolTable.cpp

    src/irgen/IRGenerator.cpp
)

add_executable(test_parser_semantic
    tests/integration/test_parser_semantic.cpp
    ${SOURCES}
)