#pragma once

#include "Abytek/RHIContextChild.hpp"
#include "Abytek/RHIResource.hpp"
#include "Abytek/RHISubmissionUtilities.hpp"


namespace Abytek
{
    class A_RHIProcess;
    class F_RHIBufferInlineAllocator;
    class F_RHIBufferInlineAllocatorPage;
    
    struct F_RHIBufferInlineAllocationLocal
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
    
    struct F_RHIBufferInlineAllocation : F_RHIBufferInlineAllocationLocal
    {
        TW<F_RHIBufferInlineAllocatorPage> Page;
        
        TS_Valid<A_RHIResource> GetBuffer() const;
    };
    
    template<typename __F_Data>
    struct TF_RHIBufferInlineAllocation : F_RHIBufferInlineAllocation
    {
        using F_Data = __F_Data;
        
        ABYTEK_FORCE_INLINE auto GetNumInInstances() const noexcept
        {
            return GetSizeInBytes() / sizeof(__F_Data);
        }
        
        void Upload(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer, const __F_Data* Data, const F_Name& DebugName = {}) const;
        void Upload(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer, const __F_Data& Data, const F_Name& DebugName = {}) const;
        
        void Bind(const TS<A_RHIBindGroup>& BindGroup, U32 SlotIndex, const F_RHIResourceAccess& Access) const;
        void Bind(const TS<A_RHIBindGroup>& BindGroup, const F_Name& SlotName, const F_RHIResourceAccess& Access) const;
    };

    template <typename __F_Data>
    void TF_RHIBufferInlineAllocation<__F_Data>::Upload(
        const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer, 
        const __F_Data* Data,
        const F_Name& DebugName
    ) const
    {
        H_RHISubmissionUtilities::UploadBuffer(
            SubmissionItemContainer,
            SubmissionItemContainer->GetProcess()->GetArena()->CacheData(
                TF_Span<const U8>(
                    (const U8*)(Data),
                    (const U8*)(Data + GetNumInInstances())
                )
            ),
            GetBuffer(),
            BeginOffsetInBytes,
            DebugName
        );
    }
    template <typename __F_Data>
    void TF_RHIBufferInlineAllocation<__F_Data>::Upload(
        const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer, 
        const __F_Data& Data,
        const F_Name& DebugName
    ) const
    {
        for (U32 Idx = 0; Idx < GetNumInInstances(); ++Idx)
        {
            H_RHISubmissionUtilities::UploadBuffer(
                SubmissionItemContainer,
                SubmissionItemContainer->GetProcess()->GetArena()->CacheData(
                    TF_Span<const U8>(
                        (const U8*)(&Data),
                        (const U8*)(&Data + 1)
                    )
                ),
                GetBuffer(),
                BeginOffsetInBytes + Idx * sizeof(__F_Data),
                DebugName
            );
        }
    }

    struct F_RHIBufferInlineAllocatorPageBuildParams : F_RHIContextChildBuildParams
    {
        TW<F_RHIBufferInlineAllocator> Allocator;
        U32 Index = 0;
        Sz SizeInBytes = 0;
    };
    class ABYTEK_ENGINE_RHI_API F_RHIBufferInlineAllocatorPage : public A_RHIContextChild
    {
    private:
        TW<F_RHIBufferInlineAllocator> _Allocator;
        U32 _Index = 0;
        Sz _SizeInBytes = 0;
        Sz _UsageInBytes = 0;
        TS<A_RHIResource> _Buffer;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetAllocator() const noexcept
        {
            return _Allocator;
        }
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
        ABYTEK_RA_DECLARE_OBJECT_CREATABLE(F_RHIBufferInlineAllocatorPage);
        virtual void Build(const F_RHIBufferInlineAllocatorPageBuildParams& BuildParams);
        void Release() override;
        
    public:
        TF_Optional<F_RHIBufferInlineAllocation> Allocate(Sz SizeInBytes, Sz AlignmentInBytes);
        
    public:
#ifdef ABYTEK_DEBUG_INFO
        void SetDebugName(const F_DebugName& Value) noexcept override;
#endif
    };
    
