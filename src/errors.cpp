#include "errors.h"

void errors::print_error(){
    std::cerr << "\033[0;31mCompilation Error\033[0m";
}

void errors::print_error(const std::string& msg){
    errors::print_error();
    std::cerr <<": "<<msg<<"."<<std::endl;
}

void errors::print_usage(){
    std::cerr <<"\033[0;32mUsage\033[0m";
}

void errors::print_usage(const std::string& msg){
    errors::print_usage();
    std::cerr <<": "<<msg<<std::endl;
}