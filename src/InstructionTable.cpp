#include "InstructionTable.h"

InstructionTable::InstructionTable() : currentAddress_(1) {
}

// Add instruction row in the instruction table
void InstructionTable::generateInstruction(const std::string& op, const std::string& operand) {
    InstructionEntry newInstruction;
    newInstruction.address = currentAddress_;
    newInstruction.op = op;
    newInstruction.operand = operand;

    instructions_.push_back(newInstruction);
    currentAddress_++;
}

