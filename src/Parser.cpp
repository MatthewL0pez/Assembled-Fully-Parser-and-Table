#include "Parser.h"
#include <cstdlib>
#include <iostream>

Parser::Parser(Lexer& lexer, std::ofstream& outFile, bool printRules)
    : lexer_(lexer), out_(outFile), printRules_(printRules) {
    advance();
}

void Parser::advance() {
    currentToken_ = lexer_.lexer();
    printToken();
}

void Parser::printToken() {
    out_ << "Token: " << tokenName(currentToken_.tokenCategory)
         << " Lexeme: " << currentToken_.lexeme << '\n';
}

void Parser::printProduction(const std::string& rule) {
    if (printRules_) {
        out_ << rule << '\n';
    }
}

void Parser::match(int expectedCategory, const std::string& expectedLexeme) {
    if (currentToken_.tokenCategory != expectedCategory) {
        error("unexpected token category");
    }

    if (!expectedLexeme.empty() && currentToken_.lexeme != expectedLexeme) {
        error("expected lexeme '" + expectedLexeme + "'");
    }

    advance();
}

void Parser::error(const std::string& message) {
    out_ << "Syntax Error at line " << currentToken_.numberLine
         << ": " << message
         << " | Found token: " << tokenName(currentToken_.tokenCategory)
         << " | Lexeme: " << currentToken_.lexeme << '\n';

    std::cerr << "Syntax Error at line " << currentToken_.numberLine
              << ": " << message
              << " | Found token: " << tokenName(currentToken_.tokenCategory)
              << " | Lexeme: " << currentToken_.lexeme << std::endl;

    std::exit(1);
}

bool Parser::isQualifier() const {
    return currentToken_.tokenCategory == T_Keyword &&
           (currentToken_.lexeme == "integer" ||
            currentToken_.lexeme == "boolean");
}

// PERSON 3 - PARSER STATE NEEDS 
bool Parser::isStatementStart() const {
    return currentToken_.tokenCategory == T_Identifier ||
           currentToken_.lexeme == "if" ||
           currentToken_.lexeme == "return" ||
           currentToken_.lexeme == "write" ||
           currentToken_.lexeme == "read" ||
           currentToken_.lexeme == "while" ||
           currentToken_.lexeme == "{";
}

bool Parser::isRelopToken() const {
    return currentToken_.tokenCategory == T_Operator &&
           (currentToken_.lexeme == "==" ||
            currentToken_.lexeme == "!=" ||
            currentToken_.lexeme == ">"  ||
            currentToken_.lexeme == "<"  ||
            currentToken_.lexeme == "<=" ||
            currentToken_.lexeme == "=>");
}

bool Parser::isExpressionStart() const {
    return currentToken_.tokenCategory == T_Identifier ||
           currentToken_.tokenCategory == T_Integer ||
           currentToken_.lexeme == "(" ||
           currentToken_.lexeme == "-" ||
           currentToken_.lexeme == "true" ||
           currentToken_.lexeme == "false";
}

// R1
void Parser::Rat26S() {
    printProduction("<Rat26S> -> @ <Opt Function Definitions> @ <Opt Declaration List> @ <Statement List> @");

    match(T_Seperator, "@");
    OptFunctionDefinitions();
    match(T_Seperator, "@");
    OptDeclarationList();
    match(T_Seperator, "@");
    StatementList();
    match(T_Seperator, "@");

    if (currentToken_.tokenCategory != T_FileEnd) {
        error("expected end of file after final @");
    }
}

// R2
void Parser::OptFunctionDefinitions() {
    printProduction("<Opt Function Definitions> -> <Empty>");
    Empty();
}

// R3
void Parser::FunctionDefinitions() {
    printProduction("<Function Definitions> -> <Function> <Function Definitions> | <Function>");

    Function();

    if (currentToken_.tokenCategory == T_Keyword &&
        currentToken_.lexeme == "function") {
        FunctionDefinitions();
    }
}

// R4
void Parser::Function() {
    printProduction("<Function> -> function <Identifier> ( <Opt Parameter List> ) <Opt Declaration List> <Body>");

    match(T_Keyword, "function");
    match(T_Identifier);
    match(T_Seperator, "(");
    OptParameterList();
    match(T_Seperator, ")");
    OptDeclarationList();
    Body();
}

// R5
void Parser::OptParameterList() {
    printProduction("<Opt Parameter List> -> <Parameter List> | <Empty>");

    if (currentToken_.tokenCategory == T_Identifier) {
        ParameterList();
    } else {
        Empty();
    }
}

