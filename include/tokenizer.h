#ifndef __TOKENIZER__H__
#define __TOKENIZER__H__

#include <vector>
#include <string>
#include <optional>
#include <sstream>

#include "token.h"
#include "keywords.h"
#include "errors.h"

class TokenizedData{
    private:
    int index;
    std::vector<Token> tokens;
    std::vector<Token> tokenize(const std::string&);
    public:
    TokenizedData();
    TokenizedData(const std::string& data);
    ~TokenizedData();
    int size();
    void move_forward();
    std::optional<Token> peek_next_token();
    void move_backwards();
    std::optional<Token> peek_previous_token();
    std::optional<Token> current_token();
    bool has_next();
    bool has_previous();
    bool is_empty();
    void reset_pointer();
};

#endif
