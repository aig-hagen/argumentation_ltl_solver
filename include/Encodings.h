#ifndef ENCODINGS_H
#define ENCODINGS_H

#include "AF.h"
#include "Util.h"

#if defined(SAT_CMSAT)
#include "CryptoMiniSatSolver.h"
typedef CryptoMiniSatSolver SAT_Solver;
#else
#include "ExternalLtlSolver.h"
typedef ExternalLtlSolver SAT_Solver;
#endif

namespace Encodings {

std::vector<int> add_nonempty(const AF & af, bool reduct);
std::vector<std::vector<int>> add_admissible(const AF & af, bool reduct);
std::vector<std::vector<int>> add_reduct(const AF & af);

}

#endif