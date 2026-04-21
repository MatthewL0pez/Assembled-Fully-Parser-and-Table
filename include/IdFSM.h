#pragma once            // prevents header being used more then once per .cpp file
#include <string> 

class IDFSM {

public: 
    IDFSM();    // below are the FSM's controls for implementation

    void FSMstart();
    bool makeCheck(char c);
    bool validTokenAccept() const;
    int startIdentifier(const std::string& s, int start); // looks for the start position


    
private:
    static const int State_Start = 0;       // IDFSM states of opperation indicated by integer assign
    static const int State_Identified = 1;
    static const int State_End  = -1;
    int state_ = State_Start;

    bool isAletter_(char c) const;      // chekcs if a letter true or false
    bool isAnumber_(char c) const;      // checks if a number true or false
};