#pragma once

#include "Abytek/RHIContextChild.hpp"
#include "Abytek/RHIFormat.hpp" 
#include "Abytek/RHISampleDesc.hpp"
#include "Abytek/RHIImage.hpp"
#include "Abytek/RHIResourceAccess.hpp"


namespace Abytek
{
    using F_RHITextureElement = F_RHIImageElement;
    using F_RHITextureElementSet = F_RHIImageElementSet;

    using E_RHITextureFlag = E_RHIImageFlag;
    using E_RHITextureLayout = E_RHIImageLayout;
    
    struct F_RHITextureClearValue
    {
        E_RHIFormat Format = E_RHIFormat::NONE;  ///< Format of the resource
        F_Vector4_F32 Color = F_Vector4_F32::One();  ///< Clear color for color resources
        F32 Depth = 0.0f;                          ///< Clear depth value
        U8 Stencil = 0;                           ///< Clear stencil value
    };

    enum class E_RHIResourceArchetype : U16
    {
        NONE = 0x0,
        
        BUFFER  = 0x1,
        TEXTURE = 0x2,
        RTAS = 0x4
    };
    ABYTEK_DEFINE_FLAG_OPERATORS(E_RHIResourceArchetype);

    struct F_RHIBufferAspect
    {
        U64 SizeInBytes = 0;  ///< Size of the buffer in bytes
        U32 StrideInBytes = 0;       ///< Stride between elements in bytes
    };
    struct F_RHITextureAspect : F_RHIImageSettingMinimal
    {
        F_RHITextureClearValue ClearValue;
        F_RHISampleDesc SampleDesc;
    };
    struct F_RHIRTASAspect
    {
    };

    template<class __F_Allocator = TF_DefaultAllocator<U8>>
    using TF_RHIBufferData = ContainerTemplates::TF_Vector<U8, TF_RebindAllocator<U8, __F_Allocator>>;
    using F_RHIBufferData = TF_RHIBufferData<>;
    using F_RHIBufferDataView = TF_Span<const U8>;
    
    template<class __F_Allocator = TF_DefaultAllocator<U8>>
    using TF_RHITextureData = TF_RHIImage<__F_Allocator>;
    using F_RHITextureData = TF_RHITextureData<>;
    template<class __F_Allocator = TF_DefaultAllocator<U8>>
    using TF_RHITextureDataView = TF_RHIImageView<false>;
    using F_RHITextureDataView = TF_RHITextureDataView<>;

    template<class __F_Allocator = TF_DefaultAllocator<U8>>
    struct TF_RHIRTASData
    {
    };
    using F_RHIRTASData = TF_RHIRTASData<>;
    using F_RHIRTASDataView = F_RHIRTASData;

    enum class E_RHIResourceAdditionalFlag : U16
    {
        NONE = 0x0,
        CROSS_CONTEXT = 0x1,
        AUTO_PLACED = 0x2,
        RELEASE_AFTER_SUBMIT = 0x4,
        STATIC = 0x8,
        
        // This mode is unsafe
        STATIC_ON_WRITE_ACCESS = 0x10,
        
        STATIC_AFTER_SUBMIT = 0x20,
        
        TRANSIENT = AUTO_PLACED | RELEASE_AFTER_SUBMIT,
        PERSISTENT = NONE,
        
        DEFAULT = PERSISTENT
    };
    ABYTEK_DEFINE_FLAG_OPERATORS(E_RHIResourceAdditionalFlag);
    
    inline B8 RHIResourceCanUseAsStatic(const F_RHIResourceAccess& Access, E_RHIResourceAdditionalFlag AdditionalFlags)
    {
        if (RHIHasCPUWriteAccess(Access.CPU) || RHIHasGPUWriteAccess(Access.GPU)) 
        {
            return FlagHas(AdditionalFlags, E_RHIResourceAdditionalFlag::STATIC_ON_WRITE_ACCESS);
        }
        return true;
    }
    
    struct F_RHIResourceBuildParamsBase : F_RHIContextChildBuildParams
    {
        E_RHIFormat Format = E_RHIFormat::NONE;
        F_RHIResourceAccess AccessCapabilities;
        TF_Optional<F_RHIResourceAccess> InitialAccess;
        TF_Optional<F_RHIResourceAccess> StaticAccess;
        E_RHIResourceAdditionalFlag AdditionalFlags = E_RHIResourceAdditionalFlag::DEFAULT;
    };
    
