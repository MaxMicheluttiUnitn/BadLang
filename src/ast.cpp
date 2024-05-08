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
std::vector<AST*> AST::get_children()const{
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