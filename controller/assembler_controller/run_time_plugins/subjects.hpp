#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <cmath>
#include <regex>
#include <sstream>

// Базовый класс узла
struct Expr {
    virtual ~Expr() = default;
    virtual double eval(const std::unordered_map<std::string, double>& vars) const = 0;
    virtual std::string toString() const = 0;
    virtual std::vector<std::string> getVariables() const = 0;
};

// Узел для чисел
struct Number : Expr {
    double value;
    Number(double v) : value(v) {}
    
    double eval(const std::unordered_map<std::string, double>&) const override {
        return value;
    }
    
    std::string toString() const override {
        return std::to_string(value);
    }
    
    std::vector<std::string> getVariables() const override {
        return {};
    }
};

// Узел для переменных
struct Variable : Expr {
    std::string name;
    Variable(const std::string& n) : name(n) {}
    
    double eval(const std::unordered_map<std::string, double>& vars) const override {
        auto it = vars.find(name);
        if (it != vars.end()) {
            return it->second;
        }
        throw std::runtime_error("Variable not found: " + name);
    }
    
    std::string toString() const override {
        return name;
    }
    
    std::vector<std::string> getVariables() const override {
        return {name};
    }
};

// Узел для вектора
struct VectorExpr : Expr {
    std::vector<std::unique_ptr<Expr>> elements;
    
    VectorExpr(std::vector<std::unique_ptr<Expr>> elems) 
        : elements(std::move(elems)) {}
    
    double eval(const std::unordered_map<std::string, double>& vars) const override {
        double sum = 0;
        for (const auto& elem : elements) {
            sum += elem->eval(vars);
        }
        return sum;
    }
    
    std::string toString() const override {
        std::string result = "{";
        for (size_t i = 0; i < elements.size(); ++i) {
            if (i > 0) result += ", ";
            result += elements[i]->toString();
        }
        result += "}";
        return result;
    }
    
    std::vector<std::string> getVariables() const override {
        std::vector<std::string> vars;
        for (const auto& elem : elements) {
            auto subVars = elem->getVariables();
            vars.insert(vars.end(), subVars.begin(), subVars.end());
        }
        return vars;
    }
};

// Узел для бинарных операций
struct BinaryOp : Expr {
    enum OpType { ADD, SUB, MUL, DIV, POW };
    
    OpType op;
    std::unique_ptr<Expr> left;
    std::unique_ptr<Expr> right;
    
    BinaryOp(OpType o, std::unique_ptr<Expr> l, std::unique_ptr<Expr> r)
        : op(o), left(std::move(l)), right(std::move(r)) {}
    
    double eval(const std::unordered_map<std::string, double>& vars) const override {
        double l = left->eval(vars);
        double r = right->eval(vars);
        
        switch(op) {
            case ADD: return l + r;
            case SUB: return l - r;
            case MUL: return l * r;
            case DIV: return l / r;
            case POW: return std::pow(l, r);
        }
        return 0;
    }
    
    std::string toString() const override {
        std::string opStr;
        switch(op) {
            case ADD: opStr = "+"; break;
            case SUB: opStr = "-"; break;
            case MUL: opStr = "*"; break;
            case DIV: opStr = "/"; break;
            case POW: opStr = "^"; break;
        }
        return "(" + left->toString() + " " + opStr + " " + right->toString() + ")";
    }
    
    std::vector<std::string> getVariables() const override {
        auto vars = left->getVariables();
        auto rightVars = right->getVariables();
        vars.insert(vars.end(), rightVars.begin(), rightVars.end());
        return vars;
    }
};

// Узел для унарных операций
struct UnaryOp : Expr {
    enum OpType { 
        NEG, SQRT, SIN, COS, EXP, LOG, LOG10, TAN, COT, 
        SINH, COSH, TANH, SEC, CSC, 
        ARCTG, ARCCOS, ARCSIN, ARCCTG, 
        ABS, SIGN, FLOOR, CEIL, ROUND 
    };
    
    OpType op;
    std::unique_ptr<Expr> operand;
    
    UnaryOp(OpType o, std::unique_ptr<Expr> opd) : op(o), operand(std::move(opd)) {}
    
    double eval(const std::unordered_map<std::string, double>& vars) const override {
        double val = operand->eval(vars);
        
        switch(op) {
            case NEG:    return -val;
            case SQRT:   return std::sqrt(val);
            case SIN:    return std::sin(val);
            case COS:    return std::cos(val);
            case EXP:    return std::exp(val);
            case LOG:    return std::log(val);
            case LOG10:  return std::log10(val);                    // ✅ Добавлено
            case TAN:    return std::tan(val);
            case COT:    return 1.0 / std::tan(val);
            case SINH:   return std::sinh(val);
            case COSH:   return std::cosh(val);
            case TANH:   return std::tanh(val);                    // ✅ Добавлено
            case SEC:    return 1.0 / std::cos(val);
            case CSC:    return 1.0 / std::sin(val);               // ✅ Добавлено
            case ARCTG:  return std::atan(val);
            case ARCCOS: return std::acos(val);
            case ARCSIN: return std::asin(val);
            case ARCCTG: return std::atan(1.0 / val);              // ✅ Добавлено
            case ABS:    return std::abs(val);                     // ✅ Добавлено
            case SIGN:   return (val > 0) ? 1.0 : (val < 0) ? -1.0 : 0.0; // ✅ Добавлено
            case FLOOR:  return std::floor(val);                   // ✅ Добавлено
            case CEIL:   return std::ceil(val);                    // ✅ Добавлено
            case ROUND:  return std::round(val);                   // ✅ Добавлено
        }
        return 0;
    }
    
    std::string toString() const override {
        std::string opStr;
        switch(op) {
            case NEG:    opStr = "neg"; break;
            case SQRT:   opStr = "sqrt"; break;
            case SIN:    opStr = "sin"; break;
            case COS:    opStr = "cos"; break;
            case EXP:    opStr = "exp"; break;
            case LOG:    opStr = "log"; break;
            case LOG10:  opStr = "log10"; break;                  // ✅ Добавлено
            case TAN:    opStr = "tan"; break;
            case COT:    opStr = "ctg"; break;
            case SINH:   opStr = "sh"; break;
            case COSH:   opStr = "ch"; break;
            case TANH:   opStr = "tanh"; break;                   // ✅ Добавлено
            case SEC:    opStr = "sec"; break;
            case CSC:    opStr = "csc"; break;                    // ✅ Добавлено
            case ARCTG:  opStr = "arctg"; break;
            case ARCCOS: opStr = "arccos"; break;
            case ARCSIN: opStr = "arcsin"; break;
            case ARCCTG: opStr = "arcctg"; break;                 // ✅ Добавлено
            case ABS:    opStr = "abs"; break;                    // ✅ Добавлено
            case SIGN:   opStr = "sign"; break;                   // ✅ Добавлено
            case FLOOR:  opStr = "floor"; break;                  // ✅ Добавлено
            case CEIL:   opStr = "ceil"; break;                   // ✅ Добавлено
            case ROUND:  opStr = "round"; break;                  // ✅ Добавлено
        }
        return opStr + "(" + operand->toString() + ")";
    }
    
    std::vector<std::string> getVariables() const override {
        return operand->getVariables();
    }
};