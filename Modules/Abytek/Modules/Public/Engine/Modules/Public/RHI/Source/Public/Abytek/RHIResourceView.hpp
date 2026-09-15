#pragma once

#include "Abytek/RHIResource.hpp"
#include "Abytek/RHIBindless.hpp"


namespace Abytek
{
    enum class E_RHIResourceViewArchetype : U8
    {
        NONE = 0x0,
        
        BUFFER_VIEW  = 0x1,
        TEXTURE_VIEW = 0x2,
        RTAS_VIEW = 0x4
    };
    ABYTEK_DEFINE_FLAG_OPERATORS(E_RHIResourceViewArchetype);
    
    struct F_RHIResourceViewBuildParamsBase : F_RHIContextChildBuildParams
    {
        TS<A_RHIResource> Resource;
        E_RHIFormat Format = E_RHIFormat::NONE;
        F_RHIResourceAccess Access;
        B8 AllowBindless = false;
    };
    struct F_RHIBufferViewAspect
    {
        U64 SizeInBytes = 0;
        U64 OffsetInBytes = 0;
        U32 StrideInBytes = 1;
        
        B8 RawAccess = false;
    };
    struct F_RHITextureViewAspect
    {
        E_RHITextureFlag Flags = E_RHITextureFlag::DEFAULT;
        
        U32 FirstMipLevel = 0;
        U32 MipLevelCount = 1;
        U32 FirstArraySize = 0;
        U32 ArraySize = 1;
        U32 PlaneIndex = 0;
        
        B8 UAVClearable = false;
    };
    struct F_RHIRTASViewAspect
    {
    };

    struct F_RHIBufferViewBuildParams : F_RHIResourceViewBuildParamsBase
    {
        F_RHIBufferViewAspect BufferViewAspect;
    };
    struct F_RHITextureViewBuildParams : F_RHIResourceViewBuildParamsBase
    {
        F_RHITextureViewAspect TextureViewAspect;
    };
    struct F_RHIRTASViewBuildParams : F_RHIResourceViewBuildParamsBase
    {
        F_RHIRTASViewAspect RTASViewAspect;
    };
    
    struct F_RHIResourceViewBindlessRange : F_RHIBindlessRange
    {
        static F_RHIResourceViewBindlessRange Make(U32 InBeginIndex, U32 InEndIndex)
        {
            F_RHIResourceViewBindlessRange Result;
            Result.BeginIndex = InBeginIndex;
            Result.EndIndex = InEndIndex;
            return Result;
        }
    };
    
    class ABYTEK_ENGINE_RHI_API A_RHIResourceView :
        public A_RHIContextChild
    {
    private:
        TS<A_RHIResource> _Resource;
        E_RHIFormat _Format = E_RHIFormat::NONE;
        F_RHIResourceAccess _Access;
        B8 _AllowBindless = false;

        E_RHIResourceViewArchetype _Archetype = E_RHIResourceViewArchetype::NONE;
        F_RHIBufferViewAspect _BufferViewAspect;
        F_RHITextureViewAspect _TextureViewAspect;
        F_RHIRTASViewAspect _RTASViewAspect;
        
        F_RHIBindlessId _BindlessId = INVALID_RHI_BINDLESS_ID;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetResource() const noexcept
        {
            return _Resource;
        }
        ABYTEK_FORCE_INLINE auto& InjectResource() noexcept
        {
            return _Resource;
        }
        ABYTEK_FORCE_INLINE E_RHIFormat GetFormat() const noexcept
        {
            return _Format;
        }
        ABYTEK_FORCE_INLINE auto& InjectFormat() noexcept
        {
            return _Format;
        }
        ABYTEK_FORCE_INLINE auto GetAccess() const noexcept
        {
            return _Access;
        }
        ABYTEK_FORCE_INLINE auto& InjectAccess() noexcept
        {
            return _Access;
        }
        ABYTEK_FORCE_INLINE auto GetAllowBindless() const noexcept
        {
            return _AllowBindless;
        }
        ABYTEK_FORCE_INLINE auto& InjectAllowBindless() noexcept
        {
            return _AllowBindless;
        }
        
        ABYTEK_FORCE_INLINE auto GetArchetype() const noexcept
        {
            return _Archetype;
        }
        ABYTEK_FORCE_INLINE auto& InjectArchetype() noexcept
        {
            return _Archetype;
        }
        
        ABYTEK_FORCE_INLINE const auto& GetBufferViewAspect() const noexcept
        {
            return _BufferViewAspect;
        }
        ABYTEK_FORCE_INLINE auto& InjectBufferViewAspect() noexcept
        {
            return _BufferViewAspect;
        }
        ABYTEK_FORCE_INLINE const auto& GetTextureViewAspect() const noexcept
        {
            return _TextureViewAspect;
        }
        ABYTEK_FORCE_INLINE auto& InjectTextureViewAspect() noexcept
        {
            return _TextureViewAspect;
        }
        ABYTEK_FORCE_INLINE const auto& GetRTASViewAspect() const noexcept
        {
            return _RTASViewAspect;
        }
        ABYTEK_FORCE_INLINE auto& InjectRTASViewAspect() noexcept
        {
            return _RTASViewAspect;
        }
        
        ABYTEK_FORCE_INLINE auto GetBindlessId() const noexcept
        {
            return _BindlessId;
        }
        ABYTEK_FORCE_INLINE auto& InjectBindlessId() noexcept
        {
            return _BindlessId;
        }
        
    public:
        ABYTEK_RA_DECLARE_OBJECT_CREATABLE(A_RHIResourceView);
        virtual void Release() override;

    public:
        virtual void Build(const F_RHIBufferViewBuildParams& BuildParams);
        virtual void Build(const F_RHITextureViewBuildParams& BuildParams);
        virtual void Build(const F_RHIRTASViewBuildParams& BuildParams);

    public:
        virtual void Build(const TS_Valid<A_RHIResource>& Resource, const F_RHIResourceAccess& Access);

    protected:
        virtual void Build(const F_RHIResourceViewBuildParamsBase& BuildParams);
        
    protected:
        virtual TS<A_RHIContextChildProxy> CreateProxy() override;
    };
}
