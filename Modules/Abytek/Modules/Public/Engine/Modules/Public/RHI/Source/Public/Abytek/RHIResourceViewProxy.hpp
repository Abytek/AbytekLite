#pragma once

#include "Abytek/RHIContextChild.hpp"
#include "Abytek/RHIResourceView.hpp"


namespace Abytek
{
    class A_RHIResourceView;
    class A_RHIResourceProxy;

    class ABYTEK_ENGINE_RHI_API A_RHIResourceViewProxy :
        public A_RHIContextChildProxy
    {
    private:
        TS<A_RHIResourceProxy> _ResourceProxy;
        E_RHIFormat _Format = E_RHIFormat::NONE;
        F_RHIResourceAccess _Access;
        B8 _AllowBindless = false;

        E_RHIResourceViewArchetype _Archetype = E_RHIResourceViewArchetype::NONE;
        F_RHIBufferViewAspect _BufferViewAspect;
        F_RHITextureViewAspect _TextureViewAspect;
        F_RHIRTASViewAspect _RTASViewAspect;
        
    public:
        ABYTEK_FORCE_INLINE TW_Valid<A_RHIResourceView> GetResourceView() const noexcept
        {
            return GetContextChild().FastCast<A_RHIResourceView>();
        }
        
        ABYTEK_FORCE_INLINE const auto& GetResourceProxy() const noexcept
        {
            return _ResourceProxy;
        }
        ABYTEK_FORCE_INLINE auto& InjectResourceProxy() noexcept
        {
            return _ResourceProxy;
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
        
    public:
        ABYTEK_RA_DECLARE_OBJECT_CREATABLE(A_RHIResourceViewProxy);
        void LateBuildResourceView();
        void LateBuildBufferView();
        void LateBuildTextureView();
        void LateBuildRTASView();
        virtual void Release() override;
    };
}
