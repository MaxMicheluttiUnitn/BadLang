#include "lr1_automaton.h"

DottedRule::DottedRule(){}
DottedRule::DottedRule(Reduction reduction){
    this->rule = reduction;
    this->dot_index = 0;
}
TokenType DottedRule::get_right_at(int id)const{
    return this->rule.get_right_index(id);
}
LookaheadRule::~LookaheadRule(){}
LookaheadRule::LookaheadRule(){}
void LookaheadRule::move_dot(){
    if(this->dot_index < this->right_size())
        this->dot_index++;
}
void LookaheadRule::print()const{
    std::cout<<(*this);
}
TokenType LookaheadRule::get_dotted_symbol()const{
    if(this->dot_index == this->right_size()){
        return TokenType::NoneType;
    }
    return this->get_right_at(this->dot_index);
}
TokenType LookaheadRule::get_follow_dotted_symbol()const{
    if(this->dot_index<this->right_size() - 1){
        return this->get_right().at(this->dot_index+1);
    }
    return TokenType::NoneType;
}
LookaheadRule::LookaheadRule(const LookaheadRule& other){
    this->dot_index = other.dot_index;
    this->rule = other.rule;
    this->lookahead = other.lookahead;
}
LookaheadRule::LookaheadRule(const DottedRule& other,std::set<TokenType> la){
    this->dot_index = other.dot_index;
    this->rule = other.rule;
    this->lookahead = la;
}
LookaheadRule::LookaheadRule(Reduction r, std::set<TokenType> la){
    this->rule = r;
    this->dot_index = 0;
    this->lookahead = la;
}
void LookaheadRule::set_lookahead(std::set<TokenType> la){
    this->lookahead = la;
}
int DottedRule::right_size()const{
    return this->get_right().size();
}
std::vector<TokenType> DottedRule::get_remaining(){
    std::vector<TokenType> res;
    for(int i=this->dot_index + 1;i<this->right_size();i++){
        res.push_back(this->get_right_at(i));
    }
    return res;
}
DottedRule LookaheadRule::without_lookahead()const{
    DottedRule res = DottedRule(this->rule);
    res.set_dot(this->dot_index);
    return res;
}
std::set<TokenType> LookaheadRule::get_lookahead()const{
    return this->lookahead;
}
TokenType DottedRule::get_follow_dotted_symbol()const{
    if(this->dot_index<this->right_size() - 1){
        return this->get_right().at(this->dot_index+1);
    }
    return TokenType::NoneType;
}
Reduction DottedRule::get_reduction()const{
    return this->rule;
}
bool DottedRule::operator<(const DottedRule& other)const{
    if(this->dot_index != other.dot_index){
        return this->dot_index < other.dot_index;
    }
    if(this->right_size() != other.right_size()){
        return this->right_size() < other.right_size();
    }
    int my_left = static_cast<int>(this->get_left());
    int other_left = static_cast<int>(other.get_left());
    if(my_left != other_left){
        return my_left < other_left;
    }
    for(unsigned int i=0; i<this->right_size();i++){
        my_left = static_cast<int>(this->get_right().at(i));
        other_left = static_cast<int>(other.get_right().at(i));
        if(my_left != other_left){
            return my_left < other_left;
        }
    }
    return false;
}
bool LookaheadRule::operator<(const LookaheadRule& other)const{
    if(this->dot_index != other.dot_index){
        return this->dot_index < other.dot_index;
    }
    if(this->right_size() != other.right_size()){
        return this->right_size() < other.right_size();
    }
    int my_left = static_cast<int>(this->rule.get_left());
    int other_left = static_cast<int>(other.rule.get_left());
    if(my_left != other_left){
        return my_left < other_left;
    }
    for(unsigned int i=0; i<this->right_size();i++){
        my_left = static_cast<int>(this->get_right_at(i));
        other_left = static_cast<int>(other.get_right_at(i));
        if(my_left != other_left){
            return my_left < other_left;
        }
    }
    if(this->lookahead.size() != other.lookahead.size()){
        return this->lookahead.size() < other.lookahead.size();
    }
    return this->lookahead<other.lookahead;
}
DottedRule::DottedRule(const DottedRule& r){
    this->rule = r.rule;
    this->dot_index = r.dot_index;
}
TokenType DottedRule::get_dotted_symbol()const{
    if(this->dot_index == this->right_size()){
        return TokenType::NoneType;
    }
    return this->get_right().at(this->dot_index);
}
DottedRule::~DottedRule(){}
int DottedRule::get_dot_index(){
    return this->dot_index;
}
TokenType DottedRule::get_left()const{
    return this->rule.get_left();
}
production DottedRule::get_right()const{
    return this->rule.get_right();

}
void DottedRule::set_dot(int pos){
    this->dot_index = pos;
}
std::ostream& operator<<(std::ostream& os, const DottedRule& r){
    os<<r.get_left()<<" -> ";
    for(int i=0; i<r.right_size(); i++){
        if(i == r.dot_index){
            os<<". ";
        }
        os<<r.get_right().at(i)<<" ";
    }
    if(r.dot_index == r.get_right().size()){
        os<<". ";
    }
    return os;
}
std::ostream& operator<<(std::ostream& os, const LookaheadRule& r){
    os<<r.get_left()<<" -> ";
    for(int i=0; i<r.right_size(); i++){
        if(i == r.dot_index){
            os<<". ";
        }
        os<<r.get_right().at(i)<<" ";
    }
    if(r.dot_index == r.right_size()){
        os<<". ";
    }
    os<<"{";
    for(const TokenType& t: r.lookahead){
        os<<t<<", ";
    }
    os<<"}";
    return os;
}
void DottedRule::print()const{
    std::cout<<(*this);
}
bool DottedRule::operator==(const DottedRule& other)const{
    if(this->dot_index != other.dot_index){
        return false;
    }
    if(this->get_left() != other.get_left()){
        return false;
    }
    if(this->right_size() != other.right_size()){
        return false;
    }
    for(unsigned int i=0;i<right_size();i++){
        if(this->get_right_at(i) != other.get_right_at(i)){
            return false;
        }
    }
    return true;
}
bool LookaheadRule::operator==(const LookaheadRule& other)const{
    if(this->dot_index != other.dot_index){
        return false;
    }
    if(this->get_left() != other.get_left()){
        return false;
    }
    if(this->right_size() != other.right_size()){
        return false;
    }
    for(unsigned int i=0;i<right_size();i++){
        if(this->get_right_at(i) != other.get_right_at(i)){
            return false;
        }
    }
    return this->lookahead == other.lookahead;
}
void DottedRule::move_dot(){
    if(this->dot_index < this->right_size())
        this->dot_index++;
}

