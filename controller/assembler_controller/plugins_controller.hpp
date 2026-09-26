#pragma once
#include <iostream>
#include <string>
#include <type_traits>
#include <algorithm>

#include "../chek_args_f.hpp"
#include "../to_upper.hpp"
#include "../../container/container.hpp"
#include "plugins/plugins_method.hpp"
#include "plugins/plugins_function.hpp"
#include "plugins/enum.hpp"
#include "run_time_plugins/export.hpp"


#include "../../debug/debag.hpp"
/*
CollectPlug::collect_impl
принимает лямбду функцию и вызывает ее с указаетлем на функцию
за который отвечает переданный аргумент.
*/
class CollectPlug{
protected:
    // Основной метод - проверяет наличие в map
    template<typename T, typename Func>
    void collect_impl(Func func, const std::string plugin) {
        auto it = PluginsMap.find(toUpper(plugin));
        
        if (it != PluginsMap.end()) {
            // Строка ЕСТЬ в map - вызываем перегрузку для ключа
            collect<T>(func, plugin);
        }else {
            // Строки НЕТ в map - вызываем перегрузку для обычной строки
            collectFromString<T>(func, plugin);
        }
    }

private:
    // Перегрузка для случая, когда строка есть в map (ключ)
    template<typename T, typename Func>
    static void collect(Func func, const std::string& name) {
        auto it = PluginsMap.find(toUpper(name));
        if (it == PluginsMap.end()) {
            std::cerr << "Plugin not found: " << name << std::endl;
            return;
        }
        const int plugin_id = it->second;
        #define X(plugin_name, i, ...) \
        if constexpr (i) { \
            if (plugin_id == static_cast<int>(PLUGINS::plugin_name)) { \
                constexpr int N = funcs_for(PLUGINS::plugin_name); \
                func(plugin_name##_<T>::f_(), \
                     std::integral_constant<int, N>{}); \
                return; \
            } \
        }
        PLUGINS_ALL
        #undef X
    }
    
    // Перегрузка для случая, когда строки НЕТ в map (обычная строка)
    template<typename T, typename Func>
    void collectFromString(Func func, const std::string& str) {
        // Обрабатываем как обычную строку
        MathExpression<T> expr(str);
        // Из строки получается ровно одна функция — число вспомогательных = 1
        func(expr.f(), std::integral_constant<int, 1>{});
    }
};

