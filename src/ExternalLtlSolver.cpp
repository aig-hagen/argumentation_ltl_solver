#include "ExternalLtlSolver.h"
#include "Encodings.h"

#include <iostream>

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

int ExternalLtlSolver::solve(const AF & af, const int arg) {
    ofstream process;
    process.open("encoding.tmp");
    exists(process);
    forall(process);
    formula(af, arg, process);
    main(af, process);
    process.close();

    return 10;
}

void ExternalLtlSolver::formula(const AF & af, const int arg, ofstream & process) {
    process << "\nMODULE formula(";
    for (uint32_t i = 1; i <= 4*af.args-1; i++) {
        process << "x" << i << ",";
    }
    process << "x" << 4*af.args;

    process << ")\n\nDEFINE\n";

    // ADM Clauses
    auto adm_clauses = Encodings::add_admissible(af, false);
    for (uint32_t c = 0; c < adm_clauses.size(); c++) {
        auto clause = adm_clauses[c];
        process << "\tc" << c << " := ";
        for (uint32_t i = 0; i < clause.size(); i++) {
            if (clause[i] > 0) {
                process << "x" << clause[i];
            } else {
                process << "!x" << abs(clause[i]);
            }
            if (i != clause.size() - 1) {
                process << " | ";
            }
        }
        process << ";\n";
    }
    process << "\tadm := (";
    for (uint32_t c = 0; c < adm_clauses.size(); c++) {
        process << "c" << c;
        if (c != adm_clauses.size()-1) process << " & ";
    }
    process << ");\n";

    process << "\tin1 := !x" << arg << ";\n";

    // ADM clauses - reduct
    auto adm_clauses_r = Encodings::add_admissible(af, true);
    for (uint32_t c = 0; c < adm_clauses_r.size(); c++) {
        auto clause = adm_clauses_r[c];
        process << "\td" << c << " := ";
        for (uint32_t i = 0; i < clause.size(); i++) {
            if (clause[i] > 0) {
                process << "x" << clause[i];
            } else {
                process << "!x" << abs(clause[i]);
            }
            if (i != clause.size() - 1) {
                process << " | ";
            }
        }
        process << ";\n";
    }
    process << "\tadmr := (";
    for (uint32_t c = 0; c < adm_clauses_r.size(); c++) {
        process << "d" << c;
        if (c != adm_clauses_r.size()-1) process << " & ";
    }
    process << ");\n";


    // superset of X
    for(int i = 1; i <= af.args; i++) {
        process << "\te" << i << " := ";
        process << "!x" << i << " | " << "x" << 2*af.args + i << "!x" << af.args + i << " | " << "x" << 3*af.args + i << ";\n";
    }
    process << "\tmax1 := ";
    for(int i = 1; i <= af.args; i++) {
        process << "e" << i;
        if (i != af.args) {
            process << " & ";
        }
    }
    process << ";\n";
    process << "\tmax2 := ";
    for(int i = 1; i <= af.args; i++) {
        process << "(!x" << af.args + i << " | " << "x" << i << ")";
        if (i != af.args) {
            process " & ";
        }
    }
    process ";\n";

    process << "\tsat := !adm | x" << arg << " | (admr & max1 & max2 & x" << arg << ");\n";
    /*
    // Non-empty clause - reduct
    vector<int> ne_clause = Encodings::add_nonempty(af, true);
    process << "\tner := ";
    for (uint32_t i = 0; i < ne_clause.size(); i++) {
        if (ne_clause[i] > 0) {
            process << "x" << ne_clause[i];
        } else {
            process << "!x" << abs(ne_clause[i]);
        }
        if (i != ne_clause.size() - 1) {
            process << " | ";
        }
    }
    process << ";\n";

    // Reduct clauses
    auto reduct_clauses = Encodings::add_reduct(af);
    for (uint32_t c = 0; c < reduct_clauses.size(); c++) {
        auto clause = reduct_clauses[c];
        process << "\te" << c << " := ";
        for (uint32_t i = 0; i < clause.size(); i++) {
            if (clause[i] > 0) {
                process << "x" << clause[i];
            } else {
                process << "!x" << abs(clause[i]);
            }
            if (i != clause.size() - 1) {
                process << " | ";
            }
        }
        process << ";\n";
    }
    process << "\tred := ";
    for (uint32_t c = 0; c < reduct_clauses.size(); c++) {
        process << "e" << c;
        if (c != reduct_clauses.size()-1) process << " & ";
    }
    process << ";\n";

    process << "\tsat := !adm | x" << arg << "| (admr & ner & red & x" << arg << ");\n";
    */
}

void ExternalLtlSolver::main(const AF & af, ofstream & process) {
    process << "\nMODULE main\n\n";
    process << "VAR\n";
    process << "\t\tx" << 4*af.args << "\t: forall(clauses.sat);\n";
    for (int v = 4*af.args-1; v > 2*af.args; v--) {
        process << "\t\tx" << v << "\t: forall(x" << v+1 << ".carry-out);\n";
    }
    process << "\t\tx" << 2*af.args << "\t: exists(x" << 2*af.args+1 << ".carry-out);\n";
    for (int v = 2*af.args-1; v > 0; v--) {
        process << "\t\tx" << v << "\t: exists(x" << v+1 << ".carry-out);\n";
    }
    process << "clauses\t: formula(";
    process << "x1.value";
    for (int x = 2; x <= 4*af.args; x++) {
        process << ",x" << x << ".value";
    }
    process << ");\n";
    process << "SPEC AF (!clauses.sat)\n";
}

void ExternalLtlSolver::forall(ofstream & process) {
    process << "\nMODULE forall(carry-in)\n";
    process << "VAR\n";
    process << "    value : boolean;\n";
    process << "ASSIGN\n";
    process << "    init (value) := FALSE;\n";
    process << "    next (value) := value & carry-in;\n";
    process << "DEFINE\n";
    process << "    carry-out := value & carry-in;\n";
}


void ExternalLtlSolver::exists(ofstream & process) {
    process << "\nMODULE exists(carry-in)\n";
    process << "VAR\n";
    process << "    value : boolean;\n";
    process << "ASSIGN\n";
    process << "    init (value) := {FALSE,TRUE};\n";
    process << "    next (value) :=\n";
    process << "      case\n";
    process << "        carry-in : {FALSE,TRUE};\n";
    process << "        TRUE     : value;\n";
    process << "      esac;\n";
    process << "DEFINE carry-out := carry-in;\n";
}

void ExternalLtlSolver::free() {
    clauses.clear();
    model.clear();
}