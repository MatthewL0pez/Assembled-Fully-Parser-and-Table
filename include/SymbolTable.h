#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <string>
#include <vector>
#include <fstream>

struct SymbolTableEntry {   // One row in the symbol table
    std::string identifier;
    int memoryLocation;
    std::string type;
};

