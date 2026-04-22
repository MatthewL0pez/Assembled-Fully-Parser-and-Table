#ifndef INSTRUCTION_TABLE_H
#define INSTRUCTION_TABLE_H

#include <string>
#include <vector>
#include <fstream>

// One row in instruction table
struct InstructionEntry {
    int address;
    std::string op;
    std::string operand;
};