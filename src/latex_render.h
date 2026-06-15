/********************************************
 * Copyright (c) 2026 Shun/修海 (@shun4midx) *
 * Project: Shun4miTeX                      *
 * File Type: C++ Header file               *
 * File: latex_render.h                     *
 ****************************************** */

#pragma once

#include <filesystem>
#include <string>

namespace Shun4miTeX {

std::filesystem::path renderLaTeXSnippet(const std::string& latex, const std::filesystem::path& jobs_base_dir);

}