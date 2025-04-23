#pragma once

#define DEFINE_FREE_FN(ReturnType, Name, ParamList, ...) HIVE_API ReturnType Name ParamList;

#define DEFINE_FN_PTR(ReturnType, Name, ParamList, ...) ReturnType (*PFN_##Name) ParamList;

#define DEFINE_EXTERN_FN_PTR(ReturnType, Name, ParamList, ...) extern ReturnType (*PFN_##Name) ParamList;

#define IMPL_EXTERN_FN_PTR(ReturnType, Name, ParamList, ...) ReturnType (*PFN_##Name) ParamList = nullptr;

#define IMPL_FREE_FN(ReturnType, Name, ParamList, ...) \
    ReturnType Name ParamList { \
        HIVE_ASSERT(PFN_##Name != nullptr, "The function is not setup"); \
        return PFN_##Name(__VA_ARGS__); \
    }


//TODO find a more suitable location for this
namespace hive::gfx
{
    enum MemoryPropertyFlagBits
    {
        DEVICE_LOCAL_BIT = 0x00000001,
        HOST_VISIBLE_BIT = 0x00000002,
        HOST_COHERENT_BIT = 0x00000004,
    };
}
