#ifndef PROBLEMS_H
#define PROBLEMS_H

#include "Encodings.h"

namespace Problems {

// DS-PR
bool mt_ds_preferred(const AF & af, std::string const & arg);
bool ds_preferred_qbf(const AF & af, int arg);

}
#endif