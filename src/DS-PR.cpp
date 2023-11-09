#include "Problems.h"						// Header for all Problem methods

using namespace std;

namespace Problems {
bool ds_preferred_qbf(const AF & af, int arg) {
	SAT_Solver solver = ExternalLtlSolver(4*af.args, af.solver_path);

    solver.solve(af, arg);

	return true;
}

}