    struct F_RHIBufferBuildParams : F_RHIResourceBuildParamsBase
    {
        F_RHIBufferAspect BufferAspect;
        TF_Optional<F_RHIBufferDataView> BufferDataView;
    };
    struct F_RHITextureBuildParams : F_RHIResourceBuildParamsBase
    {
        F_RHITextureAspect TextureAspect;
        TF_Optional<F_RHITextureDataView> TextureDataView;
    };
    struct F_RHIRTASBuildParams : F_RHIResourceBuildParamsBase
    {
        F_RHIRTASAspect RTASAspect;
        TF_Optional<F_RHIRTASDataView> RTASDataView;
    };

    class ABYTEK_ENGINE_RHI_API A_RHIResource :
        public A_RHIContextChild
    {
    private:
        E_RHIFormat _Format = E_RHIFormat::NONE;
        F_RHIResourceAccess _AccessCapabilities;
        TF_Optional<F_RHIResourceAccess> _InitialAccess;
        TF_Optional<F_RHIResourceAccess> _StaticAccess;
        E_RHIResourceAdditionalFlag _AdditionalFlags = E_RHIResourceAdditionalFlag::DEFAULT;

        E_RHIResourceArchetype _Archetype = E_RHIResourceArchetype::NONE;
        F_RHIBufferAspect _BufferAspect;
        F_RHITextureAspect _TextureAspect;
        F_RHIRTASAspect _RTASAspect;
        
    public:
        ABYTEK_FORCE_INLINE E_RHIFormat GetFormat() const noexcept
        {
            return _Format;
        }
        ABYTEK_FORCE_INLINE auto& InjectFormat() noexcept
        {
            return _Format;
        }
        ABYTEK_FORCE_INLINE const auto& GetAccessCapabilities() const noexcept
        {
            return _AccessCapabilities;
        }
        ABYTEK_FORCE_INLINE auto& InjectAccessCapabilities() noexcept
        {
            return _AccessCapabilities;
        }
        ABYTEK_FORCE_INLINE const auto& GetInitialAccess() const noexcept
        {
            return _InitialAccess;
        }
        ABYTEK_FORCE_INLINE auto& InjectInitialAccess() noexcept
        {
            return _InitialAccess;
        }
        ABYTEK_FORCE_INLINE const auto& GetStaticAccess() const noexcept
        {
            return _StaticAccess;
        }
        ABYTEK_FORCE_INLINE auto& InjectStaticAccess() noexcept
        {
            return _StaticAccess;
        }
        ABYTEK_FORCE_INLINE auto GetAdditionalFlags() const noexcept
        {
            return _AdditionalFlags;
        }
        ABYTEK_FORCE_INLINE auto& InjectAdditionalFlags() noexcept
        {
            return _AdditionalFlags;
        }
        
        ABYTEK_FORCE_INLINE auto GetArchetype() const noexcept
        {
            return _Archetype;
        }
        ABYTEK_FORCE_INLINE auto& InjectArchetype() noexcept
        {
            return _Archetype;
        }
        
        ABYTEK_FORCE_INLINE const auto& GetBufferAspect() const noexcept
        {
            return _BufferAspect;
        }
        ABYTEK_FORCE_INLINE auto& InjectBufferAspect() noexcept
        {
            return _BufferAspect;
        }
        ABYTEK_FORCE_INLINE const auto& GetTextureAspect() const noexcept
        {
            return _TextureAspect;
        }
        ABYTEK_FORCE_INLINE auto& InjectTextureAspect() noexcept
        {
            return _TextureAspect;
        }
        ABYTEK_FORCE_INLINE const auto& GetRTASAspect() const noexcept
        {
            return _RTASAspect;
        }
        ABYTEK_FORCE_INLINE auto& InjectRTASAspect() noexcept
        {
            return _RTASAspect;
        }
        
    public:
        ABYTEK_RA_DECLARE_OBJECT_CREATABLE(A_RHIResource);
        virtual void Release() override;

    public:
        virtual void Build(const F_RHIBufferBuildParams& BuildParams);
        virtual void Build(const F_RHITextureBuildParams& BuildParams);
        virtual void Build(const F_RHIRTASBuildParams& BuildParams);

    protected:
        virtual void Build(
            const F_RHIResourceBuildParamsBase& BuildParamsBase,
            const F_RHIBufferAspect& BufferAspect
        );
        virtual void Build(
            const F_RHIResourceBuildParamsBase& BuildParamsBase,
            const F_RHITextureAspect& TextureAspect
        );
        virtual void Build(
            const F_RHIResourceBuildParamsBase& BuildParamsBase,
            const F_RHIRTASAspect& RTASAspect
        );
        virtual void Build(const F_RHIResourceBuildParamsBase& BuildParams);
        
    public:
        virtual void ReleaseAfterSubmit();
        
    public:
        virtual void EnableStaticAfterSubmit(const F_RHIResourceAccess& StaticAccess);
        virtual void DisableStatic();
        
    protected:
        virtual TS<A_RHIContextChildProxy> CreateProxy() override;
    };
}
