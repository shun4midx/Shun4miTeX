/********************************************
 * Copyright (c) 2026 Shun/修海 (@shun4midx) *
 * Project: Shun4miTeX                      *
 * File Type: C++ Header file               *
 * File: natural_math.h                     *
 ****************************************** */

#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <utility>
#include <cctype>

// ======== COMMON MAPS ======== //
const std::unordered_map<std::string, std::string> SUPERSCRIPTS = {
    {"⁰", "0"}, {"¹", "1"}, {"²", "2"}, {"³", "3"}, {"⁴", "4"},
    {"⁵", "5"}, {"⁶", "6"}, {"⁷", "7"}, {"⁸", "8"}, {"⁹", "9"}
};

const std::unordered_map<std::string, std::string> SUBSCRIPTS = {
    {"₀", "0"}, {"₁", "1"}, {"₂", "2"}, {"₃", "3"}, {"₄", "4"},
    {"₅", "5"}, {"₆", "6"}, {"₇", "7"}, {"₈", "8"}, {"₉", "9"}
};

const std::unordered_map<std::string, std::string> UNICODE_SYMBOLS = {
    {"∞", "\\infty "},
    {"π", "\\pi "},
    {"θ", "\\theta "},
    {"≤", "\\le "},
    {"≥", "\\ge "},
    {"≠", "\\ne "},
    {"≈", "\\approx "},
    {"→", "\\to "},
    {"←", "\\leftarrow "},
    {"↔", "\\leftrightarrow "},
    {"∈", "\\in "},
    {"∉", "\\notin "},
    {"⊂", "\\subset "},
    {"⊆", "\\subseteq "},
    {"∪", "\\cup "},
    {"∩", "\\cap "},
    {"∅", "\\varnothing "},
    {"×", "\\times "},
    {"·", "\\cdot "},
    {"±", "\\pm "},
    {"∠", "\\angle "},
    {"°", "^{\\circ} "},
    {"△", "\\triangle "}
};

const std::unordered_map<std::string, std::string> MATHBB_LETTERS = {
    {"RR", "\\mathbb{R}"}, {"ℝ", "\\mathbb{R}"},
    {"ZZ", "\\mathbb{Z}"}, {"ℤ", "\\mathbb{Z}"},
    {"NN", "\\mathbb{N}"}, {"ℕ", "\\mathbb{N}"},
    {"QQ", "\\mathbb{Q}"}, {"ℚ", "\\mathbb{Q}"},
    {"CC", "\\mathbb{C}"}, {"ℂ", "\\mathbb{C}"},
    {"FF", "\\mathbb{F}"}, {"𝔽", "\\mathbb{F}"},
    {"AA", "\\mathbb{A}"}, {"𝔸", "\\mathbb{A}"},
    {"HH", "\\mathbb{H}"}, {"ℍ", "\\mathbb{H}"},
    {"PP", "\\mathbb{P}"}, {"ℙ", "\\mathbb{P}"}
};

