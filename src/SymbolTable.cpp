#include "SymbolTable.h"

// begin the memory location at 10000 from assignment instructions and EXAMPLE
SymbolTable::SymbolTable() : nextMemLocation_(10000) {
}

// Add a new identifier if it is not already in the symbol table
bool SymbolTable::add(const std::string& name, const std::string& type) {   
    if (exists(name)) {
        return false; 
    }

    SymbolTableEntry newEntry;
    newEntry.identifier = name;
    newEntry.memLocation = nextMemLocation_;
    newEntry.type = type;

    entries_.push_back(newEntry);
    nextMemLocation_++;

    return true;
}

// Check if an identifier is already declared in the Table 
bool SymbolTable::exists(const std::string& name) const {
    for (const auto& entry : entries_) {
        if (entry.identifier == name) {
            return true;
        }
    }

    return false;
}

// Print the symbol table out
void SymbolTable::print(std::ofstream& out) const {
    out << "\nSymbol Table\n";
    out << "Identifier\tMemoryLocation\tType\n";

    for (const auto& entry : entries_) {
        out << entry.identifier << '\t'
            << entry.memLocation << '\t'
            << entry.type << '\n';
    }
}