#ifndef __AST_H__
#define __AST_H__
#include "token.h"
#include <vector>
#include <iostream>

class AST{
    Token item;
    std::vector<AST*> children;
    void print_depth(std::ostream&,int)const;
    public:
    AST();
    ~AST();
    AST(Token,std::vector<AST*>);
    AST(Token);
    Token get_token()const;
    TokenType get_type()const;
    std::vector<AST*> get_children()const;
    void print(std::ostream&)const;
};

#endif
