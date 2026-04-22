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

// Go back, fill jump target for recent saved jump
void InstructionTable::backPatch(int jumpTarget) {
    if (jumpStack_.empty()) {
        return;
    }

    int savedAddress = jumpStack_.back();
    jumpStack_.pop_back();

    for (auto& instruction : instructions_) {
        if (instruction.address == savedAddress) {
            instruction.operand = std::to_string(jumpTarget);
            return;
        }
    }
}

// Print instructions table
void InstructionTable::print(std::ofstream& out) const {
    out << "\nInstruction Table\n";
    out << "Address\tOp\tOperand\n";

    for (const auto& instruction : instructions_) {
        out << instruction.address << '\t'
            << instruction.op << '\t'
            << instruction.operand << '\n';
    }
}