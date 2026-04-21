#pragma once

#include <iostream>
#include <string>
#include <cctype> // This allows me to use helper functions isdigit,isalpha.

class rFSM {
    public:

    rFSM();
    void rFSMstart();
    bool makeCheck(char c);
    bool validTokenAccept() const;
    bool startReal(const std::string & j);



    private:

    enum class State {
        Start,
        Integer,
        NoDecimalInt,
        Fraction,
        DecimalAfterInt,
        Exponent,
        ExponentSign,
        ExponentDigits
    };

    State state_ = State::Start;

    bool isDigit_(char c) const;
};