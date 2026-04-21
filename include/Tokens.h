#pragma once       // prevents header being used more then once per .cpp file
#include <string> 

const int T_Identifier = 1;   // These are the token idetification codes with int assignment
const int T_Keyword = 2;
const int T_Integer = 3;
const int T_Real = 4;
const int T_Operator = 5;
const int T_Seperator = 6;
const int T_UnknownTokenType = 7;
const int T_FileEnd = 8;

struct Token {  // Lexeme's return 
    int tokenCategory = T_UnknownTokenType;
    std::string lexeme;   // Text from input (Token actual value)
    int numberLine = 1;    
};

inline std::string tokenName(int a) {   // takes the token identification into the printed name 
  if (a == T_Identifier) return "Identifier";
  if (a == T_Keyword) return "Keyword";
  if (a == T_Integer) return "Integer";
  if (a == T_Real) return "Real";
  if (a == T_Operator) return "Operator";
  if (a == T_Seperator) return "Separator";
  if (a == T_FileEnd) return "End of file";
  return "Unknown";
}