// R6
void Parser::ParameterList() {
    printProduction("<Parameter List> -> <Parameter> , <Parameter List> | <Parameter>");

    Parameter();

    if (currentToken_.tokenCategory == T_Seperator &&
        currentToken_.lexeme == ",") {
        match(T_Seperator, ",");
        ParameterList();
    }
}

// R7
void Parser::Parameter() {
    printProduction("<Parameter> -> <IDs> <Qualifier>");
    IDs();
    Qualifier();
}

// R8
void Parser::Qualifier() {
    printProduction("<Qualifier> -> integer | boolean");

    if (currentToken_.tokenCategory == T_Keyword &&
        currentToken_.lexeme == "integer") {
        currentDeclarationType_ = "integer";
        match(T_Keyword, "integer");
    }
    else if (currentToken_.tokenCategory == T_Keyword &&
             currentToken_.lexeme == "boolean") {
        currentDeclarationType_ = "boolean";
        match(T_Keyword, "boolean");
    }
    else {
        error("expected qualifier (integer or boolean)");
    }
}

// R9
void Parser::Body() {
    printProduction("<Body> -> { <Statement List> }");

    match(T_Seperator, "{");
    StatementList();
    match(T_Seperator, "}");
}

// R10
void Parser::OptDeclarationList() {
    printProduction("<Opt Declaration List> -> <Declaration List> | <Empty>");

    if (isQualifier()) {
        DeclarationList();
    } else {
        Empty();
    }
}

// R11
void Parser::DeclarationList() {
    printProduction("<Declaration List> -> <Declaration> ; <Declaration List> | <Declaration> ;");

    Declaration();
    match(T_Seperator, ";");

    if (isQualifier()) {
        DeclarationList();
    }
}

// R12
void Parser::Declaration() {
    printProduction("<Declaration> -> <Qualifier> <IDs>");

    Qualifier();
    IDs();

    // done with this declaration
    currentDeclarationType_.clear();
}

// R13
void Parser::IDs() {
    printProduction("<IDs> -> <Identifier> , <IDs> | <Identifier>");

    if (currentToken_.tokenCategory != T_Identifier) {
        error("expected identifier");
    }

    std::string idName = currentToken_.lexeme;

    // only add identifiers during declarations
    if (!currentDeclarationType_.empty()) {
        if (!symbolTable_.add(idName, currentDeclarationType_)) {
            error("duplicate declaration for identifier '" + idName + "'");
        }
    }

    match(T_Identifier);

    if (currentToken_.tokenCategory == T_Seperator &&
        currentToken_.lexeme == ",") {
        match(T_Seperator, ",");
        IDs();
    }
}

// R29
void Parser::Empty() {
    printProduction("<Empty> -> ε");
}

//R14 Statement List
void Parser::StatementList() {
    printProduction("<Statement List> -> <Statement> <Statement List> | <Statement>");

    Statement();

    if (isStatementStart()) {
        StatementList();
    }
}

//R15 Statement
void Parser::Statement() {
    printProduction("<Statement> -> <Compound> | <Assign> | <If> | <Return> | <Print> | <Scan> | <While>");

    if (currentToken_.tokenCategory == T_Identifier) {
        Assign();
    }
    else if (currentToken_.lexeme == "if") {
        If();
    }
    else if (currentToken_.lexeme == "while") {
        While();
    }
    else if (currentToken_.lexeme == "return") {
        Return();
    }
    else if (currentToken_.lexeme == "write") {
        Print();
    }
    else if (currentToken_.lexeme == "read") {
        Scan();
    }
    else if (currentToken_.lexeme == "{") {
        Compound();
    }
    else {
        error("expected start of statement");
    }
}

//R16 Bracket 

void Parser::Compound(){
    printProduction("<Compound> -> { <Statement List> }");

    match(T_Seperator, "{");
    StatementList();
    match(T_Seperator, "}");

}

//R17 Assign
void Parser::Assign() {
    printProduction("<Assign> -> <Identifier> = <Expression> ;");

    std::string idName = currentToken_.lexeme;

    if (!symbolTable_.exists(idName)) {
        error("identifier '" + idName + "' used before declaration");
    }

    std::string idType = symbolTable_.getIDType(idName);
    int address = symbolTable_.getMemAddress(idName);

    match(T_Identifier);
    match(T_Operator, "=");

    ExpressionResult expr = Expression();

    if (idType != expr.type) {
        error("type mismatch in assignment to '" + idName + "'");
    }

    instructionTable_.generateInstruction("POPM", std::to_string(address));

    match(T_Seperator, ";");
}

