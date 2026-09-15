#pragma once

#include "Abytek/RHIContextChild.hpp"
#include "Abytek/RHIResource.hpp"


namespace Abytek
{
    class A_RHIResource;

    class ABYTEK_ENGINE_RHI_API A_RHIResourceProxy :
        public A_RHIContextChildProxy
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
        ABYTEK_FORCE_INLINE TW_Valid<A_RHIResource> GetResource() const noexcept
        {
            return GetContextChild().FastCast<A_RHIResource>();
        }
        
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
        ABYTEK_RA_DECLARE_OBJECT_CREATABLE(A_RHIResourceProxy);
        void LateBuildResource();
        void LateBuildBuffer();
        void LateBuildTexture();
        void LateBuildRTAS();
        virtual void Release() override;
    };
}
