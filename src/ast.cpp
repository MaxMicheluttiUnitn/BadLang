#include "ast.h"

AST::AST(){
    this->item = Token();
    this->children = std::vector<AST*>();
}
AST::~AST(){
    // recursively free children
    for(AST* child: this->children){
        delete child;
    }
}
AST::AST(Token t,std::vector<AST*> children, ReductionKind reduction_id){
    this->item = t;
    this->children = children;
    this->reduction_kind = reduction_id;
}
AST::AST(Token t){
    this->item = t;
    this->children = std::vector<AST*>();
    this->reduction_kind = ReductionKind::NOT_A_REDUCTION;
}
Token AST::get_token()const{
    return this->item;
}
TokenType AST::get_type()const{
    return this->item.type;
}

std::string AST::compile_rec(std::set<std::string>& vars, bool rvalue){
    switch (this->reduction_kind)
    {
        case ReductionKind::START__CODE:
        {
            return this->children[0]->compile_rec(vars,false);
        }
        case ReductionKind::CODE__STATEMENT:
        {
            return this->children[0]->compile_rec(vars, false);
        }
        case ReductionKind::CODE__STATEMENT_SEMI_CODE:
        {
            std::string left = this->children[0]->compile_rec(vars, false);
            return left + this->children[2]->compile_rec(vars, false);
        }
        case ReductionKind::STATEMENT__EQUALITY:
        {
            return this->children[0]->compile_rec(vars, rvalue);
        }
        case ReductionKind::STATEMENT__RETURN:
        {
            return this->children[0]->compile_rec(vars, rvalue);
        }
        case ReductionKind::RETURN__RET_ITEM:
        {
            return "\tmov rdi, " + this->children[1]->compile_rec(vars, true) + "\n\tmov rax, 60\n\tsyscall\n";
        }
        case ReductionKind::ITEM__NAME:
        {
            return this->children[0]->compile_rec(vars, rvalue);
        }
        case ReductionKind::ITEM__NUMBER:
        {
            return this->children[0]->compile_rec(vars, false);
        }
        case ReductionKind::EQUALITY__NAME_EQ_MATHOP:
        {
            std::string value = this->children[2]->compile_rec(vars, true);
            std::string var = this->children[0]->compile_rec(vars, false);
            return value + "\tpop rax\n\tmov " + var + ", rax\n";
        }
        case ReductionKind::MATHOP__MATHOPE:
        {
            return this->children[0]->compile_rec(vars, rvalue);
        }
        case ReductionKind::MATHOPE__MATHOPT_PLUS_MATHOPE:
        {
            std::string left = this->children[0]->compile_rec(vars, true);
            std::string right = this->children[2]->compile_rec(vars, true);
            return left + right + "\tpop rdi\n\tpop rax\n\tadd rax, rdi\n\tpush rax\n";
        }
        case ReductionKind::MATHOPE__MATHOPT_MINUS_MATHOPE:
        {
            std::string left = this->children[0]->compile_rec(vars, true);
            std::string right = this->children[2]->compile_rec(vars, true);
            return left + right + "\tpop rdi\n\tpop rax\n\tsub rax, rdi\n\tpush rax\n";
        }
        case ReductionKind::MOTHOPE__MATHOPT:
        {
            return this->children[0]->compile_rec(vars, rvalue);
        }   
        case ReductionKind::MATHOPT__MATHOPF:
        {
            return this->children[0]->compile_rec(vars, rvalue);
        }
        case ReductionKind::MATHOPT__MATHOPF_DIV_MATHOPT:
        {
            std::string left = this->children[0]->compile_rec(vars, true);
            std::string right = this->children[2]->compile_rec(vars, true);
            return left + right + "\tpop rdi\n\tpop rax\n\tmov rdx, 0\n\tidiv rdi\n\tpush rax\n";
        }
        case ReductionKind::MATHOPT__MATHOPF_TIMES_MATHOPT:
        {
            std::string left = this->children[0]->compile_rec(vars, true);
            std::string right = this->children[2]->compile_rec(vars, true);
            return left + right + "\tpop rdi\n\tpop rax\n\timul rax\n\tpush rax\n";
        }
        case ReductionKind::MATHOPF__ITEM:
        {
            std::string item_info = this->children[0]->compile_rec(vars, rvalue);
            return "\tmov rax, " + item_info + "\n\t push rax\n";
        }
        case ReductionKind::MATHOPF__OPENBRACKETS_MATHOPE_CLOSEBRACKETS:
        {
            return this->children[1]->compile_rec(vars, rvalue);
        }
        case ReductionKind::MATHOPF__MINUS_MATHOPF:
        {
            std::string item_info = this->children[1]->compile_rec(vars, rvalue);
            return item_info + "\tpop rax\n\tneg rax\n\tpush rax\n";
        }
        case ReductionKind::NOT_A_REDUCTION:
            {
                if(this->item.type == TokenType::name){
                    if(!rvalue){
                        vars.insert(this->item.value.value());
                    }else{
                        if(vars.find(this->item.value.value()) == vars.end()){
                            errors::print_error("variable "+this->item.value.value()+" not defined");
                            exit(EXIT_FAILURE);
                        }
                    }
                    return "[var_" + this->item.value.value()+"]";
                }
                else if(this->item.type == TokenType::int_literal){
                    return this->item.value.value();
                }
                else{
                    std::cout<<"ERROR: "<<this->item<<std::endl;
                    return std::string();
                }
            }
        default:
            errors::print_error("Unknown reduction kind");
            exit(EXIT_FAILURE);
    }
}

std::string AST::get_data(){
    return "";
}

std::string AST::get_text(){
    return "section .text\n\tglobal _start\n\n";
}

std::string AST::get_bss(const std::set<std::string>& vars){
    if(vars.empty()){
        return "";
    }
    std::string res = "section .bss\n";
    for(std::string var: vars){
        res += "\tvar_" + var + " resq 1\n";
    }
    return res+"\n";
}

std::string AST::compile()
{
    std::set<std::string> defined_variables = std::set<std::string>();
    std::string main_code = "_start:\n" + this->compile_rec(defined_variables,false);
    std::string bss_code = this->get_bss(defined_variables);
    std::string data_code = this->get_data();
    std::string text_code = this->get_text();

    return data_code + bss_code + text_code + main_code;
}
std::vector<AST *> AST::get_children() const
{
    return this->children;
}
void AST::print_depth(std::ostream& os,int depth)const{
    for(int i=0;i<depth;i++){
        os<<"\t";
    }
    os<<this->item<<"\t"<<this->reduction_kind<<std::endl;
    for(AST* c: this->children){
        c->print_depth(os,depth+1);
    }
}
void AST::print(std::ostream& os)const{
    this->print_depth(os,0);
    os.flush();
}