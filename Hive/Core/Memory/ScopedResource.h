#pragma once

namespace hive
{
    /**
     * @brief Follow the RAII principle for stuff that are using C like interface
     *
     * Useful for abstract interface wrapper that need allocation, initialisation, de-allocation and shutdown...
     *
     * @tparam T the Type of the reference
     */
    template <typename T>
    class HIVE_API ScopedResource
    {
    public:
        using PFN_DestroyResource = std::function<void(T*)>;
        explicit ScopedResource(T* resource, PFN_DestroyResource fn) : p_res(resource), p_fn_destroy_resource(fn)
        {
            p_res->Init();
        }
        ~ScopedResource()
        {
            p_res->Shutdown();
            p_fn_destroy_resource(p_res);
        }

        T* operator->() { return p_res; }
        T* Get() { return p_res; }
    private:
        T* p_res = nullptr;
        PFN_DestroyResource p_fn_destroy_resource;
    };
}