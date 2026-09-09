#pragma once

// #define __PARSEMATH__

#define REGISTER_FUNCTION \
static auto f_() { \
    return &f; \
}
#define REGISTER_FUNCTION \
static auto f_() { \
    return &f; \
}

#ifdef __PARSEMATH__
    #define FUNCTION(name, ...) \
        result<T>(name,__VA_ARGS__)
    #define TYPE(output, input) \
        std::function<output\
        (const std::unordered_map<std::string, std::vector<input>>&)>
#else
    #define FUNCTION(name, ...) \
        name(__VA_ARGS__)
    #define TYPE(output, input) \
        std::function<output(input)>
#endif


// Вместо макросов - используем вариативные шаблоны
template<typename T, typename... Args>
auto make_map(Args... args) {
    std::unordered_map<std::string, T> map;
    int i = 1;
    ((map["x" + std::to_string(i++)] = args), ...);
    return map;
}

template<typename T, typename Name, typename A>
T result(Name&& name, A&& a){
    return name.evaluate(make_map<T>(a))[0];
}
template<typename T, typename Name, typename A, typename ...Args>
std::vector<T> result(Name&& name,A&& a, Args&& ...args){
    return name.evaluate(make_map<T>(std::forward<A>(a), std::forward<Args>(args)...));
}