const std::unordered_map<std::string, std::string> GREEK_LETTERS = {
    {"ell", "\\ell "}, {"ℓ", "\\ell "},
    {"alpha", "\\alpha "}, {"α", "\\alpha "},
    {"beta", "\\beta "}, {"β", "\\beta "},
    {"gamma", "\\gamma "}, {"γ", "\\gamma "}, {"Gamma", "\\Gamma "}, {"Γ", "\\Gamma "},
    {"delta", "\\delta "}, {"δ", "\\delta "}, {"Delta", "\\Delta "}, {"Δ", "\\Delta "},
    {"epsilon", "\\varepsilon "}, {"varepsilon", "\\varepsilon "}, {"eps", "\\varepsilon "}, {"ε", "\\varepsilon "},
    {"zeta", "\\zeta "}, {"ζ", "\\zeta "},
    {"eta", "\\eta "}, {"η", "\\eta "},
    {"theta", "\\theta "}, {"θ", "\\theta "}, {"Theta", "\\Theta "}, {"Θ", "\\Theta "},
    {"iota", "\\iota "}, {"ι", "\\iota "},
    {"kappa", "\\kappa "}, {"κ", "\\kappa "},
    {"lambda", "\\lambda "}, {"λ", "\\lambda "}, {"Lambda", "\\Lambda "}, {"Λ", "\\Lambda "},
    {"mu", "\\mu "}, {"μ", "\\mu "},
    {"nu", "\\nu "}, {"ν", "\\nu "},
    {"xi", "\\xi "}, {"ξ", "\\xi "}, {"Xi", "\\Xi "}, {"Ξ", "\\Xi "},
    {"omicron", "o"}, {"ο", "o"}, {"Omicron", "O"}, {"Ο", "O"},
    {"pi", "\\pi "}, {"π", "\\pi "}, {"Pi", "\\Pi "}, {"Π", "\\Pi "},
    {"rho", "\\rho "}, {"ρ", "\\rho "},
    {"sigma", "\\sigma "}, {"σ", "\\sigma "}, {"Sigma", "\\Sigma "}, {"Σ", "\\Sigma "},
    {"varsigma", "\\varsigma "}, {"ς", "\\varsigma "},
    {"tau", "\\tau "}, {"τ", "\\tau "},
    {"upsilon", "\\upsilon "}, {"υ", "\\upsilon "}, {"Upsilon", "\\Upsilon "}, {"Υ", "\\Upsilon "},
    {"phi", "\\phi "}, {"φ", "\\phi "}, {"Phi", "\\Phi "}, {"Φ", "\\Phi "},
    {"varphi", "\\varphi "},
    {"chi", "\\chi "}, {"χ", "\\chi "},
    {"psi", "\\psi "}, {"ψ", "\\psi "}, {"Psi", "\\Psi "}, {"Ψ", "\\Psi "},
    {"omega", "\\omega "}, {"ω", "\\omega "}, {"Omega", "\\Omega "}, {"Ω", "\\Omega "}
};

const std::unordered_map<std::string, std::string> SIMPLE_OPERATORS = {
    {"sin", "\\sin "},
    {"cos", "\\cos "},
    {"tan", "\\tan "},
    {"sec", "\\sec "},
    {"csc", "\\csc "},
    {"cot", "\\cot "},
    {"arcsin", "\\arcsin "},
    {"arccos", "\\arccos "},
    {"arctan", "\\arctan "},
    {"sinh", "\\sinh "},
    {"cosh", "\\cosh "},
    {"tanh", "\\tanh "},
    {"log", "\\log "},
    {"ln", "\\ln "},
    {"exp", "\\exp "},
    {"det", "\\det "},
    {"max", "\\max "},
    {"min", "\\min "},
    {"gcd", "\\gcd "},
    {"lcm", "\\op{lcm} "},
    {"dim", "\\dim "},
    {"ker", "\\ker "},
    {"rank", "\\op{rank} "},
    {"tri", "\\triangle "},
    {"triangle", "\\triangle "}
};

const std::unordered_map<std::string, std::string> SPECIAL_FUNCTIONS = {
    {"text", "TEXT"},
    {"boxed", "BOXED"},
    {"sqrt", "SQRT"},
    {"root", "ROOT"},        // root(n, x) -> \sqrt[n]{x}
    {"nthroot", "ROOT"},     // nthroot(n, x)
    {"cuberoot", "CBRT"},    // cuberoot(x) -> \sqrt[3]{x}
    {"cubert", "CBRT"},
    {"cbrt", "CBRT"},
    {"squareroot", "SQRT"},
    {"abs", "ABS"},
    {"norm", "NORM"},
    {"int", "INT"},
    {"sum", "SUM"},
    {"prod", "PROD"},
    {"lim", "LIM"},
    {"frac", "FRAC"},
    {"deg", "DEG"},
    {"degree", "DEG"},
    {"degrees", "DEG"},
    {"angle", "ANGLE"},
    {"floor", "FLOOR"},   // floor(x) -> \left\lfloor x \right\rfloor
    {"ceil", "CEIL"},     // ceil(x) -> \left\lceil x \right\rceil
    {"ceiling", "CEIL"},
    {"deriv", "DERIV"},   // deriv(y, x) -> \frac{dy}{dx}
    {"diff", "DDX"},      // ddx(f, x) -> \frac{d}{dx} f
    {"ddx", "DDX"},
    {"pderiv", "PDERIV"},  // pderiv(f, x) -> \frac{\partial f}{\partial x}
    {"pdiff", "PDDX"},    // pdiff(f, x) -> \frac{\partial}{\partial x} f
    {"pddx", "PDDX"},    // pdiff(f, x) -> \frac{\partial}{\partial x} f
    {"matrix", "MATRIX"},
    {"pmatrix", "PMATRIX"},
    {"bmatrix", "BMATRIX"},
    {"vmatrix", "VMATRIX"}
};

