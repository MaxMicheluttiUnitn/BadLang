#ifndef __LR_1_AUTOMATON__H__
#define __LR_1_AUTOMATON__H__
#include "grammar.h"
#include "token.h"
#include <vector>
#include <set>
#include <iostream>
#include <map>
#include <queue>
#include <algorithm>

class DottedRule;
class LookaheadRule;

std::ostream& operator<<(std::ostream&, const DottedRule&);
std::ostream& operator<<(std::ostream&, const LookaheadRule&);

class DottedRule{
    protected:
    int dot_index;
    Reduction rule;
    public:
    friend std::ostream& operator<<(std::ostream&, const DottedRule&);
    DottedRule();
    DottedRule(const DottedRule&);
    DottedRule(Reduction);
    ~DottedRule();
    int get_dot_index();
    TokenType get_dotted_symbol()const;
    TokenType get_follow_dotted_symbol()const;
    TokenType get_left()const;
    std::vector<TokenType> get_remaining();
    production get_right()const;
    TokenType get_right_at(int id)const;
    Reduction get_reduction()const;
    void set_dot(int);
    int right_size()const;
    void move_dot();
    bool operator==(const DottedRule&)const;
    bool operator<(const DottedRule&)const;
    void print()const;
    friend class LookaheadRule;
};

class LookaheadRule: DottedRule{
    std::set<TokenType> lookahead;
    public:
    friend std::ostream& operator<<(std::ostream&, const LookaheadRule&);
    DottedRule without_lookahead()const;
    LookaheadRule();
    void move_dot();
    void print()const;
    TokenType get_dotted_symbol()const;
    TokenType get_follow_dotted_symbol()const;
    LookaheadRule(const LookaheadRule&);
    LookaheadRule(const DottedRule&,std::set<TokenType>);
    LookaheadRule(Reduction,std::set<TokenType>);
    ~LookaheadRule();
    void set_lookahead(std::set<TokenType>);
    std::set<TokenType> get_lookahead()const;
    bool operator==(const LookaheadRule&)const;
    bool operator<(const LookaheadRule&)const;
};

class LR1_State;

std::ostream& operator<<(std::ostream&, const LR1_State&);

class LR1_State{
    private:
    bool is_final;
    bool is_reduce;
    std::set<LookaheadRule> kernel;
    std::set<LookaheadRule> closure;
    std::map<TokenType,LR1_State*> transitions;
    std::map<TokenType,DottedRule> reductions;
    bool check_final();
    static bool tmp_closure_has_unmarked(const std::map<DottedRule,std::pair<std::set<TokenType>,bool>>& m);
    public:
    friend std::ostream& operator<<(std::ostream&, const LR1_State&);
    LR1_State(std::set<LookaheadRule>);
    ~LR1_State();
    void compute_closure(const Grammar&);
    bool operator==(const LR1_State&)const;
    void print()const;
    std::map<TokenType,LR1_State*> compute_neighbours();
    void add_neighbour(TokenType,LR1_State*);
    LR1_State* get_neighbour(TokenType);
    std::optional<DottedRule> get_reduce(TokenType);
    bool accepts(TokenType)const;
};

class LR1_Automaton;

std::ostream& operator<<(std::ostream&, const LR1_Automaton&);

class LR1_Automaton{
    private:
    std::vector<LR1_State*> states;
    public:
    LR1_State* get_initial_state();
    LR1_Automaton(const Grammar&);
    ~LR1_Automaton();
    friend std::ostream& operator<<(std::ostream&, const LR1_Automaton&);
    void add_state(LR1_State*);
    LR1_State* contains_state(LR1_State*);
    void print()const;
};

#endif
