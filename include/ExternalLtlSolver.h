#ifndef EXTERNAL_LTL_SOLVER_H
#define EXTERNAL_LTL_SOLVER_H

#include "AF.h"

#include <vector>
#include <string>
#include <cstdint>
#include <fstream>

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
    int solve(const AF & af, const int arg);
    void main(const AF & af, std::ofstream & process);
    void formula(const AF & af, const int arg, std::ofstream & process);
    void forall(std::ofstream & process);
    void exists(std::ofstream & process);
    void free();
    
};

#endif