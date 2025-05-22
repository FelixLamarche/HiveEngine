#pragma once

namespace hive
{

    struct ProfileLocationData
    {
        const char* name;
        const char* function;
        const char* file;
        uint32 line;
        uint32 color;
    };

    struct ProfileCtx
    {
        uint32 id;
        bool active;
    };



    struct ProfilerAPI
    {
        void (*PFN_profile_init)();

        void (*PFN_profile_shutdown)();

        ProfileCtx (*PFN_profile_zone_begin)(const ProfileLocationData *data);

        void (*PFN_profile_zone_end)(ProfileCtx ctx);

        void (*PFN_profile_memory_alloc)(const void *ptr, const char *pool_name, uint32 size);

        void (*PFN_profile_memory_free)(const void *ptr, const char *pool_name);
    };

    HIVE_API void profile_set_api(const ProfilerAPI *api);
    HIVE_API void profile_init();
    HIVE_API void profile_shutdown();

    HIVE_API ProfileCtx profile_zone_begin(const ProfileLocationData* data);
    HIVE_API void profile_zone_end(const ProfileCtx &ctx);

    HIVE_API void profile_memory_alloc(const void* ptr, const char* pool_name, uint32 size);
    HIVE_API void profile_memory_free(const void* ptr, const char* pool_name);

    class HIVE_API ScopedProfileCtx
    {
        ProfileCtx ctx;
    public:
        ScopedProfileCtx(const ProfileLocationData *data)
        {
            ctx = profile_zone_begin(data);
        }

        ~ScopedProfileCtx()
        {
            profile_zone_end(ctx);
        }
    };

}

#ifdef HIVE_PROFILER_ENABLED

#define CONCAT(a, b) a##b
#define ProfileCZone(ctx) static const hive::ProfileLocationData CONCAT(__profile_source_loc, __LINE__)= {NULL, __func__, __FILE__, __LINE__, 0}; hive::ProfileCtx ctx = hive::profile_zone_begin(&CONCAT(__profile_source_loc, __LINE__));
#define ProfileCZoneName(ctx, name) static const hive::ProfileLocationData CONCAT(__profile_source_loc, __LINE__)= {name, __func__, __FILE__, __LINE__, 0}; hive::ProfileCtx ctx = hive::profile_zone_begin(&CONCAT(__profile_source_loc, __LINE__));
#define ProfileCZoneEnd(ctx) hive::profile_zone_end(ctx);
#define ProfileScoped() static const hive::ProfileLocationData CONCAT(__profile_source_loc, __LINE__)= {NULL, __func__, __FILE__, __LINE__, 0}; hive::ScopedProfileCtx ctx(&CONCAT(__profile_source_loc, __LINE__));

#else

#define ProfileCZone(ctx)
#define ProfileCZoneName(ctx, name)
#define ProfileCZoneEnd(ctx)
#define ProfileScoped()
#endif