//R18 If
void Parser::If() {
    printProduction("<If> -> if ( <Condition> ) <Statement> fi | if ( <Condition> ) <Statement> otherwise <Statement> fi");

    match(T_Keyword, "if");
    match(T_Seperator, "(");
    Condition();
    match(T_Seperator, ")");

    int jmpzAddr = instructionTable_.getCurrentAddress();
    instructionTable_.generateInstruction("JMPZ", "");
    instructionTable_.pushJumpAddress(jmpzAddr);

    Statement();

    if (currentToken_.tokenCategory == T_Keyword &&
        currentToken_.lexeme == "otherwise") {

        int jmpAddr = instructionTable_.getCurrentAddress();
        instructionTable_.generateInstruction("JMP", "");

        instructionTable_.backPatch(instructionTable_.getCurrentAddress());
        instructionTable_.generateInstruction("LABEL", "");

        instructionTable_.pushJumpAddress(jmpAddr);

        match(T_Keyword, "otherwise");
        Statement();
    }

    instructionTable_.backPatch(instructionTable_.getCurrentAddress());
    instructionTable_.generateInstruction("LABEL", "");

    match(T_Keyword, "fi");
}

//R19 Return
void Parser::Return() {
    printProduction("<Return> -> return ; | return <Expression> ;");

    match(T_Keyword, "return");

    if (!(currentToken_.tokenCategory == T_Seperator &&
          currentToken_.lexeme == ";")) {
        Expression();
    }

    match(T_Seperator, ";");
}

//R20
void Parser::Print() {
    printProduction("<Print> -> write ( <Expression> );");

    match(T_Keyword, "write");
    match(T_Seperator, "(");
    Expression();
    instructionTable_.generateInstruction("SOUT", "");
    match(T_Seperator, ")");
    match(T_Seperator, ";");
}

//R21
void Parser::Scan() {
    printProduction("<Scan> -> read ( <IDs> );");

    match(T_Keyword, "read");
    match(T_Seperator, "(");

    do {
        if (currentToken_.tokenCategory != T_Identifier) {
            error("expected identifier in read statement");
        }
        std::string idName = currentToken_.lexeme;
        if (!symbolTable_.exists(idName)) {
            error("identifier '" + idName + "' used before declaration");
        }
        int address = symbolTable_.getMemAddress(idName);
        instructionTable_.generateInstruction("SIN", "");
        instructionTable_.generateInstruction("POPM", std::to_string(address));
        match(T_Identifier);
    } while (currentToken_.tokenCategory == T_Seperator &&
             currentToken_.lexeme == "," &&
             (match(T_Seperator, ","), true));

    match(T_Seperator, ")");
    match(T_Seperator, ";");
}

//R22
void Parser::While() {
    printProduction("<While> -> while ( <Condition> ) <Statement>");

    match(T_Keyword, "while");

    int topAddr = instructionTable_.getCurrentAddress();
    instructionTable_.generateInstruction("LABEL", "");

    match(T_Seperator, "(");
    Condition();
    match(T_Seperator, ")");

    int jmpzAddr = instructionTable_.getCurrentAddress();
    instructionTable_.generateInstruction("JMPZ", "");
    instructionTable_.pushJumpAddress(jmpzAddr);

    Statement();

    instructionTable_.generateInstruction("JMP", std::to_string(topAddr));
    instructionTable_.backPatch(instructionTable_.getCurrentAddress());
    instructionTable_.generateInstruction("LABEL", "");
}

// R23
void Parser::Condition() {
    printProduction("<Condition> -> <Expression> <Relop> <Expression>");

    Expression();
    std::string op = Relop();
    Expression();

    if      (op == ">")  instructionTable_.generateInstruction("GRT", "");
    else if (op == "<")  instructionTable_.generateInstruction("LES", "");
    else if (op == "==") instructionTable_.generateInstruction("EQU", "");
    else if (op == "!=") instructionTable_.generateInstruction("NEQ", "");
    else if (op == "=>") instructionTable_.generateInstruction("GEQ", "");
    else if (op == "<=") instructionTable_.generateInstruction("LEQ", "");
}

// R24
// void Parser::Relop() {
//     printProduction("<Relop> -> == | != | > | < | <= | =>");
//
//     if (isRelopToken()) {
//         std::string relopLexeme = currentToken_.lexeme;
//         match(T_Operator, relopLexeme);
//     } else {
//         error("expected relational operator");
//     }
// }

// R25  (rewritten to remove left recursion)
ExpressionResult Parser::Expression() {
    printProduction("<Expression> -> <Term> <Expression Prime>");

    ExpressionResult left = Term();
    return ExpressionPrime(left);
}


