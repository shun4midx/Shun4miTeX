/********************************************
 * Copyright (c) 2026 Shun/修海 (@shun4midx) *
 * Project: Shun4miTeX                      *
 * File Type: C++ file                      *
 * File: natural_math.cpp                   *
 ****************************************** */

#include "natural_math.h"
#include <stdexcept>

// ======== FUNCTIONS ======== //
namespace Shun4miTeX::Natural {

// Main function
std::string naturalToLaTeX(const std::string& full_natural_text) {
    std::vector<NaturalChunk> spliced_chunks = spliceNaturalText(full_natural_text);

    for (auto& chunk : spliced_chunks) {
        if (chunk.kind != NaturalChunkKind::Text) {
            chunk.text = naturalSegmentToLaTeX(chunk.text);
        }
    }

    return fuseSplicedText(spliced_chunks);
}

// Parse and stitch sections
std::vector<NaturalChunk> spliceNaturalText(const std::string& full_natural_text) {
    std::vector<NaturalChunk> chunks;

    std::size_t i = 0;
    std::size_t text_start = 0;

    auto pushText = [&](std::size_t end) {
        if (end > text_start) {
            chunks.push_back({full_natural_text.substr(text_start, end - text_start), NaturalChunkKind::Text});
        }
    };

    while (i < full_natural_text.size()) {
        std::string delimiter;
        NaturalChunkKind math_kind;

        if (full_natural_text.compare(i, 3, "@@@") == 0) {
            delimiter = "@@@";
            math_kind = NaturalChunkKind::DisplayMath;
        } else if (full_natural_text.compare(i, 2, "@@") == 0) {
            delimiter = "@@";
            math_kind = NaturalChunkKind::InlineMath;
        } else {
            ++i;
            continue;
        }

        pushText(i);

        std::size_t math_start = i + delimiter.size();
        std::size_t math_end = full_natural_text.find(delimiter, math_start);

        // Unmatched delimiter: treat the rest as normal text.
        if (math_end == std::string::npos) {
            chunks.push_back({full_natural_text.substr(i), NaturalChunkKind::Text});
            return chunks;
        }

        chunks.push_back({full_natural_text.substr(math_start, math_end - math_start), math_kind});

        i = math_end + delimiter.size();
        text_start = i;
    }

    if (text_start < full_natural_text.size()) {
        chunks.push_back({full_natural_text.substr(text_start), NaturalChunkKind::Text});
    }

    return chunks;
}

std::string fuseSplicedText(const std::vector<NaturalChunk>& spliced_text) {
    std::string result;

    for (const auto& chunk : spliced_text) {
        if (chunk.kind == NaturalChunkKind::Text) {
            result += chunk.text;
        } else if (chunk.kind == NaturalChunkKind::InlineMath) {
            result += ("$" + chunk.text + "$");
        } else if (chunk.kind == NaturalChunkKind::DisplayMath) {
            result += ("\\[" + chunk.text + "\\]");
        }
    }

    return result;
}

std::string joinVector(const std::vector<std::string>& str_vec, const std::string& sep) {
    std::string result;

    for (std::size_t i = 0; i < str_vec.size(); ++i) {
        if (i > 0) {
            result += sep;
        }
        result += str_vec[i];
    }

    return result;
}

// Helpful functions for conversion
bool startsWithAt(const std::string& s, std::size_t pos,const std::string& token) {
    return (pos + token.size() <= s.size()) && (s.compare(pos, token.size(), token) == 0);
}

std::size_t utf8CharLen(unsigned char c) {
    if ((c & 0b10000000) == 0) return 1;
    if ((c & 0b11100000) == 0b11000000) return 2;
    if ((c & 0b11110000) == 0b11100000) return 3;
    if ((c & 0b11111000) == 0b11110000) return 4;
    return 1;
}

std::string replaceAllFromMap(const std::string& input, const std::unordered_map<std::string, std::string>& replacements) {
    std::string result = input;

    for (const auto& [from, to] : replacements) {
        std::size_t pos = 0;

        while ((pos = result.find(from, pos)) != std::string::npos) {
            result.replace(pos, from.size(), to);
            pos += to.size();
        }
    }

    return result;
}

std::string matchTokenAt(const std::string& s, std::size_t pos, const std::unordered_map<std::string, std::string>& mp) {
    for (const auto& [key, value] : mp) {
        if (startsWithAt(s, pos, key)) {
            return key;
        }
    }

    return "";
}

std::string convertSuperSubDigits(const std::string& input) {
    std::string out;

    for (std::size_t i = 0; i < input.size();) {
        std::string key = matchTokenAt(input, i, SUPERSCRIPTS);

        if (!key.empty()) {
            std::string digits;

            while (!key.empty()) {
                digits += SUPERSCRIPTS.at(key);
                i += key.size();
                key = matchTokenAt(input, i, SUPERSCRIPTS);
            }

            out += "^{" + digits + "}";
            continue;
        }

        key = matchTokenAt(input, i, SUBSCRIPTS);

        if (!key.empty()) {
            std::string digits;

            while (!key.empty()) {
                digits += SUBSCRIPTS.at(key);
                i += key.size();
                key = matchTokenAt(input, i, SUBSCRIPTS);
            }

            out += "_{" + digits + "}";
            continue;
        }

        std::size_t len = utf8CharLen(static_cast<unsigned char>(input[i]));
        out.append(input, i, len);
        i += len;
    }

    return out;
}

bool isWordChar(char c) {
    return std::isalnum(static_cast<unsigned char>(c)) || c == '_';
}

bool hasWordBoundary(const std::string& s, std::size_t pos, std::size_t len) {
    // Do not replace inside already-created LaTeX commands e.g. \pi
    if (pos > 0 && s[pos - 1] == '\\') {
        return false;
    }

    bool left_ok = (pos == 0) || !isWordChar(s[pos - 1]);
    bool right_ok = (pos + len >= s.size()) || !isWordChar(s[pos + len]);
    return left_ok && right_ok;
}


std::string replaceWholeWordFromMap(const std::string& input, const std::unordered_map<std::string, std::string>& replacements) {
    std::string result = input;

    for (const auto& [from, to] : replacements) {
        std::size_t pos = 0;

        while ((pos = result.find(from, pos)) != std::string::npos) {
            if (!hasWordBoundary(result, pos, from.size())) {
                pos += from.size();
                continue;
            }

            result.replace(pos, from.size(), to);
            pos += to.size();
        }
    }

    return result;
}

// Convert dedicated segments
std::size_t findMatchingParen(const std::string& s, std::size_t open_pos) {
    if (open_pos >= s.size() || s[open_pos] != '(') {
        return std::string::npos;
    }

    int depth = 0;

    for (std::size_t i = open_pos; i < s.size(); ++i) {
        if (s[i] == '(') {
            ++depth;
        } else if (s[i] == ')') {
            --depth;
            if (depth == 0) {
                return i;
            }
        }
    }

    return std::string::npos;
}

std::string convertParenthesizedExponents(const std::string& input) {
    std::string out;

    for (std::size_t i = 0; i < input.size();) {
        if (i + 1 < input.size() && input[i] == '^' && input[i + 1] == '(') {
            std::size_t open_pos = i + 1;
            std::size_t close_pos = findMatchingParen(input, open_pos);

            if (close_pos != std::string::npos) {
                std::string inside = input.substr(open_pos + 1, close_pos - open_pos - 1);
                out += "^{" + naturalSegmentToLaTeX(inside) + "}";
                i = close_pos + 1;
                continue;
            }
        }

        std::size_t len = utf8CharLen(static_cast<unsigned char>(input[i]));
        out.append(input, i, len);
        i += len;
    }

    return out;
}

std::string convertSpecialFunctions(const std::string& input);

std::string naturalSegmentToLaTeX(const std::string& natural_segment) {
    std::string result = natural_segment;

    result = replaceAllFromMap(result, UNICODE_SYMBOLS);
    result = convertSuperSubDigits(result);
    result = convertParenthesizedExponents(result);
    result = convertSpecialFunctions(result);

    result = replaceWholeWordFromMap(result, MATHBB_LETTERS);
    result = replaceWholeWordFromMap(result, GREEK_LETTERS);
    result = replaceWholeWordFromMap(result, SIMPLE_OPERATORS);
    result = replaceWholeWordFromMap(result, OPERATORS);
    result = replaceWholeWordFromMap(result, ALIAS_COMMANDS);

    return result;
}

// Parsing through individual special functions
std::vector<std::string> splitTopLevel(const std::string& s, char delimiter) {
    std::vector<std::string> parts;
    std::size_t start = 0;
    int paren_depth = 0;
    int brace_depth = 0;
    int bracket_depth = 0;

    for (std::size_t i = 0; i < s.size(); ++i) {
        char c = s[i];

        if (c == '(') ++paren_depth;
        else if (c == ')') --paren_depth;
        else if (c == '{') ++brace_depth;
        else if (c == '}') --brace_depth;
        else if (c == '[') ++bracket_depth;
        else if (c == ']') --bracket_depth;
        else if (c == delimiter && paren_depth == 0 && brace_depth == 0 && bracket_depth == 0) {
            parts.push_back(s.substr(start, i - start));
            start = i + 1;
        }
    }

    parts.push_back(s.substr(start));
    return parts;
}

std::string trim(const std::string& s) {
    std::size_t start = 0;
    while (start < s.size() && std::isspace(static_cast<unsigned char>(s[start]))) {
        ++start;
    }

    std::size_t end = s.size();
    while (end > start && std::isspace(static_cast<unsigned char>(s[end - 1]))) {
        --end;
    }

    return s.substr(start, end - start);
}

std::string specialFunctionToLaTeX(const std::string& function_name, const std::vector<std::string>& raw_args) {
    auto it = SPECIAL_FUNCTIONS.find(function_name);
    if (it == SPECIAL_FUNCTIONS.end()) {
        throw std::runtime_error("Unknown special function: " + function_name);
    }

    std::string kind = it->second;

    auto parseArg = [](const std::string& s) {
        return naturalSegmentToLaTeX(trim(s));
    };

    if (kind == "TEXT") {
        if (raw_args.size() != 1) {
            throw std::runtime_error(function_name + " expects 1 argument");
        }

        return "\\text{" + raw_args[0] + "}";
    }

    else if (kind == "BOXED") {
        if (raw_args.size() != 1) {
            throw std::runtime_error(function_name + " expects 1 argument");
        }

        return "\\boxed{" + parseArg(raw_args[0]) + "}";
    }

    else if (kind == "SQRT") {
        if (raw_args.size() != 1) {
            throw std::runtime_error(function_name + " expects 1 argument");
        }
        return "\\sqrt{" + parseArg(raw_args[0]) + "}";
    }

    else if (kind == "FRAC") {
        if (raw_args.size() != 2) {
            throw std::runtime_error(function_name + " expects 2 arguments");
        }

        return "\\frac{" + parseArg(raw_args[0]) + "}{" + parseArg(raw_args[1]) + "}";
    }

    else if (kind == "ROOT") {
        if (raw_args.size() != 2) {
            throw std::runtime_error(function_name + " expects 2 arguments");
        }

        return "\\sqrt[" + parseArg(raw_args[0]) + "]{" + parseArg(raw_args[1]) + "}";
    }

    else if (kind == "CBRT") {
        if (raw_args.size() != 1) {
            throw std::runtime_error(function_name + " expects 1 argument");
        }

        return "\\sqrt[3]{" + parseArg(raw_args[0]) + "}";
    }

    else if (kind == "ABS") {
        if (raw_args.size() != 1) {
            throw std::runtime_error(function_name + " expects 1 argument");
        }

        return "\\left|" + parseArg(raw_args[0]) + "\\right|";
    }

    else if (kind == "NORM") {
        if (raw_args.size() != 1) {
            throw std::runtime_error(function_name + " expects 1 argument");
        }

        return "\\left\\|" + parseArg(raw_args[0]) + "\\right\\|";
    }

    else if (kind == "INT") {
        if (raw_args.size() != 4) {
            throw std::runtime_error(function_name + " expects 4 arguments");
        }

        return "\\int_{" + parseArg(raw_args[0]) + "}^{" + parseArg(raw_args[1]) + "} " + parseArg(raw_args[2]) + "\\,d" + parseArg(raw_args[3]) + " ";
    }

    else if (kind == "SUM") {
        if (raw_args.size() != 4) {
            throw std::runtime_error(function_name + " expects 4 arguments");
        }

        return "\\sum_{" + parseArg(raw_args[0]) + "=" + parseArg(raw_args[1]) + "}^{" + parseArg(raw_args[2]) + "} " + parseArg(raw_args[3]) + " ";
    }

    else if (kind == "PROD") {
        if (raw_args.size() != 4) {
            throw std::runtime_error(function_name + " expects 4 arguments");
        }

        return "\\prod_{" + parseArg(raw_args[0]) + "=" + parseArg(raw_args[1]) + "}^{" + parseArg(raw_args[2]) + "} " + parseArg(raw_args[3]) + " ";
    }

    else if (kind == "LIM") {
        if (raw_args.size() != 3) {
            throw std::runtime_error(function_name + " expects 3 arguments");
        }

        return "\\lim_{" + parseArg(raw_args[0]) + "\\to " + parseArg(raw_args[1]) + "} " + parseArg(raw_args[2]) + " ";
    }

    else if (kind == "FRAC") {
        if (raw_args.size() != 2) {
            throw std::runtime_error(function_name + " expects 2 arguments");
        }

        return "\\frac{" + parseArg(raw_args[0]) + "}{" + parseArg(raw_args[1]) + "}";
    }

    else if (kind == "DEG") {
        if (raw_args.size() != 1) {
            throw std::runtime_error(function_name + " expects 1 argument");
        }

        return parseArg(raw_args[0]) + "^{\\circ}";
    }

    else if (kind == "ANGLE") {
        if (raw_args.size() != 1) {
            throw std::runtime_error(function_name + " expects 1 argument");
        }

        return "\\angle{" + parseArg(raw_args[0]) + "}";
    }

    else if (kind == "FLOOR") {
        if (raw_args.size() != 1) {
            throw std::runtime_error(function_name + " expects 1 argument");
        }

        return "\\left\\lfloor " + parseArg(raw_args[0]) + "\\right\\rfloor ";
    }

    else if (kind == "CEIL") {
        if (raw_args.size() != 1) {
            throw std::runtime_error(function_name + " expects 1 argument");
        }

        return "\\left\\lceil " + parseArg(raw_args[0]) + "\\right\\rceil ";
    }

    else if (kind == "DERIV") {
        if (raw_args.size() != 2) {
            throw std::runtime_error(function_name + " expects 2 arguments");
        }

        return "\\frac{d" + parseArg(raw_args[0]) + "}{d" + parseArg(raw_args[1]) + "}";
    }

    else if (kind == "DDX") {
        if (raw_args.size() != 2) {
            throw std::runtime_error(function_name + " expects 2 arguments");
        }

        return "\\frac{d}{d" + parseArg(raw_args[1]) + "}\\left(" + parseArg(raw_args[0]) + "\\right)";
    }

    else if (kind == "PDERIV") {
        if (raw_args.size() != 2) {
            throw std::runtime_error(function_name + " expects 2 arguments");
        }

        return "\\frac{\\partial " + parseArg(raw_args[0]) + "}{\\partial " + parseArg(raw_args[1]) + "}";
    }

    else if (kind == "PDDX") {
        if (raw_args.size() != 2) {
            throw std::runtime_error(function_name + " expects 2 arguments");
        }

        return "\\frac{\\partial}{\\partial " + parseArg(raw_args[1]) + "}\\left(" + parseArg(raw_args[0]) + "\\right)";
    }

    throw std::runtime_error("Unknown special function: " + function_name);
}

std::string matrixToLaTeX(const std::string& op_name, const std::string& matrix_body) {
    std::string env_name = op_name;

    std::vector<std::string> rows = splitTopLevel(matrix_body, ';');
    std::vector<std::string> latex_rows;

    std::size_t expected_cols = 0;

    for (std::string row : rows) {
        std::vector<std::string> entries = splitTopLevel(row, ',');

        if (expected_cols == 0) {
            expected_cols = entries.size();
        } else if (entries.size() != expected_cols) {
            throw std::runtime_error("Matrix rows must have the same number of entries");
        }

        std::vector<std::string> parsed_entries;
        for (const std::string& entry : entries) {
            parsed_entries.push_back(naturalSegmentToLaTeX(trim(entry)));
        }

        latex_rows.push_back(joinVector(parsed_entries, " & "));
    }

    return "\\begin{" + env_name + "}" + joinVector(latex_rows, " \\\\ ") + "\\end{" + env_name + "}";
}

std::string convertSpecialFunctions(const std::string& input) {
    std::string out;

    for (std::size_t i = 0; i < input.size();) {
        if (std::isalpha(static_cast<unsigned char>(input[i]))) {
            std::size_t start = i;
            while (i < input.size() && isWordChar(input[i])) ++i;

            std::string name = input.substr(start, i - start);

            if (SPECIAL_FUNCTIONS.count(name) && i < input.size() && input[i] == '(') {
                std::size_t close = findMatchingParen(input, i);
                if (close != std::string::npos) {
                    std::string body = input.substr(i + 1, close - i - 1);

                    if (name == "matrix" || name == "pmatrix" || name == "bmatrix" || name == "vmatrix") {
                        out += matrixToLaTeX(name, body);
                    } else {
                        out += specialFunctionToLaTeX(name, splitTopLevel(body, ','));
                    }

                    i = close + 1;
                    continue;
                }
            }

            out += name;
            continue;
        }

        std::size_t len = utf8CharLen(static_cast<unsigned char>(input[i]));
        out.append(input, i, len);
        i += len;
    }

    return out;
}

}