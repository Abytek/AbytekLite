#pragma once

#include "Abytek/RHIContextChild.hpp"
#include "Abytek/RHIResource.hpp"
#include "Abytek/RHIResourceView.hpp"


namespace Abytek
{
    class F_RHIConstantDataPage;
    class A_RHISubmissionItemContainer;
    class A_RHIResourceProxy;
    
    struct F_RHIConstantDataRangeProxy
    {
        TS<A_RHIResourceProxy> BufferProxy;
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
    struct F_RHIConstantDataRange
    {
        TW<F_RHIConstantDataPage> Page;
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
        TS_Valid<A_RHIResourceView> CreateCBV() const;
        void Upload(
            const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer,
            const F_RHIBufferDataView& BufferDataView,
            const F_DebugName& DebugName = {}
        ) const;
        F_RHIConstantDataRangeProxy ConvertToProxy() const;
    };
    
    struct F_RHIConstantDataPageBuildParams : F_RHIContextChildBuildParams
    {
        U32 Index = 0;
        Sz SizeInBytes = 0;
    };
    
    class ABYTEK_ENGINE_RHI_API F_RHIConstantDataPage : public A_RHIContextChild
    {
    private:
        U32 _Index = 0;
        Sz _SizeInBytes = 0;
        TS<A_RHIResource> _Buffer;
        
        F_LinearAllocationDistributor _Distributor;
        
    public:
        ABYTEK_FORCE_INLINE auto GetIndex() const noexcept
        {
            return _Index;
        }
        ABYTEK_FORCE_INLINE auto GetSizeInBytes() const noexcept
        {
            return _SizeInBytes;
        }
        ABYTEK_FORCE_INLINE const auto& GetBuffer() const noexcept
        {
            return _Buffer;
        }
        
    public:
        ABYTEK_RA_DECLARE_OBJECT_CREATABLE(F_RHIConstantDataPage);
        virtual void Build(const F_RHIConstantDataPageBuildParams& BuildParams);
        virtual void Release() override;
        
    public:
        TF_Optional<F_RHIConstantDataRange> Allocate(Sz SizeInBytes);
        void Deallocate(const F_RHIConstantDataRange& Range);
    };
    
    struct F_RHIConstantDataManagerBuildParams : F_RHIContextChildBuildParams
    {
        Sz MinPageSizeInBytes = Sz(512) * Sz(1024);
        Sz MaxPageSizeInBytes = Sz(256) * Sz(1024) * Sz(1024);
    };
    
    class ABYTEK_ENGINE_RHI_API F_RHIConstantDataManager : public A_RHIContextChild
    {
    private:
        Sz _MinPageSizeInBytes = 0;
        Sz _MaxPageSizeInBytes = 0;
        
    public:
        struct F_SectionData
        {
            TF_Vector<TS<F_RHIConstantDataPage>> Pages;
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
        ABYTEK_RA_DECLARE_OBJECT_CREATABLE(F_RHIConstantDataManager);
        virtual void Build(const F_RHIConstantDataManagerBuildParams& BuildParams);
        virtual void Release() override;
        
    public:
        F_RHIConstantDataRange Allocate(Sz SizeInBytes);
        void AddNewPage(Sz SizeInBytes);
    };
}