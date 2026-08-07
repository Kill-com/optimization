#include <typeinfo>
#include <cxxabi.h>  // для demangling
#include <cstdlib>
#include <string>

//Для получения типа данных

template <typename T>
std::string type_name() {
    int status;
    char* demangled = abi::__cxa_demangle(typeid(T).name(), nullptr, nullptr, &status);
    std::string result = (status == 0) ? demangled : typeid(T).name();
    free(demangled);
    return result;
}