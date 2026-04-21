#pragma  once
#include <string>

class INTFSM {

public:
INTFSM();
void FSMstart();
bool makeCheck(char c);
bool validTokenAccept() const;
int startInteger(const std::string& s, int start);

private:
static const int Start_State = 0;
static const int State_End = -1;
static const int Start_InInt = 1;
int state_ = Start_State;
bool isAnumber_(char c) const;

};