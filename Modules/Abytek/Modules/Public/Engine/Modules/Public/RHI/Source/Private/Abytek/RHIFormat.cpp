#include "Abytek/RHIFormat.hpp"


namespace Abytek
{
    namespace Internal::RHIFormat
    {
        Sz Sizes[] = {
            16, // R32G32B32A32_FLOAT
            16, // R32G32B32_FLOAT
            8, // R32G32_FLOAT
            4, // R32_FLOAT
            8, // R16G16B16A16_FLOAT
            4, // R16G16_FLOAT
            2, // R16_FLOAT
            4, // D32_FLOAT
            8, // R16G16B16A16_UNORM
            4, // R16G16_UNORM
            2, // R16_UNORM
            4, // R8G8B8A8_UNORM
            2, // R8G8_UNORM
            1, // R8_UNORM
            8, // R16G16B16A16_SNORM
            4, // R16G16_SNORM
            2, // R16_SNORM
            4, // R8G8B8A8_SNORM
            2, // R8G8_SNORM
            1, // R8_SNORM
            16, // R32G32B32A32_UINT
            16, // R32G32B32_UINT
            8, // R32G32_UINT
            4, // R32_UINT
            8, // R16G16B16A16_UINT
            4, // R16G16_UINT
            2, // R16_UINT
            4, // R8G8B8A8_UINT
            2, // R8G8_UINT
            1, // R8_UINT
            16, // R32G32B32A32_SINT
            16, // R32G32B32_SINT
            8, // R32G32_SINT
            4, // R32_SINT
            8, // R16G16B16A16_SINT
            4, // R16G16_SINT
            2, // R16_SINT
            4, // R8G8B8A8_SINT
            2, // R8G8_SINT
            1, // R8_SINT
            16, // R32G32B32A32_TYPELESS
            16, // R32G32B32_TYPELESS
            8, // R32G32_TYPELESS
            4, // R32_TYPELESS
            8, // R16G16B16A16_TYPELESS
            4, // R16G16_TYPELESS
            2, // R16_TYPELESS
            4, // R8G8B8A8_TYPELESS
            2, // R8G8_TYPELESS
            1, // R8_TYPELESS 
            4 // NV12
        };
        
        Sz PlaneCounts[] = {
            1, // R32G32B32A32_FLOAT
            1, // R32G32B32_FLOAT
            1, // R32G32_FLOAT
            1, // R32_FLOAT
            1, // R16G16B16A16_FLOAT
            1, // R16G16_FLOAT
            1, // R16_FLOAT
            1, // D32_FLOAT
            1, // R16G16B16A16_UNORM
            1, // R16G16_UNORM
            1, // R16_UNORM
            1, // R8G8B8A8_UNORM
            1, // R8G8_UNORM
            1, // R8_UNORM
            1, // R16G16B16A16_SNORM
            1, // R16G16_SNORM
            1, // R16_SNORM
            1, // R8G8B8A8_SNORM
            1, // R8G8_SNORM
            1, // R8_SNORM
            1, // R32G32B32A32_UINT
            1, // R32G32B32_UINT
            1, // R32G32_UINT
            1, // R32_UINT
            1, // R16G16B16A16_UINT
            1, // R16G16_UINT
            1, // R16_UINT
            1, // R8G8B8A8_UINT
            1, // R8G8_UINT
            1, // R8_UINT
            1, // R32G32B32A32_SINT
            1, // R32G32B32_SINT
            1, // R32G32_SINT
            1, // R32_SINT
            1, // R16G16B16A16_SINT
            1, // R16G16_SINT
            1, // R16_SINT
            1, // R8G8B8A8_SINT
            1, // R8G8_SINT
            1, // R8_SINT
            1, // R32G32B32A32_TYPELESS
            1, // R32G32B32_TYPELESS
            1, // R32G32_TYPELESS
            1, // R32_TYPELESS
            1, // R16G16B16A16_TYPELESS
            1, // R16G16_TYPELESS
            1, // R16_TYPELESS
            1, // R8G8B8A8_TYPELESS
            1, // R8G8_TYPELESS
            1, // R8_TYPELESS
            2 // NV12
        };
        
