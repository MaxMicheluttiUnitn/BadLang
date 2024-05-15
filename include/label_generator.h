#ifndef __LABEL_GENERATOR__H__
#define __LABEL_GENERATOR__H__

#include <string>

class Labelgenerator{
    private:
    int label_count;
    public:
    Labelgenerator();
    ~Labelgenerator();
    std::string get_label();
};

#endif