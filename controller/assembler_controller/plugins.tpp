// ============================================================
//  AUTOMATICALLY GENERATED FILE
//  DO NOT EDIT MANUALLY
// ============================================================

#include <string>
#include "../to_upper.hpp"
#include "plugins_controller.hpp"
#include "plugins/plugins_method.hpp"
#include "plugins/plugins_function.hpp"
#include "plugins/enum.hpp"
template<typename T,typename Func>
void CollectPlug::collect(Func func, const std::string name) {
    switch(PluginsMap[toUpper(name)]){
    case 2: func(F_LIST_<T>::f_()); break;
    case 1: func(PORABOLA_<T>::f_()); break;
    case 0: func(GOLD_SECH_<T>::f_()); break;
    }}
