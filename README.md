Assignment 3 structure:
We need to add a symbol table for handling and assembly code generation to our Assignment 2 parser.

Rat26S version:
- no function definitions
- no real type
- booleans cannot be used in arithmetic
- variables must be declared before use
- duplicate declarations should give an error

Files we will mostly work in:
- include/Parser.h
- src/Parser.cpp
- main.cpp if needed for change in output 

New files for Assignment 3:
- include/SymbolTable.h
- src/SymbolTable.cpp
- include/InstructionTable.h
- src/InstructionTable.cpp
- include/ExpressionResult.h

PERSON 1

Files to work in:
- include/Parser.h
- src/Parser.cpp
- include/SymbolTable.h
- src/SymbolTable.cpp
- include/InstructionTable.h
- src/InstructionTable.cpp

What to do... 
- Set up the main Assignment 3 structure
- Change parser to simplified Rat26S
- Remove function definition handling for this assignment
- Remove real type handling
- Create symbol table
- Add insert, lookup, get address, get type functions
- Catch undeclared variables
- Catch duplicate declarations
- Create instruction table
- Add generate instruction function
- Add back patch / jump stack helpers
- I NEED TO FINISH FIRST FOR OTHER PEOPLE TO CONT..

PERSON 2

Files to work in...
- include/Parser.h
- src/Parser.cpp
- include/ExpressionResult.h
- include/InstructionTable.h
- src/InstructionTable.cpp

What to do...
- Work on expression code generation
- Handle integer literals, identifiers, true, false, and parentheses correctly
- Add arithmetic instruction generation for + - * /
- Add type checking for expressions
- Make sure booleans are not allowed in arithmetic
- Work on assignment statement code generation
- Make sure assignment types match
- WORK AFTER PERSON 1 COMPLETE

PERSON 3

Files to work in...
- include/Parser.h
- src/Parser.cpp
- include/InstructionTable.h
- src/InstructionTable.cpp
- src/main.cpp if needed for output change 

What to do...
- Work on statement level code generation
- Add condition handling and relational operators
- Add if statement generation
- Add otherwise branch handling for if
- Add while loop generation
- Add compound statement handling
- Add read statement generation
- Add write statement generation
- Help connect all statement parsing together
- WORK AFTER PERSON 2 COMPLETE

*** FOR TESTING ***
- test under 10 lines
- test under 21 lines
- test over 21 lines
- Make sure the symbol table and instruction table get printed 

Note for assignment 
- Only use the instructions given in Assignment 3 to implement
- Print both the instruction table and symbol table in the final output
- Make sure otherwise is handled and not just a plain if
- Make sure type mismatches give errors
- Make sure undeclared identifiers give errors

order:
1. Person 1 first
2. Person 2 next
3. Person 3 next
4. Testing and documentation and submission