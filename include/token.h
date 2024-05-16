#ifndef __TOKEN__H__
#define __TOKEN__H__
#include <optional>
#include <string>
#include <iostream>

enum class TokenType{
    NoneType,
    _end,
    START,
    CODE,
    STATEMENT,
    EQUALITY,
    RETURN,
    OP,
    BOOL_OP_E,
    BOOL_OP_T,
    CMP_OP,
    BITWISE_OP_T,
    BITWISE_OP_E,
    MATH_OP_E,
    MATH_OP_T,
    MATH_OP_F,
    ITEM,
    BLOCK,
    CONDITIONAL,
    _return,
    _if,
    _else,
    _for,
    _while,
    _elif,
    name,
    int_literal,
    semicolon,
    _true,
    _false,
    _equals,
    _plus,
    _minus,
    _times,
    _divide,
    _modulus,
    _open_brackets,
    _close_brackets,
    _open_curly,
    _close_curly,
    _and,
    _or,
    _bitwise_and,
    _bitwise_or,
    _equal_equal,
    _not_equal,
    _lt_eq,
    _gt_eq,
    _not,
    _lt,
    _gt,
    _tilde,
    _break,
    _continue,
};

class Token;

std::ostream & operator<<(std::ostream &, const Token&);

std::ostream & operator<<(std::ostream &, const TokenType&);

bool tokentype_is_literal(const TokenType&);
bool tokentype_is_production_symbol(const TokenType&);

class Token{
    public:
    TokenType type;
    std::optional<std::string> value;

    Token();
    Token(const TokenType& type);
    Token(const TokenType& type, const std::string& value);
    Token(const Token& token);
    ~Token();

    friend std::ostream & operator<<(std::ostream &, const Token&);
};

#endif
