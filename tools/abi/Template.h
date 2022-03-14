#pragma once

#include "AbiDef.h"
#include <string>
namespace venachain {

    std::string generateAbiCPlusPlus(ContractDef &contractDef, ABIDef &abiDef);
}