        boost::container::small_vector<U32, 1> PlaneDataOffsets[] = {
            { 0 }, // R32G32B32A32_FLOAT
            { 0 }, // R32G32B32_FLOAT
            { 0 }, // R32G32_FLOAT
            { 0 }, // R32_FLOAT
            { 0 }, // R16G16B16A16_FLOAT
            { 0 }, // R16G16_FLOAT
            { 0 }, // R16_FLOAT
            { 0 }, // D32_FLOAT
            { 0 }, // R16G16B16A16_UNORM
            { 0 }, // R16G16_UNORM
            { 0 }, // R16_UNORM
            { 0 }, // R8G8B8A8_UNORM
            { 0 }, // R8G8_UNORM
            { 0 }, // R8_UNORM
            { 0 }, // R16G16B16A16_SNORM
            { 0 }, // R16G16_SNORM
            { 0 }, // R16_SNORM
            { 0 }, // R8G8B8A8_SNORM
            { 0 }, // R8G8_SNORM
            { 0 }, // R8_SNORM
            { 0 }, // R32G32B32A32_UINT
            { 0 }, // R32G32B32_UINT
            { 0 }, // R32G32_UINT
            { 0 }, // R32_UINT
            { 0 }, // R16G16B16A16_UINT
            { 0 }, // R16G16_UINT
            { 0 }, // R16_UINT
            { 0 }, // R8G8B8A8_UINT
            { 0 }, // R8G8_UINT
            { 0 }, // R8_UINT
            { 0 }, // R32G32B32A32_SINT
            { 0 }, // R32G32B32_SINT
            { 0 }, // R32G32_SINT
            { 0 }, // R32_SINT
            { 0 }, // R16G16B16A16_SINT
            { 0 }, // R16G16_SINT
            { 0 }, // R16_SINT
            { 0 }, // R8G8B8A8_SINT
            { 0 }, // R8G8_SINT
            { 0 }, // R8_SINT
            { 0 }, // R32G32B32A32_TYPELESS
            { 0 }, // R32G32B32_TYPELESS
            { 0 }, // R32G32_TYPELESS
            { 0 }, // R32_TYPELESS
            { 0 }, // R16G16B16A16_TYPELESS
            { 0 }, // R16G16_TYPELESS
            { 0 }, // R16_TYPELESS
            { 0 }, // R8G8B8A8_TYPELESS
            { 0 }, // R8G8_TYPELESS
            { 0 }, // R8_TYPELESS
            { 0, 1 } // NV12
        };
        
        boost::container::small_vector<U32, 1> PlaneDataSizes[] = {
            { 16 }, // R32G32B32A32_FLOAT
            { 12 }, // R32G32B32_FLOAT
            { 8 }, // R32G32_FLOAT
            { 4 }, // R32_FLOAT
            { 8 }, // R16G16B16A16_FLOAT
            { 4 }, // R16G16_FLOAT
            { 2 }, // R16_FLOAT
            { 4 }, // D32_FLOAT
            { 8 }, // R16G16B16A16_UNORM
            { 4 }, // R16G16_UNORM
            { 2 }, // R16_UNORM
            { 4 }, // R8G8B8A8_UNORM
            { 2 }, // R8G8_UNORM
            { 1 }, // R8_UNORM
            { 8 }, // R16G16B16A16_SNORM
            { 4 }, // R16G16_SNORM
            { 2 }, // R16_SNORM
            { 4 }, // R8G8B8A8_SNORM
            { 2 }, // R8G8_SNORM
            { 1 }, // R8_SNORM
            { 16 }, // R32G32B32A32_UINT
            { 12 }, // R32G32B32_UINT
            { 8 }, // R32G32_UINT
            { 4 }, // R32_UINT
            { 8 }, // R16G16B16A16_UINT
            { 4 }, // R16G16_UINT
            { 2 }, // R16_UINT
            { 4 }, // R8G8B8A8_UINT
            { 2 }, // R8G8_UINT
            { 1 }, // R8_UINT
            { 16 }, // R32G32B32A32_SINT
            { 12 }, // R32G32B32_SINT
            { 8 }, // R32G32_SINT
            { 4 }, // R32_SINT
            { 8 }, // R16G16B16A16_SINT
            { 4 }, // R16G16_SINT
            { 2 }, // R16_SINT
            { 4 }, // R8G8B8A8_SINT
            { 2 }, // R8G8_SINT
            { 1 }, // R8_SINT
            { 16 }, // R32G32B32A32_TYPELESS
            { 12 }, // R32G32B32_TYPELESS
            { 8 }, // R32G32_TYPELESS
            { 4 }, // R32_TYPELESS
            { 8 }, // R16G16B16A16_TYPELESS
            { 4 }, // R16G16_TYPELESS
            { 2 }, // R16_TYPELESS
            { 4 }, // R8G8B8A8_TYPELESS
            { 2 }, // R8G8_TYPELESS
            { 1 }, // R8_TYPELESS
            { 1, 2 } // NV12
        };
        
