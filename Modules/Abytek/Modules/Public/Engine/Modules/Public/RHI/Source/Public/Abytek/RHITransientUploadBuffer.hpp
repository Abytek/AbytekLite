#pragma once

#include "Abytek/RHIContextChild.hpp"
#include "Abytek/RHIResource.hpp"


namespace Abytek
{
    class F_RHITransientUploadBufferPage_V2;
    
    struct F_RHITransientUploadBufferRangeLocal_V2
    {
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
    };
    
    struct F_RHITransientUploadBufferRange_V2 : F_RHITransientUploadBufferRangeLocal_V2
    {
        TW<F_RHITransientUploadBufferPage_V2> Page;
        
        TS_Valid<A_RHIResource> GetBuffer() const;
        void Upload(const F_RHIBufferDataView& BufferDataView, Sz AdditionalOffsetInBytes = 0) const;
    };
    
    struct F_RHITransientUploadBufferCandidate_V2 : F_RHITransientUploadBufferRangeLocal_V2
    {
        F_RHIBufferDataView BufferDataView;
    };
    
    struct F_RHITransientUploadBufferPageBuildParams_V2 : F_RHIContextChildBuildParams
    {
        U32 Index = 0;
        Sz SizeInBytes = 0;
    };
    class ABYTEK_ENGINE_RHI_API F_RHITransientUploadBufferPage_V2 : public A_RHIContextChild
    {
    private:
        U32 _Index = 0;
        Sz _SizeInBytes = 0;
        Sz _UsageInBytes = 0;
        TS<A_RHIResource> _Buffer;
        
    public:
        TF_ConcurrentQueue<F_RHITransientUploadBufferCandidate_V2> Queue;
        
    public:
        ABYTEK_FORCE_INLINE auto GetIndex() const noexcept
        {
            return _Index;
        }
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
        ABYTEK_RA_DECLARE_OBJECT_CREATABLE(F_RHITransientUploadBufferPage_V2);
        virtual void Build(const F_RHITransientUploadBufferPageBuildParams_V2& BuildParams);
        void Release() override;
        
    public:
        TF_Optional<F_RHITransientUploadBufferRange_V2> Allocate(Sz SizeInBytes);
    };
    
    struct F_RHITransientUploadBufferManagerBuildParams_V2 : F_RHIContextChildBuildParams
    {
        Sz MinPageSizeInBytes = Sz(512) * Sz(1024);
        Sz MaxPageSizeInBytes = Sz(256) * Sz(1024) * Sz(1024);
    };
    
    class ABYTEK_ENGINE_RHI_API F_RHITransientUploadBufferManager_V2 : public A_RHIContextChild
    {
    private:
        Sz _MinPageSizeInBytes = 0;
        Sz _MaxPageSizeInBytes = 0;
        F_AtomicFlag _EnqueuedToReleasePages;
        
    public:
        struct F_SectionData
        {
            TF_Vector<TS<F_RHITransientUploadBufferPage_V2>> Pages;
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
        ABYTEK_RA_DECLARE_OBJECT_CREATABLE(F_RHITransientUploadBufferManager_V2);
        virtual void Build(const F_RHITransientUploadBufferManagerBuildParams_V2& BuildParams);
        void Release() override;
        
    public:
        void ReleasePages();
        
    public:
        F_RHITransientUploadBufferRange_V2 Allocate(Sz SizeInBytes);
        void AddNewPage(Sz SizeInBytes);
    };
}