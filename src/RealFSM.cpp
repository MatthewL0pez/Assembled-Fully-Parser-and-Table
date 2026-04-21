#include "RealFSM.h"

rFSM::rFSM(){
    rFSMstart();
}

void rFSM::rFSMstart(){
    state_ = State::Start;
}

bool rFSM::isDigit_(char c) const{ // checks stream for valid digits
    if (c >= '0' && c <= '9')
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool rFSM::makeCheck(char c){
    switch (state_) // swithces case (state) depedning on the next character 'c'
    {
    case State::Start:
        if (isDigit_(c)){ // checks if c, is a digit -> state = Integer
            state_ = State::Integer;
            return true;
        }
        else if (c == '.'){
            state_ = State::NoDecimalInt; // checks if no digit infront of decimal
            return true;
        }
        else{
            return false;
        }

    case State::Integer:
        if (isDigit_(c)){
            state_ = State::Integer; //checks if followed by another integer, if yes stays in integer state
            return true;
        }
        else if (c == '.'){
            state_ = State::DecimalAfterInt; // if decimal then changes case to decimal and keeps checking the next character
            return true;
        }
        else if (c == 'e' || c == 'E'){
            state_ = State::Exponent;
            return true;
        }
        else{
            return false;
        }

    case State::NoDecimalInt:
        if (isDigit_(c)){
            state_ = State::Fraction;
            return true;
        }
        else{
            return false;
        }

    case State::DecimalAfterInt:
        if (isDigit_(c)){
            state_ = State::Fraction;
            return true;
        }
        else{
            return false;
        }

    case State::Fraction:
        if (isDigit_(c)){
            state_ = State::Fraction;
            return true;
        }
        else if (c == 'e' || c == 'E'){
            state_ = State::Exponent;
            return true;
        }
        else{
            return false;
        }

    case State::Exponent:
        if (c == '+' || c == '-'){
            state_ = State::ExponentSign;
            return true;
        }
        else if (isDigit_(c)){
            state_ = State::ExponentDigits;
            return true;
        }
        else{
            return false;
        }

    case State::ExponentSign:
        if (isDigit_(c)){
            state_ = State::ExponentDigits;
            return true;
        }
        else{
            return false;
        }

    case State::ExponentDigits:
        if (isDigit_(c)){
            state_ = State::ExponentDigits;
            return true;
        }
        else{
            return false;
        }
    default:
        return false;
    }
}

bool rFSM::validTokenAccept() const // defines the acceptable states
{
    if (state_ == State::Integer || state_ == State::Fraction || state_ == State::ExponentDigits){
        return true;
    }
    else{
        return false;
    }
}

bool rFSM::startReal(const std::string & j){ // driver, resets for every instance and determines if acceptable tokens
    rFSMstart();
    for(char c : j){
        if(!makeCheck(c)){
            return false;
        }
    }
    return validTokenAccept();
}
