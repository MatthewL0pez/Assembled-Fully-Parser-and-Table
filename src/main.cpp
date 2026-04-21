#include <iostream>
#include <fstream>
#include <string>

#include "Lexer.h"
#include "Tokens.h"
#include "Parser.h"

int main(int argc, char* argv[]) {

    //makes sure theres at least one input file
    if (argc < 3) {
        std::cout << "Usage: " << argv[0] << " <input_file> <output_file> " << std::endl;
        return 1;
    }

    std::string inputFileName = argv[1];
    std::string outputFileName = argv[2];

    // create the lexer using the input file name
    Lexer lexer(inputFileName);

    if (!lexer.isOpen()) {
        std::cerr << "Error: could not open input file: " << inputFileName << std::endl; //checks if file is acctually opened 
        return 1;
    }

    std::ofstream outFile(outputFileName);
    if (!outFile.is_open()) {
        std::cerr << "Error: could not open output file: " << outputFileName << std::endl;
        return 1;
    }

    Parser parser(lexer, outFile, true);
    parser.Rat26S();

    outFile << "\nParsing completed successfully. No syntax errors found.\n";
    outFile.close();

    return 0;
}