#ifndef PROBLEMS_H
#define PROBLEMS_H

#include "Util.h"
#include "Encodings.h"

namespace Problems {

// DS-PR
bool mt_ds_preferred(const AF & af, std::string const & arg);
bool ds_preferred_qbf(const AF & af, std::string const & arg);

}
#endif