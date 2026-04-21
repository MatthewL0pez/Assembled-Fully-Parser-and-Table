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
            currentToken_.lexeme == "boolean" ||
            currentToken_.lexeme == "real");
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
           currentToken_.tokenCategory == T_Real ||
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
    printProduction("<Opt Function Definitions> -> <Function Definitions> | <Empty>");

    if (currentToken_.tokenCategory == T_Keyword &&
        currentToken_.lexeme == "function") {
        FunctionDefinitions();
    } else {
        Empty();
    }
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
    printProduction("<Qualifier> -> integer | boolean | real");

    if (currentToken_.tokenCategory == T_Keyword &&
        currentToken_.lexeme == "integer") {
        match(T_Keyword, "integer");
    } else if (currentToken_.tokenCategory == T_Keyword &&
               currentToken_.lexeme == "boolean") {
        match(T_Keyword, "boolean");
    } else if (currentToken_.tokenCategory == T_Keyword &&
               currentToken_.lexeme == "real") {
        match(T_Keyword, "real");
    } else {
        error("expected qualifier (integer, boolean, or real)");
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
}

// R13
void Parser::IDs() {
    printProduction("<IDs> -> <Identifier> , <IDs> | <Identifier>");

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
void Parser::Assign(){
    printProduction("<Assign> -> <Identifier> = <Expression> ;");
    match(T_Identifier);
    match(T_Operator, "=");
    Expression();
    match(T_Seperator, ";");
};

//R18 If
void Parser::If() {
    printProduction("<If> -> if ( <Condition> ) <Statement> fi | if ( <Condition> ) <Statement> otherwise <Statement> fi");

    match(T_Keyword, "if");
    match(T_Seperator, "(");
    Condition();
    match(T_Seperator, ")");
    Statement();

    if (currentToken_.tokenCategory == T_Keyword &&
        currentToken_.lexeme == "otherwise") {
        match(T_Keyword, "otherwise");
        Statement();
    }

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
    match(T_Seperator, ")");
    match(T_Seperator, ";");
}

//R21
void Parser::Scan() {
    printProduction("<Scan> -> read ( <IDs> );");

    match(T_Keyword, "read");
    match(T_Seperator, "(");
    IDs();
    match(T_Seperator, ")");
    match(T_Seperator, ";");
}

//R22
void Parser::While() {
    printProduction("<While> -> while ( <Condition> ) <Statement>");

    match(T_Keyword, "while");
    match(T_Seperator, "(");
    Condition();
    match(T_Seperator, ")");
    Statement();
}

// R23
void Parser::Condition() {
    printProduction("<Condition> -> <Expression> <Relop> <Expression>");

    Expression();
    Relop();
    Expression();
}

// R24 
void Parser::Relop() {
    printProduction("<Relop> -> == | != | > | < | <= | =>");

    if (isRelopToken()) {
        std::string relopLexeme = currentToken_.lexeme;
        match(T_Operator, relopLexeme);
    } else {
        error("expected relational operator");
    }
}

// R25  (rewritten to remove left recursion)
void Parser::Expression() {
    printProduction("<Expression> -> <Term> <Expression Prime>");

    Term();
    ExpressionPrime();
}

void Parser::ExpressionPrime() {
    printProduction("<Expression Prime> -> + <Term> <Expression Prime> | - <Term> <Expression Prime> | ε");

    if (currentToken_.tokenCategory == T_Operator &&
        currentToken_.lexeme == "+") {
        match(T_Operator, "+");
        Term();
        ExpressionPrime();
    }
    else if (currentToken_.tokenCategory == T_Operator &&
             currentToken_.lexeme == "-") {
        match(T_Operator, "-");
        Term();
        ExpressionPrime();
    }
}

// R26 (rewritten to remove left recursion)
void Parser::Term() {
    printProduction("<Term> -> <Factor> <Term Prime>");

    Factor();
    TermPrime();
}

void Parser::TermPrime() {
    printProduction("<Term Prime> -> * <Factor> <Term Prime> | / <Factor> <Term Prime> | ε");

    if (currentToken_.tokenCategory == T_Operator &&
        currentToken_.lexeme == "*") {
        match(T_Operator, "*");
        Factor();
        TermPrime();
    }
    else if (currentToken_.tokenCategory == T_Operator &&
             currentToken_.lexeme == "/") {
        match(T_Operator, "/");
        Factor();
        TermPrime();
    }
}

// R27
void Parser::Factor() {
    printProduction("<Factor> -> - <Primary> | <Primary>");

    if (currentToken_.tokenCategory == T_Operator &&
        currentToken_.lexeme == "-") {
        match(T_Operator, "-");
        Primary();
    } else {
        Primary();
    }
}

// R28
void Parser::Primary() {
    printProduction("<Primary> -> <Identifier> | <Integer> | <Identifier> ( <IDs> ) | ( <Expression> ) | <Real> | true | false");

    if (currentToken_.tokenCategory == T_Identifier) {
        match(T_Identifier);

        if (currentToken_.tokenCategory == T_Seperator &&
            currentToken_.lexeme == "(") {
            match(T_Seperator, "(");
            IDs();
            match(T_Seperator, ")");
        }
    }
    else if (currentToken_.tokenCategory == T_Integer) {
        match(T_Integer);
    }
    else if (currentToken_.tokenCategory == T_Real) {
        match(T_Real);
    }
    else if (currentToken_.tokenCategory == T_Seperator &&
             currentToken_.lexeme == "(") {
        match(T_Seperator, "(");
        Expression();
        match(T_Seperator, ")");
    }
    else if (currentToken_.tokenCategory == T_Keyword &&
             currentToken_.lexeme == "true") {
        match(T_Keyword, "true");
    }
    else if (currentToken_.tokenCategory == T_Keyword &&
             currentToken_.lexeme == "false") {
        match(T_Keyword, "false");
    }
    else {
        error("expected primary");
    }
}


