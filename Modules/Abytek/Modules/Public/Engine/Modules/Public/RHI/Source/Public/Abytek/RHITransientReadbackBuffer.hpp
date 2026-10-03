#pragma once

#include "Abytek/RHIContextChild.hpp"
#include "Abytek/RHIResource.hpp"
#include "Abytek/RHIReadbackBufferPass.hpp"


namespace Abytek
{
    class F_RHITransientReadbackBufferPage;
    
    struct F_RHITransientReadbackBufferRangeLocal
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
    
    struct F_RHITransientReadbackBufferRange : F_RHITransientReadbackBufferRangeLocal
    {
        TW<F_RHITransientReadbackBufferPage> Page;
        
        TS_Valid<A_RHIResource> GetBuffer() const;
        void Readback(F_RHIReadbackBufferCallback&& Callback, Sz ManualSizeInBytes = 0, Sz AdditionalOffsetInBytes = 0) const;
    };
    
    struct F_RHITransientReadbackBufferCandidate : F_RHITransientReadbackBufferRangeLocal
    {
        F_RHIReadbackBufferCallback Callback;
    };
    
    struct F_RHITransientReadbackBufferPageBuildParams : F_RHIContextChildBuildParams
    {
        U32 Index = 0;
        Sz SizeInBytes = 0;
    };
    class ABYTEK_ENGINE_RHI_API F_RHITransientReadbackBufferPage : public A_RHIContextChild
    {
    private:
        U32 _Index = 0;
        Sz _SizeInBytes = 0;
        Sz _UsageInBytes = 0;
        TS<A_RHIResource> _Buffer;
        
    public:
        TF_ConcurrentQueue<F_RHITransientReadbackBufferCandidate> Queue;
        
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
        ABYTEK_RA_DECLARE_OBJECT_CREATABLE(F_RHITransientReadbackBufferPage);
        virtual void Build(const F_RHITransientReadbackBufferPageBuildParams& BuildParams);
        void Release() override;
        
    public:
        TF_Optional<F_RHITransientReadbackBufferRange> Allocate(Sz SizeInBytes, Sz AlignmentInBytes);
        
    public:
#ifdef ABYTEK_DEBUG_INFO
        void SetDebugName(const F_DebugName& Value) noexcept override;
#endif
    };
    
    struct F_RHITransientReadbackBufferManagerBuildParams : F_RHIContextChildBuildParams
    {
        Sz MinPageSizeInBytes = Sz(512) * Sz(1024);
        Sz MaxPageSizeInBytes = Sz(256) * Sz(1024) * Sz(1024);
    };
    
    class ABYTEK_ENGINE_RHI_API F_RHITransientReadbackBufferManager : public A_RHIContextChild
    {
    private:
        Sz _MinPageSizeInBytes = 0;
        Sz _MaxPageSizeInBytes = 0;
        F_AtomicFlag _EnqueuedToReleasePages;
        
    public:
        struct F_SectionData
        {
            TF_Vector<TS<F_RHITransientReadbackBufferPage>> Pages;
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
        ABYTEK_RA_DECLARE_OBJECT_CREATABLE(F_RHITransientReadbackBufferManager);
        virtual void Build(const F_RHITransientReadbackBufferManagerBuildParams& BuildParams);
        void Release() override;
        
    public:
        void ReleasePages();
        
    public:
        F_RHITransientReadbackBufferRange Allocate(Sz SizeInBytes, Sz AlignmentInBytes = 256);
        void AddNewPage(Sz SizeInBytes);
    };
}