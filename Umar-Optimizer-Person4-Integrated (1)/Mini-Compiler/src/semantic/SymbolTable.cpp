#include "SymbolTable.h"

namespace semantic {

SymbolTable::SymbolTable() {
    // Global scope
    enterScope();
}

void SymbolTable::enterScope() {
    scopes.push_back(std::unordered_map<std::string, Symbol>());
}

void SymbolTable::exitScope() {
    if (!scopes.empty()) {
        scopes.pop_back();
    }
}

bool SymbolTable::declareVariable(const std::string& name, const std::string& type) {
    if (scopes.empty()) return false;
    
    auto& currentScope = scopes.back();
    if (currentScope.find(name) != currentScope.end()) {
        return false; // Already declared in this scope
    }
    
    currentScope[name] = Symbol{name, type, false};
    return true;
}

bool SymbolTable::initializeVariable(const std::string& name) {
    Symbol* sym = resolveVariable(name);
    if (sym) {
        sym->isInitialized = true;
        return true;
    }
    return false;
}

Symbol* SymbolTable::resolveVariable(const std::string& name) {
    // Search from innermost scope to outermost
    for (auto it = scopes.rbegin(); it != scopes.rend(); ++it) {
        auto found = it->find(name);
        if (found != it->end()) {
            return &(found->second);
        }
    }
    return nullptr; // Not found
}

} // namespace semantic
