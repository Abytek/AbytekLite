#pragma once

#include "Abytek/Engine.RHI.prerequisites.hpp"


namespace Abytek
{
    struct F_RHIRawTexel
    {
        U8 Bytes[16];
    };
    
    enum class E_RHIFormat
    {
        NONE,
        
        R32G32B32A32_FLOAT,
        R32G32B32_FLOAT,
        R32G32_FLOAT,
        R32_FLOAT,
        R16G16B16A16_FLOAT,
        R16G16_FLOAT,
        R16_FLOAT,
        D32_FLOAT,
        R16G16B16A16_UNORM,
        R16G16_UNORM,
        R16_UNORM,
        R8G8B8A8_UNORM,
        R8G8_UNORM,
        R8_UNORM,
        R16G16B16A16_SNORM,
        R16G16_SNORM,
        R16_SNORM,
        R8G8B8A8_SNORM,
        R8G8_SNORM,
        R8_SNORM,
        R32G32B32A32_UINT,
        R32G32B32_UINT,
        R32G32_UINT,
        R32_UINT,
        R16G16B16A16_UINT,
        R16G16_UINT,
        R16_UINT,
        R8G8B8A8_UINT,
        R8G8_UINT,
        R8_UINT,
        R32G32B32A32_SINT,
        R32G32B32_SINT,
        R32G32_SINT,
        R32_SINT,
        R16G16B16A16_SINT,
        R16G16_SINT,
        R16_SINT,
        R8G8B8A8_SINT,
        R8G8_SINT,
        R8_SINT,
        R32G32B32A32_TYPELESS,
        R32G32B32_TYPELESS,
        R32G32_TYPELESS,
        R32_TYPELESS,
        R16G16B16A16_TYPELESS,
        R16G16_TYPELESS,
        R16_TYPELESS,
        R8G8B8A8_TYPELESS,
        R8G8_TYPELESS,
        R8_TYPELESS,
        
        NV12,

        NUM
    };
    ABYTEK_ENUM_REFLECTOR_LOCALNS(E_RHIFormat)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::E_RHIFormat"));

        ABYTEK_REFLECT_ENUM_VALUE(NONE);

        ABYTEK_REFLECT_ENUM_VALUE(R32G32B32A32_FLOAT);
        ABYTEK_REFLECT_ENUM_VALUE(R32G32B32_FLOAT);
        ABYTEK_REFLECT_ENUM_VALUE(R32G32_FLOAT);
        ABYTEK_REFLECT_ENUM_VALUE(R32_FLOAT);
        ABYTEK_REFLECT_ENUM_VALUE(R16G16B16A16_FLOAT);
        ABYTEK_REFLECT_ENUM_VALUE(R16G16_FLOAT);
        ABYTEK_REFLECT_ENUM_VALUE(R16_FLOAT);
        ABYTEK_REFLECT_ENUM_VALUE(D32_FLOAT);
        ABYTEK_REFLECT_ENUM_VALUE(R16G16B16A16_UNORM);
        ABYTEK_REFLECT_ENUM_VALUE(R16G16_UNORM);
        ABYTEK_REFLECT_ENUM_VALUE(R16_UNORM);
        ABYTEK_REFLECT_ENUM_VALUE(R8G8B8A8_UNORM);
        ABYTEK_REFLECT_ENUM_VALUE(R8G8_UNORM);
        ABYTEK_REFLECT_ENUM_VALUE(R8_UNORM);
        ABYTEK_REFLECT_ENUM_VALUE(R16G16B16A16_SNORM);
        ABYTEK_REFLECT_ENUM_VALUE(R16G16_SNORM);
        ABYTEK_REFLECT_ENUM_VALUE(R16_SNORM);
        ABYTEK_REFLECT_ENUM_VALUE(R8G8B8A8_SNORM);
        ABYTEK_REFLECT_ENUM_VALUE(R8G8_SNORM);
        ABYTEK_REFLECT_ENUM_VALUE(R8_SNORM);
        ABYTEK_REFLECT_ENUM_VALUE(R32G32B32A32_UINT);
        ABYTEK_REFLECT_ENUM_VALUE(R32G32B32_UINT);
        ABYTEK_REFLECT_ENUM_VALUE(R32G32_UINT);
        ABYTEK_REFLECT_ENUM_VALUE(R32_UINT);
        ABYTEK_REFLECT_ENUM_VALUE(R16G16B16A16_UINT);
        ABYTEK_REFLECT_ENUM_VALUE(R16G16_UINT);
        ABYTEK_REFLECT_ENUM_VALUE(R16_UINT);
        ABYTEK_REFLECT_ENUM_VALUE(R8G8B8A8_UINT);
        ABYTEK_REFLECT_ENUM_VALUE(R8G8_UINT);
        ABYTEK_REFLECT_ENUM_VALUE(R8_UINT);
        ABYTEK_REFLECT_ENUM_VALUE(R32G32B32A32_SINT);
        ABYTEK_REFLECT_ENUM_VALUE(R32G32B32_SINT);
        ABYTEK_REFLECT_ENUM_VALUE(R32G32_SINT);
        ABYTEK_REFLECT_ENUM_VALUE(R32_SINT);
        ABYTEK_REFLECT_ENUM_VALUE(R16G16B16A16_SINT);
        ABYTEK_REFLECT_ENUM_VALUE(R16G16_SINT);
        ABYTEK_REFLECT_ENUM_VALUE(R16_SINT);
        ABYTEK_REFLECT_ENUM_VALUE(R8G8B8A8_SINT);
        ABYTEK_REFLECT_ENUM_VALUE(R8G8_SINT);
        ABYTEK_REFLECT_ENUM_VALUE(R8_SINT);
        ABYTEK_REFLECT_ENUM_VALUE(R32G32B32A32_TYPELESS);
        ABYTEK_REFLECT_ENUM_VALUE(R32G32B32_TYPELESS);
        ABYTEK_REFLECT_ENUM_VALUE(R32G32_TYPELESS);
        ABYTEK_REFLECT_ENUM_VALUE(R32_TYPELESS);
        ABYTEK_REFLECT_ENUM_VALUE(R16G16B16A16_TYPELESS);
        ABYTEK_REFLECT_ENUM_VALUE(R16G16_TYPELESS);
        ABYTEK_REFLECT_ENUM_VALUE(R16_TYPELESS);
        ABYTEK_REFLECT_ENUM_VALUE(R8G8B8A8_TYPELESS);
        ABYTEK_REFLECT_ENUM_VALUE(R8G8_TYPELESS);
        ABYTEK_REFLECT_ENUM_VALUE(R8_TYPELESS);

