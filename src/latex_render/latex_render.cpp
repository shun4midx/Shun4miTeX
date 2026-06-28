/********************************************
 * Copyright (c) 2026 Shun/修海 (@shun4midx) *
 * Project: Shun4miTeX                      *
 * File Type: C++ file                      *
 * File: latex_render.cpp                   *
 ****************************************** */

#include "latex_render.h"

#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <random>
#include <stdexcept>
#include <string>
#include <vector>

namespace fs = std::filesystem;

namespace Shun4miTeX {

namespace {

std::string randomHex(std::size_t len = 12) {
    static const char* chars = "0123456789abcdef";
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(0, 15);

    std::string s;
    for (std::size_t i = 0; i < len; ++i) {
        s += chars[dist(gen)];
    }
    return s;
}

std::string shellQuote(const std::string& s) {
    std::string out = "'";
    for (char c : s) {
        if (c == '\'') out += "'\\''";
        else out += c;
    }
    out += "'";
    return out;
}

fs::path makeJobDir(const fs::path& jobs_base_dir) {
    fs::create_directories(jobs_base_dir);

    fs::path job_dir = jobs_base_dir / ("job_" + randomHex());
    fs::create_directories(job_dir);

    return job_dir;
}

void writeTexFile(const fs::path& tex_path, const std::string& latex) {
    std::ofstream out(tex_path);

    if (!out) {
        throw std::runtime_error("Could not write eq.tex");
    }

    out << R"(\documentclass{article}
\usepackage{amsmath, amsfonts, amssymb, amsthm, booktabs, mathtools}
\usepackage[dvipsnames,usenames]{xcolor}
\usepackage{tcolorbox}
\usepackage{subcaption}
\usepackage{enumitem}
\theoremstyle{definition}
\newtheorem{definition}{Definition}[subsection]
\newtheorem{statement}{Statement}[subsection]
\newtheorem{theorem}{Theorem}
\newtheorem{lemma}[theorem]{Lemma}
\newtheorem{claim}[theorem]{Claim}

\usepackage[active,tightpage]{preview}
\setlength\PreviewBorder{3pt}

\newcommand{\bs}{\textbackslash}
\newcommand{\boxtxt}[1]{\boxed{\text{#1}}}
\newcommand{\op}[1]{\operatorname{#1}}
\newcommand{\OPT}{\mathrm{OPT}}

\newcommand{\bluebox}[1]{%
  \begin{tcolorbox}[colframe=blue!50!black, colback=blue!20]
    #1
  \end{tcolorbox}%
}

\newcommand{\purplebox}[1]{%
  \begin{tcolorbox}[colframe=violet!80!black, colback=violet!20]
    #1
  \end{tcolorbox}%
}

\newcommand{\pinkbox}[2]{%
  \begin{tcolorbox}[colframe=magenta!50!black, colback=magenta!20, title={#1}]
    #2
  \end{tcolorbox}%
}

\newcommand{\purplebigbox}[2]{%
  \begin{tcolorbox}[colframe=violet!80!black, colback=violet!20, title={#1}]
    #2
  \end{tcolorbox}%
}

\newcommand{\bluebigbox}[2]{%
  \begin{tcolorbox}[colframe=blue!50!black, colback=blue!20, title={#1}, toptitle=5pt, bottomtitle=5pt]
    #2
  \end{tcolorbox}%
}

\newcommand{\bluesec}[1]{%
  \bluebox{\section*{#1}}
}

\newcommand{\purplesec}[1]{%
  \purplebox{\subsection*{#1}}
}

\newcommand{\newchap}[1]{%
  \begin{tcolorbox}[colframe=green!60!blue, colback=green!20, title=\section*{#1}]
  \end{tcolorbox}
}

\DeclareRobustCommand{\hlit}[1]{\colorbox{yellow}{\textit{#1}}}
\DeclareRobustCommand{\hlbf}[1]{\colorbox{yellow}{\textbf{#1}}}
\DeclareRobustCommand{\hl}[1]{\colorbox{yellow}{#1}}
\DeclareRobustCommand{\ghl}[1]{\colorbox{green}{#1}}

\begin{document}
\pagecolor{black}
\begin{preview}
\color{white}
)"
        << latex
        << R"(
\end{preview}
\end{document}
)";
}

void runDockerRenderer(const fs::path& job_dir) {
    std::string cmd =
        "docker run --rm -v " +
        shellQuote(job_dir.string() + ":/work") +
        " shun4mitex-renderer eq.tex";

    std::cout << "Running:\n" << cmd << "\n\n";

    int code = std::system(cmd.c_str());

    if (code != 0) {
        throw std::runtime_error("Docker renderer failed");
    }
}

} // namespace

fs::path renderLaTeXSnippet(const std::string& latex, const std::filesystem::path& jobs_base_dir) {
    if (latex.empty()) {
        throw std::runtime_error("Empty LaTeX input");
    }

    if (latex.size() > 2000) {
        throw std::runtime_error("LaTeX input too long");
    }

    fs::path job_dir = makeJobDir(jobs_base_dir);
    fs::path tex_path = job_dir / "eq.tex";
    fs::path out_path = job_dir / "out.png";

    writeTexFile(tex_path, latex);
    runDockerRenderer(job_dir);

    if (!fs::exists(out_path)) {
        throw std::runtime_error("Renderer did not produce out.png");
    }

    return out_path;
}

} // namespace Shun4miTeX