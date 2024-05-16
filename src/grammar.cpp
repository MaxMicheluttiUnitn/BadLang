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
    // for(TokenType t: word){
    //     std::cout<<t<<std::endl;
    // }
    // std::cout<<"---"<<std::endl;
    // std::cout.flush();
    std::set<TokenType> res = std::set<TokenType>();
    if(word.size()==0){
        res.insert(TokenType::NoneType);
        return res;
    }else if(word.size()==1){
        TokenType left = TokenType(word.at(0));
        if(tokentype_is_literal(left)){
            //std::cout<<"literal"<<std::endl;
            res.insert(left);
            return res;
        }else{
            productions prods = this->get_productions(left);
            for(Reduction p: prods){
                res.merge(this->first(p.get_right()));
            }
            return res;
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
    // CODE -> CONDITIONAL
    right.clear();
    right.push_back(TokenType::CONDITIONAL);
    g.add_rule(Reduction(TokenType::CODE,right),ReductionKind::CODE__CONDITIONAL);
    // CODE -> CONDITIONAL CODE
    right.clear();
    right.push_back(TokenType::CONDITIONAL);
    right.push_back(TokenType::CODE);
    g.add_rule(Reduction(TokenType::CODE,right),ReductionKind::CODE__CONDITIONAL_CODE);
    // CONDITIONAL -> _if ( OP ) BLOCK
    right.clear();
    right.push_back(TokenType::_if);
    right.push_back(TokenType::_open_brackets);
    right.push_back(TokenType::OP);
    right.push_back(TokenType::_close_brackets);
    right.push_back(TokenType::BLOCK);
    g.add_rule(Reduction(TokenType::CONDITIONAL,right),ReductionKind::CONDITIONAL__IF_OPEN_OP_CLOSE_BLOCK);
    // CONDITIONAL -> _if ( OP ) BLOCK _else BLOCK
    right.clear();
    right.push_back(TokenType::_if);
    right.push_back(TokenType::_open_brackets);
    right.push_back(TokenType::OP);
    right.push_back(TokenType::_close_brackets);
    right.push_back(TokenType::BLOCK);
    right.push_back(TokenType::_else);
    right.push_back(TokenType::BLOCK);
    g.add_rule(Reduction(TokenType::CONDITIONAL,right),ReductionKind::CONDITIONAL__IF_OPEN_OP_CLOSE_BLOCK_ELSE_BLOCK);
    // BLOCK -> _open_curly CODE _close_curly
    right.clear();
    right.push_back(TokenType::_open_curly);
    right.push_back(TokenType::CODE);
    right.push_back(TokenType::_close_curly);
    g.add_rule(Reduction(TokenType::BLOCK,right),ReductionKind::BLOCK__OPENCURLY_CODE_CLOSECURLY);
    // EQUALITY -> NAME = MATH_OP
    right.clear();
    right.push_back(TokenType::name);
    right.push_back(TokenType::_equals);
    right.push_back(TokenType::OP);
    g.add_rule(Reduction(TokenType::EQUALITY,right),ReductionKind::EQUALITY__NAME_EQ_OP);
    // RETURN -> return OP
    right.clear();
    right.push_back(TokenType::_return);
    right.push_back(TokenType::OP);
    g.add_rule(Reduction(TokenType::RETURN,right),ReductionKind::RETURN__RET_OP);
    // ITEM -> NAME
    right.clear();
    right.push_back(TokenType::name);
    g.add_rule(Reduction(TokenType::ITEM,right),ReductionKind::ITEM__NAME);
    // ITEM -> NUMBER
    right.clear();
    right.push_back(TokenType::int_literal);
    g.add_rule(Reduction(TokenType::ITEM,right),ReductionKind::ITEM__NUMBER);
    // ITEM -> _true
    right.clear();
    right.push_back(TokenType::_true);
    g.add_rule(Reduction(TokenType::ITEM,right),ReductionKind::ITEM__TRUE);
    // ITEM -> _false
    right.clear();
    right.push_back(TokenType::_false);
    g.add_rule(Reduction(TokenType::ITEM,right),ReductionKind::ITEM__FALSE);
    // OP -> MATH_OP_E
    // right.clear();
    // right.push_back(TokenType::MATH_OP_E);
    // g.add_rule(Reduction(TokenType::OP,right),ReductionKind::OP__MATHOPE);
    // MATH_OP_E -> MATH_OP_E + MATH_OP_T
    right.clear();
    right.push_back(TokenType::MATH_OP_E);
    right.push_back(TokenType::_plus);
    right.push_back(TokenType::MATH_OP_T);
    g.add_rule(Reduction(TokenType::MATH_OP_E,right),ReductionKind::MATHOPE__MATHOPE_PLUS_MATHOPT);
    // MATH_OP_E -> MATH_OP_E - MATH_OP_T
    right.clear();
    right.push_back(TokenType::MATH_OP_E);
    right.push_back(TokenType::_minus);
    right.push_back(TokenType::MATH_OP_T);
    g.add_rule(Reduction(TokenType::MATH_OP_E,right),ReductionKind::MATHOPE__MATHOPE_MINUS_MATHOPT);
    // MATH_OP_E -> MATH_OP_T
    right.clear();
    right.push_back(TokenType::MATH_OP_T);
    g.add_rule(Reduction(TokenType::MATH_OP_E,right),ReductionKind::MOTHOPE__MATHOPT);
    // MATH_OP_T -> MATH_OP_T * MATH_OP_F
    right.clear();
    right.push_back(TokenType::MATH_OP_T);
    right.push_back(TokenType::_times);
    right.push_back(TokenType::MATH_OP_F);
    g.add_rule(Reduction(TokenType::MATH_OP_T,right),ReductionKind::MATHOPT__MATHOPT_TIMES_MATHOPF);
    // MATH_OP_T -> MATH_OP_T / MATH_OP_F
    right.clear();
    right.push_back(TokenType::MATH_OP_T);
    right.push_back(TokenType::_divide);
    right.push_back(TokenType::MATH_OP_F);
    g.add_rule(Reduction(TokenType::MATH_OP_T,right),ReductionKind::MATHOPT__MATHOPT_DIV_MATHOPF);
    // MATH_OP_T -> MATH_OP_T % MATH_OP_F
    right.clear();
    right.push_back(TokenType::MATH_OP_T);
    right.push_back(TokenType::_modulus);
    right.push_back(TokenType::MATH_OP_F);
    g.add_rule(Reduction(TokenType::MATH_OP_T,right),ReductionKind::MATHOPT__MATHOPT_MOD_MATHOPF);
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
    // MATH_OP_F -> ( OP )
    right.clear();
    right.push_back(TokenType::_open_brackets);
    right.push_back(TokenType::OP);
    right.push_back(TokenType::_close_brackets);
    g.add_rule(Reduction(TokenType::MATH_OP_F,right),ReductionKind::MATHOPF__OPENBRACKETS_OP_CLOSEBRACKETS);
    // MATH_OP_F -> ITEM
    right.clear();
    right.push_back(TokenType::ITEM);
    g.add_rule(Reduction(TokenType::MATH_OP_F,right),ReductionKind::MATHOPF__ITEM);
    // CONDITIONAL -> WHILE ( OP ) BLOCK
    right.clear();
    right.push_back(TokenType::_while);
    right.push_back(TokenType::_open_brackets);
    right.push_back(TokenType::OP);
    right.push_back(TokenType::_close_brackets);
    right.push_back(TokenType::BLOCK);
    g.add_rule(Reduction(TokenType::CONDITIONAL,right),ReductionKind::CONDITIONAL__WHILE_OPEN_OP_CLOSE_BLOCK);
    // OP -> BOOL_OP_E
    right.clear();
    right.push_back(TokenType::BOOL_OP_E);
    g.add_rule(Reduction(TokenType::OP,right),ReductionKind::OP__BOOLOPE);
    // BOOL_OP_E -> BOOL_OP_E && BOOL_OP_T
    right.clear();
    right.push_back(TokenType::BOOL_OP_E);
    right.push_back(TokenType::_and);
    right.push_back(TokenType::BOOL_OP_T);
    g.add_rule(Reduction(TokenType::BOOL_OP_E,right),ReductionKind::BOOLOPE__BOOLOPE_AND_BOOLOPT);
    // BOOL_OP_E -> BOOL_OP_E || BOOL_OP_T
    right.clear();
    right.push_back(TokenType::BOOL_OP_E);
    right.push_back(TokenType::_or);
    right.push_back(TokenType::BOOL_OP_T);
    g.add_rule(Reduction(TokenType::BOOL_OP_E,right),ReductionKind::BOOLOPE__BOOLOPE_OR_BOOLOPT);
    // BOOL_OP_E -> BOOL_OP_E ^^ BOOL_OP_T
    right.clear();
    right.push_back(TokenType::BOOL_OP_E);
    right.push_back(TokenType::_xor);
    right.push_back(TokenType::BOOL_OP_T);
    g.add_rule(Reduction(TokenType::BOOL_OP_E,right),ReductionKind::BOOLOPE__BOOLOPE_XOR_BOOLOPT);
    // BOOL_OP_E -> BOOL_OP_T
    right.clear();
    right.push_back(TokenType::BOOL_OP_T);
    g.add_rule(Reduction(TokenType::BOOL_OP_E,right),ReductionKind::BOOLOPE__BOOLOPT);
    // BOOL_OP_T -> ! CMP_OP
    right.clear();
    right.push_back(TokenType::_not);
    right.push_back(TokenType::CMP_OP);
    g.add_rule(Reduction(TokenType::BOOL_OP_T,right),ReductionKind::BOOLOPT__NOT_CMPOP);
    // BOOL_OP_T -> CMP_OP
    right.clear();
    right.push_back(TokenType::CMP_OP);
    g.add_rule(Reduction(TokenType::BOOL_OP_T,right),ReductionKind::BOOLOPT__CMPOP);
    // CMP_OP -> CMP_OP == BITWISE_OP_E
    right.clear();
    right.push_back(TokenType::CMP_OP);
    right.push_back(TokenType::_equal_equal);
    right.push_back(TokenType::BITWISE_OP_E);
    g.add_rule(Reduction(TokenType::CMP_OP,right),ReductionKind::CMPOP__CMPOP_EQEQ_BITWISEOPE);
    // CMP_OP -> CMP_OP != BITWISE_OP_E
    right.clear();
    right.push_back(TokenType::CMP_OP);
    right.push_back(TokenType::_not_equal);
    right.push_back(TokenType::BITWISE_OP_E);
    g.add_rule(Reduction(TokenType::CMP_OP,right),ReductionKind::CMPOP__CMPOP_NOTEQ_BITWISEOPE);
    // CMP_OP -> CMP_OP < BITWISE_OP_E
    right.clear();
    right.push_back(TokenType::CMP_OP);
    right.push_back(TokenType::_lt);
    right.push_back(TokenType::BITWISE_OP_E);
    g.add_rule(Reduction(TokenType::CMP_OP,right),ReductionKind::CMPOP__CMPOP_LT_BITWISEOPE);
    // CMP_OP -> CMP_OP <= BITWISE_OP_E
    right.clear();
    right.push_back(TokenType::CMP_OP);
    right.push_back(TokenType::_lt_eq);
    right.push_back(TokenType::BITWISE_OP_E);
    g.add_rule(Reduction(TokenType::CMP_OP,right),ReductionKind::CMPOP__CMPOP_LTEQ_BITWISEOPE);
    // CMP_OP -> CMP_OP > BITWISE_OP_E
    right.clear();
    right.push_back(TokenType::CMP_OP);
    right.push_back(TokenType::_gt);
    right.push_back(TokenType::BITWISE_OP_E);
    g.add_rule(Reduction(TokenType::CMP_OP,right),ReductionKind::CMPOP__CMPOP_GT_BITWISEOPE);
    // CMP_OP -> CMP_OP >= BITWISE_OP_E
    right.clear();
    right.push_back(TokenType::CMP_OP);
    right.push_back(TokenType::_gt_eq);
    right.push_back(TokenType::BITWISE_OP_E);
    g.add_rule(Reduction(TokenType::CMP_OP,right),ReductionKind::CMPOP__CMPOP_GTEQ_BITWISEOPE);
    // CMP_OP -> BITWISE_OP_E
    right.clear();
    right.push_back(TokenType::BITWISE_OP_E);
    g.add_rule(Reduction(TokenType::CMP_OP,right),ReductionKind::CMPOP__BITWISEOPE);
    // BITWISE_OP_E -> BITWISE_OP_E & BITWISE_OP_T
    right.clear();
    right.push_back(TokenType::BITWISE_OP_E);
    right.push_back(TokenType::_bitwise_and);
    right.push_back(TokenType::BITWISE_OP_T);
    g.add_rule(Reduction(TokenType::BITWISE_OP_E,right),ReductionKind::BITWISEOPE__BITWISEOPE_BWAND_BITWISEOPT);
    // BITWISE_OP_E -> BITWISE_OP_E | BITWISE_OP_T
    right.clear();
    right.push_back(TokenType::BITWISE_OP_E);
    right.push_back(TokenType::_bitwise_or);
    right.push_back(TokenType::BITWISE_OP_T);
    g.add_rule(Reduction(TokenType::BITWISE_OP_E,right),ReductionKind::BITWISEOPE__BITWISEOPE_BWOR_BITWISEOPT);
    // BITWISE_OP_E -> BITWISE_OP_E ^ BITWISE_OP_T
    right.clear();
    right.push_back(TokenType::BITWISE_OP_E);
    right.push_back(TokenType::_bitwise_xor);
    right.push_back(TokenType::BITWISE_OP_T);
    g.add_rule(Reduction(TokenType::BITWISE_OP_E,right),ReductionKind::BITWISEOPE__BITWISEOPE_BWXOR_BITWISEOPT);
    // BITWISE_OP_E -> BITWISE_OP_T
    right.clear();
    right.push_back(TokenType::BITWISE_OP_T);
    g.add_rule(Reduction(TokenType::BITWISE_OP_E,right),ReductionKind::BITWISEOPE__BITWISEOPT);
    // BITWISE_OP_T -> ~ BITWISE_OP_T
    right.clear();
    right.push_back(TokenType::_tilde);
    right.push_back(TokenType::BITWISE_OP_T);
    g.add_rule(Reduction(TokenType::BITWISE_OP_T,right),ReductionKind::BITWISEOPT__TILDE_BITWISEOPT);
    // BITWISE_OP_T -> MATH_OP_E
    right.clear();
    right.push_back(TokenType::MATH_OP_E);
    g.add_rule(Reduction(TokenType::BITWISE_OP_T,right),ReductionKind::BITWISEOPT__MATHOPE);
    // STATEMENT -> _break
    right.clear();
    right.push_back(TokenType::_break);
    g.add_rule(Reduction(TokenType::STATEMENT,right),ReductionKind::STATEMENT__BREAK);
    // STATEMENT -> _continue
    right.clear();
    right.push_back(TokenType::_continue);
    g.add_rule(Reduction(TokenType::STATEMENT,right),ReductionKind::STATEMENT__CONTINUE);
    // CONDITIONAL -> _for ( EQUALITY ; OP ; EQUALITY ) BLOCK
    right.clear();
    right.push_back(TokenType::_for);
    right.push_back(TokenType::_open_brackets);
    right.push_back(TokenType::EQUALITY);
    right.push_back(TokenType::semicolon);
    right.push_back(TokenType::OP);
    right.push_back(TokenType::semicolon);
    right.push_back(TokenType::EQUALITY);
    right.push_back(TokenType::_close_brackets);
    right.push_back(TokenType::BLOCK);
    g.add_rule(Reduction(TokenType::CONDITIONAL,right),ReductionKind::CONDITIONAL__FOR_OPEN_EQUALITY_SEMI_OP_SEMI_EQUALITY_CLOSE_BLOCK);
    // EQUALITY -> NAME ++
    right.clear();
    right.push_back(TokenType::name);
    right.push_back(TokenType::_plusplus);
    g.add_rule(Reduction(TokenType::EQUALITY,right),ReductionKind::EQUALITY__NAME_PLUPLUS);
    // EQUALITY -> NAME --
    right.clear();
    right.push_back(TokenType::name);
    right.push_back(TokenType::_minusminus);
    g.add_rule(Reduction(TokenType::EQUALITY,right),ReductionKind::EQUALITY__NAME_MINUSMINUS);
    // EQUALITY -> NAME += OP
    right.clear();
    right.push_back(TokenType::name);
    right.push_back(TokenType::_pluseq);
    right.push_back(TokenType::OP);
    g.add_rule(Reduction(TokenType::EQUALITY,right),ReductionKind::EQUALITY__NAME_PLUSEQ_OP);
    // EQUALITY -> NAME -= OP
    right.clear();
    right.push_back(TokenType::name);
    right.push_back(TokenType::_minuseq);
    right.push_back(TokenType::OP);
    g.add_rule(Reduction(TokenType::EQUALITY,right),ReductionKind::EQUALITY__NAME_MINUSEQ_OP);
    // EQUALITY -> NAME *= OP
    right.clear();
    right.push_back(TokenType::name);
    right.push_back(TokenType::_timeseq);
    right.push_back(TokenType::OP);
    g.add_rule(Reduction(TokenType::EQUALITY,right),ReductionKind::EQUALITY__NAME_TIMESEQ_OP);
    // EQUALITY -> NAME /= OP
    right.clear();
    right.push_back(TokenType::name);
    right.push_back(TokenType::_divideeq);
    right.push_back(TokenType::OP);
    g.add_rule(Reduction(TokenType::EQUALITY,right),ReductionKind::EQUALITY__NAME_DIVIDEEQ_OP);
    // EQUALITY -> NAME %= OP
    right.clear();
    right.push_back(TokenType::name);
    right.push_back(TokenType::_moduluseq);
    right.push_back(TokenType::OP);
    g.add_rule(Reduction(TokenType::EQUALITY,right),ReductionKind::EQUALITY__NAME_MODULUSEQ_OP);

    return g;
}