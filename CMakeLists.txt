cmake_minimum_required(VERSION 3.10)
project(MiniCompiler)

set(CMAKE_CXX_STANDARD 17)

# Include directories
include_directories(src)

# Core sources (Person 2 stuff right now)
set(SOURCES
    src/parser/Parser.cpp
    src/parser/ExpressionParser.cpp
    src/semantic/SemanticAnalyzer.cpp
    src/semantic/SymbolTable.cpp
)

# Test executable
add_executable(test_parser_semantic tests/integration/test_parser_semantic.cpp ${SOURCES})
