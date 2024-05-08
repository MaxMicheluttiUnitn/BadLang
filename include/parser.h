#ifndef __PARSER__H__
#define __PARSER__H__
#include "lr1_automaton.h"
#include "tokenizer.h"
#include "errors.h"
#include <stack>
#include <iostream>
#include "ast.h"

class Parser;

class Parser{
    LR1_Automaton automaton;
    std::stack<LR1_State*> state_stack;
    std::stack<Token> symbol_stack;
    public:
    Parser(const Grammar&);
    ~Parser();
    AST* parse(const Grammar&,TokenizedData&);
};

#endif
