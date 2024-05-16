#ifndef __AST_H__
#define __AST_H__
#include "token.h"
#include <vector>
#include <iostream>
#include <string>
#include <set>
#include <regex>
#include <vector>
#include "reduction_kind.h"
#include "token.h"
#include "errors.h"
#include "label_generator.h"

std::string remove_push_pop(std::string);

class AST{
    Token item;
    ReductionKind reduction_kind;
    std::vector<AST*> children;
    void print_depth(std::ostream&,int)const;
    std::string compile_rec(std::set<std::string>&, Labelgenerator&, bool,const std::string&, const std::string&);
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