    struct F_RHIBufferInlineAllocatorBuildParams : F_RHIContextChildBuildParams
    {
        Sz MinPageSizeInBytes = Sz(64) * Sz(1024);
        Sz MaxPageSizeInBytes = Sz(64) * Sz(1024) * Sz(1024);
        F_RHIResourceAccess ResourceAccessCapabilities;
        E_RHIResourceAdditionalFlag ResourceAdditionalFlags = E_RHIResourceAdditionalFlag::DEFAULT;
    };
    
    class ABYTEK_ENGINE_RHI_API F_RHIBufferInlineAllocator : public A_RHIContextChild
    {
    private:
        Sz _MinPageSizeInBytes = 0;
        Sz _MaxPageSizeInBytes = 0;
        F_RHIResourceAccess _ResourceAccessCapabilities;
        E_RHIResourceAdditionalFlag _ResourceAdditionalFlags = E_RHIResourceAdditionalFlag::NONE;
        TF_Vector<TS<F_RHIBufferInlineAllocatorPage>> _Pages;
        
    public:
        ABYTEK_FORCE_INLINE auto GetMinPageSizeInBytes() const noexcept
        {
            return _MinPageSizeInBytes;
        }
        ABYTEK_FORCE_INLINE auto GetMaxPageSizeInBytes() const noexcept
        {
            return _MaxPageSizeInBytes;
        }
        ABYTEK_FORCE_INLINE const auto& GetResourceAccessCapabilities() const noexcept
        {
            return _ResourceAccessCapabilities;
        }
        ABYTEK_FORCE_INLINE auto GetResourceAdditionalFlags() const noexcept
        {
            return _ResourceAdditionalFlags;
        }
        ABYTEK_FORCE_INLINE const auto& GetPages() const noexcept
        {
            return _Pages;
        }
        
    public:
        ABYTEK_RA_DECLARE_OBJECT_CREATABLE(F_RHIBufferInlineAllocator);
        virtual void Build(const F_RHIBufferInlineAllocatorBuildParams& BuildParams);
        void Release() override;
        
    public:
        F_RHIBufferInlineAllocation Allocate(Sz SizeInBytes, Sz AlignmentInBytes = 256);
        template<typename __F_Data>
        TF_RHIBufferInlineAllocation<__F_Data> Allocate(U32 NumInInstances = 1)
        {
            TF_RHIBufferInlineAllocation<__F_Data> Result;
            static_cast<F_RHIBufferInlineAllocation&>(Result) = Allocate(sizeof(__F_Data) * NumInInstances, ABYTEK_ALIGNOF(__F_Data));
            return Result;
        }
        void AddNewPage(Sz SizeInBytes);
        
    public:
        void Clear();
        
    public:
#ifdef ABYTEK_DEBUG_INFO
        void SetDebugName(const F_DebugName& Value) noexcept override;
#endif
    };

    template <typename __F_Data>
    void TF_RHIBufferInlineAllocation<__F_Data>::Bind(const TS<A_RHIBindGroup>& BindGroup, U32 SlotIndex, const F_RHIResourceAccess& Access) const
    {
        F_RHIBufferViewBuildParams ViewBuildParams;
        ViewBuildParams.Context = Page->GetContext();
        ViewBuildParams.BufferViewAspect.SizeInBytes = GetSizeInBytes();
        ViewBuildParams.BufferViewAspect.StrideInBytes = sizeof(__F_Data);
        ViewBuildParams.Resource = GetBuffer();
        ViewBuildParams.Access = Access;
        auto View = RACreateAndBuildShared<A_RHIResourceView>(ViewBuildParams);
        BindGroup->BindResourceView(SlotIndex, View);
    }
    template <typename __F_Data>
    void TF_RHIBufferInlineAllocation<__F_Data>::Bind(const TS<A_RHIBindGroup>& BindGroup, const F_Name& SlotName, const F_RHIResourceAccess& Access) const
    {
        F_RHIBufferViewBuildParams ViewBuildParams;
        ViewBuildParams.Context = Page->GetContext();
        ViewBuildParams.BufferViewAspect.SizeInBytes = GetSizeInBytes();
        ViewBuildParams.BufferViewAspect.StrideInBytes = sizeof(__F_Data);
        ViewBuildParams.Resource = GetBuffer();
        ViewBuildParams.Access = Access;
        auto View = RACreateAndBuildShared<A_RHIResourceView>(ViewBuildParams);
        BindGroup->BindResourceView(SlotName, View);
    }
}