#pragma once

#include "subjects.hpp"

class Parser {
public:
    Parser(const std::string& expr) : expression(expr), pos(0) {}
    
    std::unique_ptr<Expr> parse() {
        auto expr = parseExpression();
        // Проверяем, что достигнут конец выражения
        skipWhitespace();
        if (pos < expression.length()) {
            throw std::runtime_error("Unexpected character at end");
        }
        // Автоматически собираем все переменные
        auto vars = expr->getVariables();
        // Удаляем дубликаты
        std::sort(vars.begin(), vars.end());
        vars.erase(std::unique(vars.begin(), vars.end()), vars.end());
        variables = vars;
        return expr;
    }
    
    std::vector<std::string> getVariables() const {
        return variables;
    }
    
private:
    std::string expression;
    size_t pos;
    std::vector<std::string> variables;
    
    void skipWhitespace() {
        while (pos < expression.length() && std::isspace(expression[pos])) {
            pos++;
        }
    }
    
    char peek() {
        skipWhitespace();
        if (pos < expression.length()) {
            return expression[pos];
        }
        return '\0';
    }
    
    char get() {
        skipWhitespace();
        if (pos < expression.length()) {
            return expression[pos++];
        }
        return '\0';
    }
    
    bool match(char expected) {
        if (peek() == expected) {
            get();
            return true;
        }
        return false;
    }
    
    // expression : term (('+' | '-') term)*
    std::unique_ptr<Expr> parseExpression() {
        auto left = parseTerm();
        
        while (true) {
            char op = peek();
            if (op == '+' || op == '-') {
                get();
                auto right = parseTerm();
                BinaryOp::OpType opType = (op == '+') ? BinaryOp::ADD : BinaryOp::SUB;
                left = std::make_unique<BinaryOp>(opType, std::move(left), std::move(right));
            } else {
                break;
            }
        }
        return left;
    }
    
    // term : factor (('*' | '/' | '^') factor)*
    std::unique_ptr<Expr> parseTerm() {
        auto left = parseFactor();
        
        while (true) {
            char op = peek();
            if (op == '*' || op == '/' || op == '^') {
                get();
                auto right = parseFactor();
                BinaryOp::OpType opType;
                switch(op) {
                    case '*': opType = BinaryOp::MUL; break;
                    case '/': opType = BinaryOp::DIV; break;
                    case '^': opType = BinaryOp::POW; break;
                    default: throw std::runtime_error("Unexpected operator");
                }
                left = std::make_unique<BinaryOp>(opType, std::move(left), std::move(right));
            } else {
                break;
            }
        }
        return left;
    }
    
    // factor : number | variable | '(' expression ')' | '{' vector '}' | function '(' expression ')' | unary
    std::unique_ptr<Expr> parseFactor() {
        char c = peek();
        
        // Унарный плюс (игнорируем)
        if (c == '+') {
            get();
            return parseFactor();
        }
        
        // Унарный минус
        if (c == '-') {
            get();
            auto operand = parseFactor();
            return std::make_unique<UnaryOp>(UnaryOp::NEG, std::move(operand));
        }
        
        // Скобки
        if (c == '(') {
            get();
            auto expr = parseExpression();
            if (!match(')')) {
                throw std::runtime_error("Expected ')'");
            }
            return expr;
        }
        
        // Вектор
        if (c == '{') {
            return parseVector();
        }
        
        // Число
        if (std::isdigit(c) || c == '.') {
            return parseNumber();
        }
        
        // Идентификатор (переменная или функция)
        if (std::isalpha(c) || c == '_') {
            return parseIdentifier();
        }
        
        throw std::runtime_error(std::string("Unexpected character: ") + c);
    }
    
    // vector : '{' expression (',' expression)* '}'
    std::unique_ptr<Expr> parseVector() {
        get(); // '{'
        std::vector<std::unique_ptr<Expr>> elements;
        
        while (true) {
            auto expr = parseExpression();
            elements.push_back(std::move(expr));
            
            char c = peek();
            if (c == ',') {
                get(); // ','
                continue;
            } else if (c == '}') {
                get(); // '}'
                break;
            } else {
                throw std::runtime_error("Expected ',' or '}' in vector");
            }
        }
        
        return std::make_unique<VectorExpr>(std::move(elements));
    }
    
    std::unique_ptr<Number> parseNumber() {
        std::string num;
        bool hasDot = false;
        
        while (pos < expression.length()) {
            char c = expression[pos];
            if (std::isdigit(c)) {
                num += c;
                pos++;
            } else if (c == '.' && !hasDot) {
                num += c;
                hasDot = true;
                pos++;
            } else {
                break;
            }
        }
        
        if (num.empty()) {
            throw std::runtime_error("Invalid number");
        }
        
        return std::make_unique<Number>(std::stod(num));
    }
    
    std::unique_ptr<Expr> parseIdentifier() {
        std::string name;
        while (pos < expression.length() && (std::isalnum(expression[pos]) || expression[pos] == '_')) {
            name += expression[pos++];
        }
        
        // Проверяем функции
        if (peek() == '(') {
            get(); // '('
            auto arg = parseExpression();
            if (!match(')')) {
                throw std::runtime_error("Expected ')'");
            }
            
            UnaryOp::OpType opType;
            if (name == "sin") opType = UnaryOp::SIN;
            else if (name == "cos") opType = UnaryOp::COS;
            else if (name == "tan") opType = UnaryOp::TAN;
            else if (name == "ctg" || name == "cot") opType = UnaryOp::COT;
            else if (name == "sqrt") opType = UnaryOp::SQRT;
            else if (name == "exp") opType = UnaryOp::EXP;
            else if (name == "log" || name == "ln") opType = UnaryOp::LOG;
            else if (name == "log10") opType = UnaryOp::LOG10;
            else if (name == "sh" || name == "sinh") opType = UnaryOp::SINH;
            else if (name == "ch" || name == "cosh") opType = UnaryOp::COSH;
            else if (name == "th" || name == "tanh") opType = UnaryOp::TANH;
            else if (name == "sec") opType = UnaryOp::SEC;
            else if (name == "csc") opType = UnaryOp::CSC;
            else if (name == "arctg" || name == "atan") opType = UnaryOp::ARCTG;
            else if (name == "arccos" || name == "acos") opType = UnaryOp::ARCCOS;
            else if (name == "arcsin" || name == "asin") opType = UnaryOp::ARCSIN;
            else if (name == "arcctg" || name == "acot") opType = UnaryOp::ARCCTG;
            else if (name == "abs") opType = UnaryOp::ABS;
            else if (name == "sign") opType = UnaryOp::SIGN;
            else if (name == "floor") opType = UnaryOp::FLOOR;
            else if (name == "ceil") opType = UnaryOp::CEIL;
            else if (name == "round") opType = UnaryOp::ROUND;
            else {
                throw std::runtime_error("Unknown function: " + name);
            }
            
            return std::make_unique<UnaryOp>(opType, std::move(arg));
        }
        
        // Переменная
        return std::make_unique<Variable>(name);
    }
};