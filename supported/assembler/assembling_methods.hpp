#include <iostream>
#include <functional>
#include <memory>
#include <fstream>
#include <string>

// ====== Кроссплатформенная загрузка ======
#ifdef _WIN32
    #include <windows.h>
    #define DL_HANDLE HMODULE
    #define DL_LOAD(name) LoadLibraryA(name)
    #define DL_SYM(handle, name) GetProcAddress(handle, name)
    #define DL_CLOSE(handle) FreeLibrary(handle)
    #define DL_ERROR() GetLastError()
    #define PLUGIN_EXT ".dll"
#else
    #include <dlfcn.h>
    #define DL_HANDLE void*
    #define DL_LOAD(name) dlopen(name, RTLD_LAZY)
    #define DL_SYM(handle, name) dlsym(handle, name)
    #define DL_CLOSE(handle) dlclose(handle)
    #define DL_ERROR() dlerror()
    #define PLUGIN_EXT ".so"
#endif

// ====== Умный указатель ======
struct LibraryDeleter {
    void operator()(DL_HANDLE handle) const {
        if (handle) DL_CLOSE(handle);
    }
};
using LibraryPtr = std::unique_ptr<std::remove_pointer<DL_HANDLE>::type, LibraryDeleter>;

// ====== PluginLoader ======
class PluginLoader {
public:
    static LibraryPtr loadLibrary(const std::string& name) {
        std::string lib_name = name;
        
        // Добавляем расширение если его нет
        if (lib_name.find(PLUGIN_EXT) == std::string::npos) {
            lib_name += PLUGIN_EXT;
        }
        
        // Проверяем существование файла
        std::ifstream file(lib_name);
        if (!file.good()) {
            std::cerr << "File not found: " << lib_name << std::endl;
            return nullptr;
        }
        file.close();
        
        // Загружаем библиотеку
        DL_HANDLE handle = DL_LOAD(lib_name.c_str());
        if (!handle) {
            std::cerr << "Failed to load library: " << lib_name 
                      << " Error: " << DL_ERROR() << std::endl;
            return nullptr;
        }
        
        std::cout << "Loaded: " << lib_name << std::endl;
        return LibraryPtr(handle);
    }

    template<typename FuncType>
    static FuncType* getFunction(DL_HANDLE handle, const std::string& name) {
        if (!handle) {
            std::cerr << "Invalid handle" << std::endl;
            return nullptr;
        }
        
        auto* func = reinterpret_cast<FuncType*>(DL_SYM(handle, name.c_str()));
        if (!func) {
            std::cerr << "Function not found: " << name << std::endl;
        }
        return func;
    }
};