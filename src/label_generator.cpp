#include "label_generator.h"

Labelgenerator::Labelgenerator(){
    this->label_count = 0;
}
Labelgenerator::~Labelgenerator(){}
std::string Labelgenerator::get_label(){
    this->label_count++;
    return std::string("_label_") + std::to_string(this->label_count);
}