bool LR1_State::check_final(){
    production accept;
    accept.push_back(TokenType::CODE);
    std::set<TokenType> accept_set;
    accept_set.insert(TokenType::_end);
    LookaheadRule accepting_item = LookaheadRule(Reduction(TokenType::START,accept),accept_set);
    accepting_item.move_dot();
    for(LookaheadRule r: this->kernel){
        if(r == accepting_item){
            return true;
        }
    }
    return false;
}
std::ostream& operator<<(std::ostream& os, const LR1_State& r){
    os<<"Final: "<<r.is_final<<std::endl;
    os<<"Reduce: "<<r.is_reduce<<std::endl;
    os<<"Kernel:"<<std::endl;
    for(const LookaheadRule& dr: r.kernel){
        os<<dr;
        os<<std::endl;
    }
    os<<"Closure:"<<std::endl;
    for(const LookaheadRule& dr: r.closure){
        os<<dr;
        os<<std::endl;
    }
    if(!r.transitions.empty()){
        os<<"Shift:"<<std::endl;
        for(const std::pair<TokenType,LR1_State*> item: r.transitions){
            os<<item.first<<": "<<item.second<<std::endl;
        }
    }
    if(!r.reductions.empty()){
        os<<"Reduce:"<<std::endl;
        for(const std::pair<TokenType,DottedRule> item: r.reductions){
            os<<item.first<<": "<<item.second<<std::endl;
        }
    }
    return os;
}
void LR1_State::print()const{
    std::cout<<(*this);
}
bool LR1_State::accepts(TokenType t)const{
    return (t == TokenType::_end && this->is_final);
}
bool LR1_State::operator==(const LR1_State& other)const{
    if(this->kernel.size() != other.kernel.size()){
        return false;
    }
    return this->kernel == other.kernel;
}
LR1_State::LR1_State(std::set<LookaheadRule> k){
    this->kernel = k;
    this->is_final = this->check_final();
    this->is_reduce = false;
    this->closure = std::set<LookaheadRule>();
    //this->compute_clusure(g);
}
LR1_State::~LR1_State(){}
bool LR1_State::tmp_closure_has_unmarked(const std::map<DottedRule, std::pair<std::set<TokenType>,bool>>& m){
    for(const std::pair<DottedRule,std::pair<std::set<TokenType>,bool>>& p: m){
        if(!p.second.second){
            return true;
        }
    }
    return false;
}
LR1_State* LR1_State::get_neighbour(TokenType t){
    if(this->transitions.contains(t)){
        return this->transitions.at(t);
    }
    return nullptr;
}
std::optional<DottedRule> LR1_State::get_reduce(TokenType t){
    if(this->reductions.contains(t)){
        return DottedRule(this->reductions.at(t));
    }
    return std::nullopt;
}
void LR1_State::compute_closure(const Grammar& g){
    std::map<DottedRule,std::pair<std::set<TokenType>,bool>> tmp_closure;
    for(LookaheadRule dr: this->kernel){
        tmp_closure.insert(std::make_pair(
            dr.without_lookahead(),
            std::make_pair(
                dr.get_lookahead(),
                false
            )));
    }
    while(LR1_State::tmp_closure_has_unmarked(tmp_closure)){
        DottedRule item = DottedRule();
        for(std::pair<DottedRule,std::pair<std::set<TokenType>,bool>> p: tmp_closure){
            if(!p.second.second){
                item = p.first;
                break;
            }
        }
        tmp_closure.at(item).second = true;
        TokenType current_dotted_tokentype = item.get_dotted_symbol();
        if(tokentype_is_production_symbol(current_dotted_tokentype)){
            std::set<TokenType> delta_one = std::set<TokenType>();
            std::vector<TokenType> remaining = item.get_remaining();
            std::set<TokenType> items = tmp_closure.at(item).first;
            for(TokenType t: items ){
                std::vector<TokenType> betad = std::vector<TokenType>(remaining);
                betad.push_back(t);
                std::set<TokenType> first_beta_d = g.first(betad);
                delta_one.merge(first_beta_d);
            }
            productions prods = g.get_productions(current_dotted_tokentype);
            for(Reduction p_prime: prods){
                // DottedRule dr = DottedRule(current_dotted_tokentype,p_prime,delta_one);
                DottedRule without_lookahead =  DottedRule(p_prime);
                if(! tmp_closure.contains(without_lookahead)){
                    tmp_closure.insert(
                        std::make_pair(
                            without_lookahead,
                            std::make_pair(delta_one,false)
                        )
                    );
                }else{
                    std::set<TokenType> gamma = tmp_closure.at(without_lookahead).first;
                    bool delta_one_has_new_element = false;
                    for(TokenType t: delta_one){
                        if(! gamma.contains(t)){
                            delta_one_has_new_element = true;
                            break;
                        }
                    }
                    if(delta_one_has_new_element){
                        for(TokenType t: delta_one){
                            gamma.insert(t);
                        }
                        tmp_closure.erase(without_lookahead);
                        tmp_closure.insert(
                            std::make_pair(
                                without_lookahead,
                                std::make_pair(gamma,false)
                            )
                        );
                    }
                }
            }
        }
    }
    for(std::pair<DottedRule,std::pair<std::set<TokenType>,bool>> item: tmp_closure){
        LookaheadRule dr = LookaheadRule(item.first,item.second.first);
        this->closure.insert(dr);
    }
}
std::map<TokenType,LR1_State*> LR1_State::compute_neighbours(){
    std::map<TokenType,std::set<LookaheadRule>> transition_keys;
    std::set<TokenType> reduction_keys;
    for(const LookaheadRule& dr: this->closure){
        TokenType symbol = dr.get_dotted_symbol();
        if(symbol == TokenType::NoneType){
            // SKIP REDUCE ITEMS
            this->is_reduce= true;
            for(TokenType tt: dr.get_lookahead()){
                if(this->reductions.contains(tt)){
                    std::cerr<<"RR conflict"<<std::endl;
                    exit(EXIT_FAILURE);
                }else{
                    this->reductions.insert(std::make_pair(
                        tt,dr.without_lookahead()
                    ));
                    reduction_keys.insert(tt);
                }
            }
            continue;
        }
        if(!(transition_keys.contains(symbol))){
            transition_keys.insert(std::make_pair(
                symbol,std::set<LookaheadRule>()
            ));
        }
        LookaheadRule dr_clone = LookaheadRule(dr);
        dr_clone.move_dot();
        transition_keys.at(dr.get_dotted_symbol()).insert(dr_clone);
    }
    std::map<TokenType,LR1_State*> res;
    for(std::pair<TokenType,std::set<LookaheadRule>> item: transition_keys){
        if(reduction_keys.contains(item.first)){
            std::cerr<<"SR conflict"<<std::endl;
            exit(EXIT_FAILURE);
        }
        LR1_State* s = new LR1_State(item.second);
        res.insert(std::make_pair(item.first,s));
    }
    return res;
}
void LR1_State::add_neighbour(TokenType tp,LR1_State* n){
    this->transitions.insert(std::make_pair(tp,n));
}

