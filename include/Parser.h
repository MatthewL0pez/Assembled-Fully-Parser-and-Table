#ifndef PARSER_H
#define PARSER_H

#include "Lexer.h"
#include "Tokens.h"
#include "SymbolTable.h"
#include "InstructionTable.h"
#include "ExpressionResult.h"

#include <fstream> 
#include <string> 

class Parser {
public:
    Parser(Lexer& lexer, std::ofstream& outFile, bool printRules = true);
    
    // grammer 
    void Rat26S();
    void OptFunctionDefinitions();
    void FunctionDefinitions();
    void Function();
    void ParameterList();
    void Parameter();
    void Qualifier();
    void Body();
    void OptDeclarationList();
    void OptParameterList();
    void DeclarationList();
    void Declaration();
    void IDs();
    void Empty();

    // STATEMENTS
    void StatementList();
    void Statement();
    void Compound();
    void Assign();
    void If();
    void Return();
    void Print();
    void Scan();
    void While();
    
    // Expreesions, conditions
    void Condition();
    void Relop();
    void Expression();
    void ExpressionPrime();
    void Term();
    void TermPrime();
    void Factor();
    void Primary();

    // FOR ASSIGNMENT 3 TABLES
    void printSymbolTable(); 
    void printInstructionTable();
    
private: 
    Lexer& lexer_; //connects lexer 
    Token currentToken_; //stores current token being analyzed 
    std::ofstream& out_; // this gives the output file 
    bool printRules_; //allows for the printing of the grammar rules to be printed or not 

    // FOR ASSIGNMENT 3 TABLES
    SymbolTable symbolTable_;
    InstructionTable instructionTable_;
    std::string currentDeclarationType_; 

    void advance(); //moves to next token 
    void match(int expectedCategory, const std::string& expectedLexeme = ""); //checks if token matches 
    void printToken(); //prints token + lexeme 
    void printProduction(const std::string& rule); //prints grammar rule 
    void error(const std::string& message); //hanndles sytax errors

    bool isQualifier() const; //checks if token is int bool or real 
    bool isStatementStart() const; // check to begin statement
    bool isRelopToken() const; // check tokecn to RELOOP through statemtn
    bool isExpressionStart() const; // check to inialize an expression







};
#endif 