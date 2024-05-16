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
        case TokenType::_close_brackets: os<<"_closed_brackets";break;
        case TokenType::_open_brackets: os<<"_opened_brackets";break;
        case TokenType::START: os<<"START SYMBOL";break;
        case TokenType::STATEMENT: os<<"STATEMENT";break;
        case TokenType::CODE: os<<"CODE";break;
        case TokenType::OP: os<<"OP";break;
        case TokenType::MATH_OP_E: os<<"MATH_OP_E";break;
        case TokenType::MATH_OP_F: os<<"MATH_OP_F";break;
        case TokenType::MATH_OP_T: os<<"MATH_OP_T";break;
        case TokenType::EQUALITY: os<<"EQUALITY";break;
        case TokenType::RETURN: os<<"RETURN";break;
        case TokenType::ITEM: os<<"ITEM";break;
        case TokenType::_end: os<<"$";break;
        case TokenType::BLOCK: os<<"BLOCK";break;
        case TokenType::_open_curly: os<<"_open_curly";break;
        case TokenType::_close_curly: os<<"_close_curly";break;
        case TokenType::CONDITIONAL: os<<"CONDITIONAL";break;
        case TokenType::_modulus: os<<"_modulus";break;
        case TokenType::_true: os<<"_true";break;
        case TokenType::_false: os<<"_false";break;
        case TokenType::BOOL_OP_E: os<<"BOOL_OP_E";break;
        case TokenType::BOOL_OP_T: os<<"BOOL_OP_T";break;
        case TokenType::CMP_OP: os<<"CMP_OP";break;
        case TokenType::BITWISE_OP_E: os<<"BITWISE_OP_E";break;
        case TokenType::BITWISE_OP_T: os<<"BITWISE_OP_T";break;
        case TokenType::_and: os<<"_and";break;
        case TokenType::_or: os<<"_or";break;
        case TokenType::_not: os<<"_not";break;
        case TokenType::_lt: os<<"_lt";break;
        case TokenType::_gt: os<<"_gt";break;
        case TokenType::_equal_equal: os<<"_equal_equal";break;
        case TokenType::_not_equal: os<<"_not_equal";break;
        case TokenType::_lt_eq: os<<"_lt_eq";break;
        case TokenType::_gt_eq: os<<"_gt_eq";break;
        case TokenType::_bitwise_and: os<<"_bitwise_and";break;
        case TokenType::_bitwise_or: os<<"_bitwise_or";break;
        case TokenType::_tilde: os<<"_tilde";break;
        case TokenType::_break: os<<"_break";break;
        case TokenType::_continue: os<<"_continue";break;
        case TokenType::_plusplus: os<<"_plusplus";break;
        case TokenType::_minusminus: os<<"_minusminus";break;
        case TokenType::_xor: os<<"_xor";break;
        case TokenType::_bitwise_xor: os<<"_bitwise_xor";break;
        case TokenType::_pluseq: os<<"_pluseq";break;
        case TokenType::_minuseq: os<<"_minuseq";break;
        case TokenType::_timeseq: os<<"_timeseq";break;
        case TokenType::_divideeq: os<<"_divideeq";break;
        case TokenType::_moduluseq: os<<"_moduluseq";break;
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
        case TokenType::OP:
        case TokenType::BOOL_OP_E:
        case TokenType::BOOL_OP_T:
        case TokenType::CMP_OP:
        case TokenType::BITWISE_OP_E:
        case TokenType::BITWISE_OP_T:
        case TokenType::MATH_OP_E:
        case TokenType::MATH_OP_F:
        case TokenType::MATH_OP_T:
        case TokenType::EQUALITY:
        case TokenType::RETURN:
        case TokenType::ITEM: 
        case TokenType::BLOCK:
        case TokenType::CONDITIONAL:
        case TokenType::NoneType: return false;
        default: return true;
    }
}
bool tokentype_is_production_symbol(const TokenType& t){
    switch(t){
        case TokenType::START:
        case TokenType::STATEMENT:
        case TokenType::CODE:
        case TokenType::OP:
        case TokenType::BOOL_OP_E:
        case TokenType::BOOL_OP_T:
        case TokenType::CMP_OP:
        case TokenType::BITWISE_OP_E:
        case TokenType::BITWISE_OP_T:
        case TokenType::MATH_OP_E:
        case TokenType::MATH_OP_F:
        case TokenType::MATH_OP_T:
        case TokenType::EQUALITY:
        case TokenType::RETURN:
        case TokenType::BLOCK:
        case TokenType::CONDITIONAL:
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