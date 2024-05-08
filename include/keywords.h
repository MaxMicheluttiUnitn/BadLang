#ifndef __KEYWORDS__H__
#define __KEYWORDS__H__
#include <map>
#include <string>
#include "token.h"

class Keywords{
    private:
    std::map<std::string,Token> items;
    public:
    Keywords();
    ~Keywords();
    bool iskeyword(const std::string&);
    Token getkwtoken(const std::string&);
};

#endif