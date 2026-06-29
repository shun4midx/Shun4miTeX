/********************************************
 * Copyright (c) 2026 Shun/修海 (@shun4midx) *
 * Project: Shun4miTeX                      *
 * File Type: C++ file                      *
 * File: calculator.cpp                     *
 ****************************************** */

#include "calculator.h"

#include <sstream>

namespace Shun4miTeX {

/* Recall:
struct CalcResult {
    bool ok;
    double value;
    std::string error;
};
*/

// ======== FUNCTIONS ======== //
CalcResult calc(const std::string_view& input) {
    return INVALID_RESULT;
}

std::string calcString(std::string_view input) {
    CalcResult result = calc(input);

    if (!result.ok) {
        return "Error: " + result.error;
    }

    std::ostringstream out;
    out << result.value;
    return out.str();
}

}