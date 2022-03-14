#pragma once
#include "boost/filesystem.hpp"

#include "AbiDef.h"

namespace venachain {
    void outputJsonABI(const ABIDef &abiDef, const ContractDef &contractDef, std::ofstream &fs);
}

