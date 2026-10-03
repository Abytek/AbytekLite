#pragma once

#include "RHIDrawPass.hpp"
#include "Abytek/RHIContextChild.hpp"
#include "Abytek/RHIResource.hpp"
#include "Abytek/RHIIndirectConfig.hpp"


namespace Abytek
{
    class A_RHISubmissionItemContainer;
    class A_RHIProcess;
    class F_RHIIndirectArgumentAllocator;
    class F_RHIIndirectArgumentAllocatorPage;
    
    struct F_RHIIndirectArgumentAllocationLocal
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
    
    struct F_RHIIndirectArgumentAllocation : F_RHIIndirectArgumentAllocationLocal
    {
        TW<F_RHIIndirectArgumentAllocatorPage> Page;
        
        const TS<A_RHIResource>& GetUAVBuffer() const;
        const TS<A_RHIResource>& GetIndirectArgumentBuffer() const;
        
        void Commit(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer, const F_Name& DebugName = {}) const;
    };
    
    struct F_RHIIndirectArgumentList : F_RHIIndirectArgumentAllocation
    {
        Sz StrideInBytes = 0;
        E_RHIIndirectArgumentType Type = E_RHIIndirectArgumentType::NONE;
        
        ABYTEK_FORCE_INLINE auto GetNumInArguments() const noexcept
        {
            return GetSizeInBytes() / StrideInBytes;
        }
        ABYTEK_FORCE_INLINE auto GetList(Sz OffsetInArguments, Sz NumInArguments = 1) const noexcept
        {
            ABYTEK_ENGINE_RHI_ASSERT((OffsetInArguments + NumInArguments) <= GetNumInArguments());
            F_RHIIndirectArgumentList Result = *this;
            Result.BeginOffsetInBytes += OffsetInArguments * StrideInBytes;
            Result.EndOffsetInBytes = Result.BeginOffsetInBytes + NumInArguments * StrideInBytes;
            return Result;
        }
        
        void Clear(
            const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer, 
            U32 IndexInArguments = 0,
            U32 NumInArguments = 0,
            const F_DebugName& DebugName = {}
        ) const;
        void Upload_DrawNonIndexed(
            const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer, 
            const F_RHIDrawNonIndexedConfig& Argument,
            U32 IndexInArguments = 0,
            U32 NumInArguments = 0,
            const F_DebugName& DebugName = {}
        ) const;
        void Upload_DrawIndexed(
            const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer, 
            const F_RHIDrawIndexedConfig& Argument,
            U32 IndexInArguments = 0,
            U32 NumInArguments = 0,
            const F_DebugName& DebugName = {}
        ) const;
        void Upload_DispatchMesh(
            const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer, 
            const F_Vector3_U32& Argument,
            U32 IndexInArguments = 0,
            U32 NumInArguments = 0,
            const F_DebugName& DebugName = {}
        ) const;
        void Upload_DispatchCompute(
            const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer, 
            const F_Vector3_U32& Argument,
            U32 IndexInArguments = 0,
            U32 NumInArguments = 0,
            const F_DebugName& DebugName = {}
        ) const;
        
        F_RHIIndirectConfig GetIndirectConfig(U32 IndexInArguments = 0) const;
        
        void BindUAV(const TS<A_RHIBindGroup>& BindGroup, U32 SlotIndex) const;
        void BindUAV(const TS<A_RHIBindGroup>& BindGroup, const F_Name& SlotName) const;
    };
    
    struct F_RHIIndirectArgumentAllocatorPageBuildParams : F_RHIContextChildBuildParams
    {
        TW<F_RHIIndirectArgumentAllocator> Allocator;
        U32 Index = 0;
        Sz SizeInBytes = 0;
    };
    class ABYTEK_ENGINE_RHI_API F_RHIIndirectArgumentAllocatorPage : public A_RHIContextChild
    {
    private:
        TW<F_RHIIndirectArgumentAllocator> _Allocator;
        U32 _Index = 0;
        Sz _SizeInBytes = 0;
        Sz _UsageInBytes = 0;
        TS<A_RHIResource> _UAVBuffer;
        TS<A_RHIResource> _IndirectArgumentBuffer;
        
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
        ABYTEK_FORCE_INLINE const auto& GetUAVBuffer() const noexcept
        {
            return _UAVBuffer;
        }
        ABYTEK_FORCE_INLINE const auto& GetIndirectArgumentBuffer() const noexcept
        {
            return _IndirectArgumentBuffer;
        }
        
    public:
        ABYTEK_RA_DECLARE_OBJECT_CREATABLE(F_RHIIndirectArgumentAllocatorPage);
        virtual void Build(const F_RHIIndirectArgumentAllocatorPageBuildParams& BuildParams);
        void Release() override;
        
    public:
        TF_Optional<F_RHIIndirectArgumentAllocation> Allocate(Sz SizeInBytes, Sz AlignmentInBytes);
        TF_Optional<F_RHIIndirectArgumentList> Allocate(E_RHIIndirectArgumentType Type);
        
    public:
#ifdef ABYTEK_DEBUG_INFO
        void SetDebugName(const F_DebugName& Value) noexcept override;
#endif
    };
    
    struct F_RHIIndirectArgumentAllocatorBuildParams : F_RHIContextChildBuildParams
    {
        Sz MinPageSizeInBytes = Sz(64) * Sz(1024);
        Sz MaxPageSizeInBytes = Sz(64) * Sz(1024);
        E_RHIResourceAdditionalFlag ResourceAdditionalFlags = E_RHIResourceAdditionalFlag::DEFAULT;
    };
    
    class ABYTEK_ENGINE_RHI_API F_RHIIndirectArgumentAllocator : public A_RHIContextChild
    {
    private:
        Sz _MinPageSizeInBytes = 0;
        Sz _MaxPageSizeInBytes = 0;
        E_RHIResourceAdditionalFlag _ResourceAdditionalFlags = E_RHIResourceAdditionalFlag::NONE;
        TF_Vector<TS<F_RHIIndirectArgumentAllocatorPage>> _Pages;
        
    public:
        ABYTEK_FORCE_INLINE auto GetMinPageSizeInBytes() const noexcept
        {
            return _MinPageSizeInBytes;
        }
        ABYTEK_FORCE_INLINE auto GetMaxPageSizeInBytes() const noexcept
        {
            return _MaxPageSizeInBytes;
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
        ABYTEK_RA_DECLARE_OBJECT_CREATABLE(F_RHIIndirectArgumentAllocator);
        virtual void Build(const F_RHIIndirectArgumentAllocatorBuildParams& BuildParams);
        void Release() override;
        
    public:
        F_RHIIndirectArgumentAllocation Allocate(Sz SizeInBytes, Sz AlignmentInBytes = 256);
        void AddNewPage(Sz SizeInBytes);
        
    public:
        void Clear();
        
    public:
#ifdef ABYTEK_DEBUG_INFO
        void SetDebugName(const F_DebugName& Value) noexcept override;
#endif
    };
}