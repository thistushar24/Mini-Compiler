@echo off
REM build.bat — Build all Mini Compiler targets using g++ directly (no CMake needed)
REM Usage: build.bat

set SRC=src
set OUT=build

if not exist %OUT% mkdir %OUT%

set SOURCES=^
    %SRC%/lexer/Lexer.cpp ^
    %SRC%/parser/Parser.cpp ^
    %SRC%/parser/ExpressionParser.cpp ^
    %SRC%/semantic/SemanticAnalyzer.cpp ^
    %SRC%/semantic/SymbolTable.cpp ^
    %SRC%/irgen/IRGenerator.cpp ^
    %SRC%/cfg/BasicBlock.cpp ^
    %SRC%/cfg/ControlFlowGraph.cpp ^
    %SRC%/codegen/CodeGenerator.cpp ^
    %SRC%/optimizer/IROptimizer.cpp

echo.
echo [1/5] Building main demo (mini_compiler)...
g++ -std=c++17 -I%SRC% -o %OUT%/mini_compiler.exe %SRC%/main.cpp %SOURCES%
if errorlevel 1 ( echo FAILED & exit /b 1 )
echo OK

echo.
echo [2/5] Building test_parser_semantic...
g++ -std=c++17 -I%SRC% -o %OUT%/test_parser_semantic.exe tests/integration/test_parser_semantic.cpp %SOURCES%
if errorlevel 1 ( echo FAILED & exit /b 1 )
echo OK

echo.
echo [3/5] Building test_ir_generator...
g++ -std=c++17 -I%SRC% -o %OUT%/test_ir_generator.exe tests/ir/test_ir_generator.cpp %SOURCES%
if errorlevel 1 ( echo FAILED & exit /b 1 )
echo OK

echo.
echo [4/5] Building test_cfg...
g++ -std=c++17 -I%SRC% -o %OUT%/test_cfg.exe tests/cfg/test_cfg.cpp %SOURCES%
if errorlevel 1 ( echo FAILED & exit /b 1 )
echo OK

echo.
echo [5/5] Building test_codegen...
g++ -std=c++17 -I%SRC% -o %OUT%/test_codegen.exe tests/codegen/test_codegen.cpp %SOURCES%
if errorlevel 1 ( echo FAILED & exit /b 1 )
echo OK

echo.
echo ============================================================
echo All targets built successfully!
echo Run:   build\mini_compiler.exe     — full pipeline demo
echo        build\test_parser_semantic.exe
echo        build\test_ir_generator.exe
echo        build\test_cfg.exe
echo        build\test_codegen.exe
echo ============================================================
