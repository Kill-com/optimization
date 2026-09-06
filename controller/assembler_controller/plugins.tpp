// ============================================================
//  AUTOMATICALLY GENERATED FILE
//  DO NOT EDIT MANUALLY
// ============================================================

#include <string>
#include "plugins_controller.hpp"
#include "plugins/f_list.cpp"
#include "plugins/porabola.cpp"
#include "plugins/gold_sech.cpp"
template<typename T,typename Func>
void CollectPlug::collect(Func func, const std::string plugin_name) {
    if (plugin_name == "f_list.cpp") func(f_list<T>());
    else if (plugin_name == "porabola.cpp") func(porabola<T>());
    else if (plugin_name == "gold_sech.cpp") func(gold_sech<T>());
    }