/* 
Отвечает за сбор легких функций и вызов метода наследумоего класса
@tparam T Тип класса который имеет метод exect
*/
template<typename T>
class AssemblerSimple: public CollectPlug{
private:
    T container_class;
    containerVectorStr container_func;
    size_t count;   // runtime-число имён в container_func
    /**
     * @brief рекурсивно собирает все функции по имени из container_func
     * @details Рекурсивно собитрает функции сохраняя их в funcs и проверяет собраны ли все
     * когда все собраны вызывает метод exect класса container_class со всеми параметрами
     * @tparam TypeArg Тип для шаблона функций
     * @tparam count_ Compile-time счётчик оставшихся шагов рекурсии.
     *                Берётся из integral_constant<int, N>, где N = funcs_for(PLUGINS::...).
     *                Должен быть >= 1. Уменьшается на каждом шаге.
     * @param process Главный метод с которым будут вызваны вспомогательные функции
     * @param idx Runtime-индекс текущего имени в container_func (уменьшается с каждым шагом)
     * @param funcs Все собранные функции во время работы
     */
    template<typename TypeArg, size_t count_, typename PluginProcces, typename... Funcs_Assembling>
    void compiled_simple_impl(PluginProcces&& process, 
                              size_t idx,
                              Funcs_Assembling&&... funcs) {
        // idx — runtime-индекс, count_ — compile-time счётчик оставшихся шагов.
        // На входе idx = min(count, count_) - 1, то есть idx == count_ - 1
        // (если count >= count_; иначе idx < count_ - 1, и часть шагов
        //  "съедается" первым же сравнением idx > 0 == false).

        if constexpr(count_ > 1){
            // Ещё есть что собирать, кроме последней функции.
            // Собираем функцию с индексом idx, рекурсивно идём к idx-1 с count_-1.
            // Защита: если idx == 0, но count_ > 1 (пользователь передал меньше имён,
            // чем функций у метода) — вызываем exect с тем, что есть.
            if (idx == 0) {
                // Собираем единственную оставшуюся функцию и вызываем exect
        std::cout<<"\nPIZDA";

                auto wrapper = [this, process, &funcs...](auto&& wrapped_args, auto&& /*num_funcs*/){
                    this->container_class.exect(process,
                        std::forward<decltype(wrapped_args)>(wrapped_args),
                        std::forward<Funcs_Assembling>(funcs)...
                    );
                };
                collect_impl<TypeArg>(wrapper, container_func[0]);
                return;
            }
        std::cout<<"\nPIZDA332";

            /**
             * @brief лямбда вызова compiled_simple_impl
             * @param wrapped_args собранный указатель на функцию
             * @param num_funcs    compile-time число (integral_constant), пробрасываем и игнорируем —
             *                     оно уже учтено в шаблонном count_ на верхнем уровне
             */
            auto wrapper = [this, process, idx, &funcs...](auto&& wrapped_args, auto&& /*num_funcs*/) {
                // Рекурсивный вызов со следующим индексом
                this->template compiled_simple_impl<TypeArg, count_ - 1>(
                    process,
                    idx - 1,
                    std::forward<decltype(wrapped_args)>(wrapped_args),
                    std::forward<Funcs_Assembling>(funcs)...
                );
            };
            collect_impl<TypeArg>(wrapper, container_func[idx]);
            return;
        }
        else if constexpr(count_ == 1){
            // Осталась последняя функция — собираем её и вызываем exect.
            // idx на этом уровне должен быть равен 0.
            /**
             * @brief лямбда вызова container_class.exect
             * @param wrapped_args собранный указатель на функцию
             * @param num_funcs    compile-time число (integral_constant), пробрасываем и игнорируем
             */
            auto wrapper = [this, process, &funcs...](auto&& wrapped_args, auto&& /*num_funcs*/){
                // Финальный вызов
        std::cout<<"\nPIZDA333";

                this->container_class.exect(process,
                    std::forward<decltype(wrapped_args)>(wrapped_args),
                    std::forward<Funcs_Assembling>(funcs)...
                );
            };
            collect_impl<TypeArg>(wrapper, container_func[idx]);
        }
        else {
            // count_ == 0 — сюда попадать не должны: начальный count_ >= 1,
            // потому что compiled_simple проверяет N > 0 перед вызовом.
            std::cerr << "compiled_simple_impl: unexpected count_ == 0" << std::endl;
        }
    }   
protected:
    /**
     * @brief Вызывает compiled_simple_impl с необходимыми аргументами
     * @tparam TypeArg Шаблонн для послеющего сбора функции по нему
     * @param process Собранный раннее метод
     * @param N       compile-time число вспомогательных функций (из funcs_for).
     *                Используется как стартовое значение шаблонного count_.
     */
    template<typename TypeArg, typename PluginProcces, int N>
    void compiled_simple(PluginProcces&& process, std::integral_constant<int, N>) {
        // N — compile-time, поэтому можно инстанцировать шаблон compiled_simple_impl<TypeArg, N>.
        // Стартовый idx = min(count, N) - 1, где count — runtime-число имён в container_func.
        // Если N == 0 — у метода нет std::function-аргументов, вызываем exect сразу.
        std::cout<<"\n"<<N<<":"<<count;
        if constexpr (N > 0) {
            if (count > 0) {
                compiled_simple_impl<
                    TypeArg,
                    static_cast<size_t>(N)
                >(
                    std::forward<PluginProcces>(process),
                    std::min<size_t>(count, static_cast<size_t>(N)) - 1
                );
            } else {
                // Вспомогательных функций нет — вызываем exect с одним методом.
                container_class.exect(std::forward<PluginProcces>(process));
            }
        } else {
            // N == 0: метод не принимает std::function-аргументов.
            container_class.exect(std::forward<PluginProcces>(process));
        }
    }

public:
    /**
     * @brief Construct a new Assembler Simple object
     * 
     * @param t Обьект класса который хранить args для вызова метода
     * @param container  Класс вектор который хранит имена вспомогательных функций
     */
    AssemblerSimple(T&& t, containerVectorStr& container):
    container_class(t), container_func(container){
        count=container_func.getsize();
    };
};


// Сбор сложных функции
class AssemblerComplex: public CollectPlug{
protected:
    /* 
        @brief Вызывает next с собранным методом для дальнейшего сбора
        @param TypeArg Шаблонный параметр указывает шаблонный тип метода
        @param next Следующая в очереди функци
        @param name_method Имя метода
    */
    template<typename TypeArg,typename Next>
    void compiled_complex(Next next, std::string name){
        // wrapper принимает два аргумента: указатель на функцию и integral_constant<int,N>,
        // и пробрасывает оба в next — чтобы compile-time число дошло до compiled_simple
        auto wrapper = [&next](auto&& process, auto&& num_funcs) {
            next(std::forward<decltype(process)>(process),
                 std::forward<decltype(num_funcs)>(num_funcs));
        };
        collect_impl<TypeArg>(wrapper, name);
    }
};

/*
Класс StartPlug
ключевой метод start_plug
отвечает за сбор всех необходимых плагинов и последующий вызов
метода переданого класса
*/
template<typename T>
class StartPlug: public AssemblerSimple<T>, public AssemblerComplex{
public:
    // Наследование конструктора от AssemblerSimple
    using AssemblerSimple<T>::AssemblerSimple;
    
    /* 
        @brief Вызывает compiled_simple с собранным методом для дальнейшего сбора
        @tparam TypeArg Шаблонный параметр указывает шаблонный тип метода
        @param name_method Имя метода
    */
   template<typename TypeArg>
   void start_plug(std::string name_method){
        //Создание лямбды для передачи в compiled_complex
        // wrapper принимает два аргумента: указатель на метод и integral_constant<int,N>,
        // и пробрасывает оба в compiled_simple<TypeArg>
        auto wrapper = [this](auto&& method, auto&& num_funcs) {
            this->template compiled_simple<TypeArg>(
                std::forward<decltype(method)>(method),
                std::forward<decltype(num_funcs)>(num_funcs)
            );
        };
        this->compiled_complex<TypeArg>(wrapper,name_method);
    }
};