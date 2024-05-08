#ifndef __GRAMMAR__H__
#define __GRAMMAR__H__
#include <vector>
#include <map>
#include <iostream>
#include <set>
#include "token.h"

typedef std::vector<std::vector<TokenType>> productions;
typedef std::vector<TokenType> production;

class Reduction{
    TokenType left;
    production right;
    public:
    Reduction();
    Reduction(TokenType,production);
    ~Reduction();
    TokenType get_left()const;
    production get_right()const;
};

class Grammar;
std::ostream& operator<<(std::ostream&, const Grammar& g);

class Grammar{
    private:
    std::map<TokenType,productions> rules;
    Grammar();
    void add_rule(TokenType,production);
    public:
    int size();
    ~Grammar();
    static Grammar get_badlang_grammar();
    void print()const;
    productions get_productions(const TokenType&)const;
    friend std::ostream& operator<<(std::ostream&, const Grammar& g);
    std::set<TokenType> first(const std::vector<TokenType>&)const;
};

#endif
