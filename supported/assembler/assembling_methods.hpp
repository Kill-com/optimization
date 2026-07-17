#include <iostream>
#include <functional>
#include <memory>

// ====== Кроссплатформенная загрузка ======
#ifdef _WIN32
    #include <windows.h>
    #define DL_HANDLE HMODULE
    #define DL_LOAD(name) LoadLibraryA(name)
    #define DL_SYM(handle, name) GetProcAddress(handle, name)
    #define DL_CLOSE(handle) FreeLibrary(handle)
    #define DL_ERROR() GetLastError()
#else
    #include <dlfcn.h>
    #define DL_HANDLE void*
    #define DL_LOAD(name) dlopen(name, RTLD_LAZY)
    #define DL_SYM(handle, name) dlsym(handle, name)
    #define DL_CLOSE(handle) dlclose(handle)
    #define DL_ERROR() dlerror()
#endif

// ====== Умный указатель для автоматического закрытия ======
struct LibraryDeleter {
    void operator()(DL_HANDLE handle) const {
        if (handle) DL_CLOSE(handle);
    }
};
using LibraryPtr = std::unique_ptr<std::remove_pointer<DL_HANDLE>::type, LibraryDeleter>;

// ====== Основной класс для работы с плагинами ======
class PluginLoader {
public:
    // Загружает библиотеку по имени (хранится в string_view)
    static LibraryPtr loadLibrary(std::string name) {
        
        DL_HANDLE handle = DL_LOAD(name.c_str());
        if (!handle) {
            std::cerr << "Failed to load library: " << name 
                      << " Error: " << DL_ERROR() << std::endl;
            return nullptr;
        }
        return LibraryPtr(handle);
    }

    // Получает функцию из загруженной библиотеки
    template<typename FuncType>
    static FuncType* getFunction(DL_HANDLE handle, std::string name) {
        auto* func = reinterpret_cast<FuncType*>(DL_SYM(handle, name.c_str()));
        if (!func) {
            std::cerr << "Function not found: " << name << std::endl;
        }
        return func;
    }
};