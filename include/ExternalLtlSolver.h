#ifndef EXTERNAL_LTL_SOLVER_H
#define EXTERNAL_LTL_SOLVER_H

#include "AF.h"

#include <vector>
#include <string>
#include <cstdint>

/*
*/
class ExternalLtlSolver {
public:
    std::vector<bool> model;
    std::vector<std::vector<int>> clauses;
    uint32_t n_vars;
    int num_clauses;
    bool last_clause_closed;
    std::string solver_path;

    ExternalLtlSolver(uint32_t number_of_vars, std::string path_to_solver);
    void addClause(std::vector<int> & clause);
    int solve();
    void main(const AF & af);
    void formula(const AF & af);
    void forall();
    void exists();
    void free();
    
};

#endif