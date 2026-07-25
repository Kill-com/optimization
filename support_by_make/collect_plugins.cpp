#include <fstream>
#include <cstdio>
#include <vector>
#include <algorithm>
auto remove_extension = [](const std::string& str) -> std::string {
        size_t pos = str.find_last_of('.');
        if (pos != std::string::npos) {
            return str.substr(0, pos);
        }
        return str;  // Нет расширения
    };

int main(int argc, char* argv[]){

    const std::string path="../supported/assembler/plugins.cpp";
    const std::string plugin_path="../plugins/";
    std::ofstream file(path,std::ios::trunc);
    file<<"// ============================================================"<<"\n"
    <<"//  AUTOMATICALLY GENERATED FILE"<<"\n"
    <<"//  DO NOT EDIT MANUALLY"<<"\n"
    <<"// ============================================================"<<"\n"<<"\n"
    <<"#include <string>"<<"\n";
    for(int i=1; i<argc;i++){
        file<<"#include \"../"<<plugin_path<<argv[i]<<"\"\n";
    }
    file<<"namespace CollectPlugin{\n"
    <<"template<typename Func, typename... Args>\n"
    <<"void collect(Func func, const std::string& plugin_name, Args&&... args){\n"
    <<"if(plugin_name=="<<remove_extension(argv[1])<<") "
    <<"func("<<remove_extension(argv[1])<<"(),std::forward<Args>(args)...);\n";
    for(int i=2; i<argc;i++){
        file<<"else if(plugin_name=="<<remove_extension(argv[i])<<") "
        <<"func("<<remove_extension(argv[i])<<"(),std::forward<Args>(args)...);\n";
    }
    file<<"}\n}";
    file.close();
}