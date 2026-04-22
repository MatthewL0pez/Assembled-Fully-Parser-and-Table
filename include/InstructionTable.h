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

class InstructionTable {
public:
    InstructionTable();

    // add an instruction InstanceTable
    void generateInstruction(const std::string& op, const std::string& operand);

    // get current instruction address
    int getCurrentAddress() const;

    // save jump instruction address for back patching
    void pushJumpAddress(int address);

    // update recent jump instruction
    void backPatch(int jumpTarget);

    // print the  instruction table
    void print(std::ofstream& out) const;

private:
    std::vector<InstructionEntry> instructions_;
    std::vector<int> jumpStack_;
    int currentAddress_;
};

#endif