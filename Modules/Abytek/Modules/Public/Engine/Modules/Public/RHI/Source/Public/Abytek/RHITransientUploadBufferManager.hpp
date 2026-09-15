#pragma once

#include "Abytek/RHIContextChild.hpp"
#include "Abytek/RHIResource.hpp"


namespace Abytek
{
    class F_RHITransientUploadBufferPage;
    
    struct F_RHITransientUploadBufferRange
    {
        TW<F_RHITransientUploadBufferPage> Page;
        Sz BeginOffsetInBytes = 0;
        Sz EndOffsetInBytes = 0;
        
        ABYTEK_FORCE_INLINE auto GetSizeInBytes() const noexcept
        {
            return EndOffsetInBytes - BeginOffsetInBytes;
        }
        ABYTEK_FORCE_INLINE auto IsValid() const noexcept
        {
            return EndOffsetInBytes > BeginOffsetInBytes;
        }
        ABYTEK_FORCE_INLINE explicit operator B8 () const noexcept
        {
            return IsValid();
        }
        TS_Valid<A_RHIResource> GetBuffer() const;
    };
    
    struct F_RHITransientUploadBufferPageBuildParams : F_RHIContextChildBuildParams
    {
        Sz SizeInBytes = 0;
    };
    
    class ABYTEK_ENGINE_RHI_API F_RHITransientUploadBufferPage : public A_RHIContextChild
    {
    private:
        Sz _SizeInBytes = 0;
        Sz _UsageInBytes = 0;
        TS<A_RHIResource> _Buffer;
        
    public:
        ABYTEK_FORCE_INLINE auto GetSizeInBytes() const noexcept
        {
            return _SizeInBytes;
        }
        ABYTEK_FORCE_INLINE auto GetUsageInBytes() const noexcept
        {
            return _UsageInBytes;
        }
        ABYTEK_FORCE_INLINE const auto& GetBuffer() const noexcept
        {
            return _Buffer;
        }
        
    public:
        ABYTEK_RA_DECLARE_OBJECT_CREATABLE(F_RHITransientUploadBufferPage);
        virtual void Build(const F_RHITransientUploadBufferPageBuildParams& BuildParams);
        virtual void Release() override;
        
    public:
        TF_Optional<F_RHITransientUploadBufferRange> Allocate(Sz SizeInBytes);
    };
    
    struct F_RHITransientUploadBufferManagerBuildParams : F_RHIContextChildBuildParams
    {
        Sz MinPageSizeInBytes = Sz(512) * Sz(1024);
        Sz MaxPageSizeInBytes = Sz(256) * Sz(1024) * Sz(1024);
    };
    
    class ABYTEK_ENGINE_RHI_API F_RHITransientUploadBufferManager : public A_RHIContextChild
    {
    private:
        Sz _MinPageSizeInBytes = 0;
        Sz _MaxPageSizeInBytes = 0;
        F_AtomicFlag _EnqueuedToReleasePages;
        
    public:
        struct F_SectionData
        {
            TF_Vector<TS<F_RHITransientUploadBufferPage>> Pages;
        } SectionData;
        
    public:
        ABYTEK_FORCE_INLINE auto GetMinPageSizeInBytes() const noexcept
        {
            return _MinPageSizeInBytes;
        }
        ABYTEK_FORCE_INLINE auto GetMaxPageSizeInBytes() const noexcept
        {
            return _MaxPageSizeInBytes;
        }
        
    public:
        ABYTEK_RA_DECLARE_OBJECT_CREATABLE(F_RHITransientUploadBufferManager);
        virtual void Build(const F_RHITransientUploadBufferManagerBuildParams& BuildParams);
        virtual void Release() override;
        
    public:
        void ReleasePages();
        
    public:
        F_RHITransientUploadBufferRange Allocate(Sz SizeInBytes);
        void AddNewPage(Sz SizeInBytes);
    };
}