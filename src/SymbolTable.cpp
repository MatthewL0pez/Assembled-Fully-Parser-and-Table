#include "SymbolTable.h"

// begin the memory location at 10000 from assignment instructions and EXAMPLE
SymbolTable::SymbolTable() : nextMemLocation_(10000) {
}

// Add a new id if it is not already in the symbol table
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