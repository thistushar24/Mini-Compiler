#pragma once
#include <string>
#include <unordered_map>
#include <vector>

namespace semantic {

struct Symbol {
    std::string name;
    std::string type; // e.g., "int"
    bool isInitialized;
};

class SymbolTable {
public:
    SymbolTable();

    void enterScope();
    void exitScope();

    bool declareVariable(const std::string& name, const std::string& type);
    bool initializeVariable(const std::string& name);
    
    Symbol* resolveVariable(const std::string& name);

private:
    std::vector<std::unordered_map<std::string, Symbol>> scopes;
};

} // namespace semantic
