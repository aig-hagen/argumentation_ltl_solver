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
    formula(af, arg, process);
    main(af, process);
    process.close();

    return 10;
}

void ExternalLtlSolver::formula(const AF & af, const int arg, ofstream & process) {
    process << "MODULE formula(";
    for (int i = 1; i <= af.args; i++) {
        process << "a" << i;
        if (i!=af.args) {
            process << ",";
        }
        

    }
    process << ")\n"
    
    // conflict-free
    int cf_clauses=0;
    for (int i = 1; i <= af.args; i++) {
		for (int j = 0; j < af.attackers[i].size(); j++) {
            process << "\tcf" << cf_clauses << " := (";
            for (int x = 0; x < i; x++) {
                process << "X ";
            }
            process << "!a" << i << " | ";
            for (int x = 0; x < af.attackers[i][j]; x++) {
                process << "X ";
            }
            process << "!a" << af.attackers[i][j] << ");\n";
            cf_clauses++;
		}
	}
    process << "\tcf := (";
    for (int i = 0; i < cf_clauses; i++) {
        process << "cf" << i;
        if (i!=cf_clauses-1) process << " | ";
    }
    process << ");\n";

    // self-defending
    for (int i = 1; i <= af.args; i++) {
        process << "\tsd" << i << " := (";
        for (int x = 0; x < i; x++) {
            process << "X ";
        }
        process << "a" << i;
        if (af.attackers[i].size()==0) {
            process << ");\n";
            continue;
        }
        process << " -> (";
        for (int j = 0; j < af.attackers[i].size(); j++) {
            process << "(";
            for (int k = 0; k < af.attackers[j].size(); k++) {
                for (int x = 0; x < k; x++) {
                    process << "X ";
                }
                process << "a" << k;
                if (k != af.attackers[j].size()) process << " | ";
            }
            process << ")";
            if (j != af.attackers[i].size()) process << " & ";
        }
        process << ")";
        if (i!=af.args) process << " & ";
    }
    process << ");\n";
    process << "\tself_defending := (";
    for (int x = 1; x <= af.args; x++) {
        process << "sd" << x;
        if (x!=af.args) process << " & ";
    }
    process << ");\n";

    process << "\tadm := cf & self_defending;\n";
    
    // expand
    for (int i = 1; i <= af.args; i++) {
        process << "\te" << i << " := (";
        for (int j = 0; j < i; j++) {
            process << "X ";
        }
        process << "a" << i << " -> G (send -> ";
        for (int j = 0; j < i; j++) {
            process << "X ";
        }
        process << "a" << i << "));\n";
    }
    process << "\texpand := ";
    for (int i = 1; i <= af.args; i++) {
        process << "e" << i;
        if (i!=af.args) process << " & ";
    }
    process << ";\n";

    // expand_str
    for (int i = 1; i <= af.args; i++) {
        process << "\tg" << i << " := (";
        for (int j = 0; j < i; j++) {
            process << "X ";
        }
        process << "!a" << i << " -> F (send & ";
        for (int j = 0; j < i; j++) {
            process << "X ";
        }
        process << "a" << i << "));\n";
    }
    process << "\texpand_str := ";
    for (int i = 1; i <= af.args; i++) {
        process << "g" << i;
        if (i!=af.args) process << " & ";
    }
    process << ";\n";

    // all_inner
    for (int i = 1; i <= af.args; i++) {
        for (int j = i+1; j <= af.args; j++) {
            process << "\th" << i << " := (";
            for (int k = 0; k < i; k++) {
                process << "X ";
            }
            process << "!a" << i << " & ";
            for (int k = 0; k < j; k++) {
                process << "X ";
            }
            process << "!a" << j << ") -> F (send & (";
            for (int k = 0; k < i; k++) {
                process << "X ";
            }
            process << "a" << i << " & ";
            for (int k = 0; k < j; k++) {
                process << "X ";
            }
            process << "!a" << j << ")) & F (send & (";
            for (int k = 0; k < i; k++) {
                process << "X ";
            }
            process << "!a" << i << " & ";
            for (int k = 0; k < j; k++) {
                process << "X ";
            }
            process << "a" << j << ")) & F (send & (";
            for (int k = 0; k < i; k++) {
                process << "X ";
            }
            process << "a" << i << " & ";
            for (int k = 0; k < j; k++) {
                process << "X ";
            }
            process << "a" << j << "));\n";
        }
    }
    process << "\tall_inner := ";
    for (int i = 1; i <= af.args; i++) {
        process << "h" << i;
        if (i!=af.args) process << " & ";
    }
    process << ";\n";

    //all_ex_path
    process << "\tall_ex_path := G (send -> all_inner);\n";

    //ex_non_add
    process << "\tex_non_add(ai) := (";
    for (int k = 0; k < af.args+1; k++) {
        process << "X ";
    }
    process << "G (send -> !adm));\n";

    // skeptical preferred
    process << "\tformula := (adm & expand & expand_str & all_ex_path & ex_non_add) -> ";
    for (int k = 0; k < arg; k++) {
        process << "X ";
    }
    process << "a" << arg << ";\n";
}

void ExternalLtlSolver::main(const AF & af, ofstream & process) {
    process << "\nMODULE main\n";
    process << "VAR\n";
    for (int a = 1; a <= af.args; a++) {
        process << "\t\ta" << a << "\t: boolean;\n";
    }
    process << "\t\tstate\t: {";
    for (int v = 1; v <= af.args; v++) {
        process << "v" << v << ",";
    }
    for (int f = 1; f <= af.args; f++) {
        process << "f" << f << ",";
    }
    process << "send};\n\n";

    process << "ASSIGN\n";
    process << "\tinit(state) := send;\n\n";
    process << "\tnext(state) := case\n";
    for (int v = 1; v < af.args; v++) {
        process << "\t\tstate = v" << v << " | state = f" << v << " :\t{v" << v+1 << ",f" << v+1 << "};\n";
    }
    process << "\t\tstate = v" << af.args << " | state = f" << af.args << " :\tsend;\n";
    process << "\t\tstate = send : {v1,f1};\n";
    process << "esac;\n\n";

    for (int a = 1; a <= af.args; a++) {
        process << "\tinit(a" << a << ") := FALSE;\n";
    }

    process << "\n\tnext(ai) := case // 1 <= i <= n\n";
    process << "\t\tnext(state) = vi : TRUE;\n";
    process << "\t\tTRUE : FALSE;\n";
    process << "esac;\n\n";

    process << "LTLSPEC <>";
}


void ExternalLtlSolver::free() {
    clauses.clear();
    model.clear();
}