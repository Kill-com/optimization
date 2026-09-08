// collect_plugins.cpp
#include <fstream>      // это Ксюша
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include "../controller/assembler_controller/plugins/enum.hpp"
#include "../controller/to_upper.hpp"

// Подключаем filesystem с запасным вариантом для старых компиляторов
#if __has_include(<filesystem>)
#  include <filesystem>
   namespace fs = std::filesystem;
#elif __has_include(<experimental/filesystem>)
#  include <experimental/filesystem>
   namespace fs = std::experimental::filesystem;
#else
#  error "Compiler does not support <filesystem>"
#endif


int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <plugin1.cpp> [plugin2.cpp ...]\n";
        return 1;
    }
    // Собираем уникальные имена файлов
    std::vector<std::string> plugins;
    for (int i = 1; i < argc; ++i) {
        fs::path p(argv[i]);
        plugins.push_back(p.string());
    }

    if (plugins.empty()) {
        plugins.reserve(PluginsMap.size());
        for (const auto& [name, type] : PluginsMap) {
            plugins.push_back(name);
        }
    }

    // Генерируем plugins.cpp
    const std::string output_path = "../controller/assembler_controller/plugins.tpp";
    std::ofstream file(output_path, std::ios::trunc);
    if (!file) {
        std::cerr << "Failed to create " << output_path << "\n";
        return 1;
    }

    file<< "// ============================================================\n"
        << "//  AUTOMATICALLY GENERATED FILE\n"
        << "//  DO NOT EDIT MANUALLY\n"
        << "// ============================================================\n\n"
        << "#include <string>\n"
        << "#include \"../to_upper.hpp\"\n"
        << "#include \"plugins_controller.hpp\"\n"
        << "#include \"plugins/plugins_method.hpp\"\n"
        << "#include \"plugins/plugins_function.hpp\"\n"
        << "#include \"plugins/enum.hpp\"\n"
        << "template<typename T,typename Func>\n"
        << "void CollectPlug::collect(Func func, const std::string name) {\n"
        << "    switch(PluginsMap[toUpper(name)]){\n";

    // Генерируем цепочку if-else
    for (size_t i = 0; i < plugins.size(); ++i) {
        int name = PluginsMap[toUpper(plugins[i])];
            file << "    case " <<name
                 << ": func(" << toUpper(plugins[i]) << "_<T>::f_()); "
                 <<"break;\n";
    }
    file << "    }}\n";

    file.close();
    std::cout << "Generated " << output_path << " with " << plugins.size() << " plugin(s).\n";
    return 0;
}