ExpressionResult Parser::ExpressionPrime(ExpressionResult left) {
    printProduction("<Expression Prime> -> + <Term> <Expression Prime> | - <Term> <Expression Prime> | ε");

    if (currentToken_.tokenCategory == T_Operator &&
        currentToken_.lexeme == "+") {

        match(T_Operator, "+");
        ExpressionResult right = Term();

        if (left.type != "integer" || right.type != "integer") {
            error("booleans cannot be used in arithmetic");
        }

        instructionTable_.generateInstruction("A", "");
        left.type = "integer";

        return ExpressionPrime(left);
    }
    else if (currentToken_.tokenCategory == T_Operator &&
             currentToken_.lexeme == "-") {

        match(T_Operator, "-");
        ExpressionResult right = Term();

        if (left.type != "integer" || right.type != "integer") {
            error("booleans cannot be used in arithmetic");
        }

        instructionTable_.generateInstruction("S", "");
        left.type = "integer";

        return ExpressionPrime(left);
    }

    return left;
}


// R26 (rewritten to remove left recursion)
ExpressionResult Parser::Term() {
    printProduction("<Term> -> <Factor> <Term Prime>");

    ExpressionResult left = Factor();
    return TermPrime(left);
}

ExpressionResult Parser::TermPrime(ExpressionResult left) {
    printProduction("<Term Prime> -> * <Factor> <Term Prime> | / <Factor> <Term Prime> | ε");

    if (currentToken_.tokenCategory == T_Operator &&
        currentToken_.lexeme == "*") {

        match(T_Operator, "*");
        ExpressionResult right = Factor();

        if (left.type != "integer" || right.type != "integer") {
            error("booleans cannot be used in arithmetic");
        }

        instructionTable_.generateInstruction("M", "");
        left.type = "integer";

        return TermPrime(left);
    }
    else if (currentToken_.tokenCategory == T_Operator &&
             currentToken_.lexeme == "/") {

        match(T_Operator, "/");
        ExpressionResult right = Factor();

        if (left.type != "integer" || right.type != "integer") {
            error("booleans cannot be used in arithmetic");
        }

        instructionTable_.generateInstruction("D", "");
        left.type = "integer";

        return TermPrime(left);
    }

    return left;
}

// R27
ExpressionResult Parser::Factor() {
    printProduction("<Factor> -> - <Primary> | <Primary>");

    if (currentToken_.tokenCategory == T_Operator &&
        currentToken_.lexeme == "-") {

        match(T_Operator, "-");

        ExpressionResult result = Primary();

        if (result.type != "integer") {
            error("negative sign can only be used with integers");
        }

        instructionTable_.generateInstruction("PUSHI", "-1");
        instructionTable_.generateInstruction("M", "");

        return result;
    }

    return Primary();
}

// R28
ExpressionResult Parser::Primary() {
    printProduction("<Primary> -> <Identifier> | <Integer> | ( <Expression> ) | true | false");

    ExpressionResult result;

    if (currentToken_.tokenCategory == T_Identifier) {
        std::string idName = currentToken_.lexeme;

        if (!symbolTable_.exists(idName)) {
            error("identifier '" + idName + "' used before declaration");
        }

        result.type = symbolTable_.getIDType(idName);

        int address = symbolTable_.getMemAddress(idName);
        instructionTable_.generateInstruction("PUSHM", std::to_string(address));

        match(T_Identifier);
        return result;
    }
    else if (currentToken_.tokenCategory == T_Integer) {
        result.type = "integer";

        instructionTable_.generateInstruction("PUSHI", currentToken_.lexeme);

        match(T_Integer);
        return result;
    }
    else if (currentToken_.tokenCategory == T_Seperator &&
             currentToken_.lexeme == "(") {

        match(T_Seperator, "(");
        result = Expression();
        match(T_Seperator, ")");

        return result;
    }
    else if (currentToken_.tokenCategory == T_Keyword &&
             currentToken_.lexeme == "true") {

        result.type = "boolean";
        instructionTable_.generateInstruction("PUSHI", "1");

        match(T_Keyword, "true");
        return result;
    }
    else if (currentToken_.tokenCategory == T_Keyword &&
             currentToken_.lexeme == "false") {

        result.type = "boolean";
        instructionTable_.generateInstruction("PUSHI", "0");

        match(T_Keyword, "false");
        return result;
    }

    error("expected primary");
    return result;
}

// ASSIGNMENT 3 
void Parser::printSymbolTable() {
    symbolTable_.print(out_);
}

void Parser::printInstructionTable() {
    instructionTable_.print(out_);
}

std::string Parser::Relop() {
    printProduction("<Relop> -> == | != | > | < | <= | =>");

    if (isRelopToken()) {
        std::string op = currentToken_.lexeme;
        match(T_Operator, op);
        return op;

    }

    error("Expected Relational Operator");
    return "";
}

