// collect_plugins.cpp
#include <fstream>      // это Ксюша
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

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

// Удаление расширения у имени файла
std::string remove_extension(const std::string& str) {
    size_t pos = str.find_last_of('.');
    if (pos != std::string::npos)
        return str.substr(0, pos);
    return str;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <plugin1.cpp> [plugin2.cpp ...]\n";
        return 1;
    }

    // Собираем уникальные имена файлов
    std::vector<std::string> plugins;
    for (int i = 1; i < argc; ++i) {
        fs::path p(argv[i]);
        if (!fs::exists(p)) {
            std::cerr << "Warning: file " << p << " does not exist, skipping.\n";
            continue;
        }
        // Проверяем расширение .cpp
        if (p.extension() != ".cpp") {
            std::cerr << "Warning: " << p << " is not a .cpp file, skipping.\n";
            continue;
        }
        plugins.push_back(p.string());
    }

    if (plugins.empty()) {
        std::cerr << "No valid plugin files provided.\n";
        return 1;
    }

    // Генерируем plugins.cpp
    const std::string output_path = "plugins.cpp";
    std::ofstream file(output_path, std::ios::trunc);
    if (!file) {
        std::cerr << "Failed to create " << output_path << "\n";
        return 1;
    }

    file << "// ============================================================\n"
         << "//  AUTOMATICALLY GENERATED FILE\n"
         << "//  DO NOT EDIT MANUALLY\n"
         << "// ============================================================\n\n"
         << "#include <string>\n";

    // Включаем все переданные файлы
    for (const auto& p : plugins) {
        file << "#include \"" << p << "\"\n";
    }

    file << "\nnamespace CollectPlugin {\n"
         << "template<typename Func, typename... Args>\n"
         << "void collect(Func func, const std::string& plugin_name, Args&&... args) {\n";

    // Генерируем цепочку if-else
    for (size_t i = 0; i < plugins.size(); ++i) {
        fs::path p(plugins[i]);
        std::string name = p.stem().string();   // имя без пути и расширения
        if (i == 0) {
            file << "    if (plugin_name == \"" << name << "\") "
                 << "func(" << name << "(), std::forward<Args>(args)...);\n";
        } else {
            file << "    else if (plugin_name == \"" << name << "\") "
                 << "func(" << name << "(), std::forward<Args>(args)...);\n";
        }
    }
    file << "    }\n"
         << "} // namespace CollectPlugin\n";

    file.close();
    std::cout << "Generated " << output_path << " with " << plugins.size() << " plugin(s).\n";
    return 0;
}
