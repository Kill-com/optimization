#pragma once
#include <iostream>
#include <string>

#include "../chek_args_f.hpp"
#include "../../container/container.hpp"

/*
CollectPlug::collect
принимает лямбду функцию и вызывает ее с указаетлем на функцию
за который отвечает переданный аргумент.
*/
class CollectPlug{
protected:
    /**
     * @brief Вызывает переданую лямбду с укаазетлем на функцию по ее имени
     * 
     * @tparam T Тип для шаблона функций
     * @tparam Func Лямбда функция для посдующегор вызова
     */
    template<typename T,typename Func>
    void collect(Func, const std::string);

    // template<typename T,typename Func, typename Func_str>
    // void collect(Func func, Func_str plugin){
    //     func(plugin);
    // }
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
    size_t count;
    /**
     * @brief рекурсивно собирает все функции по имени из container_func
     * @details Рекурсивно собитрает функции сохраняя их в funcs и проверяет собраны ли все
     * когда все собраны вызывает метод exect класса container_class со всеми параметрами
     * @tparam TypeArg Тип для шаблона функций
     * @tparam count_ Номер рекурсии compilet-time
     * @param process Главный метод с которым будут вызваны вспомогательные функции
     * @param idx Номер рекурсии
     * @param funcs Все собранные функции во время работы
     */
    template<typename TypeArg, size_t count_,typename PluginProcces, typename... Funcs_Assembling>
    void compiled_simple_impl(PluginProcces&& process, 
                              size_t idx,
                              Funcs_Assembling&&... funcs) {
        if (idx >= count) return;
        if constexpr(count_>0){
            if(idx>0){
                /**
                 * @brief лямбда вызова compiled_simple_impl
                 * @param wrapped_args собранный указаетль на функцию
                 */
                auto wrapper = [this, process, idx, &funcs...](auto&& wrapped_args) {
                    // Рекурсивный вызов со следующим индексом
                    this->compiled_simple_impl<TypeArg,count_-1>(
                        process,
                        idx - 1,
                        std::forward<decltype(wrapped_args)>(wrapped_args),
                        std::forward<Funcs_Assembling>(funcs)...
                    );
                };
                collect<TypeArg>(wrapper, container_func[idx]);
                return; //Выход из рекурсии
            }
            if(idx==0){
                /**
                 * @brief лямбда вызова container_class.exect
                 * @param wrapped_args собранный указаетль на функцию
                 */
                auto wrapper = [this, process, idx, &funcs...](auto&& wrapped_args){
                    // Финальный вызов
                    this->container_class.exect(process,
                        std::forward<decltype(wrapped_args)>(wrapped_args),
                        std::forward<Funcs_Assembling>(funcs)...
                    );
                };
                collect<TypeArg>(wrapper, container_func[idx]);
            }
        }
    }   
protected:
    /**
     * @brief Вызывает compiled_simple_impl с необходимыми аргументами
     * @tparam TypeArg Шаблонн для послеющего сбора функции по нему
     * @param process Собранный раннее метод
     */
    template<typename TypeArg, typename PluginProcces>
    void compiled_simple(PluginProcces&& process) {
        if (count > 0) {
            // Начинаем с последнего индекса (count - 1)
            compiled_simple_impl<
            TypeArg,
            //Получение количества std::function которые принимает метод
            function_counter<std::decay_t<PluginProcces>>::value
            >(
                std::forward<PluginProcces>(process),
                count - 1
            );
        }
    }

public:
    /**
     * @brief Construct a new Assembler Simple object
     * 
     * @param t Обьект класса который хранить args для вызова метода
     * @param container  Класс вектор который хранит имена вспомогательных функций
     */
    AssemblerSimple(const T& t,const containerVectorStr& container):
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
        auto wrapper = [&next](auto&& process) {
            next(std::forward<decltype(process)>(process));
        };
        collect<TypeArg>(wrapper, name);
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
        auto wrapper = [this](auto&& method) {
            this->template compiled_simple<TypeArg>(
                std::forward<decltype(method)>(method)
            );
        };
        this->compiled_complex<TypeArg>(wrapper,name_method);
    }
};

//include автоматически сгенерированого файла
#include "plugins.tpp"

