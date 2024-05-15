#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "errors.h"
#include "tokenizer.h"
#include "grammar.h"
#include "lr1_automaton.h"
#include "parser.h"
#include "ast.h"

int main(int argc, char* argv[]){
    if(argc <= 1){
        errors::print_error("no input file");
        errors::print_usage("badlang FILE.bl [options]");
        exit(EXIT_FAILURE);
    }
    char* filename = argv[1];

    std::ifstream in(filename);
    if(!in.is_open()){
        errors::print_error("file "+std::string(filename)+" not found");
        exit(EXIT_FAILURE);
    }
    std::stringstream raw_data_stream;
    raw_data_stream << in.rdbuf();
    in.close();
    std::string raw_data = raw_data_stream.str();

    TokenizedData token_data(raw_data);

    // while(token_data.has_next()){
    //     std::cout<<token_data.current_token().value_or(Token())<<std::endl;
    //     token_data.move_forward();
    // }

    Grammar g = Grammar::get_badlang_grammar();

    // std::cout<<g<<std::endl;

    Parser parser(g);

    AST* ast = parser.parse(g,token_data);

    //ast->print(std::cout);

    std::string code = ast->compile();

    std::ofstream out("tmp.asm");
    out<<code;    
    out.close();

    // ASSMBLE AND LINK WITH NASM AND LD
    int res = system("nasm -f elf64 -g tmp.asm");
    if(res != 0){
        errors::print_error("nasm failed");
        exit(EXIT_FAILURE);
    }
    res = system("ld -g tmp.o -o a.out");
    if(res != 0){
        errors::print_error("ld failed");
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}

