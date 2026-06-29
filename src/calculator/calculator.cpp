/********************************************
 * Copyright (c) 2026 Shun/修海 (@shun4midx) *
 * Project: Shun4miTeX                      *
 * File Type: C++ file                      *
 * File: calculator.cpp                     *
 ****************************************** */

#include "calculator.h"

#include <cmath>
#include <cstddef>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>
#include <numbers>
#include <cstdlib>
#include <cctype>
#include <algorithm>
#include <sstream>

namespace Shun4miTeX {

/* Recall:
struct CalcResult {
    bool ok;
    double value;
    std::string error;
};
*/

namespace { // Tokenizer, Parser, helper functions

class CalcError : public std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};

// ======== USEFUL FUNCTIONS ======== //
long long requireNonNegativeInteger(double x, const std::string& name);
double factorial(double x);
double choose(double n, double k);
double perm(double n, double k);

// ======== CLASSES AND DEFINITIONS ======== //
enum class TokenType {
    Number,
    Identifier,
    Plus,
    Minus,
    Star,
    Slash,
    Caret,
    Bang,
    LParen,
    RParen,
    Comma,
    End,
    Invalid
};

struct Token {
    TokenType type;
    std::string text;
    double number = 0.0;
};

class Tokenizer {
public:
    explicit Tokenizer(std::string_view input) : input_(input) {
        next();
    }

    const Token& peek() const;
    Token consume();

private:
    void next();

    std::string input_;
    size_t pos_ = 0;
    Token current_;
};

class Parser {
public:
    explicit Parser(std::string_view input);

    CalcResult parse();

private:
    double parseExpression();
    double parseAddSub();
    double parseMulDiv();
    double parseUnary();
    double parsePower();
    double parsePostfix();
    double parsePrimary();

    std::vector<double> parseArguments();

    bool match(TokenType type);
    void expect(TokenType type, const std::string& message);

    double callFunction(const std::string& name, const std::vector<double>& args);
    double constantValue(const std::string& name);

    [[noreturn]] void fail(const std::string& message) {
        throw CalcError(message);
    }

