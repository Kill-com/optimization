#include "c_analisis.h"


//функция анализа 
void ANALIS_EXECT::exect(std::string_view method, std::string_view function){
    std::cout<<"start of analisis"<<std::endl;

    //Получаем начальный такт процессора
    auto start = CycleCounter::rdtsc();

    //Запуск основной работы
    ex->exect(method, function);
    
    
    //Получаем конечный такт процессора
    auto end = CycleCounter::rdtsc();
    auto cycles = end - start;
    std::cout<<"end of analisis"<<std::endl;



    std::cout<<"save results in created file"<<std::endl;
}