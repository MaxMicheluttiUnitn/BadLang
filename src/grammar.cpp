#include "grammar.h"

Reduction::Reduction(){
    this->left = TokenType::NoneType;
    this->right = production();
}
Reduction::Reduction(TokenType type,production p){
    this->left = type;
    this->right = p;
}
Reduction::~Reduction(){}
TokenType Reduction::get_left()const{
    return this->left;
}
production Reduction::get_right()const{
    return this->right;
}


Grammar::Grammar(){}
Grammar::~Grammar(){}
std::ostream& operator<<(std::ostream& os, const Grammar& g){
    for(const std::pair<TokenType,productions>& item: g.rules){    
        for(const production& p: item.second){
            os<<item.first<<" -> ";
            for(const TokenType& tok: p){
                os<<tok<<" ";
            }
            os<<std::endl;
        }
    }
    return os;
}
void Grammar::add_rule(TokenType type, production p){
    if(this->rules.find(type) == this->rules.end()){
        productions prods;
        prods.push_back(p);
        this->rules.insert(std::make_pair(type,prods));
    }else{
        this->rules.at(type).push_back(p);
    }
}
int Grammar::size(){
    return this->rules.size();
}
void Grammar::print()const{
    std::cout<<(*this);
}
productions Grammar::get_productions(const TokenType& t)const{
    if(! this->rules.contains(t)){
        return productions();
    }else{
        return this->rules.at(t);
    }
}
std::set<TokenType> Grammar::first(const std::vector<TokenType>& word)const{
    std::set<TokenType> res = std::set<TokenType>();
    if(word.size()==0){
        res.insert(TokenType::NoneType);
        return res;
    }else if(word.size()==1){
        TokenType left = word.at(0);
        if(tokentype_is_literal(left)){
            res.insert(left);
            return res;
        }else{
            productions prods = this->get_productions(left);
            for(production p: prods){
                res.merge(this->first(p));
            }
        }
    }else{
        int j=0;
        while(j < word.size()){
            std::vector<TokenType> jth_vec;
            jth_vec.push_back(word.at(j));
            std::set<TokenType> jth_first = this->first(jth_vec);
            res.merge(jth_first);
            if(res.contains(TokenType::NoneType)){
                res.erase(TokenType::NoneType);
            }
            if(jth_first.contains(TokenType::NoneType)){
                j++;
            }else{
                break;
            }
        }
        if(j==word.size()){
            res.insert(TokenType::NoneType);
        }
        return res;
    }
}

Grammar Grammar::get_badlang_grammar(){
    Grammar g = Grammar();
    // S -> CODE
    std::vector<TokenType> right;
    right.push_back(TokenType::CODE);
    g.add_rule(TokenType::START,right);
    // CODE -> STATEMENT;
    right.clear();
    right.push_back(TokenType::STATEMENT);
    right.push_back(TokenType::semicolon);
    g.add_rule(TokenType::CODE,right);
    // CODE -> STATEMENT; CODE
    right.clear();
    right.push_back(TokenType::STATEMENT);
    right.push_back(TokenType::semicolon);
    right.push_back(TokenType::CODE);
    g.add_rule(TokenType::CODE,right);
    // STATEMENT -> EQUALITY
    right.clear();
    right.push_back(TokenType::EQUALITY);
    g.add_rule(TokenType::STATEMENT,right);
    // STATEMENT -> RETURN
    right.clear();
    right.push_back(TokenType::RETURN);
    g.add_rule(TokenType::STATEMENT,right);
    // EQUALITY -> NAME = MATH_OP
    right.clear();
    right.push_back(TokenType::name);
    right.push_back(TokenType::_equals);
    right.push_back(TokenType::MATH_OP);
    g.add_rule(TokenType::EQUALITY,right);
    // RETURN -> return ITEM
    right.clear();
    right.push_back(TokenType::_return);
    right.push_back(TokenType::ITEM);
    g.add_rule(TokenType::RETURN,right);
    // ITEM -> NAME
    right.clear();
    right.push_back(TokenType::name);
    g.add_rule(TokenType::ITEM,right);
    // ITEM -> NUMBER
    right.clear();
    right.push_back(TokenType::int_literal);
    g.add_rule(TokenType::ITEM,right);
    // MATH_OP -> MATH_OP_E
    right.clear();
    right.push_back(TokenType::MATH_OP_E);
    g.add_rule(TokenType::MATH_OP,right);
    // MATH_OP_E -> MATH_OP_T + MATH_OP_E
    right.clear();
    right.push_back(TokenType::MATH_OP_T);
    right.push_back(TokenType::_plus);
    right.push_back(TokenType::MATH_OP_E);
    g.add_rule(TokenType::MATH_OP_E,right);
    // MATH_OP_E -> MATH_OP_T - MATH_OP_E
    right.clear();
    right.push_back(TokenType::MATH_OP_T);
    right.push_back(TokenType::_minus);
    right.push_back(TokenType::MATH_OP_E);
    g.add_rule(TokenType::MATH_OP_E,right);
    // MATH_OP_E -> MATH_OP_T
    right.clear();
    right.push_back(TokenType::MATH_OP_T);
    g.add_rule(TokenType::MATH_OP_E,right);
    // MATH_OP_T -> MATH_OP_F * MATH_OP_T
    right.clear();
    right.push_back(TokenType::MATH_OP_F);
    right.push_back(TokenType::_times);
    right.push_back(TokenType::MATH_OP_T);
    g.add_rule(TokenType::MATH_OP_T,right);
    // MATH_OP_T -> MATH_OP_F / MATH_OP_T
    right.clear();
    right.push_back(TokenType::MATH_OP_F);
    right.push_back(TokenType::_divide);
    right.push_back(TokenType::MATH_OP_T);
    g.add_rule(TokenType::MATH_OP_T,right);
    // MATH_OP_T -> MATH_OP_F
    right.clear();
    right.push_back(TokenType::MATH_OP_F);
    g.add_rule(TokenType::MATH_OP_T,right);
    // // MATH_OP_F -> - ITEM
    // right.clear();
    // right.push_back(TokenType::_minus);
    // right.push_back(TokenType::ITEM);
    // g.add_rule(TokenType::MATH_OP_F,right);
    // // MATH_OP_F -> - ( MATH_OP_E )
    // right.clear();
    // right.push_back(TokenType::_minus);
    // right.push_back(TokenType::_open_brackets);
    // right.push_back(TokenType::MATH_OP_E);
    // right.push_back(TokenType::_close_brackets);
    // g.add_rule(TokenType::MATH_OP_F,right);
    // MATH_OP_F -> - MATH_OP_F
    right.clear();
    right.push_back(TokenType::_minus);
    right.push_back(TokenType::MATH_OP_F);
    g.add_rule(TokenType::MATH_OP_F,right);
    // MATH_OP_F -> ( MATH_OP_E )
    right.clear();
    right.push_back(TokenType::_open_brackets);
    right.push_back(TokenType::MATH_OP_E);
    right.push_back(TokenType::_close_brackets);
    g.add_rule(TokenType::MATH_OP_F,right);
    // MATH_OP_F -> ITEM
    right.clear();
    right.push_back(TokenType::ITEM);
    g.add_rule(TokenType::MATH_OP_F,right);

    return g;
}