#pragma once

#include <string>
#include <vector>

#include "Tokens.h"
#include "IdFSM.h"
#include "IntFSM.h"
#include "RealFSM.h"

class Lexer {
public:
    Token lexer(); // CREATION lexer() function in requirements which will be called 

    explicit Lexer(const std::string& filename); // loads the lexer with file to memory

    bool isOpen() const; 

private:

    bool fileOpened_ = false;
    std::string source_;
    int index_ = 0;     // current character
    int line_ = 1;      // line number 

    IDFSM idFsm_; // our FSM's
    INTFSM intFsm_;
    rFSM realFsm_;

    std::vector<std::string> keywords_; // LIST FOR KEYWORDS

    void keywordload_();
    bool keyword_(const std::string& s) const;

    void emptyspaceSKIPPING_();
    bool commentstart_() const;
    void commentskip_();

    int realLength_(int startIndex);

    bool isLetter_(char c) const;
    bool isDigit_(char c) const;

    Token createToken_(int category, const std::string& lexeme);
};