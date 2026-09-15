#pragma once

#include "Abytek/Engine.RHI.prerequisites.hpp"


namespace Abytek
{
    enum class E_RHIAPI
    {
        NONE,
        DIRECTX12,
        DIRECTX11,
        VULKAN,
        OPENGL,
        WEBGPU,
        METAL
    };

    ABYTEK_FORCE_INLINE F_FeedbackStatus RHIParseAPI(const F_Name& Name, E_RHIAPI& OutValue) noexcept
    {
        if (Name == ABYTEK_NAME("DirectX12"))
        {
            OutValue = E_RHIAPI::DIRECTX12;
        }
        else if (Name == ABYTEK_NAME("DirectX11"))
        {
            OutValue = E_RHIAPI::DIRECTX12;
        }
        else if (Name == ABYTEK_NAME("Vulkan"))
        {
            OutValue = E_RHIAPI::VULKAN;
        }
        else if (Name == ABYTEK_NAME("OpenGL"))
        {
            OutValue = E_RHIAPI::OPENGL;
        }
        else if (Name == ABYTEK_NAME("WebGPU"))
        {
            OutValue = E_RHIAPI::WEBGPU;
        }
        else if (Name == ABYTEK_NAME("Metal"))
        {
            OutValue = E_RHIAPI::METAL;
        }
        else
        {
            return F_FeedbackStatus::MakeFailed(ABYTEK_TEXT("Unknown RHI API: ") + *Name);
        }
        return F_FeedbackStatus::MakeSucceeded();
    }
    ABYTEK_FORCE_INLINE F_Name RHIGetAPIName(E_RHIAPI API) noexcept
    {
        switch (API)
        {
        case E_RHIAPI::NONE:
            return {};
        case E_RHIAPI::DIRECTX12:
            return ABYTEK_NAME("DirectX12");
        case E_RHIAPI::DIRECTX11:
            return ABYTEK_NAME("DirectX11");
        case E_RHIAPI::VULKAN:
            return ABYTEK_NAME("Vulkan");
        case E_RHIAPI::OPENGL:
            return ABYTEK_NAME("OpenGL");
        case E_RHIAPI::WEBGPU:
            return ABYTEK_NAME("WebGPU");
        case E_RHIAPI::METAL:
            return ABYTEK_NAME("Metal");
        default:
            ABYTEK_LOG_FATAL() << "Unknown RHI API";
            break;
        }
        return {};
    }
    ABYTEK_FORCE_INLINE TF_Vector<E_RHIAPI> RHIGetDefaultAPIs()
    {
        TF_Vector<E_RHIAPI> Result;
#if defined(ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12)
        Result.push_back(E_RHIAPI::DIRECTX12);
#elif defined(ABYTEK_ENGINE_RHI_ENABLE_DIRECTX11)
        Result.push_back(E_RHIAPI::DIRECTX11);
#elif defined(ABYTEK_ENGINE_RHI_ENABLE_VULKAN)
        Result.push_back(E_RHIAPI::VULKAN);
#elif defined(ABYTEK_ENGINE_RHI_ENABLE_OPENGL)
        Result.push_back(E_RHIAPI::OPENGL);
#elif defined(ABYTEK_ENGINE_RHI_ENABLE_METAL)
        Result.push_back(E_RHIAPI::METAL);
#elif defined(ABYTEK_ENGINE_RHI_ENABLE_WEBGPU)
        Result.push_back(E_RHIAPI::WEBGPU);
#endif
        return boost::move(Result);
    }
    ABYTEK_FORCE_INLINE TF_Vector<F_Name> RHIGetDefaultAPINames() noexcept
    {
        TF_Vector<F_Name> Result;
        for (const auto& API : RHIGetDefaultAPIs())
        {
            Result.push_back(RHIGetAPIName(API));
        }
        return Result;
    }
}