LR1_Automaton::LR1_Automaton(const Grammar& g){
    std::set<LookaheadRule> first_kernel;
    production first_production;
    first_production.push_back(TokenType::CODE);
    std::set<TokenType> first_lahead;
    first_lahead.insert(TokenType::_end);
    first_kernel.insert(LookaheadRule(Reduction(TokenType::START,first_production),first_lahead));
    LR1_State* s = new LR1_State(first_kernel);
    std::queue<LR1_State*> construction_queue;
    construction_queue.push(s);
    while(!construction_queue.empty()){
        LR1_State* s = construction_queue.front();
        construction_queue.pop();
        this->add_state(s);
        s->compute_closure(g);
        std::map<TokenType,LR1_State*> neighbours = s->compute_neighbours();
        for(std::pair<TokenType,LR1_State*> neigh_pair: neighbours){
            LR1_State* old = this->contains_state(neigh_pair.second);
            if(old == nullptr){
                s->add_neighbour(neigh_pair.first,neigh_pair.second);
                construction_queue.push(neigh_pair.second);
            }else{
                s->add_neighbour(neigh_pair.first,old);
                delete neigh_pair.second;
            }
        }
    }
}
LR1_State* LR1_Automaton::get_initial_state(){
    if(this->states.size() == 0){
        return nullptr;
    }
    return this->states.at(0);
}
LR1_Automaton::~LR1_Automaton(){
    for(LR1_State* state: this->states){
        delete state;
    }
}
void LR1_Automaton::add_state(LR1_State* s){
    if(this->contains_state(s))
        return;
    this->states.push_back(s);
}
LR1_State* LR1_Automaton::contains_state(LR1_State* s){
    for(LR1_State* state: states){
        if((*state) == (*s)){
            return state;
        }
    }
    return nullptr;
}
std::ostream& operator<<(std::ostream& os, const LR1_Automaton& a){
    int count = 0;
    for(LR1_State* s: a.states){
        os<<(count++)<<": "<<s<<std::endl;
        os<<"["<<std::endl;
        os<<(*s)<<"]"<<std::endl;
    }
    return os;
}
void LR1_Automaton::print()const{
    std::cout<<(*this);
}