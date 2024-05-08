#ifndef __ERRORS__H__
#define __ERRORS__H__
#include <string>
#include <iostream>

namespace errors{
    void print_error(const std::string&);
    void print_error();

    void print_usage();
    void print_usage(const std::string&);
}

#endif