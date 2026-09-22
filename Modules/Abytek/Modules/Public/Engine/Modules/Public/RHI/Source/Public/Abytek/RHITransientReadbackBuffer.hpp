#pragma once

#include "Abytek/RHIContextChild.hpp"
#include "Abytek/RHIResource.hpp"
#include "Abytek/RHIReadbackBufferPass.hpp"


namespace Abytek
{
    class F_RHITransientReadbackBufferPage_V2;
    
    struct F_RHITransientReadbackBufferRangeLocal_V2
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
    
    struct F_RHITransientReadbackBufferRange_V2 : F_RHITransientReadbackBufferRangeLocal_V2
    {
        TW<F_RHITransientReadbackBufferPage_V2> Page;
        
        TS_Valid<A_RHIResource> GetBuffer() const;
        void Readback(F_RHIReadbackBufferCallback&& Callback, Sz ManualSizeInBytes = 0, Sz AdditionalOffsetInBytes = 0) const;
    };
    
    struct F_RHITransientReadbackBufferCandidate_V2 : F_RHITransientReadbackBufferRangeLocal_V2
    {
        F_RHIReadbackBufferCallback Callback;
    };
    
    struct F_RHITransientReadbackBufferPageBuildParams_V2 : F_RHIContextChildBuildParams
    {
        U32 Index = 0;
        Sz SizeInBytes = 0;
    };
    class ABYTEK_ENGINE_RHI_API F_RHITransientReadbackBufferPage_V2 : public A_RHIContextChild
    {
    private:
        U32 _Index = 0;
        Sz _SizeInBytes = 0;
        Sz _UsageInBytes = 0;
        TS<A_RHIResource> _Buffer;
        
    public:
        TF_ConcurrentQueue<F_RHITransientReadbackBufferCandidate_V2> Queue;
        
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
        ABYTEK_RA_DECLARE_OBJECT_CREATABLE(F_RHITransientReadbackBufferPage_V2);
        virtual void Build(const F_RHITransientReadbackBufferPageBuildParams_V2& BuildParams);
        void Release() override;
        
    public:
        TF_Optional<F_RHITransientReadbackBufferRange_V2> Allocate(Sz SizeInBytes);
    };
    
    struct F_RHITransientReadbackBufferManagerBuildParams_V2 : F_RHIContextChildBuildParams
    {
        Sz MinPageSizeInBytes = Sz(512) * Sz(1024);
        Sz MaxPageSizeInBytes = Sz(256) * Sz(1024) * Sz(1024);
    };
    
    class ABYTEK_ENGINE_RHI_API F_RHITransientReadbackBufferManager_V2 : public A_RHIContextChild
    {
    private:
        Sz _MinPageSizeInBytes = 0;
        Sz _MaxPageSizeInBytes = 0;
        F_AtomicFlag _EnqueuedToReleasePages;
        
    public:
        struct F_SectionData
        {
            TF_Vector<TS<F_RHITransientReadbackBufferPage_V2>> Pages;
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
        ABYTEK_RA_DECLARE_OBJECT_CREATABLE(F_RHITransientReadbackBufferManager_V2);
        virtual void Build(const F_RHITransientReadbackBufferManagerBuildParams_V2& BuildParams);
        void Release() override;
        
    public:
        void ReleasePages();
        
    public:
        F_RHITransientReadbackBufferRange_V2 Allocate(Sz SizeInBytes);
        void AddNewPage(Sz SizeInBytes);
    };
}