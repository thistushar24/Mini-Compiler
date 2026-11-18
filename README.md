# Mini Compiler — System Programming Course Project

A fully working, end-to-end compiler for **MiniLang**, a small C-like language, written in **C++17**.  
The compiler runs every classical compilation phase in sequence and was built as a group course project.

---

## Language — MiniLang

MiniLang supports:

| Feature | Syntax |
|---|---|
| Variable declaration | `let x = 10;` |
| Assignment | `x = x + 1;` |
| Arithmetic | `+  -  *  /  %` |
| Comparison | `==  !=  <  >  <=  >=` |
| If / else | `if (cond) { ... } else { ... }` |
| While loop | `while (cond) { ... }` |
| Nested blocks | `{ ... { ... } ... }` |

---

## Pipeline

```
Source Code
    │
    ▼
┌─────────────┐
│    Lexer    │  → Token stream
└─────────────┘
    │
    ▼
┌─────────────┐
│   Parser    │  → Abstract Syntax Tree (AST)
└─────────────┘
    │
    ▼
┌──────────────────┐
│ Semantic Analyzer│  → Type / scope / init checks
└──────────────────┘
    │
    ▼
┌──────────────┐
│ IR Generator │  → Three-address IR
└──────────────┘
    │
    ▼
┌──────────────┐
│ IR Optimizer │  → (identity pass, extensible)
└──────────────┘
    │
    ▼
┌─────────────────────┐
│ Control Flow Graph  │  → Basic blocks + edges
└─────────────────────┘
    │
    ▼
┌────────────────┐
│ Code Generator │  → Register-allocated assembly
└────────────────┘
```

---

## Project Structure

```
Mini-Compiler/
├── src/
│   ├── lexer/           Lexer, Token, TokenType
│   ├── ast/             AST node classes + Visitor interface
│   ├── parser/          Recursive-descent parser (Parser + ExpressionParser)
│   ├── semantic/        SemanticAnalyzer + SymbolTable
│   ├── ir/              IROperand, IRInstruction, IRProgram
│   ├── irgen/           IRGenerator (AST → three-address IR)
│   ├── cfg/             BasicBlock + ControlFlowGraph
│   ├── codegen/         CodeGenerator (IR → register assembly)
│   ├── optimizer/       Optimizer interface + IROptimizer
│   └── main.cpp         Full-pipeline demo (runs all stages)
├── tests/
│   ├── integration/     test_parser_semantic.cpp
│   ├── ir/              test_ir_generator.cpp
│   ├── cfg/             test_cfg.cpp
│   └── codegen/         test_codegen.cpp
├── CMakeLists.txt       CMake build (requires CMake ≥ 3.10)
├── build.bat            Direct g++ build script (no CMake needed)
└── README.md
```

---

## Building

### Option A — build.bat (Windows, g++ only, no CMake needed)

```bat
build.bat
```

Produces all executables inside `build/`.

### Option B — CMake

```bash
mkdir build && cd build
cmake ..
cmake --build .
```

### Manual g++ (single command)

```bash
g++ -std=c++17 -Isrc -o mini_compiler \
    src/main.cpp \
    src/lexer/Lexer.cpp \
    src/parser/Parser.cpp \
    src/parser/ExpressionParser.cpp \
    src/semantic/SemanticAnalyzer.cpp \
    src/semantic/SymbolTable.cpp \
    src/irgen/IRGenerator.cpp \
    src/cfg/BasicBlock.cpp \
    src/cfg/ControlFlowGraph.cpp \
    src/codegen/CodeGenerator.cpp \
    src/optimizer/IROptimizer.cpp
```

---

## Running the Demo

```
./build/mini_compiler
```

The demo compiles this MiniLang program through every stage and prints the output of each phase:

```c
let x = 10;
let y = 20;
let result = 0;
x = x + y;
if (x > 20) {
    result = x * 2;
} else {
    result = x + 1;
}
let counter = 5;
while (counter > 0) {
    result = result + counter;
    counter = counter - 1;
}
```

**Sample output (abbreviated):**

```
===== STAGE 1: LEXER =====
Tokens produced: 70
  Token(LET, "let", line: 2, column: 9)
  ...

===== STAGE 4: IR GENERATION =====
x = 10
y = 20
result = 0
t1 = x + y
x = t1
t2 = x > 20
BRANCH t2, L1, L2
L1:
t3 = x * 2
result = t3
JUMP L3
...

===== STAGE 6: CONTROL FLOW GRAPH =====
Block 0:
    x = 10  ...  BRANCH t2, L1, L2
    Successors: B1, B2

===== STAGE 7: CODE GENERATION =====
MOV R0, 10
MOV R1, 20
...
CMPGT R4, R0, 20
BRANCH R4, L1, L2
...
```

---

## Running the Tests

```bash
./build/test_parser_semantic   # Lexer → Parser → Semantic Analyzer
./build/test_ir_generator      # + IR Generation
./build/test_cfg               # + Control Flow Graph
./build/test_codegen           # + Code Generation
```

All four tests exit with code 0 on success.

---

## Semantic Checks

The semantic analyzer catches:

- **Undeclared variable** — using a variable before `let`
- **Uninitialized variable** — declared with `let x;` but read before assignment
- **Redeclaration** — `let x = 1; let x = 2;` in the same scope
- Nested scopes are handled correctly (`{ let x = 1; }` does not conflict with outer `x`)

---

## Architecture Notes

- The **Visitor pattern** (`ASTVisitor`) cleanly separates the AST structure from the operations performed on it (semantic analysis, IR generation).
- The **IR** uses three-address code with explicit temporaries (`t1`, `t2`, …) and labels (`L1`, `L2`, …).
- The **CFG** partitions the IR into basic blocks and links them with predecessor/successor edges.
- The **code generator** does a simple linear scan register allocation (`R0`, `R1`, …), one register per unique name.
- The **optimizer** is a no-op identity pass, structured for future constant-folding / dead-code-elimination passes.

---

## Requirements

- C++17 compiler (GCC 7+, Clang 5+, MSVC 19.14+)
- CMake ≥ 3.10 (optional — `build.bat` works without it)
