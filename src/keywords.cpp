#include "keywords.h"

Keywords::Keywords(){
    this->items = std::map<std::string,Token>();
    this->items.insert(std::make_pair(std::string("return"),Token(TokenType::_return)));
    // this->items.insert(std::make_pair(std::string("for"),Token(TokenType::_for)));
    // this->items.insert(std::make_pair(std::string("while"),Token(TokenType::_while)));
    this->items.insert(std::make_pair(std::string("if"),Token(TokenType::_if)));
    this->items.insert(std::make_pair(std::string("else"),Token(TokenType::_else)));
    // this->items.insert(std::make_pair(std::string("elif"),Token(TokenType::_elif)));
}
Keywords::~Keywords(){}

bool Keywords::iskeyword(const std::string& item){
    return this->items.contains(item);    
}
Token Keywords::getkwtoken(const std::string& item){
    if(!(this->iskeyword(item))){
        return Token();
    }
    return Token((*(this->items.find(item))).second);

}