    Tokenizer tokenizer_;
};

// ======== IMPLEMENTATIONS ======== //

// ~~~~~~~~ TOKENIZER IMPLEMENTATION ~~~~~~~~ //
const Token& Tokenizer::peek() const {
    return current_;
}

Token Tokenizer::consume() {
    Token old = current_;
    next();
    return old;
}

void Tokenizer::next() {
    while (pos_ < input_.size() && std::isspace(static_cast<unsigned char>(input_[pos_]))) {
        ++pos_;
    }

    if (pos_ >= input_.size()) {
        current_ = {TokenType::End, ""};
        return;
    }

    char c = input_[pos_];

    switch (c) {
        case '+':
            ++pos_;
            current_ = {TokenType::Plus, "+"};
            return;
        case '-':
            ++pos_;
            current_ = {TokenType::Minus, "-"};
            return;
        case '*':
            ++pos_;
            current_ = {TokenType::Star, "*"};
            return;
        case '/':
            ++pos_;
            current_ = {TokenType::Slash, "/"};
            return;
        case '^':
            ++pos_;
            current_ = {TokenType::Caret, "^"};
            return;
        case '!':
            ++pos_;
            current_ = {TokenType::Bang, "!"};
            return;
        case '(':
            ++pos_;
            current_ = {TokenType::LParen, "("};
            return;
        case ')':
            ++pos_;
            current_ = {TokenType::RParen, ")"};
            return;
        case ',':
            ++pos_;
            current_ = {TokenType::Comma, ","};
            return;
    }

    if (std::isdigit(static_cast<unsigned char>(c)) || c == '.') {
        const char* begin = input_.c_str() + pos_;
        char* end = nullptr;

        double value = std::strtod(begin, &end);

        if (end == begin) {
            ++pos_;
            current_ = {TokenType::Invalid, std::string(1, c)};
            return;
        }

        size_t length = static_cast<size_t>(end - begin);
        std::string text = input_.substr(pos_, length);

        pos_ += length;
        current_ = {TokenType::Number, text, value};
        return;
    }

    if (std::isalpha(static_cast<unsigned char>(c)) || c == '_') {
        size_t start = pos_;

        while (pos_ < input_.size()) {
            char ch = input_[pos_];

            if (!std::isalnum(static_cast<unsigned char>(ch)) && ch != '_') {
                break;
            }

            ++pos_;
        }

        current_ = {
            TokenType::Identifier,
            input_.substr(start, pos_ - start)
        };
        return;
    }

    ++pos_;
    current_ = {TokenType::Invalid, std::string(1, c)};
}

// ~~~~~~~~ PARSER IMPLEMENTATION ~~~~~~~~ //

Parser::Parser(std::string_view input) : tokenizer_(input) {}

bool Parser::match(TokenType type) {
    if (tokenizer_.peek().type == type) {
        tokenizer_.consume();
        return true;
    }

    return false;
}

void Parser::expect(TokenType type, const std::string& message) {
    if (!match(type)) {
        fail(message);
    }
}

CalcResult Parser::parse() {
    try {
        double value = parseExpression();

        if (tokenizer_.peek().type != TokenType::End) {
            if (tokenizer_.peek().type == TokenType::Invalid) {
                fail("Invalid token: " + tokenizer_.peek().text);
            }

            fail("Unexpected token: " + tokenizer_.peek().text);
        }

        return {true, value, ""};
    } catch (const CalcError& e) {
        return {false, 0.0, e.what()};
    }
}

double Parser::parseExpression() {
    return parseAddSub();
}

double Parser::parseAddSub() {
    double lhs = parseMulDiv();

    while (true) {
        if (match(TokenType::Plus)) {
            lhs += parseMulDiv();
        } else if (match(TokenType::Minus)) {
            lhs -= parseMulDiv();
        } else {
            break;
        }
    }

    return lhs;
}

double Parser::parseMulDiv() {
    double lhs = parseUnary();

    while (true) {
        if (match(TokenType::Star)) {
            lhs *= parseUnary();
        } else if (match(TokenType::Slash)) {
            double rhs = parseUnary();

            if (rhs == 0.0) {
                fail("Division by zero");
            }

            lhs /= rhs;
        } else {
            break;
        }
    }

    return lhs;
}

double Parser::parseUnary() {
    if (match(TokenType::Plus)) {
        return parseUnary();
    }

    if (match(TokenType::Minus)) {
        return -parseUnary();
    }

    return parsePower();
}

double Parser::parsePower() {
    double lhs = parsePostfix();

    if (match(TokenType::Caret)) {
        double rhs = parseUnary();
        return std::pow(lhs, rhs);
    }

    return lhs;
}

double Parser::parsePostfix() {
    double value = parsePrimary();

    while (match(TokenType::Bang)) {
        value = factorial(value);
    }

    return value;
}

double Parser::parsePrimary() {
    Token token = tokenizer_.peek();

    if (match(TokenType::Number)) {
        return token.number;
    }

    if (match(TokenType::Identifier)) {
        std::string name = token.text;

        if (match(TokenType::LParen)) {
            std::vector<double> args;

            if (tokenizer_.peek().type != TokenType::RParen) {
                args = parseArguments();
            }

            expect(TokenType::RParen, "Expected ')' after function arguments");
            return callFunction(name, args);
        }

        return constantValue(name);
    }

    if (match(TokenType::LParen)) {
        double value = parseExpression();
        expect(TokenType::RParen, "Expected ')'");
        return value;
    }

    if (tokenizer_.peek().type == TokenType::Invalid) {
        fail("Invalid token: " + tokenizer_.peek().text);
    }

    fail("Expected number, constant, function, or '('");
}

std::vector<double> Parser::parseArguments() {
    std::vector<double> args;

    args.push_back(parseExpression());

    while (match(TokenType::Comma)) {
        args.push_back(parseExpression());
    }

    return args;
}

double Parser::constantValue(const std::string& name) {
    if (name == "pi") {
        return std::numbers::pi;
    }

    if (name == "e") {
        return std::numbers::e;
    }

    fail("Unknown constant: " + name);
}

// ~~~~~~~~ HELPER FUNCTION IMPLEMENTATION ~~~~~~~~ //

long long requireNonNegativeInteger(double x, const std::string& name) {
    if (!std::isfinite(x)) {
        throw CalcError(name + " must be finite");
    }

    if (x < 0) {
        throw CalcError(name + " must be non-negative");
    }

    double rounded = std::round(x);

    if (std::abs(x - rounded) > 1e-9) {
        throw CalcError(name + " must be an integer");
    }

    return static_cast<long long>(rounded);
}

double factorial(double x) {
    long long n = requireNonNegativeInteger(x, "factorial argument");

    double result = 1.0;

    for (long long i = 2; i <= n; ++i) {
        result *= static_cast<double>(i);

        if (!std::isfinite(result)) {
            throw CalcError("factorial overflow");
        }
    }

    return result;
}

double choose(double n_raw, double k_raw) {
    long long n = requireNonNegativeInteger(n_raw, "choose n");
    long long k = requireNonNegativeInteger(k_raw, "choose k");

    if (k > n) {
        return 0.0;
    }

    k = std::min(k, n - k);

    double result = 1.0;

    for (long long i = 1; i <= k; ++i) {
        result *= static_cast<double>(n - k + i);
        result /= static_cast<double>(i);

        if (!std::isfinite(result)) {
            throw CalcError("choose overflow");
        }
    }

    return result;
}

double perm(double n_raw, double k_raw) {
    long long n = requireNonNegativeInteger(n_raw, "perm n");
    long long k = requireNonNegativeInteger(k_raw, "perm k");

    if (k > n) {
        return 0.0;
    }

    double result = 1.0;

    for (long long i = 0; i < k; ++i) {
        result *= static_cast<double>(n - i);

        if (!std::isfinite(result)) {
            throw CalcError("perm overflow");
        }
    }

    return result;
}

double Parser::callFunction(const std::string& name, const std::vector<double>& args) {
    auto requireArgs = [&](size_t n) {
        if (args.size() != n) {
            fail(name + " expects " + std::to_string(n) + " argument(s)");
        }
    };

    if (name == "sqrt") {
        requireArgs(1);
        if (args[0] < 0) {
            fail("sqrt domain error");
        }
        return std::sqrt(args[0]);
    }

    if (name == "root") {
        requireArgs(2);
        if (args[1] == 0.0) {
            fail("root degree cannot be zero");
        }
        return std::pow(args[0], 1.0 / args[1]);
    }

    if (name == "abs") {
        requireArgs(1);
        return std::abs(args[0]);
    }

    if (name == "exp") {
        requireArgs(1);
        return std::exp(args[0]);
    }

    if (name == "ln") {
        requireArgs(1);
        if (args[0] <= 0.0) {
            fail("ln domain error");
        }
        return std::log(args[0]);
    }

    if (name == "log") {
        if (args.size() == 1) {
            if (args[0] <= 0.0) {
                fail("log domain error");
            }
            return std::log10(args[0]);
        }

        if (args.size() == 2) {
            if (args[0] <= 0.0) {
                fail("log domain error");
            }

            if (args[1] <= 0.0 || args[1] == 1.0) {
                fail("invalid log base");
            }

            return std::log(args[0]) / std::log(args[1]);
        }

        fail("log expects 1 or 2 arguments");
    }

    if (name == "sin") {
        requireArgs(1);
        return std::sin(args[0]);
    }

    if (name == "cos") {
        requireArgs(1);
        return std::cos(args[0]);
    }

    if (name == "tan") {
        requireArgs(1);
        return std::tan(args[0]);
    }

    if (name == "arcsin") {
        requireArgs(1);
        if (args[0] < -1.0 || args[0] > 1.0) {
            fail("arcsin domain error");
        }
        return std::asin(args[0]);
    }

    if (name == "arccos") {
        requireArgs(1);
        if (args[0] < -1.0 || args[0] > 1.0) {
            fail("arccos domain error");
        }
        return std::acos(args[0]);
    }

    if (name == "arctan") {
        requireArgs(1);
        return std::atan(args[0]);
    }

    if (name == "rad") {
        requireArgs(1);
        return args[0] * std::numbers::pi / 180.0;
    }

    if (name == "deg") {
        requireArgs(1);
        return args[0] * 180.0 / std::numbers::pi;
    }

    if (name == "choose") {
        requireArgs(2);
        return choose(args[0], args[1]);
    }

    if (name == "perm") {
        requireArgs(2);
        return perm(args[0], args[1]);
    }

    fail("Unknown function: " + name);
}

} // namespace

// ======== FUNCTIONS ======== //
CalcResult calc(std::string_view input) {
    Parser parser(input);
    return parser.parse();
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