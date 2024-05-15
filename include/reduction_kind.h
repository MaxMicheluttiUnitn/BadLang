#ifndef __BADLANG_REDUCTIONS_H__
#define __BADLANG_REDUCTIONS_H__

#include <iostream>

enum class ReductionKind{
    NOT_A_REDUCTION,
    START__CODE,
    CODE__STATEMENT,
    CODE__STATEMENT_SEMI_CODE,
    STATEMENT__EQUALITY,
    STATEMENT__RETURN,
    EQUALITY__NAME_EQ_MATHOP,
    RETURN__RET_ITEM,
    ITEM__NAME,
    ITEM__NUMBER,
    MATHOP__MATHOPE,
    MATHOPE__MATHOPE_PLUS_MATHOPT,
    MATHOPE__MATHOPE_MINUS_MATHOPT,
    MOTHOPE__MATHOPT,
    MATHOPT__MATHOPT_TIMES_MATHOPF,
    MATHOPT__MATHOPT_DIV_MATHOPF,
    MATHOPT__MATHOPF,
    MATHOPF__MINUS_MATHOPF,
    MATHOPF__OPENBRACKETS_MATHOPE_CLOSEBRACKETS,
    MATHOPF__ITEM
};

// for each reduction kind, print its name
const char* reduction_kind_to_string(ReductionKind kind);

std::ostream& operator<<(std::ostream& os, const ReductionKind& kind);

#endif