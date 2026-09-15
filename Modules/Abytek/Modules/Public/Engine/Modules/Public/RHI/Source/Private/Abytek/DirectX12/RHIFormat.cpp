#include "Abytek/DirectX12/RHIFormat.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    namespace Internal
    {
        TF_Optional<DXGI_FORMAT> RHIFormatToD3DFormatTable[static_cast<U32>(E_RHIFormat::NUM)];
        TF_Optional<E_RHIFormat> D3DFormatToRHIFormatTable[static_cast<U32>(190)];

        void DirectX12RHISetupFormatTables()
        {
            RHIFormatToD3DFormatTable[static_cast<U32>(E_RHIFormat::NONE)] = DXGI_FORMAT_UNKNOWN;

            RHIFormatToD3DFormatTable[static_cast<U32>(E_RHIFormat::R32G32B32A32_FLOAT)] = DXGI_FORMAT_R32G32B32A32_FLOAT;
            RHIFormatToD3DFormatTable[static_cast<U32>(E_RHIFormat::R32G32B32_FLOAT)] = DXGI_FORMAT_R32G32B32_FLOAT;
            RHIFormatToD3DFormatTable[static_cast<U32>(E_RHIFormat::R32G32_FLOAT)] = DXGI_FORMAT_R32G32_FLOAT;
            RHIFormatToD3DFormatTable[static_cast<U32>(E_RHIFormat::R32_FLOAT)] = DXGI_FORMAT_R32_FLOAT;
            RHIFormatToD3DFormatTable[static_cast<U32>(E_RHIFormat::R16G16B16A16_FLOAT)] = DXGI_FORMAT_R16G16B16A16_FLOAT;
            RHIFormatToD3DFormatTable[static_cast<U32>(E_RHIFormat::R16G16_FLOAT)] = DXGI_FORMAT_R16G16_FLOAT;
            RHIFormatToD3DFormatTable[static_cast<U32>(E_RHIFormat::R16_FLOAT)] = DXGI_FORMAT_R16_FLOAT;
            RHIFormatToD3DFormatTable[static_cast<U32>(E_RHIFormat::D32_FLOAT)] = DXGI_FORMAT_D32_FLOAT;
            RHIFormatToD3DFormatTable[static_cast<U32>(E_RHIFormat::R16G16B16A16_UNORM)] = DXGI_FORMAT_R16G16B16A16_UNORM;
            RHIFormatToD3DFormatTable[static_cast<U32>(E_RHIFormat::R16G16_UNORM)] = DXGI_FORMAT_R16G16_UNORM;
            RHIFormatToD3DFormatTable[static_cast<U32>(E_RHIFormat::R16_UNORM)] = DXGI_FORMAT_R16_UNORM;
            RHIFormatToD3DFormatTable[static_cast<U32>(E_RHIFormat::R8G8B8A8_UNORM)] = DXGI_FORMAT_R8G8B8A8_UNORM;
            RHIFormatToD3DFormatTable[static_cast<U32>(E_RHIFormat::R8G8_UNORM)] = DXGI_FORMAT_R8G8_UNORM;
            RHIFormatToD3DFormatTable[static_cast<U32>(E_RHIFormat::R8_UNORM)] = DXGI_FORMAT_R8_UNORM;
            RHIFormatToD3DFormatTable[static_cast<U32>(E_RHIFormat::R16G16B16A16_SNORM)] = DXGI_FORMAT_R16G16B16A16_SNORM;
            RHIFormatToD3DFormatTable[static_cast<U32>(E_RHIFormat::R16G16_SNORM)] = DXGI_FORMAT_R16G16_SNORM;
            RHIFormatToD3DFormatTable[static_cast<U32>(E_RHIFormat::R16_SNORM)] = DXGI_FORMAT_R16_SNORM;
            RHIFormatToD3DFormatTable[static_cast<U32>(E_RHIFormat::R8G8B8A8_SNORM)] = DXGI_FORMAT_R8G8B8A8_SNORM;
            RHIFormatToD3DFormatTable[static_cast<U32>(E_RHIFormat::R8G8_SNORM)] = DXGI_FORMAT_R8G8_SNORM;
            RHIFormatToD3DFormatTable[static_cast<U32>(E_RHIFormat::R8_SNORM)] = DXGI_FORMAT_R8_SNORM;
            RHIFormatToD3DFormatTable[static_cast<U32>(E_RHIFormat::R32G32B32A32_UINT)] = DXGI_FORMAT_R32G32B32A32_UINT;
            RHIFormatToD3DFormatTable[static_cast<U32>(E_RHIFormat::R32G32B32_UINT)] = DXGI_FORMAT_R32G32B32_UINT;
            RHIFormatToD3DFormatTable[static_cast<U32>(E_RHIFormat::R32G32_UINT)] = DXGI_FORMAT_R32G32_UINT;
            RHIFormatToD3DFormatTable[static_cast<U32>(E_RHIFormat::R32_UINT)] = DXGI_FORMAT_R32_UINT;
            RHIFormatToD3DFormatTable[static_cast<U32>(E_RHIFormat::R16G16B16A16_UINT)] = DXGI_FORMAT_R16G16B16A16_UINT;
            RHIFormatToD3DFormatTable[static_cast<U32>(E_RHIFormat::R16G16_UINT)] = DXGI_FORMAT_R16G16_UINT;
            RHIFormatToD3DFormatTable[static_cast<U32>(E_RHIFormat::R16_UINT)] = DXGI_FORMAT_R16_UINT;
            RHIFormatToD3DFormatTable[static_cast<U32>(E_RHIFormat::R8G8B8A8_UINT)] = DXGI_FORMAT_R8G8B8A8_UINT;
            RHIFormatToD3DFormatTable[static_cast<U32>(E_RHIFormat::R8G8_UINT)] = DXGI_FORMAT_R8G8_UINT;
            RHIFormatToD3DFormatTable[static_cast<U32>(E_RHIFormat::R8_UINT)] = DXGI_FORMAT_R8_UINT;
            RHIFormatToD3DFormatTable[static_cast<U32>(E_RHIFormat::R32G32B32A32_SINT)] = DXGI_FORMAT_R32G32B32A32_SINT;
            RHIFormatToD3DFormatTable[static_cast<U32>(E_RHIFormat::R32G32B32_SINT)] = DXGI_FORMAT_R32G32B32_SINT;
            RHIFormatToD3DFormatTable[static_cast<U32>(E_RHIFormat::R32G32_SINT)] = DXGI_FORMAT_R32G32_SINT;
            RHIFormatToD3DFormatTable[static_cast<U32>(E_RHIFormat::R32_SINT)] = DXGI_FORMAT_R32_SINT;
            RHIFormatToD3DFormatTable[static_cast<U32>(E_RHIFormat::R16G16B16A16_SINT)] = DXGI_FORMAT_R16G16B16A16_SINT;
            RHIFormatToD3DFormatTable[static_cast<U32>(E_RHIFormat::R16G16_SINT)] = DXGI_FORMAT_R16G16_SINT;
            RHIFormatToD3DFormatTable[static_cast<U32>(E_RHIFormat::R16_SINT)] = DXGI_FORMAT_R16_SINT;
            RHIFormatToD3DFormatTable[static_cast<U32>(E_RHIFormat::R8G8B8A8_SINT)] = DXGI_FORMAT_R8G8B8A8_SINT;
            RHIFormatToD3DFormatTable[static_cast<U32>(E_RHIFormat::R8G8_SINT)] = DXGI_FORMAT_R8G8_SINT;
            RHIFormatToD3DFormatTable[static_cast<U32>(E_RHIFormat::R8_SINT)] = DXGI_FORMAT_R8_SINT;
            RHIFormatToD3DFormatTable[static_cast<U32>(E_RHIFormat::R32G32B32A32_TYPELESS)] = DXGI_FORMAT_R32G32B32A32_TYPELESS;
            RHIFormatToD3DFormatTable[static_cast<U32>(E_RHIFormat::R32G32B32_TYPELESS)] = DXGI_FORMAT_R32G32B32_TYPELESS;
            RHIFormatToD3DFormatTable[static_cast<U32>(E_RHIFormat::R32G32_TYPELESS)] = DXGI_FORMAT_R32G32_TYPELESS;
            RHIFormatToD3DFormatTable[static_cast<U32>(E_RHIFormat::R32_TYPELESS)] = DXGI_FORMAT_R32_TYPELESS;
            RHIFormatToD3DFormatTable[static_cast<U32>(E_RHIFormat::R16G16B16A16_TYPELESS)] = DXGI_FORMAT_R16G16B16A16_TYPELESS;
            RHIFormatToD3DFormatTable[static_cast<U32>(E_RHIFormat::R16G16_TYPELESS)] = DXGI_FORMAT_R16G16_TYPELESS;
            RHIFormatToD3DFormatTable[static_cast<U32>(E_RHIFormat::R16_TYPELESS)] = DXGI_FORMAT_R16_TYPELESS;
            RHIFormatToD3DFormatTable[static_cast<U32>(E_RHIFormat::R8G8B8A8_TYPELESS)] = DXGI_FORMAT_R8G8B8A8_TYPELESS;
            RHIFormatToD3DFormatTable[static_cast<U32>(E_RHIFormat::R8G8_TYPELESS)] = DXGI_FORMAT_R8G8_TYPELESS;
            RHIFormatToD3DFormatTable[static_cast<U32>(E_RHIFormat::R8_TYPELESS)] = DXGI_FORMAT_R8_TYPELESS;
            RHIFormatToD3DFormatTable[static_cast<U32>(E_RHIFormat::NV12)] = DXGI_FORMAT_NV12;


            
            D3DFormatToRHIFormatTable[static_cast<U32>(DXGI_FORMAT_UNKNOWN)] = E_RHIFormat::NONE;

            D3DFormatToRHIFormatTable[static_cast<U32>(DXGI_FORMAT_R32G32B32A32_FLOAT)] = E_RHIFormat::R32G32B32A32_FLOAT;
            D3DFormatToRHIFormatTable[static_cast<U32>(DXGI_FORMAT_R32G32B32_FLOAT)] = E_RHIFormat::R32G32B32_FLOAT;
            D3DFormatToRHIFormatTable[static_cast<U32>(DXGI_FORMAT_R32G32_FLOAT)] = E_RHIFormat::R32G32_FLOAT;
            D3DFormatToRHIFormatTable[static_cast<U32>(DXGI_FORMAT_R32_FLOAT)] = E_RHIFormat::R32_FLOAT;
            D3DFormatToRHIFormatTable[static_cast<U32>(DXGI_FORMAT_R16G16B16A16_FLOAT)] = E_RHIFormat::R16G16B16A16_FLOAT;
            D3DFormatToRHIFormatTable[static_cast<U32>(DXGI_FORMAT_R16G16_FLOAT)] = E_RHIFormat::R16G16_FLOAT;
            D3DFormatToRHIFormatTable[static_cast<U32>(DXGI_FORMAT_R16_FLOAT)] = E_RHIFormat::R16_FLOAT;
            D3DFormatToRHIFormatTable[static_cast<U32>(DXGI_FORMAT_D32_FLOAT)] = E_RHIFormat::D32_FLOAT;
            D3DFormatToRHIFormatTable[static_cast<U32>(DXGI_FORMAT_R16G16B16A16_UNORM)] = E_RHIFormat::R16G16B16A16_UNORM;
            D3DFormatToRHIFormatTable[static_cast<U32>(DXGI_FORMAT_R16G16_UNORM)] = E_RHIFormat::R16G16_UNORM;
            D3DFormatToRHIFormatTable[static_cast<U32>(DXGI_FORMAT_R16_UNORM)] = E_RHIFormat::R16_UNORM;
            D3DFormatToRHIFormatTable[static_cast<U32>(DXGI_FORMAT_R8G8B8A8_UNORM)] = E_RHIFormat::R8G8B8A8_UNORM;
            D3DFormatToRHIFormatTable[static_cast<U32>(DXGI_FORMAT_R8G8_UNORM)] = E_RHIFormat::R8G8_UNORM;
            D3DFormatToRHIFormatTable[static_cast<U32>(DXGI_FORMAT_R8_UNORM)] = E_RHIFormat::R8_UNORM;
            D3DFormatToRHIFormatTable[static_cast<U32>(DXGI_FORMAT_R16G16B16A16_SNORM)] = E_RHIFormat::R16G16B16A16_SNORM;
            D3DFormatToRHIFormatTable[static_cast<U32>(DXGI_FORMAT_R16G16_SNORM)] = E_RHIFormat::R16G16_SNORM;
            D3DFormatToRHIFormatTable[static_cast<U32>(DXGI_FORMAT_R16_SNORM)] = E_RHIFormat::R16_SNORM;
            D3DFormatToRHIFormatTable[static_cast<U32>(DXGI_FORMAT_R8G8B8A8_SNORM)] = E_RHIFormat::R8G8B8A8_SNORM;
            D3DFormatToRHIFormatTable[static_cast<U32>(DXGI_FORMAT_R8G8_SNORM)] = E_RHIFormat::R8G8_SNORM;
            D3DFormatToRHIFormatTable[static_cast<U32>(DXGI_FORMAT_R8_SNORM)] = E_RHIFormat::R8_SNORM;
            D3DFormatToRHIFormatTable[static_cast<U32>(DXGI_FORMAT_R32G32B32A32_UINT)] = E_RHIFormat::R32G32B32A32_UINT;
            D3DFormatToRHIFormatTable[static_cast<U32>(DXGI_FORMAT_R32G32B32_UINT)] = E_RHIFormat::R32G32B32_UINT;
            D3DFormatToRHIFormatTable[static_cast<U32>(DXGI_FORMAT_R32G32_UINT)] = E_RHIFormat::R32G32_UINT;
            D3DFormatToRHIFormatTable[static_cast<U32>(DXGI_FORMAT_R32_UINT)] = E_RHIFormat::R32_UINT;
            D3DFormatToRHIFormatTable[static_cast<U32>(DXGI_FORMAT_R16G16B16A16_UINT)] = E_RHIFormat::R16G16B16A16_UINT;
            D3DFormatToRHIFormatTable[static_cast<U32>(DXGI_FORMAT_R16G16_UINT)] = E_RHIFormat::R16G16_UINT;
            D3DFormatToRHIFormatTable[static_cast<U32>(DXGI_FORMAT_R16_UINT)] = E_RHIFormat::R16_UINT;
            D3DFormatToRHIFormatTable[static_cast<U32>(DXGI_FORMAT_R8G8B8A8_UINT)] = E_RHIFormat::R8G8B8A8_UINT;
            D3DFormatToRHIFormatTable[static_cast<U32>(DXGI_FORMAT_R8G8_UINT)] = E_RHIFormat::R8G8_UINT;
            D3DFormatToRHIFormatTable[static_cast<U32>(DXGI_FORMAT_R8_UINT)] = E_RHIFormat::R8_UINT;
            D3DFormatToRHIFormatTable[static_cast<U32>(DXGI_FORMAT_R32G32B32A32_SINT)] = E_RHIFormat::R32G32B32A32_SINT;
            D3DFormatToRHIFormatTable[static_cast<U32>(DXGI_FORMAT_R32G32B32_SINT)] = E_RHIFormat::R32G32B32_SINT;
            D3DFormatToRHIFormatTable[static_cast<U32>(DXGI_FORMAT_R32G32_SINT)] = E_RHIFormat::R32G32_SINT;
            D3DFormatToRHIFormatTable[static_cast<U32>(DXGI_FORMAT_R32_SINT)] = E_RHIFormat::R32_SINT;
            D3DFormatToRHIFormatTable[static_cast<U32>(DXGI_FORMAT_R16G16B16A16_SINT)] = E_RHIFormat::R16G16B16A16_SINT;
            D3DFormatToRHIFormatTable[static_cast<U32>(DXGI_FORMAT_R16G16_SINT)] = E_RHIFormat::R16G16_SINT;
            D3DFormatToRHIFormatTable[static_cast<U32>(DXGI_FORMAT_R16_SINT)] = E_RHIFormat::R16_SINT;
            D3DFormatToRHIFormatTable[static_cast<U32>(DXGI_FORMAT_R8G8B8A8_SINT)] = E_RHIFormat::R8G8B8A8_SINT;
            D3DFormatToRHIFormatTable[static_cast<U32>(DXGI_FORMAT_R8G8_SINT)] = E_RHIFormat::R8G8_SINT;
            D3DFormatToRHIFormatTable[static_cast<U32>(DXGI_FORMAT_R8_SINT)] = E_RHIFormat::R8_SINT;
            D3DFormatToRHIFormatTable[static_cast<U32>(DXGI_FORMAT_R32G32B32A32_TYPELESS)] = E_RHIFormat::R32G32B32A32_TYPELESS;
            D3DFormatToRHIFormatTable[static_cast<U32>(DXGI_FORMAT_R32G32B32_TYPELESS)] = E_RHIFormat::R32G32B32_TYPELESS;
            D3DFormatToRHIFormatTable[static_cast<U32>(DXGI_FORMAT_R32G32_TYPELESS)] = E_RHIFormat::R32G32_TYPELESS;
            D3DFormatToRHIFormatTable[static_cast<U32>(DXGI_FORMAT_R32_TYPELESS)] = E_RHIFormat::R32_TYPELESS;
            D3DFormatToRHIFormatTable[static_cast<U32>(DXGI_FORMAT_R16G16B16A16_TYPELESS)] = E_RHIFormat::R16G16B16A16_TYPELESS;
            D3DFormatToRHIFormatTable[static_cast<U32>(DXGI_FORMAT_R16G16_TYPELESS)] = E_RHIFormat::R16G16_TYPELESS;
            D3DFormatToRHIFormatTable[static_cast<U32>(DXGI_FORMAT_R16_TYPELESS)] = E_RHIFormat::R16_TYPELESS;
            D3DFormatToRHIFormatTable[static_cast<U32>(DXGI_FORMAT_R8G8B8A8_TYPELESS)] = E_RHIFormat::R8G8B8A8_TYPELESS;
            D3DFormatToRHIFormatTable[static_cast<U32>(DXGI_FORMAT_R8G8_TYPELESS)] = E_RHIFormat::R8G8_TYPELESS;
            D3DFormatToRHIFormatTable[static_cast<U32>(DXGI_FORMAT_R8_TYPELESS)] = E_RHIFormat::R8_TYPELESS;
            D3DFormatToRHIFormatTable[static_cast<U32>(DXGI_FORMAT_NV12)] = E_RHIFormat::NV12;
        }
    }
    
    DXGI_FORMAT RHIFormatToD3DFormat(E_RHIFormat RHIFormat)
    {
        auto ResultOpt = Internal::RHIFormatToD3DFormatTable[static_cast<U32>(RHIFormat)];
        ABYTEK_ENGINE_RHI_ASSERT(ResultOpt) << "Format table missing some values";
        return *ResultOpt;
    }
    E_RHIFormat D3DFormatToRHIFormat(DXGI_FORMAT DXGIFormat)
    {
        auto ResultOpt = Internal::D3DFormatToRHIFormatTable[static_cast<U32>(DXGIFormat)];
        ABYTEK_ENGINE_RHI_ASSERT(ResultOpt) << "Format table missing some values";
        return *ResultOpt;
    }
}
#endif