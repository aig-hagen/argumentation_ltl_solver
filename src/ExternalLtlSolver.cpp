#include "ExternalLtlSolver.h"

#include <iostream>
#include <fstream>

using namespace std;

/*
 * The following is adapted from the fudge argumentation-solver
 * and is subject to the GPL3 licence.
*/
ExternalLtlSolver::ExternalLtlSolver(uint32_t number_of_vars, string path_to_solver) {
    n_vars = number_of_vars;
    model = std::vector<bool>(n_vars+1);
    clauses = std::vector<std::vector<int>>();
    
    last_clause_closed = true;
    num_clauses = 0;
    solver_path = path_to_solver;
}


void ExternalLtlSolver::addClause(std::vector<int> & clause) {
    if(!last_clause_closed){
        // this should not happen
        printf("Previous clause not closed.");
        exit(1);
    }
    clauses.push_back(clause);
    num_clauses++;
    clauses[num_clauses-1].push_back(0);
}

int ExternalLtlSolver::solve() {
    ofstream process;
    process.open("encoding.tmp");
    process << "p cnf " << n_vars << " " << num_clauses << "\n";
    for(auto const& clause: clauses) {
        for(const int lit: clause){
            process << lit << " ";
        }
        process << "\n";
    }
    process.close();
    return 10;
}

void ExternalLtlSolver::formula(const AF & af) {
    ofstream process;
    process.open("encoding.tmp");
    process << "MODULE formula(";
    process << af.accepted_var[0];
    for (uint32_t i = 1; i < af.args; i++) {
        process << "," << af.accepted_var[i];
    }
    for (uint32_t i = 0; i < af.args; i++) {
        process << "," << af.rejected_var[i];
    }
    process << ")\n\nDEFINE\n";
    for (uint32_t c = 0; c < clauses.size(); c++) {
        auto clause = clauses[c];
        process << "\tc" << c << " := ";
        for (uint32_t i = 0; i < clause.size()-1; i++) {
            process << clause[i];
            if (i != clause.size() - 2) {
                process << " | ";
            }
        }
        process << ";\n";
    }
    process << "\tsat := ";
    for (uint32_t c = 0; c < clauses.size(); c++) {
        process << "c" << c;
        if (c != clauses.size()-1) process << " & ";
    }
    process << ";\n";
    process.close();
}

void ExternalLtlSolver::main(const AF & af) {
    ofstream process;
    process.open("encoding.tmp");
    process << "MODULE main\n\n";
    process << "VAR\n";
    // TODO add quantifiers
    process << "SPEC AF (!clauses.sat)\n";
    process.close();
}

void ExternalLtlSolver::forall() {
    ofstream process;
    process.open("encoding.tmp");
    process << "MODULE forall(carry-in)\n";
    process << "VAR\n";
    process << "    value : boolean;\n";
    process << "ASSIGN\n";
    process << "    init (value) := 0;\n";
    process << "    next (value) := value ^ carry-in;\n";
    process << "DEFINE\n";
    process << "    carry-out := value & carry-in;\n";
    process.close();
}


void ExternalLtlSolver::exists() {
    ofstream process;
    process.open("encoding.tmp");
    process << "MODULE exists(carry-in)\n";
    process << "VAR\n";
    process << "    value : boolean;\n";
    process << "ASSIGN\n";
    process << "    init (value) := {0,1};\n";
    process << "    next (value) :=\n";
    process << "      case\n";
    process << "        carry-in : {0,1};\n";
    process << "        1        : value;\n";
    process << "      esac;\n";
    process << "DEFINE carry-out := carry-in;\n";
    process.close();
}

void ExternalLtlSolver::free() {
    clauses.clear();
    model.clear();
}