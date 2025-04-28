#include "Core/Platform/Platform.h"

#include "LibraryManager.h"

bool hive::LibraryManager::Init()
{
    return true;
}

void hive::LibraryManager::Shutdown()
{
    if (!p_libraries.empty())
    {
        for (const auto &[id, library] : p_libraries)
        {
            auto shutdown_fn = (void (*)())platform_get_symbol(library, "ShutdownModule");
            if (shutdown_fn != nullptr)
                shutdown_fn();
            platform_unload_library(library);
        }
    }
}

hive::LibraryManager::LibraryID hive::LibraryManager::LoadLibrary(const char* library_name)
{
    void* library = platform_load_library(library_name);
    if (library == nullptr)
        return -1;


    typedef void (*PFN_INITMODULE)();
    if (const auto pfn_init_module = reinterpret_cast<PFN_INITMODULE>(platform_get_symbol(library, "InitModule")))
        pfn_init_module();


    p_libraries.insert({id_counter, library});
    return id_counter++;

}

void* hive::LibraryManager::LoadLibraryUnmanaged(const char* library_name)
{
    void* library = platform_load_library(library_name);
    if (library == nullptr)
        return nullptr;


    typedef void (*PFN_INITMODULE)();
    if (const auto pfn_init_module = reinterpret_cast<PFN_INITMODULE>(platform_get_symbol(library, "InitModule")))
        pfn_init_module();


    return library;
}

void hive::LibraryManager::UnloadLibraryUnmanaged(void* library)
{
    auto shutdown_fn = (void (*)())platform_get_symbol(library, "ShutdownModule");
    if (shutdown_fn != nullptr)
        shutdown_fn();
    platform_unload_library(library);
}

void hive::LibraryManager::UnloadLibrary(const LibraryID id)
{
    if (p_libraries.contains(id))
    {
        p_libraries.erase(id);
    }
}
