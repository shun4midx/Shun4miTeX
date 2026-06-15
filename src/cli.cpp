/********************************************
 * Copyright (c) 2026 Shun/修海 (@shun4midx) *
 * Project: Shun4miTeX                      *
 * File Type: C++ file                      *
 * File: cli.cpp                            *
 ****************************************** */

#include "latex_render.h"

#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <string>

namespace fs = std::filesystem;

std::string shellQuoteForCli(const std::string& s) {
    std::string out = "'";
    for (char c : s) {
        if (c == '\'') out += "'\\''";
        else out += c;
    }
    out += "'";
    return out;
}

int main(int argc, char** argv) {
    try {
        if (argc < 2) {
            std::cerr << "Usage:\n  " << argv[0] << " '<latex snippet>'\n";
            return 1;
        }

        std::string latex = argv[1];

        fs::path jobs_base_dir = fs::current_path() / "jobs";
        fs::path png = Shun4miTeX::renderLaTeXSnippet(latex, jobs_base_dir);

        std::cout << "\nRendered image:\n" << png << "\n";

    #ifdef __APPLE__
        std::string open_cmd = "open " + shellQuoteForCli(png.string());
        std::system(open_cmd.c_str());
    #endif

        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
}