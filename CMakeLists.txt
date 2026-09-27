cmake_minimum_required(VERSION 3.10)

project(MiniCompiler)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(src)

set(COMPILER_SOURCES
    src/lexer/Lexer.cpp
    src/parser/Parser.cpp
    src/parser/ExpressionParser.cpp
    src/semantic/SemanticAnalyzer.cpp
    src/semantic/SymbolTable.cpp
    src/irgen/IRGenerator.cpp
)

add_executable(test_parser_semantic
    tests/integration/test_parser_semantic.cpp
    ${COMPILER_SOURCES}
)

add_executable(test_ir_generator
    tests/ir/test_ir_generator.cpp
    ${COMPILER_SOURCES}
)