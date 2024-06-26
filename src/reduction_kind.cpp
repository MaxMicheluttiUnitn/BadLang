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
        case ReductionKind::EQUALITY__NAME_EQ_OP:
            return "EQUALITY__NAME_EQ_MATHOP";
        case ReductionKind::RETURN__RET_OP:
            return "RETURN__RET_OP";
        case ReductionKind::ITEM__NAME:
            return "ITEM__NAME";
        case ReductionKind::ITEM__NUMBER:
            return "ITEM_NUMBER";
        // case ReductionKind::OP__MATHOPE:
        //     return "MATHOP__MATHOPE";
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
        case ReductionKind::MATHOPF__OPENBRACKETS_OP_CLOSEBRACKETS:
            return "MATHOPF__OPENBRACKETS_MATHOPE_CLOSEBRACKETS";
        case ReductionKind::MATHOPF__ITEM:
            return "MATHOPF__ITEM";
        case ReductionKind::BLOCK__OPENCURLY_CODE_CLOSECURLY:
            return "BLOCK__OPENCURLY_CODE_CLOSECURLY";
        case ReductionKind::CODE__CONDITIONAL: 
            return "CODE__CONDITIONAL";
        case ReductionKind::CODE__CONDITIONAL_CODE:
            return "CODE__CONDITIONAL_CODE";
        case ReductionKind::MATHOPT__MATHOPT_MOD_MATHOPF:
            return "MATHOPT__MATHOPT_MOD_MATHOPF";
        case ReductionKind::CONDITIONAL__WHILE_OPEN_OP_CLOSE_BLOCK:
            return "CONDITIONAL__WHILE_OPEN_MATHOP_CLOSE_BLOCK";
        case ReductionKind::ITEM__TRUE: 
            return "ITEM__TRUE";
        case ReductionKind::ITEM__FALSE:
            return "ITEM__FALSE";
        case ReductionKind::OP__BOOLOPE:
            return "OP__BOOLOPE";
        case ReductionKind::BOOLOPE__BOOLOPE_AND_BOOLOPT:
            return "BOOLOPE__BOOLOPE_AND_BOOLOPT";
        case ReductionKind::BOOLOPE__BOOLOPE_OR_BOOLOPT:
            return "BOOLOPE__BOOLOPE_OR_BOOLOPT";
        case ReductionKind::BOOLOPE__BOOLOPE_XOR_BOOLOPT:
            return "BOOLOPE__BOOLOPE_XOR_BOOLOPT";
        case ReductionKind::BOOLOPE__BOOLOPT:
            return "BOOLOPE__BOOLOPT";
        case ReductionKind::BOOLOPT__NOT_CMPOP:
            return "BOOLOPT__NOT_CMPOP";
        case ReductionKind::BOOLOPT__CMPOP:
            return "BOOLOPT__CMPOP";
        case ReductionKind::CMPOP__CMPOP_EQEQ_BITWISEOPE:
            return "CMPOP__CMPOP_EQEQ_BITWISEOPE";
        case ReductionKind::CMPOP__CMPOP_NOTEQ_BITWISEOPE:
            return "CMPOP__CMPOP_NOTEQ_BITWISEOPE";
        case ReductionKind::CMPOP__CMPOP_LT_BITWISEOPE:
            return "CMPOP__CMPOP_LT_BITWISEOPE";
        case ReductionKind::CMPOP__CMPOP_LTEQ_BITWISEOPE:
            return "CMPOP__CMPOP_LTEQ_BITWISEOPE";
        case ReductionKind::CMPOP__CMPOP_GT_BITWISEOPE:
            return "CMPOP__CMPOP_GT_BITWISEOP";
        case ReductionKind::CMPOP__CMPOP_GTEQ_BITWISEOPE:
            return "CMPOP__CMPOP_GTEQ_BITWISEOPE";
        case ReductionKind::CMPOP__BITWISEOPE:
            return "CMPOP__BITWISEOPE";
        case ReductionKind::BITWISEOPE__BITWISEOPE_BWAND_BITWISEOPT:
            return "BIWISEOPE__BITWISEOPE_BWAND_BITWISEOPT";
        case ReductionKind::BITWISEOPE__BITWISEOPE_BWOR_BITWISEOPT:
            return "BITWISEOPE__BITWISEOPE_BWXOR_BITWISEOPT";
        case ReductionKind::BITWISEOPE__BITWISEOPE_BWXOR_BITWISEOPT:
            return "BITWISEOPE__BITWISEOPE_BWOR_BITWISEOPT";
        case ReductionKind::BITWISEOPT__MATHOPE:
            return "BITWISEOPT__MATHOPE";
        case ReductionKind::BITWISEOPT__TILDE_BITWISEOPT:
            return "BITWISEOPT__TILDE_BITWISEOPT";
        case ReductionKind::STATEMENT__BREAK:
            return "STATEMENT__BREAK";
        case ReductionKind::STATEMENT__CONTINUE:
            return "STATEMENT__CONTINUE";
        case ReductionKind::CONDITIONAL__FOR_OPEN_EQUALITY_SEMI_OP_SEMI_EQUALITY_CLOSE_BLOCK:
            return "CONDITIONAL__FOR_OPEN_EQUALITY_SEMI_OP_SEMI_EQUALITY_CLOSE_BLOCK";
        case ReductionKind::EQUALITY__NAME_PLUPLUS:
            return "EQUALITY__NAME_PLUPLUS";
        case ReductionKind::EQUALITY__NAME_MINUSMINUS:
            return "EQUALITY__NAME_MINUSMINUS";
        case ReductionKind::EQUALITY__NAME_PLUSEQ_OP:
            return "EQUALITY__NAME_PLUSEQ_OP";
        case ReductionKind::EQUALITY__NAME_MINUSEQ_OP:
            return "EQUALITY__NAME_MINUSEQ_OP";
        case ReductionKind::EQUALITY__NAME_TIMESEQ_OP:
            return "EQUALITY__NAME_TIMESEQ_OP";
        case ReductionKind::EQUALITY__NAME_DIVIDEEQ_OP:
            return "EQUALITY__NAME_DIVIDEEQ_OP";
        case ReductionKind::EQUALITY__NAME_MODULUSEQ_OP:
            return "EQUALITY__NAME_MODULUSEQ_OP";
        case ReductionKind::CONDITIONAL__IF_OPEN_OP_CLOSE_BLOCK:
            return "STATEMENT__IF_OPEN_MATHOP_CLOSE_BLOCK";
        case ReductionKind::CONDITIONAL__IF_OPEN_OP_CLOSE_BLOCK_ELSEIFS:
            return "CONDITIONAL__IF_OPEN_OP_CLOSE_BLOCK_ELSEIFS";
        case ReductionKind::ELSEIFS__ELIF_OPEN_OP_CLOSE_BLOCK_ELSEIFS:
            return "ELSEIFS__ELIF_OPEN_OP_CLOSE_BLOCK_ELSEIFS";
        case ReductionKind::ELSEIFS__ELIF_OPEN_OP_CLOSE_BLOCK:
            return "ELSEIFS__ELIF_OPEN_OP_CLOSE_BLOCK";
        case ReductionKind::ELSEIFS__ELSE_BLOCK:
            return "ELSEIFS__ELSE_BLOCK";
    }
    return "ERROR: UNKNOWN REDUCTION KIND";
}

std::ostream& operator<<(std::ostream& os, const ReductionKind& kind){
    os << reduction_kind_to_string(kind);
    return os;
}