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
            continue;
        }
        if(current_char == '='){
            tokens.push_back(Token(TokenType::_equals));
            continue;
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
            continue;
        }
        if(current_char == '}'){
            tokens.push_back(Token(TokenType::_close_curly));
            continue;
        }
        if(current_char == '+'){
            tokens.push_back(Token(TokenType::_plus));
            continue;
        }
        if(current_char == '-'){
            tokens.push_back(Token(TokenType::_minus));
            continue;
        }
        if(current_char == '*'){
            tokens.push_back(Token(TokenType::_times));
            continue;
        }
        if(current_char == '/'){
            tokens.push_back(Token(TokenType::_divide));
            continue;
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