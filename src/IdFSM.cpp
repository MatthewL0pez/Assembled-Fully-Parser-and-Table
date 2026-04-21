#include "IdFSM.h"

IDFSM::IDFSM() {    // STARTS FSM construction
    FSMstart();
}

void IDFSM::FSMstart() 
{    // FSMstart = goes to start of the FSM beginning state
    state_ = State_Start;
}

bool IDFSM::isAletter_(char c) const 
{  //isAletter_ = Checks text if its a letter
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'); // from a -> z
}

bool IDFSM::isAnumber_(char c) const 
{ //isAnumber_ = checks text if its a number 
    return (c >= '0' && c <= '9'); //from 0 -> 9
}

bool IDFSM::makeCheck(char c) 
{ // Takes ONE character from the text into the FSM and checks
    if (state_ == State_End)    //  to see if we keep going till fase to end the current state
        return false;

    if (state_ == State_Start) {    // Starts ONLY if the first character is a letter
        if (isAletter_(c)) {
            state_ = State_Identified;
            return true;
        }
        state_ = State_End;
        return false;
    }

    if (state_ == State_Identified) {   // Can accept letters, digits, and underscores in the running state
        if (isAletter_(c) || isAnumber_(c) || c == '_') {
            return true;
        }
        state_ = State_End;
        return false;
    }
    state_ = State_End;
    return false;
}

bool IDFSM::validTokenAccept() const 
{  // TRUE if were in an accepting state of opperation
    return state_ == State_Identified;
}

int IDFSM::startIdentifier(const std::string& s, int start) 
{ // Returns the amount of characters that create a correct valid idenifier beginning at {int start}
    FSMstart();

    int i = start;
    int acceptedLength = 0;

    while (i < (int)s.size()) // Tracking the longest length where FSM is in a still acceptible state
    {
        if (!makeCheck(s[i])) // take character -> break out if FSM exits
            break;

        if (validTokenAccept()) // If were in accepting state -> reassign the accepted length value 
            acceptedLength = (i - start) + 1;

        i++;
    }

    return acceptedLength; 
}
