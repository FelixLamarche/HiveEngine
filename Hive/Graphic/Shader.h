#pragma once

#pragma once
#include "API_utils.h"

namespace hive::gfx
{
    struct GraphicDevice;
}

namespace hive::gfx
{

    enum class ShaderType
    {
        VERTEX, FRAGMENT, GEOMETRY
    };

    enum class DescriptorType
    {
        UBO, SAMPLER
    };

    //Opaque handle for a swapchain
    struct ShaderHandle;

    struct ShaderDesc
    {
        const char* path;
        ShaderType type;
    };

    struct ShaderProgramHandle;

    struct DescriptorBinding
    {
        DescriptorType type;
        uint32 binding;
        uint32 count;
        ShaderType shader_type;
    };

    struct RenderPassHandle; //Foward declare
    struct ShaderProgramDesc
    {
        std::vector<DescriptorBinding> bindings;
        std::vector<ShaderHandle*> shader_handles;
        RenderPassHandle *render_pass;
    };

#define SHADER_API(GFX) \
    GFX(hive::gfx::ShaderHandle*, shader_create, (const hive::gfx::GraphicDevice *device, const hive::gfx::ShaderDesc &desc), device, desc) \
    GFX(void, shader_destroy, (const hive::gfx::GraphicDevice *device, hive::gfx::ShaderHandle *shader), device, shader) \
    GFX(hive::gfx::ShaderProgramHandle*, shader_program_create, (const hive::gfx::GraphicDevice *device, const hive::gfx::ShaderProgramDesc &desc), device, desc) \
    GFX(void, shader_program_destroy, (const hive::gfx::GraphicDevice *device, hive::gfx::ShaderProgramHandle *shader_program), device, shader_program) \


    struct ShaderAPI
    {
        SHADER_API(DEFINE_FN_PTR)
    };

    SHADER_API(DEFINE_FREE_FN)
    SHADER_API(DEFINE_EXTERN_FN_PTR)
}


