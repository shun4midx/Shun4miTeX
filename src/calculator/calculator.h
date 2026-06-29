/********************************************
 * Copyright (c) 2026 Shun/修海 (@shun4midx) *
 * Project: Shun4miTeX                      *
 * File Type: C++ Header file               *
 * File: calculator.h                       *
 ****************************************** */

#pragma once

#include <string>
#include <string_view>

namespace Shun4miTeX {

// ======== STRUCT ======== //
struct CalcResult {
    bool ok;
    double value;
    std::string error;
};

// ======= DEFAULT VALUES ======== //
static const CalcResult INVALID_RESULT = {false, 0.0, "Error"};

// ======== FUNCTIONS ======== //
CalcResult calc(const std::string_view& input);
std::string calcString(std::string_view input);

}