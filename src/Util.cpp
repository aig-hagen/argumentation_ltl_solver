#include "Util.h"

namespace std {
    int accepted_var(int arg, int args) {
        return arg;
    }
    int rejected_var(int arg, int args) {
        return args + arg;
    }
    int accepted_var_r(int arg, int args) {
        return args*2 + arg;
    }
    int rejected_var_r(int arg, int args) {
        return args*3 + arg;
    }
}