        ABYTEK_REFLECT_ENUM_VALUE(NV12);

        ABYTEK_REFLECT_ENUM_VALUE(NUM);
    }
    
    using F_RHIFormatIndex = TF_UInt<sizeof(E_RHIFormat)>;
    
    ABYTEK_ENGINE_RHI_API Sz RHIFormatSize(E_RHIFormat Format);
    ABYTEK_ENGINE_RHI_API U32 RHIFormatPlaneCount(E_RHIFormat Format);
    ABYTEK_ENGINE_RHI_API U32 RHIFormatPlaneDataOffset(E_RHIFormat Format, U32 PlaneIndex = 0);
    ABYTEK_ENGINE_RHI_API U32 RHIFormatPlaneDataSize(E_RHIFormat Format, U32 PlaneIndex = 0);
    ABYTEK_ENGINE_RHI_API B8 RHIFormatHasColor(E_RHIFormat Format);
    ABYTEK_ENGINE_RHI_API B8 RHIFormatHasDepth(E_RHIFormat Format);
    ABYTEK_ENGINE_RHI_API B8 RHIFormatHasStencil(E_RHIFormat Format);
    ABYTEK_ENGINE_RHI_API B8 RHIFormatIsTypeless(E_RHIFormat Format);
    
    namespace Internal::RHIFormat
    {
        template<E_RHIFormat __Format>
        struct TH_Data;
        
        template<>
        struct TH_Data<E_RHIFormat::R32G32B32A32_FLOAT>
        {
            using F = F_Vector4_F32;
        };
        template<>
        struct TH_Data<E_RHIFormat::R32G32B32_FLOAT>
        {
            using F = F_Vector3_F32;
        };
        template<>
        struct TH_Data<E_RHIFormat::R32G32_FLOAT>
        {
            using F = F_Vector2_F32;
        };
        template<>
        struct TH_Data<E_RHIFormat::R32_FLOAT>
        {
            using F = F32;
        };
        
        template<>
        struct TH_Data<E_RHIFormat::R16G16B16A16_FLOAT>
        {
            using F = F_Vector4_U16;
        };
        template<>
        struct TH_Data<E_RHIFormat::R16G16_FLOAT>
        {
            using F = F_Vector2_U16;
        };
        template<>
        struct TH_Data<E_RHIFormat::R16_FLOAT>
        {
            using F = U16;
        };
        
        template<>
        struct TH_Data<E_RHIFormat::D32_FLOAT>
        {
            using F = F32;
        };
        
        template<>
        struct TH_Data<E_RHIFormat::R16G16B16A16_UNORM>
        {
            using F = F_Vector4_U16;
        };
        template<>
        struct TH_Data<E_RHIFormat::R16G16_UNORM>
        {
            using F = F_Vector2_U16;
        };
        template<>
        struct TH_Data<E_RHIFormat::R16_UNORM>
        {
            using F = U16;
        };
        
        template<>
        struct TH_Data<E_RHIFormat::R8G8B8A8_UNORM>
        {
            using F = F_Vector4_U8;
        };
        template<>
        struct TH_Data<E_RHIFormat::R8G8_UNORM>
        {
            using F = F_Vector2_U8;
        };
        template<>
        struct TH_Data<E_RHIFormat::R8_UNORM>
        {
            using F = U8;
        };
        
        template<>
        struct TH_Data<E_RHIFormat::R16G16B16A16_SNORM>
        {
            using F = F_Vector4_I16;
        };
        template<>
        struct TH_Data<E_RHIFormat::R16G16_SNORM>
        {
            using F = F_Vector2_I16;
        };
        template<>
        struct TH_Data<E_RHIFormat::R16_SNORM>
        {
            using F = I16;
        };
        
        template<>
        struct TH_Data<E_RHIFormat::R8G8B8A8_SNORM>
        {
            using F = F_Vector4_I8;
        };
        template<>
        struct TH_Data<E_RHIFormat::R8G8_SNORM>
        {
            using F = F_Vector2_I8;
        };
        template<>
        struct TH_Data<E_RHIFormat::R8_SNORM>
        {
            using F = I8;
        };
        
        template<>
        struct TH_Data<E_RHIFormat::R32G32B32A32_UINT>
        {
            using F = F_Vector4_U32;
        };
        template<>
        struct TH_Data<E_RHIFormat::R32G32B32_UINT>
        {
            using F = F_Vector3_U32;
        };
        template<>
        struct TH_Data<E_RHIFormat::R32G32_UINT>
        {
            using F = F_Vector2_U32;
        };
        template<>
        struct TH_Data<E_RHIFormat::R32_UINT>
        {
            using F = U32;
        };
        
        template<>
        struct TH_Data<E_RHIFormat::R16G16B16A16_UINT>
        {
            using F = F_Vector4_U16;
        };
        template<>
        struct TH_Data<E_RHIFormat::R16G16_UINT>
        {
            using F = F_Vector2_U16;
        };
        template<>
        struct TH_Data<E_RHIFormat::R16_UINT>
        {
            using F = U16;
        };
        
        template<>
        struct TH_Data<E_RHIFormat::R8G8B8A8_UINT>
        {
            using F = F_Vector4_U8;
        };
        template<>
        struct TH_Data<E_RHIFormat::R8G8_UINT>
        {
            using F = F_Vector2_U8;
        };
        template<>
        struct TH_Data<E_RHIFormat::R8_UINT>
        {
            using F = U8;
        };
        
        template<>
        struct TH_Data<E_RHIFormat::R32G32B32A32_SINT>
        {
            using F = F_Vector4_I32;
        };
        template<>
        struct TH_Data<E_RHIFormat::R32G32B32_SINT>
        {
            using F = F_Vector3_I32;
        };
        template<>
        struct TH_Data<E_RHIFormat::R32G32_SINT>
        {
            using F = F_Vector2_I32;
        };
        template<>
        struct TH_Data<E_RHIFormat::R32_SINT>
        {
            using F = I32;
        };
        
        template<>
        struct TH_Data<E_RHIFormat::R16G16B16A16_SINT>
        {
            using F = F_Vector4_I16;
        };
        template<>
        struct TH_Data<E_RHIFormat::R16G16_SINT>
        {
            using F = F_Vector2_I16;
        };
        template<>
        struct TH_Data<E_RHIFormat::R16_SINT>
        {
            using F = I16;
        };
        
        template<>
        struct TH_Data<E_RHIFormat::R8G8B8A8_SINT>
        {
            using F = F_Vector4_I8;
        };
        template<>
        struct TH_Data<E_RHIFormat::R8G8_SINT>
        {
            using F = F_Vector2_I8;
        };
        template<>
        struct TH_Data<E_RHIFormat::R8_SINT>
        {
            using F = I8;
        };
    }
    template<E_RHIFormat __Format>
    using TF_RHIFormatData = typename Internal::RHIFormat::TH_Data<__Format>::F;
}