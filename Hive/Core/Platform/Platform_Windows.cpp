#include "Platform.h"

#ifdef HIVE_PLATFORM_WINDOWS
#include "windows.h"

void *hive::platform_load_library(const char *library_name)
{

    const auto path = "./" + std::string(library_name) + ".dll";
    const auto lib = LoadLibrary(path.c_str());

    if (lib == nullptr) {
        const auto error = GetLastError();

        char messageBuffer[512]; // static buffer to return const char*
        DWORD size = FormatMessageA(
            FORMAT_MESSAGE_FROM_SYSTEM |
            FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL, error,
            MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
            messageBuffer, sizeof(messageBuffer), NULL);

        HIVE_LOG_ERROR("Failed to load library: %s", messageBuffer);
    }

    return lib;
}

void hive::platform_unload_library(void* library)
{
    FreeLibrary(static_cast<HMODULE>(library));
}

void* hive::platform_get_symbol(void* library, const char* symbol)
{
    auto symbol_out = GetProcAddress(static_cast<HMODULE>(library), symbol);

    if (symbol_out == nullptr) {
        const auto error = GetLastError();
        HIVE_LOG_ERROR("Failed to get symbol: %s", symbol_out);
    }

    return (void*)symbol_out;
}
#endif