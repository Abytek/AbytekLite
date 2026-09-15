#pragma once

#include "Abytek/Engine.RHI.prerequisites.hpp"
#include "Abytek/RHIFormat.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    namespace Internal
    {
        void DirectX12RHISetupFormatTables();
    }
    
    ABYTEK_ENGINE_RHI_API DXGI_FORMAT RHIFormatToD3DFormat(E_RHIFormat RHIFormat);
    ABYTEK_ENGINE_RHI_API E_RHIFormat D3DFormatToRHIFormat(DXGI_FORMAT DXGIFormat);
}
#endif