const std::unordered_map<std::string, std::string> OPERATORS = {
    {"<=", "\\le "},
    {">=", "\\ge "},
    {"!=", "\\ne "},
    {"->", "\\to "},
    {"<-", "\\leftarrow "},
    {"=>", "\\Rightarrow "},
    {"iff", "\\iff "},
    {"in", "\\in "},
    {"notin", "\\notin "},
    {"subset", "\\subset "},
    {"subseteq", "\\subseteq "},
    {"cup", "\\cup "},
    {"cap", "\\cap "},
    {"times", "\\times "},
    {"cdot", "\\cdot "},
    {"pm", "\\pm "},
    {"propto", "\\propto "},
    {"proportional", "\\propto "},
    {"therefore", "\\therefore "}, {"hence", "\\therefore "},
    {"because", "\\because "}, {"since", "\\because "}
};

// Names of stuff you should immediately recognize
// Must handle any of the above before handling alias
const std::unordered_map<std::string, std::string> ALIAS_COMMANDS = {
    {"infty", "\\infty "},
    {"infinity", "\\infty "},
    {"infin", "\\infty "},
    {"oo", "\\infty "},

    {"real", "\\mathbb{R} "},
    {"reals", "\\mathbb{R} "},
    {"integer", "\\mathbb{Z} "},
    {"integers", "\\mathbb{Z} "},
    {"natural", "\\mathbb{N} "},
    {"naturals", "\\mathbb{N} "},
    {"rational", "\\mathbb{Q} "},
    {"rationals", "\\mathbb{Q} "},
    {"complex", "\\mathbb{C} "},

    {"empty", "\\varnothing "},
    {"emptyset", "\\varnothing "},
    {"nothing", "\\varnothing "},

    {"times", "\\times "},

    {"integral", "\\int "},
    {"partial", "\\partial "},
    {"nabla", "\\nabla "},
    {"grad", "\\nabla "},

    {"star", "^* "},
    {"dual", "^* "},

    {"forall", "\\forall "},
    {"exists", "\\exists "},
    {"and", "\\land "},
    {"or", "\\lor "},
    {"not", "\\neg "},
    {"implies", "\\implies "},
    {"to", "\\to "},
    {"mapsto", "\\mapsto "},
    {"le", "\\le "},
    {"ge", "\\ge "},
    {"neq", "\\ne "}, {"!=", "\\ne "},
    {"approx", "\\approx "},
    {"equiv", "\\equiv "},
    {"cong", "\\cong "},
    {"sim", "\\sim "},

    {"deg", "^{\\circ} "},
    {"degree", "^{\\circ} "},
    {"degrees", "^{\\circ} "},

    {"mod", "\\bmod "}
};

// ======== NAMESPACE ======== //
namespace Shun4miTeX::Natural {

// ======== STRUCTS ======== //
enum class NaturalChunkKind {
    Text,
    InlineMath,
    DisplayMath
};

struct NaturalChunk {
    std::string text;
    NaturalChunkKind kind;
};

// ======== FUNCTIONS ======== //

// Main function: Directly translates the user input without parsing
std::string naturalToLaTeX(const std::string& full_natural_text);

// Parse and stitch sections
std::vector<NaturalChunk> spliceNaturalText(const std::string& full_natural_text);

// Fused without considering the boolean btw
std::string fuseSplicedText(const std::vector<NaturalChunk>& spliced_text);

// Helpful functions for conversion
std::string replaceAllFromMap(const std::string& input, const std::unordered_map<std::string, std::string>& replacements);

std::string convertSuperSubDigits(const std::string& input);

std::string replaceWholeWordFromMap(const std::string& input, const std::unordered_map<std::string, std::string>& replacements);

// Convert dedicated segments without needing to parse already
std::string naturalSegmentToLaTeX(const std::string& natural_segment);

}