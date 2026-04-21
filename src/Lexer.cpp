#include "Lexer.h"

#include <fstream>
#include <iterator>

Lexer::Lexer(const std::string& filename) { //LOAD the file into the source_
    keywordload_();

    std::ifstream inFile(filename);
    if (!inFile.is_open()) {
        fileOpened_ = false;
        return;
    }

    fileOpened_ = true;
    source_.assign(std::istreambuf_iterator<char>(inFile),
                   std::istreambuf_iterator<char>());

    index_ = 0;
    line_ = 1;
}

// FILE CHEK INFORMATION section
bool Lexer::isOpen() const {
    return fileOpened_;
}
bool Lexer::isLetter_(char c) const{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}
bool Lexer::isDigit_(char c) const{
    return (c >= '0' && c <= '9');
}

void Lexer::keywordload_()
{
    // THE KEYWORDS for Rat
    keywords_.push_back("integer");
    keywords_.push_back("if");
    keywords_.push_back("otherwise");
    keywords_.push_back("fi");
    keywords_.push_back("while");
    keywords_.push_back("return");
    keywords_.push_back("read");
    keywords_.push_back("write");
    keywords_.push_back("function");
    keywords_.push_back("boolean");
    keywords_.push_back("real");
    keywords_.push_back("true");
    keywords_.push_back("false");
}

bool Lexer::keyword_(const std::string& s) const
{
    for (int i = 0; i < (int)keywords_.size(); i++) {
        if (keywords_[i] == s)
         return true;
    }
    return false;
}

// COMMENT FOCUS --- 
bool Lexer::commentstart_() const {// If it starts with a comment 

    if (index_ + 1 >= (int)source_.size())
        return false;

    return (source_[index_] == '/' && source_[index_ + 1] == '*');
}

void Lexer::commentskip_() {

    index_ += 2; // positioned at / of /*
    while (index_ < (int)source_.size()) {
        if (source_[index_] == '\n')
            line_++;

        if (index_ + 1 < (int)source_.size() &&  // end comment */
            source_[index_] == '*' && source_[index_ + 1] == '/') {
            index_ += 2;
            return;
        }
        index_++;
    }
}

int Lexer::realLength_(int startIndex) 
{
    realFsm_.rFSMstart();

    int i = startIndex;
    int acceptedlength = 0;

    bool dot = false;
    bool Intwithdot = false;

    while (i < (int)source_.size()) {
        char c = source_[i];

        if (!realFsm_.makeCheck(c))
            break;

        if (c == '.')
            dot = true;

        if (dot && isDigit_(c))
            Intwithdot = true;

        if (realFsm_.validTokenAccept() && dot && Intwithdot) {
            acceptedlength = (i - startIndex) + 1;
        }

        i++;
    }

    return acceptedlength;
}

void Lexer::emptyspaceSKIPPING_() { // for the whitespaces and comments 
    while (index_ < (int)source_.size()) {
        char c = source_[index_];

        if (c == ' ' || c == '\t' || c == '\r') { // whitespace
            index_++;
            continue;
        }
        if (c == '\n') { // newlines
            line_++;
            index_++;
            continue;
        }
        if (commentstart_()) { // comments
            commentskip_();
            continue;
        }
        break;
    }
}

Token Lexer::createToken_(int category, const std::string& lexeme) // MAKE TOKEN
{
    Token t;

    t.tokenCategory = category;
    t.lexeme = lexeme;
    t.numberLine = line_;

    return t;
}

Token Lexer::lexer() { // MAIN LEXER function = gets one token per time 
    
    emptyspaceSKIPPING_();

    if (index_ >= (int)source_.size()) {
        return createToken_(T_FileEnd, "EOF"); // EOFF
    }

    char c = source_[index_];

    if (isLetter_(c)) { // Identifer, Keyword
        int len = idFsm_.startIdentifier(source_, index_); 
        std::string lex = source_.substr(index_, len);
        index_ += len;

        if (keyword_(lex))
            return createToken_(T_Keyword, lex);

        return createToken_(T_Identifier, lex);
    }
    
    if (isDigit_(c)) { // Interger, Real
        int intLen = intFsm_.startInteger(source_, index_);

        int realLen = 0;
        if (intLen > 0 && (index_ + intLen) < (int)source_.size() && // only if . after integer section 
            source_[index_ + intLen] == '.') {
            realLen = realLength_(index_);
        }

        if (realLen > 0) {
            std::string lex = source_.substr(index_, realLen);
            index_ += realLen;
            return createToken_(T_Real, lex);
        }

        std::string lex = source_.substr(index_, intLen);
        index_ += intLen;
        return createToken_(T_Integer, lex);
    }


    if (index_ + 1 < (int)source_.size()) { // operators with 2 characters (Comparative opperations)
        std::string two = source_.substr(index_, 2);

        if (two == "==" || two == "!=" || two == "<=" || two == "=>") {
            index_ += 2;
            return createToken_(T_Operator, two);
        }
    }
    
    if (c == '>' || c == '<' || c == '=' || c == '+' || c == '-' || c == '*' || c == '/') { // one character operations
        index_++;
        return createToken_(T_Operator, std::string(1, c));
    }
    
    if (c == '@' || c == '(' || c == ')' || c == '{' || c == '}' || c == ';' || c == ',') { // seperators list 
        index_++;
        return createToken_(T_Seperator, std::string(1, c));
    }

    index_++;
    return createToken_(T_UnknownTokenType, std::string(1, c)); // unknown token type 
}