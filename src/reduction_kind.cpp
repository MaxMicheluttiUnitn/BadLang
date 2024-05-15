#include "reduction_kind.h"

const char* reduction_kind_to_string(ReductionKind kind){
    switch(kind){
        case ReductionKind::NOT_A_REDUCTION:
            return "NOT_A_REDUCTION";
        case ReductionKind::START__CODE:
            return "START__CODE";
        case ReductionKind::CODE__STATEMENT:
            return "CODE__STATEMENT";
        case ReductionKind::CODE__STATEMENT_SEMI_CODE:
            return "CODE__STATEMENT_SEMI_CODE";
        case ReductionKind::STATEMENT__EQUALITY:
            return "STATEMENT__EQUALITY";
        case ReductionKind::STATEMENT__RETURN:
            return "STATEMENT__RETURN";
        case ReductionKind::EQUALITY__NAME_EQ_MATHOP:
            return "EQUALITY__NAME_EQ_MATHOP";
        case ReductionKind::RETURN__RET_ITEM:
            return "RETURN__RET_ITEM";
        case ReductionKind::ITEM__NAME:
            return "ITEM__NAME";
        case ReductionKind::ITEM__NUMBER:
            return "ITEM_NUMBER";
        case ReductionKind::MATHOP__MATHOPE:
            return "MATHOP__MATHOPE";
        case ReductionKind::MATHOPE__MATHOPE_PLUS_MATHOPT:
            return "MATHOPE__MATHOPT_PLUS_MATHOPE";
        case ReductionKind::MATHOPE__MATHOPE_MINUS_MATHOPT:
            return "MATHOPE__MATHOPT_MINUS_MATHOPE";
        case ReductionKind::MOTHOPE__MATHOPT:
            return "MOTHOPE__MATHOPT";
        case ReductionKind::MATHOPT__MATHOPT_TIMES_MATHOPF:
            return "MATHOPT__MATHOPF_TIMES_MATHOPT";
        case ReductionKind::MATHOPT__MATHOPT_DIV_MATHOPF:
            return "MATHOPT__MATHOPF_DIV_MATHOPT";
        case ReductionKind::MATHOPT__MATHOPF:
            return "MATHOPT__MATHOPF";
        case ReductionKind::MATHOPF__MINUS_MATHOPF:
            return "MATHOPF__MINUS_MATHOPF";
        case ReductionKind::MATHOPF__OPENBRACKETS_MATHOPE_CLOSEBRACKETS:
            return "MATHOPF__OPENBRACKETS_MATHOPE_CLOSEBRACKETS";
        case ReductionKind::MATHOPF__ITEM:
            return "MATHOPF__ITEM";
        case ReductionKind::CONDITIONAL__IF_OPEN_MATHOP_CLOSE_BLOCK:
            return "STATEMENT__IF_OPEN_MATHOP_CLOSE_BLOCK";
        case ReductionKind::CONDITIONAL__IF_OPEN_MATHOP_CLOSE_BLOCK_ELSE_BLOCK:
            return "STATEMENT__IF_OPEN_MATHOP_CLOSE_BLOCK_ELSE_BLOCK";
        case ReductionKind::BLOCK__OPENCURLY_CODE_CLOSECURLY:
            return "BLOCK__OPENCURLY_CODE_CLOSECURLY";
        case ReductionKind::CODE__CONDITIONAL: 
            return "CODE__CONDITIONAL";
        case ReductionKind::CODE__CONDITIONAL_CODE:
            return "CODE__CONDITIONAL_CODE";
        case ReductionKind::MATHOPT__MATHOPT_MOD_MATHOPF:
            return "MATHOPT__MATHOPT_MOD_MATHOPF";
    }
    return "ERROR: UNKNOWN REDUCTION KIND";
}

std::ostream& operator<<(std::ostream& os, const ReductionKind& kind){
    os << reduction_kind_to_string(kind);
    return os;
}