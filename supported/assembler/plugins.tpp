// ============================================================
//  AUTOMATICALLY GENERATED FILE
//  DO NOT EDIT MANUALLY
// ============================================================

#include <string>
#include "plugins_get.hpp"
#include "../../plugins/f_list.cpp"
#include "../../plugins/gold_sech.cpp"
#include "../../plugins/porabola.cpp"


template<typename Func, typename... Args>
void CollectPlug::collect(Func func, const std::string& plugin_name, Args&&... args){
if(plugin_name=="f_list") func(f_list(),std::forward<Args>(args)...);
else if(plugin_name=="gold_sech") func(gold_sech(),std::forward<Args>(args)...);
else if(plugin_name=="porabola") func(porabola(),std::forward<Args>(args)...);
}