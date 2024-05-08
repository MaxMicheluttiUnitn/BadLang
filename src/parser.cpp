#include "parser.h"

Parser::Parser(const Grammar& g):automaton(g){
    this->state_stack = std::stack<LR1_State*>();
    this->symbol_stack = std::stack<Token>();
}
Parser::~Parser(){}

AST* Parser::parse(const Grammar& g, TokenizedData& token_manager){
    // while(token_manager.has_next()){
    //     std::cout<<token_manager.current_token().value()<<std::endl;
    //     token_manager.move_forward();
    // }
    // token_manager.reset_pointer();
    LR1_State* init_state = this->automaton.get_initial_state();
    if(init_state  == nullptr){
        errors::print_error("error initializing LR1 automaton");
        exit(EXIT_FAILURE);
    }
    std::stack<AST*> nodes_stack  = std::stack<AST*>();
    state_stack.push(init_state);
    token_manager.reset_pointer();
    AST* root = nullptr;
    // I can safely unwrap this
    Token b = token_manager.current_token().value();
    // std::cout<<(this->automaton)<<std::endl;
    // std::cout.flush();
    TokenType b_type = b.type;
    while(true){
        LR1_State* s = state_stack.top();
        LR1_State* shift = s->get_neighbour(b_type);
        std::optional<DottedRule> reduce = s->get_reduce(b_type);
        if(s->accepts(b_type)){
            //std::cout<<"ACCEPT"<<std::endl;
            root = nodes_stack.top();
            break;
        }else if(shift != nullptr){
            //std::cout<<"SHIFT"<<std::endl;
            symbol_stack.push(b);
            nodes_stack.push(new AST(b));
            token_manager.move_forward();
            state_stack.push(shift);
            b = token_manager.current_token().value();
            b_type = b.type;
        }else if(reduce != std::nullopt){
            //std::cout<<"REDUCE"<<std::endl;
            DottedRule reduction = reduce.value();
            std::vector<AST*> children = std::vector<AST*>(reduction.get_right().size());
            for(int i=0; i<reduction.get_right().size();i++){
                symbol_stack.pop();
                state_stack.pop();
                children.at(reduction.get_right().size() - 1 - i) = nodes_stack.top();
                nodes_stack.pop();
            }
            Reduction undotted_reduction = reduction.get_reduction();
            symbol_stack.push(Token(reduction.get_left()));
            nodes_stack.push(new AST(reduction.get_left(),children,g.get_kind_of_reduction(undotted_reduction)));
            LR1_State* tmp = state_stack.top();
            LR1_State* big_t = tmp->get_neighbour(reduction.get_left());
            if(big_t == nullptr){
                errors::print_error("syntax error null");
                exit(EXIT_FAILURE);
            }
            state_stack.push(big_t);
            //std::cout<<reduction<<std::endl;
        }else{
            errors::print_error("syntax error unexpected token");
            exit(EXIT_FAILURE);
        }
    }
    token_manager.reset_pointer();
    return root;
}