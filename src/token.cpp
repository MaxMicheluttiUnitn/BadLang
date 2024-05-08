#include "token.h"

Token::Token(){
    this->type = TokenType::NoneType;
}
Token::Token(const TokenType& type){
    this->type = type;
}
Token::Token(const TokenType& type, const std::string& value){
    this->type = type;
    this->value = value;
}
Token::~Token(){}
Token::Token(const Token& token){
    this->type = token.type;
    this->value = token.value;
}
std::ostream & operator<<(std::ostream & os, const TokenType& type){
    switch(type){
        case TokenType::name: os<<"name";break;
        case TokenType::_divide: os<<"_divide";break;
        case TokenType::_elif: os<<"_elif";break;
        case TokenType::_else: os<<"_else";break;
        case TokenType::_equals: os<<"_equals";break;
        case TokenType::_for: os<<"_for";break;
        case TokenType::_if: os<<"_if";break;
        case TokenType::_minus: os<<"_minus";break;
        case TokenType::_plus: os<<"_plus";break;
        case TokenType::_return: os<<"_return";break;
        case TokenType::_times: os<<"_times";break;
        case TokenType::_while: os<<"_while";break;
        case TokenType::int_literal: os<<"int_literal";break;
        case TokenType::semicolon: os<<"semicolon";break;
        case TokenType::_close_brackets: os<<"_closed brackets";break;
        case TokenType::_open_brackets: os<<"_opened brackets";break;
        case TokenType::START: os<<"START SYMBOL";break;
        case TokenType::STATEMENT: os<<"STATEMENT";break;
        case TokenType::CODE: os<<"CODE";break;
        case TokenType::MATH_OP: os<<"MATH_OP";break;
        case TokenType::MATH_OP_E: os<<"MATH_OP_E";break;
        case TokenType::MATH_OP_F: os<<"MATH_OP_F";break;
        case TokenType::MATH_OP_T: os<<"MATH_OP_T";break;
        case TokenType::EQUALITY: os<<"EQUALITY";break;
        case TokenType::RETURN: os<<"RETURN";break;
        case TokenType::ITEM: os<<"ITEM";break;
        case TokenType::_end: os<<"$";break;
        default: 
        case TokenType::NoneType:os<<"error_token";break;
    }
    return os;
}
bool tokentype_is_literal(const TokenType& t){
    switch(t){
        case TokenType::START:
        case TokenType::STATEMENT:
        case TokenType::CODE:
        case TokenType::MATH_OP:
        case TokenType::MATH_OP_E:
        case TokenType::MATH_OP_F:
        case TokenType::MATH_OP_T:
        case TokenType::EQUALITY:
        case TokenType::RETURN:
        case TokenType::ITEM: 
        case TokenType::NoneType: return false;
        default: return true;
    }
}
bool tokentype_is_production_symbol(const TokenType& t){
    switch(t){
        case TokenType::START:
        case TokenType::STATEMENT:
        case TokenType::CODE:
        case TokenType::MATH_OP:
        case TokenType::MATH_OP_E:
        case TokenType::MATH_OP_F:
        case TokenType::MATH_OP_T:
        case TokenType::EQUALITY:
        case TokenType::RETURN:
        case TokenType::ITEM: return true;
        default: return false;
    }
}

std::ostream & operator<<(std::ostream & os, const Token& token){
    os<<"[ TYPE = "<<token.type<<" VALUE = ";
    if(token.value.has_value()){
        os<<token.value.value();
    }else{
        os<<"None";
    }
    os<<" ]";
    return os;
}