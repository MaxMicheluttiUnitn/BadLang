#include "tokenizer.h"

TokenizedData::TokenizedData(){
    this->index = 0;
    this->tokens = std::vector<Token>();
}
TokenizedData::TokenizedData(const std::string& data){
    this->index = 0;
    this->tokens = this->tokenize(data);
}
TokenizedData::~TokenizedData(){}
int TokenizedData::size(){
    return this->tokens.size();
}
std::optional<Token> TokenizedData::peek_next_token(){
    if(this->index == this->size() - 1){
        return std::nullopt;
    }else{
        return this->tokens.at(this->index + 1);
    }
}
std::optional<Token> TokenizedData::peek_previous_token(){
     if(this->index < 1){
        return std::nullopt;
    }else{
        return this->tokens.at(this->index - 1);
    }
}
std::optional<Token> TokenizedData::current_token(){
    if(this->index < 0 || this->index == this->size()){
        return std::nullopt;
    }else{
        return this->tokens.at(this->index);
    }
}
void TokenizedData::move_forward(){
    if(this->index != this->size()){
        this->index++;
    }
}
void TokenizedData::move_backwards(){
    if(this->index>0){
        this->index--;
    }
}
bool TokenizedData::has_next(){
    return (this->index < this->size());
}
bool TokenizedData::has_previous(){
    return (this->index > 0);
}
bool TokenizedData::is_empty(){
    return (this->size() == 0);
}
std::vector<Token> TokenizedData::tokenize(const std::string& data){
    std::vector<Token> tokens;
    std::stringstream buffer;
    Keywords kws;
    bool rvalue = false;
    for(unsigned int i=0;i<data.length();i++){
        char current_char = data.at(i);
        if(iswspace(current_char) || (current_char == '\n') || (current_char == '\r') || (current_char == '\t')){continue;}
        if(current_char == '\0'){break;}
        if(isalpha(current_char) || (current_char=='_')){
            do{
                buffer << current_char;
                i++;
                if(i == data.length()){break;}
                current_char = data.at(i);
            }while(isalnum(current_char) || (current_char=='_'));
            i--;
            std::string token_data;
            buffer >> token_data;
            buffer.clear();
            if(kws.iskeyword(token_data)){
                tokens.push_back(kws.getkwtoken(token_data));
                continue;
            }else{
                tokens.push_back(Token(TokenType::name,token_data));
                continue;
            }
        }
        if(isdigit(current_char)){
            if(current_char == '0'){
                i++;
                if(i == data.length()){
                    tokens.push_back(Token(TokenType::int_literal,"0"));
                    continue;
                }
                current_char = data.at(i);
                if(isalnum(current_char) || current_char == '_' || current_char == '='){
                    errors::print_error("number starting with \"0\" and continuing found");
                    exit(EXIT_FAILURE);
                }else{
                    tokens.push_back(Token(TokenType::int_literal,"0"));
                    i--;
                    continue;
                }
            }else{
                do{
                    buffer << current_char;
                    i++;
                    if(i == data.length()){break;}
                    current_char = data.at(i);
                }while(isdigit(current_char));
                if(isalpha(current_char) || current_char == '_' || current_char == '='){
                    errors::print_error("number ending in invalid symbol found");
                    exit(EXIT_FAILURE);
                }
                i--;
                std::string token_data;
                buffer >> token_data;
                buffer.clear();
                tokens.push_back(Token(TokenType::int_literal,token_data));
                continue;
            } 
        }
        if(current_char == ';'){
            tokens.push_back(Token(TokenType::semicolon));
            rvalue = false;
            continue;
        }
        if(current_char == '='){
            i++;
            if(i == data.length()){
                tokens.push_back(Token(TokenType::_equals));
                rvalue = true;
                continue;
            }else if(data.at(i) == '='){
                tokens.push_back(Token(TokenType::_equal_equal));
                continue;
            }else{
                i--;
                tokens.push_back(Token(TokenType::_equals));
                rvalue = true;
                continue;
            }
        }
        if(current_char == '('){
            tokens.push_back(Token(TokenType::_open_brackets));
            continue;
        }
        if(current_char == ')'){
            tokens.push_back(Token(TokenType::_close_brackets));
            continue;
        }
        if(current_char == '{'){
            tokens.push_back(Token(TokenType::_open_curly));
            rvalue = false;
            continue;
        }
        if(current_char == '}'){
            tokens.push_back(Token(TokenType::_close_curly));
            continue;
        }
        if(current_char == '+'){
            i++;
            if(i == data.length()){
                tokens.push_back(Token(TokenType::_plus));
                continue;
            }else if(data.at(i) == '+'){
                tokens.push_back(Token(TokenType::_plusplus));
                continue;
            }else if(data.at(i) == '='){
                tokens.push_back(Token(TokenType::_pluseq));
                continue;
            }else{
                i--;
                tokens.push_back(Token(TokenType::_plus));
                continue;
            }
        }
        if(current_char == '-'){
            if(rvalue){
                tokens.push_back(Token(TokenType::_minus));
                continue;
            }
            i++;
            if(i == data.length()){
                tokens.push_back(Token(TokenType::_minus));
                continue;
            }else if(data.at(i) == '-'){
                tokens.push_back(Token(TokenType::_minusminus));
                continue;
            }else if(data.at(i) == '='){
                tokens.push_back(Token(TokenType::_minuseq));
                continue;
            }else{
                i--;
                tokens.push_back(Token(TokenType::_minus));
                continue;
            }
        }
        if(current_char == '*'){
            i++;
            if(i == data.length()){
                tokens.push_back(Token(TokenType::_times));
                continue;
            }else if(data.at(i) == '='){
                tokens.push_back(Token(TokenType::_timeseq));
                continue;
            }else{
                i--;
                tokens.push_back(Token(TokenType::_times));
                continue;
            }
        }
        if(current_char == '%'){
            i++;
            if(i == data.length()){
                tokens.push_back(Token(TokenType::_modulus));
                continue;
            }else if(data.at(i) == '='){
                tokens.push_back(Token(TokenType::_moduluseq));
                continue;
            }else{
                i--;
                tokens.push_back(Token(TokenType::_modulus));
                continue;
            }
        }
        if(current_char == '&'){
            i++;
            if(i == data.length()){
                tokens.push_back(Token(TokenType::_bitwise_and));
                continue;
            }else if(data.at(i) == '&'){
                tokens.push_back(Token(TokenType::_and));
                continue;
            }else{
                i--;
                tokens.push_back(Token(TokenType::_bitwise_and));
                continue;
            }
        }
        if(current_char == '|'){
            i++;
            if(i == data.length()){
                tokens.push_back(Token(TokenType::_bitwise_or));
                continue;
            }else if(data.at(i) == '|'){
                tokens.push_back(Token(TokenType::_or));
                continue;
            }else{
                i--;
                tokens.push_back(Token(TokenType::_bitwise_or));
                continue;
            }
        }
        if(current_char == '^'){
            i++;
            if(i == data.length()){
                tokens.push_back(Token(TokenType::_bitwise_xor));
                continue;
            }else if(data.at(i) == '^'){
                tokens.push_back(Token(TokenType::_xor));
                continue;
            }else{
                i--;
                tokens.push_back(Token(TokenType::_bitwise_xor));
                continue;
            }
        }
        if(current_char == '~'){
            tokens.push_back(Token(TokenType::_tilde));
            continue;
        }
        if(current_char == '!'){
            i++;
            if(i == data.length()){
                tokens.push_back(Token(TokenType::_not));
                continue;
            }else if(data.at(i) == '='){
                tokens.push_back(Token(TokenType::_not_equal));
                continue;
            }else{
                i--;
                tokens.push_back(Token(TokenType::_not));
                continue;
            }
        }
        if(current_char == '>'){
            i++;
            if(i == data.length()){
                tokens.push_back(Token(TokenType::_gt));
                continue;
            }else if(data.at(i) == '='){
                tokens.push_back(Token(TokenType::_gt_eq));
                continue;
            }else{
                i--;
                tokens.push_back(Token(TokenType::_gt));
                continue;
            }
        }
        if(current_char == '<'){
            i++;
            if(i == data.length()){
                tokens.push_back(Token(TokenType::_lt));
                continue;
            }else if(data.at(i) == '='){
                tokens.push_back(Token(TokenType::_lt_eq));
                continue;
            }else{
                i--;
                tokens.push_back(Token(TokenType::_lt));
                continue;
            }
        }
        if(current_char == '/'){
            // can be comment or division
            i++;
            if(i == data.length()){
                tokens.push_back(Token(TokenType::_divide));
                continue;
            }
            current_char = data.at(i);
            if(current_char == '/'){
                // this is a one-line comment ignore rest of line
                while (current_char != '\n' && current_char != '\0')
                {
                    i++;
                    if(i == data.length()){break;}
                    current_char = data.at(i);
                }
                i--;
                continue;
            }else if(current_char == '*'){
                // this is a multiline comment ignore unitl */ is found
                bool last_was_star = false;
                while(true){
                    i++;
                    if(i == data.length()){
                        errors::print_error("multiline comment was not closed");
                        exit(EXIT_FAILURE);
                    }
                    current_char = data.at(i);
                    if(current_char == '*'){
                        last_was_star = true;
                        continue;
                    }
                    if(current_char == '/' && last_was_star){
                        break;
                    }
                    last_was_star = false;
                }
                continue;
            }else if(current_char == '='){
                tokens.push_back(Token(TokenType::_divideeq));
                continue;
            }else{
                // this is a divide
                tokens.push_back(Token(TokenType::_divide));
                i--;
                continue;
            }
        }
        // SKIP EMPTYSPACE
        if(current_char == ' ' || current_char == '\n' || current_char == '\r' || current_char == '\t'){
            continue;
        }
        // BREAK IN CASE OF ENDING CHAR
        if(current_char == '\0'){
            break;
        }
        buffer << current_char;
        std::string error_char;
        buffer >> error_char;
        errors::print_error("invalid character \""+error_char+"\" found");
        exit(EXIT_FAILURE);
    }
    tokens.push_back(Token(TokenType::_end));
    return tokens;
}
void TokenizedData::reset_pointer(){
    this->index = 0;
}