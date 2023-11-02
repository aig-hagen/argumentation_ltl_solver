#include "Problems.h"						// Header for all Problem methods

#include <stack>							// for computing the grounded extension

using namespace std;

namespace Problems {
bool ds_preferred_qbf(const AF & af, string const & arg) {
	SAT_Solver solver = ExternalLtlSolver(af.count, af.solver_path);
	Encodings::add_admissible(af, solver);

    vector<int> clause = { -af.accepted_var[af.arg_to_int.at(arg)] };
    solver.addClause(clause);

    solver.formula(af);

	return true;
}

}