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
Reduction::Reduction(const Reduction& r){
    this->left = r.left;
    this->right = r.right;
}
TokenType Reduction::get_right_index(int i)const{
    return this->right.at(i);
}
bool Reduction::operator==(const Reduction& other)const{
    if(other.left != this->left){
        return false;
    }
    if(other.right.size() != this->right.size()){
        return false;
    }
    for(int i=0; i<other.right.size();i++){
        if(other.get_right_index(i) != this->get_right_index(i)){
            return false;
        }
    }
    return true;
}
bool Reduction::operator<(const Reduction& other)const{
    if(other.left != this->left){
        return static_cast<int>(other.left) > static_cast<int>(this->left);
    }
    if(other.right.size() != this->right.size()){
        return static_cast<int>(other.right.size()) > static_cast<int>(this->right.size());
    }
    for(int i=0; i<other.right.size();i++){
        if(other.get_right_index(i) != this->get_right_index(i)){
            return static_cast<int>(other.get_right_index(i)) > static_cast<int>(this->get_right_index(i));
        }
    }
    // they are the same reduction
    return false;
}


Grammar::Grammar(){
    this->id_to_rule = std::map<ReductionKind,Reduction>();
    this->rule_to_id = std::map<Reduction,ReductionKind>();
}
Grammar::~Grammar(){}
std::ostream& operator<<(std::ostream& os, const Grammar& g){
    for(const std::pair<TokenType,productions>& item: g.rules){    
        for(const Reduction& p: item.second){
            os<<item.first<<" -> ";
            production right = p.get_right();
            for(const TokenType& tok: right){
                os<<tok<<" ";
            }
            os<<std::endl;
        }
    }
    return os;
}
void Grammar::add_rule(Reduction p, ReductionKind index){
    // Insert into prods
    TokenType type = p.get_left();
    if(this->rules.find(type) == this->rules.end()){
        productions prods;
        prods.push_back(p);
        this->rules.insert(std::make_pair(type,prods));
    }else{
        this->rules.at(type).push_back(p);
    }
    //insert into maps
    this->id_to_rule.insert(std::make_pair(index,p));
    this->rule_to_id.insert(std::make_pair(p,index));

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
ReductionKind Grammar::get_kind_of_reduction(const Reduction& r)const{
    return this->rule_to_id.at(r);
}
Reduction Grammar::get_reduction_of_kind(ReductionKind i)const{
    return this->id_to_rule.at(i);
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
            for(Reduction p: prods){
                res.merge(this->first(p.get_right()));
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
    g.add_rule(Reduction(TokenType::START,right),ReductionKind::START__CODE);
    // CODE -> STATEMENT;
    right.clear();
    right.push_back(TokenType::STATEMENT);
    right.push_back(TokenType::semicolon);
    g.add_rule(Reduction(TokenType::CODE,right),ReductionKind::CODE__STATEMENT);
    // CODE -> STATEMENT; CODE
    right.clear();
    right.push_back(TokenType::STATEMENT);
    right.push_back(TokenType::semicolon);
    right.push_back(TokenType::CODE);
    g.add_rule(Reduction(TokenType::CODE,right),ReductionKind::CODE__STATEMENT_SEMI_CODE);
    // STATEMENT -> EQUALITY
    right.clear();
    right.push_back(TokenType::EQUALITY);
    g.add_rule(Reduction(TokenType::STATEMENT,right),ReductionKind::STATEMENT__EQUALITY);
    // STATEMENT -> RETURN
    right.clear();
    right.push_back(TokenType::RETURN);
    g.add_rule(Reduction(TokenType::STATEMENT,right),ReductionKind::STATEMENT__RETURN);
    // EQUALITY -> NAME = MATH_OP
    right.clear();
    right.push_back(TokenType::name);
    right.push_back(TokenType::_equals);
    right.push_back(TokenType::MATH_OP);
    g.add_rule(Reduction(TokenType::EQUALITY,right),ReductionKind::EQUALITY__NAME_EQ_MATHOP);
    // RETURN -> return ITEM
    right.clear();
    right.push_back(TokenType::_return);
    right.push_back(TokenType::ITEM);
    g.add_rule(Reduction(TokenType::RETURN,right),ReductionKind::RETURN__RET_ITEM);
    // ITEM -> NAME
    right.clear();
    right.push_back(TokenType::name);
    g.add_rule(Reduction(TokenType::ITEM,right),ReductionKind::ITEM__NAME);
    // ITEM -> NUMBER
    right.clear();
    right.push_back(TokenType::int_literal);
    g.add_rule(Reduction(TokenType::ITEM,right),ReductionKind::ITEM_NUMBER);
    // MATH_OP -> MATH_OP_E
    right.clear();
    right.push_back(TokenType::MATH_OP_E);
    g.add_rule(Reduction(TokenType::MATH_OP,right),ReductionKind::MATHOP__MATHOPE);
    // MATH_OP_E -> MATH_OP_T + MATH_OP_E
    right.clear();
    right.push_back(TokenType::MATH_OP_T);
    right.push_back(TokenType::_plus);
    right.push_back(TokenType::MATH_OP_E);
    g.add_rule(Reduction(TokenType::MATH_OP_E,right),ReductionKind::MATHOPE__MATHOPT_PLUS_MATHOPE);
    // MATH_OP_E -> MATH_OP_T - MATH_OP_E
    right.clear();
    right.push_back(TokenType::MATH_OP_T);
    right.push_back(TokenType::_minus);
    right.push_back(TokenType::MATH_OP_E);
    g.add_rule(Reduction(TokenType::MATH_OP_E,right),ReductionKind::MATHOPE__MATHOPT_MINUS_MATHOPE);
    // MATH_OP_E -> MATH_OP_T
    right.clear();
    right.push_back(TokenType::MATH_OP_T);
    g.add_rule(Reduction(TokenType::MATH_OP_E,right),ReductionKind::MOTHOPE__MATHOPT);
    // MATH_OP_T -> MATH_OP_F * MATH_OP_T
    right.clear();
    right.push_back(TokenType::MATH_OP_F);
    right.push_back(TokenType::_times);
    right.push_back(TokenType::MATH_OP_T);
    g.add_rule(Reduction(TokenType::MATH_OP_T,right),ReductionKind::MATHOPT__MATHOPF_TIMES_MATHOPT);
    // MATH_OP_T -> MATH_OP_F / MATH_OP_T
    right.clear();
    right.push_back(TokenType::MATH_OP_F);
    right.push_back(TokenType::_divide);
    right.push_back(TokenType::MATH_OP_T);
    g.add_rule(Reduction(TokenType::MATH_OP_T,right),ReductionKind::MATHOPT__MATHOPF_DIV_MATHOPT);
    // MATH_OP_T -> MATH_OP_F
    right.clear();
    right.push_back(TokenType::MATH_OP_F);
    g.add_rule(Reduction(TokenType::MATH_OP_T,right),ReductionKind::MATHOPT__MATHOPF);
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
    g.add_rule(Reduction(TokenType::MATH_OP_F,right),ReductionKind::MATHOPF__MINUS_MATHOPF);
    // MATH_OP_F -> ( MATH_OP_E )
    right.clear();
    right.push_back(TokenType::_open_brackets);
    right.push_back(TokenType::MATH_OP_E);
    right.push_back(TokenType::_close_brackets);
    g.add_rule(Reduction(TokenType::MATH_OP_F,right),ReductionKind::MATHOPF__OPENBRACKETS_MATHOPE_CLOSEBRACKETS);
    // MATH_OP_F -> ITEM
    right.clear();
    right.push_back(TokenType::ITEM);
    g.add_rule(Reduction(TokenType::MATH_OP_F,right),ReductionKind::MATHOPF__ITEM);

    return g;
}