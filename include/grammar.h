#ifndef __GRAMMAR__H__
#define __GRAMMAR__H__
#include <vector>
#include <map>
#include <iostream>
#include <set>
#include "token.h"
#include "reduction_kind.h"

typedef std::vector<TokenType> production;

class Reduction{
    TokenType left;
    production right;
    public:
    Reduction();
    Reduction(TokenType,production);
    Reduction(const Reduction&);
    ~Reduction();
    TokenType get_left()const;
    TokenType get_right_index(int)const;
    production get_right()const;
    bool operator==(const Reduction&)const;
    bool operator<(const Reduction&)const;
};

typedef std::vector<Reduction> productions;

class Grammar;
std::ostream& operator<<(std::ostream&, const Grammar& g);

class Grammar{
    private:
    std::map<TokenType,productions> rules;
    std::map<Reduction,ReductionKind> rule_to_id;
    std::map<ReductionKind,Reduction> id_to_rule;
    Grammar();
    void add_rule(Reduction,ReductionKind);
    public:
    int size();
    ~Grammar();
    static Grammar get_badlang_grammar();
    void print()const;
    productions get_productions(const TokenType&)const;
    friend std::ostream& operator<<(std::ostream&, const Grammar& g);
    std::set<TokenType> first(const std::vector<TokenType>&)const;
    ReductionKind get_kind_of_reduction(const Reduction& r)const;
    Reduction get_reduction_of_kind(ReductionKind)const;
};

#endif
