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

std::string AST::compile_rec(std::set<std::string>& vars, Labelgenerator& label_gen, bool rvalue, const std::string& entry_block_label, const std::string& exit_block_label){
    switch (this->reduction_kind)
    {
        case ReductionKind::START__CODE:
        {
            return this->children[0]->compile_rec(vars, label_gen, false, entry_block_label, exit_block_label);
        }
        case ReductionKind::CODE__STATEMENT:
        {
            return this->children[0]->compile_rec(vars, label_gen, false, entry_block_label, exit_block_label);
        }
        case ReductionKind::CODE__STATEMENT_SEMI_CODE:
        {
            std::string left = this->children[0]->compile_rec(vars, label_gen, false, entry_block_label, exit_block_label);
            return left + this->children[2]->compile_rec(vars, label_gen, false, entry_block_label, exit_block_label);
        }
        case ReductionKind::CODE__CONDITIONAL_CODE:
        {
            std::string left = this->children[0]->compile_rec(vars, label_gen, false, entry_block_label, exit_block_label);
            std::string right = this->children[1]->compile_rec(vars, label_gen, false, entry_block_label, exit_block_label);
            return left + right;
        }
        case ReductionKind::STATEMENT__EQUALITY:
        {
            return this->children[0]->compile_rec(vars, label_gen, rvalue, entry_block_label, exit_block_label);
        }
        case ReductionKind::STATEMENT__RETURN:
        {
            return this->children[0]->compile_rec(vars, label_gen, rvalue, entry_block_label, exit_block_label);
        }
        case ReductionKind::RETURN__RET_OP:
        {
            std::string code = this->children[1]->compile_rec(vars, label_gen, true, entry_block_label, exit_block_label);
            return code + "\tpop rdi\n\tmov rax, 60\n\tsyscall\n";
        }
        case ReductionKind::ITEM__NAME:
        {
            return this->children[0]->compile_rec(vars, label_gen, rvalue, entry_block_label, exit_block_label);
        }
        case ReductionKind::ITEM__NUMBER: case ReductionKind::ITEM__TRUE: case ReductionKind::ITEM__FALSE:
        {
            return this->children[0]->compile_rec(vars, label_gen, false, entry_block_label, exit_block_label);
        }
        case ReductionKind::EQUALITY__NAME_EQ_OP:
        {
            std::string value = this->children[2]->compile_rec(vars, label_gen, true, entry_block_label, exit_block_label);
            std::string var = this->children[0]->compile_rec(vars, label_gen, false, entry_block_label, exit_block_label);
            return value + "\tpop rax\n\tmov " + var + ", rax\n";
        }
        // case ReductionKind::OP__MATHOPE:
        // {
        //     return this->children[0]->compile_rec(vars, label_gen, rvalue);
        // }
        case ReductionKind::CODE__CONDITIONAL:
        case ReductionKind::OP__BOOLOPE: 
        case ReductionKind::BOOLOPE__BOOLOPT:
        case ReductionKind::BOOLOPT__CMPOP:
        case ReductionKind::CMPOP__BITWISEOPE:
        case ReductionKind::BITWISEOPE__BITWISEOPT:
        case ReductionKind::BITWISEOPT__MATHOPE: 
        case ReductionKind::MOTHOPE__MATHOPT: 
        case ReductionKind::MATHOPT__MATHOPF:
        {
            return this->children[0]->compile_rec(vars, label_gen, rvalue, entry_block_label, exit_block_label);
        }
        case ReductionKind::BITWISEOPE__BITWISEOPE_BWAND_BITWISEOPT:
        {
            std::string left = this->children[0]->compile_rec(vars, label_gen, true, entry_block_label, exit_block_label);
            std::string right = this->children[2]->compile_rec(vars, label_gen, true, entry_block_label, exit_block_label);
            return left + right + "\tpop rax\n\tpop rdi\n\tand rax, rdi\n\tpush rax\n";
        }
        case ReductionKind::BITWISEOPE__BITWISEOPE_BWOR_BITWISEOPT:
        {
            std::string left = this->children[0]->compile_rec(vars, label_gen, true, entry_block_label, exit_block_label);
            std::string right = this->children[2]->compile_rec(vars, label_gen, true, entry_block_label, exit_block_label);
            return left + right + "\tpop rax\n\tpop rdi\n\tor rax, rdi\n\tpush rax\n";
        }
        case ReductionKind::BITWISEOPE__BITWISEOPE_BWXOR_BITWISEOPT:
        {
            std::string left = this->children[0]->compile_rec(vars, label_gen, true, entry_block_label, exit_block_label);
            std::string right = this->children[2]->compile_rec(vars, label_gen, true, entry_block_label, exit_block_label);
            return left + right + "\tpop rax\n\tpop rdi\n\txor rax, rdi\n\tpush rax\n";
        }
        case ReductionKind::BITWISEOPT__TILDE_BITWISEOPT:
        {
            std::string value = this->children[1]->compile_rec(vars, label_gen, true, entry_block_label, exit_block_label);
            return value + "\tpop rax\n\tnot rax\n\tpush rax\n";
        }
        case ReductionKind::CMPOP__CMPOP_EQEQ_BITWISEOPE:
        {
            std::string left = this->children[0]->compile_rec(vars, label_gen, true, entry_block_label, exit_block_label);
            std::string right = this->children[2]->compile_rec(vars, label_gen, true, entry_block_label, exit_block_label);
            return left + right + "\tpop rax\n\tpop rdi\n\tcmp rax, rdi\n\tsete al\n\tmovzx rax, al\n\tpush rax\n";
        }
        case ReductionKind::CMPOP__CMPOP_NOTEQ_BITWISEOPE:
        {
            std::string left = this->children[0]->compile_rec(vars, label_gen, true, entry_block_label, exit_block_label);
            std::string right = this->children[2]->compile_rec(vars, label_gen, true, entry_block_label, exit_block_label);
            return left + right + "\tpop rax\n\tpop rdi\n\tcmp rax, rdi\n\tsetne al\n\tmovzx rax, al\n\tpush rax\n";
        }
        case ReductionKind::CMPOP__CMPOP_GT_BITWISEOPE:
        {
            std::string left = this->children[0]->compile_rec(vars, label_gen, true, entry_block_label, exit_block_label);
            std::string right = this->children[2]->compile_rec(vars, label_gen, true, entry_block_label, exit_block_label);
            return left + right + "\tpop rdi\n\tpop rax\n\tcmp rax, rdi\n\tsetg al\n\tmovzx rax, al\n\tpush rax\n";
        }   
        case ReductionKind::CMPOP__CMPOP_LT_BITWISEOPE:
        {
            std::string left = this->children[0]->compile_rec(vars, label_gen, true, entry_block_label, exit_block_label);
            std::string right = this->children[2]->compile_rec(vars, label_gen, true, entry_block_label, exit_block_label);
            return left + right + "\tpop rdi\n\tpop rax\n\tcmp rax, rdi\n\tsetl al\n\tmovzx rax, al\n\tpush rax\n";
        }   
        case ReductionKind::CMPOP__CMPOP_GTEQ_BITWISEOPE:
        {
            std::string left = this->children[0]->compile_rec(vars, label_gen, true, entry_block_label, exit_block_label);
            std::string right = this->children[2]->compile_rec(vars, label_gen, true, entry_block_label, exit_block_label);
            return left + right + "\tpop rdi\n\tpop rax\n\tcmp rax, rdi\n\tsetge al\n\tmovzx rax, al\n\tpush rax\n";
        }   
        case ReductionKind::CMPOP__CMPOP_LTEQ_BITWISEOPE:
        {
            std::string left = this->children[0]->compile_rec(vars, label_gen, true, entry_block_label, exit_block_label);
            std::string right = this->children[2]->compile_rec(vars, label_gen, true, entry_block_label, exit_block_label);
            return left + right + "\tpop rdi\n\tpop rax\n\tcmp rax, rdi\n\tsetle al\n\tmovzx rax, al\n\tpush rax\n";
        }
        case ReductionKind::BOOLOPT__NOT_CMPOP:
        {
            std::string value = this->children[1]->compile_rec(vars, label_gen, true, entry_block_label, exit_block_label);
            return value + "\tpop rax\n\tcmp rax, 0\n\tsetne al\n\tmovzx rax, al\n\tpush rax\n";
        }
        case ReductionKind::MATHOPE__MATHOPE_PLUS_MATHOPT:
        {
            std::string left = this->children[0]->compile_rec(vars, label_gen, true, entry_block_label, exit_block_label);
            std::string right = this->children[2]->compile_rec(vars, label_gen, true, entry_block_label, exit_block_label);
            return left + right + "\tpop rax\n\tpop rdi\n\tadd rax, rdi\n\tpush rax\n";
        }
        case ReductionKind::MATHOPE__MATHOPE_MINUS_MATHOPT:
        {
            std::string left = this->children[0]->compile_rec(vars, label_gen, true, entry_block_label, exit_block_label);
            std::string right = this->children[2]->compile_rec(vars, label_gen, true, entry_block_label, exit_block_label);
            return left + right + "\tpop rdi\n\tpop rax\n\tsub rax, rdi\n\tpush rax\n";
        }
        // case ReductionKind::MOTHOPE__MATHOPT:
        // {
        //     return this->children[0]->compile_rec(vars, label_gen, rvalue);
        // }   
        // case ReductionKind::MATHOPT__MATHOPF:
        // {
        //     return this->children[0]->compile_rec(vars, label_gen, rvalue);
        // }
        case ReductionKind::MATHOPT__MATHOPT_DIV_MATHOPF:
        {
            std::string left = this->children[0]->compile_rec(vars, label_gen, true, entry_block_label, exit_block_label);
            std::string right = this->children[2]->compile_rec(vars, label_gen, true, entry_block_label, exit_block_label);
            return left + right + "\tpop rdi\n\tpop rax\n\tmov rdx, 0\n\tidiv rdi\n\tpush rax\n";
        }
        case ReductionKind::MATHOPT__MATHOPT_MOD_MATHOPF:
        {
            std::string left = this->children[0]->compile_rec(vars, label_gen, true, entry_block_label, exit_block_label);
            std::string right = this->children[2]->compile_rec(vars, label_gen, true, entry_block_label, exit_block_label);
            return left + right + "\tpop rdi\n\tpop rax\n\tmov rdx, 0\n\tidiv rdi\n\tpush rdx\n";
        }
        case ReductionKind::MATHOPT__MATHOPT_TIMES_MATHOPF:
        {
            std::string left = this->children[0]->compile_rec(vars, label_gen, true, entry_block_label, exit_block_label);
            std::string right = this->children[2]->compile_rec(vars, label_gen, true, entry_block_label, exit_block_label);
            return left + right + "\tpop rax\n\tpop rdi\n\timul rdi\n\tpush rax\n";
        }
        case ReductionKind::MATHOPF__ITEM:
        {
            std::string item_info = this->children[0]->compile_rec(vars, label_gen, rvalue, entry_block_label, exit_block_label);
            return "\tmov rax, " + item_info + "\n\tpush rax\n";
        }
        case ReductionKind::MATHOPF__OPENBRACKETS_OP_CLOSEBRACKETS:
        {
            return this->children[1]->compile_rec(vars, label_gen, rvalue, entry_block_label, exit_block_label);
        }
        case ReductionKind::MATHOPF__MINUS_MATHOPF:
        {
            std::string item_info = this->children[1]->compile_rec(vars, label_gen, rvalue, entry_block_label, exit_block_label);
            return item_info + "\tpop rax\n\tneg rax\n\tpush rax\n";
        }
        case ReductionKind::BLOCK__OPENCURLY_CODE_CLOSECURLY:
        {
            return this->children[1]->compile_rec(vars, label_gen, rvalue, entry_block_label, exit_block_label);
        }
        case ReductionKind::CONDITIONAL__IF_OPEN_OP_CLOSE_BLOCK:
        {
            std::string condition = this->children[1]->compile_rec(vars, label_gen, true, entry_block_label, exit_block_label);
            std::string block = this->children[3]->compile_rec(vars, label_gen, rvalue, entry_block_label, exit_block_label);
            std::string label = label_gen.get_label();
            return condition + "\tpop rax\n\tcmp rax, 0\n\tje " + label + "\n" + block + label + ":\n";
        }
        case ReductionKind::BOOLOPE__BOOLOPE_AND_BOOLOPT:
        {
            std::string left = this->children[0]->compile_rec(vars, label_gen, true, entry_block_label, exit_block_label);
            std::string right = this->children[2]->compile_rec(vars, label_gen, true, entry_block_label, exit_block_label);
            return left + right + "\tpop rax\n\tpop rdi\n\tand rax, rdi\n\tcmp rax, 0\n\tsetne al\n\tmovzx rax, al\n\tpush rax\n";
        }
        case ReductionKind::BOOLOPE__BOOLOPE_OR_BOOLOPT:
        {
            std::string left = this->children[0]->compile_rec(vars, label_gen, true, entry_block_label, exit_block_label);
            std::string right = this->children[2]->compile_rec(vars, label_gen, true, entry_block_label, exit_block_label);
            return left + right + "\tpop rax\n\tpop rdi\n\tor rax, rdi\n\tcmp rax, 0\n\tsetne al\n\tmovzx rax, al\n\tpush rax\n";
        }
        case ReductionKind::BOOLOPE__BOOLOPE_XOR_BOOLOPT:
        {
            std::string left = this->children[0]->compile_rec(vars, label_gen, true, entry_block_label, exit_block_label);
            std::string right = this->children[2]->compile_rec(vars, label_gen, true, entry_block_label, exit_block_label);
            return left + right + "\tpop rax\n\tpop rbx\n\tcmp rax, 0\n\tsetne al\n\tcmp rbx, 0\n\tsetne bl\n\txor al, bl\n\tmovzx rax, al\n\tpush rax\n";
        }
        case ReductionKind::CONDITIONAL__WHILE_OPEN_OP_CLOSE_BLOCK:
        {
            std::string entry_label = label_gen.get_label();
            std::string exit_label = label_gen.get_label();
            std::string condition = this->children[2]->compile_rec(vars, label_gen, true, entry_block_label, exit_block_label);
            std::string block = this->children[4]->compile_rec(vars, label_gen, rvalue, entry_label, exit_label);
            return entry_label + ":\n" + condition + "\tpop rax\n\tcmp rax, 0\n\tje " + 
                exit_label + "\n" + block + "\tjmp "+entry_label+"\n" + exit_label + ":\n";
        }
        case ReductionKind::CONDITIONAL__FOR_OPEN_EQUALITY_SEMI_OP_SEMI_EQUALITY_CLOSE_BLOCK:
        {
            std::string entry_label = label_gen.get_label();
            std::string exit_label = label_gen.get_label();
            std::string init = this->children[2]->compile_rec(vars, label_gen, rvalue, entry_block_label, exit_block_label);
            std::string condition = this->children[4]->compile_rec(vars, label_gen, true, entry_block_label, exit_block_label);
            std::string update = this->children[6]->compile_rec(vars, label_gen, true, entry_block_label, exit_block_label);
            std::string block = this->children[8]->compile_rec(vars, label_gen, rvalue, entry_label, exit_label);
            return init + entry_label + ":\n" + condition + "\tpop rax\n\tcmp rax, 0\n\tje " + 
                exit_label + "\n" + block + update + "\tjmp "+entry_label+"\n" + exit_label + ":\n";
        }
        case ReductionKind::STATEMENT__BREAK:
        {
            if(exit_block_label.empty()){
                errors::print_error("break statement outside of loop");
                exit(EXIT_FAILURE);
            }
            return "\tjmp " + exit_block_label + "\n";
        }
        case ReductionKind::STATEMENT__CONTINUE:
        {
            if(exit_block_label.empty()){
                errors::print_error("continue statement outside of loop");
                exit(EXIT_FAILURE);
            }
            return "\tjmp " + entry_block_label + "\n";
        }
        case ReductionKind::CONDITIONAL__IF_OPEN_OP_CLOSE_BLOCK_ELSE_BLOCK:
        {
            std::string condition = this->children[2]->compile_rec(vars, label_gen, true, entry_block_label, exit_block_label);
            std::string block = this->children[4]->compile_rec(vars, label_gen, rvalue, entry_block_label, exit_block_label);
            std::string else_block = this->children[6]->compile_rec(vars, label_gen, rvalue, entry_block_label, exit_block_label);
            std::string label = label_gen.get_label();
            std::string label2 = label_gen.get_label();
            return condition + "\tpop rax\n\tcmp rax, 0\n\tje " + label + "\n" + block + "\tjmp " + label2 + "\n" + label + ":\n" + else_block + label2 + ":\n";
        }
        case ReductionKind::EQUALITY__NAME_PLUPLUS:
        {
            std::string var = this->children[0]->compile_rec(vars, label_gen, true, entry_block_label, exit_block_label);
            return "\tmov rax, " + var + "\n\tadd rax, 1\n\tmov " + var + ", rax\n";
        }
        case ReductionKind::EQUALITY__NAME_MINUSMINUS:
        {
            std::string var = this->children[0]->compile_rec(vars, label_gen, true, entry_block_label, exit_block_label);
            return "\tmov rax, " + var + "\n\tsub rax, 1\n\tmov " + var + ", rax\n";
        }
        case ReductionKind::EQUALITY__NAME_PLUSEQ_OP:
        {
            std::string var = this->children[0]->compile_rec(vars, label_gen, true, entry_block_label, exit_block_label);
            std::string value = this->children[2]->compile_rec(vars, label_gen, true, entry_block_label, exit_block_label);
            return value + "\tpop rax\n\tmov rdi, " + var + "\n\tadd rdi, rax\n\tmov " + var + ", rdi\n";
        }
        case ReductionKind::EQUALITY__NAME_MINUSEQ_OP:
        {
            std::string var = this->children[0]->compile_rec(vars, label_gen, true, entry_block_label, exit_block_label);
            std::string value = this->children[2]->compile_rec(vars, label_gen, true, entry_block_label, exit_block_label);
            return value + "\tpop rax\n\tmov rdi, " + var + "\n\tsub rdi, rax\n\tmov " + var + ", rdi\n";
        }
        case ReductionKind::EQUALITY__NAME_TIMESEQ_OP:
        {
            std::string var = this->children[0]->compile_rec(vars, label_gen, true, entry_block_label, exit_block_label);
            std::string value = this->children[2]->compile_rec(vars, label_gen, true, entry_block_label, exit_block_label);
            return value + "\tpop rdi\n\tmov rax, " + var + "\n\timul rdi\n\tmov " + var + ", rax\n";
        }
        case ReductionKind::EQUALITY__NAME_DIVIDEEQ_OP:
        {
            std::string var = this->children[0]->compile_rec(vars, label_gen, true, entry_block_label, exit_block_label);
            std::string value = this->children[2]->compile_rec(vars, label_gen, true, entry_block_label, exit_block_label);
            return value + "\tpop rdi\n\tmov rax, " + var + "\n\tmov rdx, 0\n\tidiv rdi\n\tmov " + var + ", rax\n";
        }
        case ReductionKind::EQUALITY__NAME_MODULUSEQ_OP:
        {
            std::string var = this->children[0]->compile_rec(vars, label_gen, true, entry_block_label, exit_block_label);
            std::string value = this->children[2]->compile_rec(vars, label_gen, true, entry_block_label, exit_block_label);
            return value + "\tpop rdi\n\tmov rax, " + var + "\n\tmov rdx, 0\n\tidiv rdi\n\tmov " + var + ", rdx\n";
        }
        case ReductionKind::NOT_A_REDUCTION:
        {
            switch(this->item.type){
                case TokenType::name:{
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
                case TokenType::int_literal:{
                    return this->item.value.value();
                }
                case TokenType::_true:{
                    return "1";
                }
                case TokenType::_false:{
                    return "0";
                }
                default:{
                    std::cout<<"ERROR: "<<this->item<<std::endl;
                    return std::string();
                }
            }
        }
        default:{
            std::cout<<"ERROR: "<<this->reduction_kind<<std::endl;
            errors::print_error("Unknown reduction kind");
            exit(EXIT_FAILURE);
        }
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
    Labelgenerator label_gen = Labelgenerator();
    std::string main_code = "_start:\n" + this->compile_rec(defined_variables,label_gen,false,"_start","");
    std::string bss_code = this->get_bss(defined_variables);
    std::string data_code = this->get_data();
    std::string text_code = this->get_text();

    main_code = remove_push_pop(main_code);

    return data_code + bss_code + text_code + main_code;
}

std::string remove_push_pop(std::string str){
    std::regex pattern("\tpush rax\n\tpop rax\n");
    return std::regex_replace(str, pattern, "");
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