#pragma once

#include "parse.hpp"

class ExpressionProcessor {
public:
    struct ParsedExpression {
        std::unique_ptr<Expr> ast;
        std::vector<std::string> variables;
        std::unordered_map<std::string, double> defaultValues;
    };
    
    static ParsedExpression parse(const std::string& expr) {
        Parser parser(expr);
        auto ast = parser.parse();
        auto vars = parser.getVariables();
        
        ParsedExpression result;
        result.ast = std::move(ast);
        result.variables = vars;
        
        // Устанавливаем значения по умолчанию для переменных
        for (const auto& var : vars) {
            result.defaultValues[var] = 0.0;
        }
        
        return result;
    }
    
    static double evaluate(const std::string& expr, 
                          const std::unordered_map<std::string, double>& vars) {
        Parser parser(expr);
        auto ast = parser.parse();
        return ast->eval(vars);
    }
};