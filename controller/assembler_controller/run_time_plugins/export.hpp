#include "expression_processor.hpp"
#include <iostream>
#include <vector>
#include <type_traits>

// Шаблонный класс для работы с выражениями
template<typename T>
class MathExpression {
public:
    MathExpression(const std::string& expr) {
        auto parsed = ExpressionProcessor::parse(expr);
        ast = std::move(parsed.ast);
        variables = parsed.variables;
    }
    
    // Универсальная функция evaluate для скаляров
    std::vector<T> evaluate(const std::unordered_map<std::string, std::vector<T>>& vars) const {
        // Определяем размер векторов
        size_t size = 0;
        for (const auto& [key, vec] : vars) {
            if (size == 0) size = vec.size();
            else if (vec.size() != size) {
                throw std::runtime_error("All vectors must have the same size");
            }
        }
        
        std::vector<T> results(size);
        for (size_t i = 0; i < size; ++i) {
            std::unordered_map<std::string, double> doubleVars;
            for (const auto& [key, vec] : vars) {
                doubleVars[key] = static_cast<double>(vec[i]);
            }
            results[i] = static_cast<T>(ast->eval(doubleVars));
        }
        return results;
    }
    
    // Получить список переменных
    const std::vector<std::string>& getVariables() const {
        return variables;
    }
    
    // Получить строковое представление
    std::string toString() const {
        return ast->toString();
    }
    std::function<std::vector<T>(const std::unordered_map<std::string, std::vector<T>>&)> f() {
        return [this](const std::unordered_map<std::string, std::vector<T>>& vars) -> std::vector<T> {
            return evaluate(vars);
        };
    }
private:
    std::unique_ptr<Expr> ast;
    std::vector<std::string> variables;
};

// Упрощенная функция evaluate для скаляров
template<typename T, typename U>
T evaluate(const std::string& expr, const std::unordered_map<std::string, U>& vars) {
    MathExpression<T> mathExpr(expr);
    return mathExpr.evaluate(vars);
}

// Упрощенная функция evaluate для векторов
template<typename T, typename U>
std::vector<T> evaluate(const std::string& expr, const std::unordered_map<std::string, std::vector<U>>& vars) {
    MathExpression<T> mathExpr(expr);
    return mathExpr.evaluate(vars);
}

// Перегрузка для пустых переменных
template<typename T>
T evaluate(const std::string& expr) {
    MathExpression<T> mathExpr(expr);
    return mathExpr.evaluate();
}