// Token.cpp
//
// Token.h already defines Token's constructors and Token::toString()
// (along with the free function tokenTypeToString()) as inline
// functions directly in the header. Re-declaring or re-defining any
// of them here would cause duplicate-definition errors, so this file
// intentionally contains no out-of-line Token implementation.
//
// It exists only so that the lexer/ directory has a matching .cpp for
// Token.h, in case the build system expects one; it compiles to an
// empty translation unit.

#include "Token.h"
#include <utility>