        B8 TypelessChecks[] = {
            false, // R32G32B32A32_FLOAT
            false, // R32G32B32_FLOAT
            false, // R32G32_FLOAT
            false, // R32_FLOAT
            false, // R16G16B16A16_FLOAT
            false, // R16G16_FLOAT
            false, // R16_FLOAT
            false, // D32_FLOAT
            false, // R16G16B16A16_UNORM
            false, // R16G16_UNORM
            false, // R16_UNORM
            false, // R8G8B8A8_UNORM
            false, // R8G8_UNORM
            false, // R8_UNORM
            false, // R16G16B16A16_SNORM
            false, // R16G16_SNORM
            false, // R16_SNORM
            false, // R8G8B8A8_SNORM
            false, // R8G8_SNORM
            false, // R8_SNORM
            false, // R32G32B32A32_UINT
            false, // R32G32B32_UINT
            false, // R32G32_UINT
            false, // R32_UINT
            false, // R16G16B16A16_UINT
            false, // R16G16_UINT
            false, // R16_UINT
            false, // R8G8B8A8_UINT
            false, // R8G8_UINT
            false, // R8_UINT
            false, // R32G32B32A32_SINT
            false, // R32G32B32_SINT
            false, // R32G32_SINT
            false, // R32_SINT
            false, // R16G16B16A16_SINT
            false, // R16G16_SINT
            false, // R16_SINT
            false, // R8G8B8A8_SINT
            false, // R8G8_SINT
            false, // R8_SINT
            true, // R32G32B32A32_TYPELESS
            true, // R32G32B32_TYPELESS
            true, // R32G32_TYPELESS
            true, // R32_TYPELESS
            true, // R16G16B16A16_TYPELESS
            true, // R16G16_TYPELESS
            true, // R16_TYPELESS
            true, // R8G8B8A8_TYPELESS
            true, // R8G8_TYPELESS
            true, // R8_TYPELESS
            false // NV12
        };
    }
    
    Sz RHIFormatSize(E_RHIFormat Format)
    {
        U32 ResultIndex = (
            static_cast<F_RHIFormatIndex>(Format)    
            - 1
        );
        ABYTEK_ENGINE_RHI_ASSERT(
            ResultIndex < GetStaticArraySize(Internal::RHIFormat::Sizes)    
        ) << "Invalid RHI format: " << static_cast<U32>(Format);
        return Internal::RHIFormat::Sizes[ResultIndex];
    }
    U32 RHIFormatPlaneCount(E_RHIFormat Format)
    {
        U32 ResultIndex = (
            static_cast<F_RHIFormatIndex>(Format)    
            - 1
        );
        ABYTEK_ENGINE_RHI_ASSERT(
            ResultIndex < GetStaticArraySize(Internal::RHIFormat::PlaneCounts)     
        ) << "Invalid RHI format: " << static_cast<U32>(Format);
        return Internal::RHIFormat::PlaneCounts[ResultIndex];
    }
    U32 RHIFormatPlaneDataOffset(E_RHIFormat Format, U32 PlaneIndex)
    {
        U32 ResultIndex = (
            static_cast<F_RHIFormatIndex>(Format)    
            - 1
        );
        ABYTEK_ENGINE_RHI_ASSERT(
            ResultIndex < GetStaticArraySize(Internal::RHIFormat::PlaneDataOffsets)      
        ) << "Invalid RHI format: " << static_cast<U32>(Format);
        ABYTEK_ENGINE_RHI_ASSERT(PlaneIndex < RHIFormatPlaneCount(Format)) 
            << "Invalid plane index, max of " 
            << RHIFormatPlaneCount(Format) 
            << " for format: " 
            << static_cast<U32>(Format);
        return Internal::RHIFormat::PlaneDataOffsets[ResultIndex][PlaneIndex];
    }
    U32 RHIFormatPlaneDataSize(E_RHIFormat Format, U32 PlaneIndex)
    {
        U32 ResultIndex = (
            static_cast<F_RHIFormatIndex>(Format)    
            - 1
        );
        ABYTEK_ENGINE_RHI_ASSERT(
            ResultIndex < GetStaticArraySize(Internal::RHIFormat::PlaneDataSizes)      
        ) << "Invalid RHI format: " << static_cast<U32>(Format);
        ABYTEK_ENGINE_RHI_ASSERT(PlaneIndex < RHIFormatPlaneCount(Format)) 
            << "Invalid plane index, max of " 
            << RHIFormatPlaneCount(Format) 
            << " for format: " 
            << static_cast<U32>(Format);
        return Internal::RHIFormat::PlaneDataSizes[ResultIndex][PlaneIndex];
    }
    B8 RHIFormatHasColor(E_RHIFormat Format)
    {
        return (
            !RHIFormatHasDepth(Format)
            && !RHIFormatHasStencil(Format)
            && !RHIFormatIsTypeless(Format)
        );
    }
    B8 RHIFormatHasDepth(E_RHIFormat Format)
    {
        return (
            (Format == E_RHIFormat::D32_FLOAT)    
        );
    }
    B8 RHIFormatHasStencil(E_RHIFormat Format)
    {
        return false;
    }
    B8 RHIFormatIsTypeless(E_RHIFormat Format)
    {
        U32 ResultIndex = (
            static_cast<F_RHIFormatIndex>(Format)    
            - 1
        );
        ABYTEK_ENGINE_RHI_ASSERT(
            ResultIndex < GetStaticArraySize(Internal::RHIFormat::TypelessChecks)      
        ) << "Invalid RHI format: " << static_cast<U32>(Format);
        return Internal::RHIFormat::TypelessChecks[ResultIndex];
    }
}
