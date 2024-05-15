#ifndef __AST_H__
#define __AST_H__
#include "token.h"
#include <vector>
#include <iostream>
#include <string>
#include <set>
#include <vector>
#include "reduction_kind.h"
#include "token.h"
#include "errors.h"

class AST{
    Token item;
    ReductionKind reduction_kind;
    std::vector<AST*> children;
    void print_depth(std::ostream&,int)const;
    std::string compile_rec(std::set<std::string>&, bool lvalue);
    std::string get_bss(const std::set<std::string>&);
    std::string get_text();
    std::string get_data();
    public:
    AST();
    ~AST();
    AST(Token,std::vector<AST*>, ReductionKind);
    AST(Token);
    Token get_token()const;
    TokenType get_type()const;
    std::string compile();
    std::vector<AST*> get_children()const;
    void print(std::ostream&)const;
};

#endif
