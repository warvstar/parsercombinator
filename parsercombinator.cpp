#include <iostream>
#include <cstdarg>
//type label equals value
//int v = 1;
//int v = 1 + 1;
//int v = 1 + 1;
struct Token {
    Token* next = nullptr;
    void* func = nullptr;
    //bool valid = false;
};
Token* root = nullptr;
void CreateToken(Token*& tok) {
    tok = new Token[11];
    tok[1] = Token();
}
enum VAL {
    TYPE, LABEL, EQUALS, PLUS, MINUS
};
enum class SEQTYPE {
    LABEL, EQUALS, EXPR, PAREN_STATEMENT, ADD, NUMBER
};
void AddChain(void* func, int count, ...) {
    va_list valist;
    va_start(valist, count);
    Token* cur = root;
    Token* last = root;
    for (int i = 0; i < count; i++) {
        //cur[va_arg(valist, int)].valid = true;
        int ii = va_arg(valist, int);
        if (!cur[ii].next)
            CreateToken(cur[ii].next);
        last = cur;
        cur = cur[ii].next;
    }
    cur->func = func;
}
void printFunc() {

}
SEQTYPE root_insts[256];
//archer2 = 1 + 2 + test(1, test(2+1, (1))) + 3;
//becomes
//label equals expr(add(add(add(1 + 2) + label ARGS(1, label ARGS(add(2+1), (1)))) + 3))
//label equals expr(add(add(add(NUMBER PLUS NUMBER) PLUS label PAREN_STATEMENT(1, label PAREN_STATEMENT(add(NUMBER PLUS NUMBER), PAREN_STATEMENT(1)))) PLUS 3))
//expr and add evaluate to NUMBER or label
//label also has a type
//PAREN_STATEMENT evaluates to NUMBER, label or expr or multiple of each
void Match(VAL* vals) {
    Token* cur = root;
    Token* last = root;
    void* func = nullptr;
    int i = 0;
restart:
    while (cur) {
        if (cur->func) {
            func = cur->func;
            break;
        }
        last = cur;
        int ii = vals[i++];
        cur = cur[ii].next;
        //cur = cur->next[vals[i++]];
    }

    if (func) {
        printf("Func\n");
        cur = root;
        last = root;
        func = nullptr;
        goto restart;
    }
    else {
        for (int i = 0; i < 11; ++i)
            if (last[i].next)
                printf("Could be \n");

    }

}
int main() {
    root_insts[0] = SEQTYPE::LABEL;
    root_insts[1] = SEQTYPE::EQUALS;
    root_insts[2] = SEQTYPE::EXPR;
    root_insts[3] = (SEQTYPE)1;//expr subsize
    root_insts[4] = SEQTYPE::ADD;
    CreateToken(root);
    VAL* seq = new VAL[9];
    seq[0] = TYPE;
    seq[1] = LABEL;
    seq[2] = EQUALS;
    seq[3] = LABEL;
    seq[4] = PLUS;
    seq[5] = PLUS;
    seq[6] = LABEL;
    seq[7] = PLUS;
    seq[8] = MINUS;
    AddChain(printFunc, 3, TYPE, LABEL, EQUALS);
    AddChain(printFunc, 3, LABEL, PLUS, PLUS);
    Match(seq);
}