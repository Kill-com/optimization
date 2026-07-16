
#ifndef DECORATOR_HPP
#define DECORATOR_HPP

#include <chrono>
#include "../exect/execution.hpp"

class CycleCounter {
public:
    static uint64_t rdtsc();
};

//Шаблон декоратора
template<typename Derived>
class DecoratorChain {
public:
    template<typename... Args>
    auto operator()(Args&&... args) {
        return static_cast<Derived*>(this)->call(std::forward<Args>(args)...);
    }
};

//шаблон для анализа, винда
template<typename Next>
class ANALIS : public DecoratorChain<ANALIS<Next>>{
    private:
    Next next;
    public:
    explicit ANALIS(Next m):next(m){};
    template<typename... Args>
    void call(Args&&... args);
};


//шаблон для линукса
template<typename Next>
class ANALIS_F_LINUX : public DecoratorChain<ANALIS_F_LINUX<Next>>{
    private:
    Next next;
    public:
    ANALIS_F_LINUX(Next m):next(m){};
    template<typename... Args>
    void call(Args&&... args);
};


//класс вызова анализируемой функции
class EXECTED : protected METHOD_L{
    private:
    METHOD_L* ex;
    public:
    EXECTED(METHOD_L* m): ex(m){};
    void exect();
};

template<typename Next>
template<typename... Args>
void ANALIS<Next>::call(Args&&... args) {
    std::cout<<"start of analisis"<<std::endl;

    //Получаем начальный такт процессора
    auto start = CycleCounter::rdtsc();

    //Запуск основной работы
    next(std::forward<Args>(args)...);
    
    
    //Получаем конечный такт процессора
    auto end = CycleCounter::rdtsc();
    auto cycles = end - start;
    std::cout<<"end of analisis"<<std::endl;



    std::cout<<"save results in created file"<<std::endl;

    // return result;
}


template<typename Next>
template<typename ... Args>
void ANALIS_F_LINUX<Next>::call(Args&& ... args){
    //анализ процессора с помощью линукса
    next(std::forward<Args>(args)...);
    //конец анализа с помощью линукса
}

#endif // DECORATOR_HPP