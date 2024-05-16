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
    EQUALITY__NAME_EQ_OP,
    RETURN__RET_OP,
    ITEM__NAME,
    ITEM__NUMBER,
    //OP__MATHOPE, removed to add boolean operators
    MATHOPE__MATHOPE_PLUS_MATHOPT,
    MATHOPE__MATHOPE_MINUS_MATHOPT,
    MOTHOPE__MATHOPT,
    MATHOPT__MATHOPT_TIMES_MATHOPF,
    MATHOPT__MATHOPT_DIV_MATHOPF,
    MATHOPT__MATHOPF,
    MATHOPF__MINUS_MATHOPF,
    MATHOPF__OPENBRACKETS_OP_CLOSEBRACKETS,
    MATHOPF__ITEM,
    CONDITIONAL__IF_OPEN_OP_CLOSE_BLOCK,
    CONDITIONAL__IF_OPEN_OP_CLOSE_BLOCK_ELSE_BLOCK,
    BLOCK__OPENCURLY_CODE_CLOSECURLY,
    CODE__CONDITIONAL,
    CODE__CONDITIONAL_CODE,
    MATHOPT__MATHOPT_MOD_MATHOPF,
    CONDITIONAL__WHILE_OPEN_OP_CLOSE_BLOCK,
    ITEM__TRUE,
    ITEM__FALSE,
    OP__BOOLOPE,
    BOOLOPE__BOOLOPE_AND_BOOLOPT,
    BOOLOPE__BOOLOPE_OR_BOOLOPT,
    BOOLOPE__BOOLOPE_XOR_BOOLOPT,
    BOOLOPE__BOOLOPT,
    BOOLOPT__NOT_CMPOP,
    BOOLOPT__CMPOP,
    CMPOP__CMPOP_EQEQ_BITWISEOPE,
    CMPOP__CMPOP_NOTEQ_BITWISEOPE,
    CMPOP__CMPOP_LT_BITWISEOPE,
    CMPOP__CMPOP_LTEQ_BITWISEOPE,
    CMPOP__CMPOP_GT_BITWISEOPE,
    CMPOP__CMPOP_GTEQ_BITWISEOPE,
    CMPOP__BITWISEOPE,
    BITWISEOPE__BITWISEOPE_BWAND_BITWISEOPT,
    BITWISEOPE__BITWISEOPE_BWOR_BITWISEOPT,
    BITWISEOPE__BITWISEOPE_BWXOR_BITWISEOPT,
    BITWISEOPE__BITWISEOPT,
    BITWISEOPT__TILDE_BITWISEOPT,
    BITWISEOPT__MATHOPE,
    STATEMENT__BREAK,
    STATEMENT__CONTINUE,
    CONDITIONAL__FOR_OPEN_EQUALITY_SEMI_OP_SEMI_EQUALITY_CLOSE_BLOCK,
    EQUALITY__NAME_PLUPLUS,
    EQUALITY__NAME_MINUSMINUS,
};

// for each reduction kind, print its name
const char* reduction_kind_to_string(ReductionKind kind);

std::ostream& operator<<(std::ostream& os, const ReductionKind& kind);

#endif