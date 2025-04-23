#pragma once
namespace hive
{

    /**
     * @brief Manage all the shared library (AKA modules) of the engine
     * This class is not thread safe and should only be used inside the main thread
     */
    class HIVE_API LibraryManager
    {
    public:
        using LibraryID = int;
        bool Init();
        void Shutdown();

        LibraryID LoadLibrary(const char* library_name);
        void UnloadLibrary(LibraryID id);

    private:
        std::unordered_map<LibraryID, void*> p_libraries;
        int id_counter = 0;
    };
}