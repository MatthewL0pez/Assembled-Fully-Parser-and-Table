#include "IntFSM.h"

INTFSM::INTFSM() {
    FSMstart();
}

void INTFSM::FSMstart() {
    state_ = Start_State;
}

bool INTFSM::isAnumber_(char c) const { //returns true for 0-9 or else false 
    if(c >= '0' && c <= '9') {
        return true;
    } else {
        return false;
    }
}


bool INTFSM::makeCheck(char c) {
if (state_ == State_End) //if at the end retruns false 
        return false;

    if (state_ == Start_State) {  //if first char is a number it comsumes if not it returns false  
        if (isAnumber_(c)) {
            state_ = Start_InInt;
            return true;
        }
        state_ = State_End; 
        return false;
    }

    if (state_ == Start_InInt) { //if we are already consuming digits 
        if (isAnumber_(c)) {     // we  keep going 
            return true;
        }
        state_ = State_End;    
        return false;
    }

    state_ = State_End; //if theres an unexpected value 
    return false;      // retuns false
}

bool INTFSM::validTokenAccept() const {
 return state_ == Start_InInt;    // token is valid if we read at least one digit
}

int INTFSM::startInteger(const std::string& s, int start) {
FSMstart();
int i = start;
int acceptedLength = 0;
while (i < (int)s.size()) { // scans until we reach the end of input 
    if(!makeCheck(s[i]))  //feeds current character to FSM
    break;


if(validTokenAccept()) //if charcter is in accepting state, records token length 
acceptedLength = (i - start) + 1;
i++;
}

return acceptedLength;
}



