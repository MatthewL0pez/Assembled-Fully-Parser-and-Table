#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <string>
#include <vector>
#include <fstream>

struct SymbolTableEntry {   // One row in the symbol table
    std::string identifier;
    int memLocation;
    std::string type;
};

class SymbolTable {
public:
    SymbolTable();

    // prints the full symbol table
    void print(std::ofstream& out) const;

    // adds a new identifier to the symbol table
    // >>> returns false if it already exists <<< 
    bool add(const std::string& name, const std::string& type);

    // checks if the identifier already exists
    bool exists(const std::string& name) const;

    // gets the memory address of an identifier
    // >>> returns -1 if not found <<<
    int getMemAddress(const std::string& name) const;

    // gets the type of an identifier
    // >>> returns empty string if not found <<<
    std::string getIDType(const std::string& name) const;

private:
    std::vector<SymbolTableEntry> entries_;
    int nextMemLocation_;
};

#endif