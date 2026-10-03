#include "Abytek/RHIIndirectArgumentAllocator.hpp"
#include "Abytek/RHIContext.hpp"
#include "Abytek/RHIIndirectUtilities.hpp"
#include "Abytek/RHIProcess.hpp"
#include "Abytek/RHISubmissionUtilities.hpp"


namespace Abytek
{
    const TS<A_RHIResource>& F_RHIIndirectArgumentAllocation::GetUAVBuffer() const
    {
        return Page->GetUAVBuffer();
    }
    const TS<A_RHIResource>& F_RHIIndirectArgumentAllocation::GetIndirectArgumentBuffer() const
    {
        return Page->GetIndirectArgumentBuffer();
    }

    void F_RHIIndirectArgumentAllocation::Commit(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer, const F_Name& DebugName) const
    {
        H_RHISubmissionUtilities::CopyBuffer(
            SubmissionItemContainer,
            GetIndirectArgumentBuffer(),
            BeginOffsetInBytes,
            GetUAVBuffer(),
            BeginOffsetInBytes,
            GetSizeInBytes(),
            DebugName
        );
    }

    void F_RHIIndirectArgumentList::Clear(
        const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer,
        U32 IndexInArguments, 
        U32 NumInArguments, 
        const F_DebugName& DebugName
    ) const
    {
        switch (Type)
        {
        case E_RHIIndirectArgumentType::DRAW_NON_INDEXED:
            Upload_DrawNonIndexed(
                SubmissionItemContainer,
                F_RHIDrawNonIndexedConfig::Make(0),
                IndexInArguments,
                NumInArguments,
                DebugName
            );
            break;
        case E_RHIIndirectArgumentType::DRAW_INDEXED:
            Upload_DrawIndexed(
                SubmissionItemContainer,
                F_RHIDrawIndexedConfig::Make(0),
                IndexInArguments,
                NumInArguments,
                DebugName
            );
            break;
        case E_RHIIndirectArgumentType::DISPATCH_MESH:
            Upload_DispatchMesh(
                SubmissionItemContainer,
                F_Vector3_U32::Zero(),
                IndexInArguments,
                NumInArguments,
                DebugName
            );
            break;
        case E_RHIIndirectArgumentType::DISPATCH_COMPUTE:
            Upload_DispatchCompute(
                SubmissionItemContainer,
                F_Vector3_U32::Zero(),
                IndexInArguments,
                NumInArguments,
                DebugName
            );
            break;
        default:
            ABYTEK_LOG_FATAL() << "Unknown indirect argument type: " << static_cast<U32>(Type);
        }
    }
    void F_RHIIndirectArgumentList::Upload_DrawNonIndexed(
        const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer, 
        const F_RHIDrawNonIndexedConfig& Argument,
        U32 IndexInArguments,
        U32 NumInArguments,
        const F_DebugName& DebugName
    ) const
    {
        ABYTEK_ENGINE_RHI_ASSERT(StrideInBytes);
        Sz ActualNumInArguments = NumInArguments;
        if (ActualNumInArguments == 0)
        {
            ActualNumInArguments = GetNumInArguments();
        }
        auto Data = SubmissionItemContainer->GetProcess()->GetArena()->AllocateData(ActualNumInArguments * StrideInBytes);
        const auto& IndirectUtilities = A_RHIIndirectUtilities::GetInstance();
        for (U32 Idx = 0; Idx < ActualNumInArguments; ++Idx)
        {
            IndirectUtilities->WriteArgument_DrawNonIndexed(((U8*)Data) + Idx * StrideInBytes, Argument);
        }
        H_RHISubmissionUtilities::UploadBuffer(
            SubmissionItemContainer,
            TF_Span<const U8>(
                (const U8*)Data,    
                ((const U8*)Data) + ActualNumInArguments * StrideInBytes   
            ),
            GetUAVBuffer(),
            BeginOffsetInBytes + IndexInArguments * StrideInBytes,
            DebugName
        );
    }
    void F_RHIIndirectArgumentList::Upload_DrawIndexed(
        const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer,
        const F_RHIDrawIndexedConfig& Argument, 
        U32 IndexInArguments,
        U32 NumInArguments,
        const F_DebugName& DebugName
    ) const
    {
        ABYTEK_ENGINE_RHI_ASSERT(StrideInBytes);
        Sz ActualNumInArguments = NumInArguments;
        if (ActualNumInArguments == 0)
        {
            ActualNumInArguments = GetNumInArguments();
        }
        auto Data = SubmissionItemContainer->GetProcess()->GetArena()->AllocateData(ActualNumInArguments * StrideInBytes);
        const auto& IndirectUtilities = A_RHIIndirectUtilities::GetInstance();
        for (U32 Idx = 0; Idx < ActualNumInArguments; ++Idx)
        {
            IndirectUtilities->WriteArgument_DrawIndexed(((U8*)Data) + Idx * StrideInBytes, Argument);
        }
        H_RHISubmissionUtilities::UploadBuffer(
            SubmissionItemContainer,
            TF_Span<const U8>(
                (const U8*)Data,    
                ((const U8*)Data) + ActualNumInArguments * StrideInBytes   
            ),
            GetUAVBuffer(),
            BeginOffsetInBytes + IndexInArguments * StrideInBytes,
            DebugName
        );
    }
    void F_RHIIndirectArgumentList::Upload_DispatchMesh(
        const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer,
        const F_Vector3_U32& Argument, 
        U32 IndexInArguments,
        U32 NumInArguments,
        const F_DebugName& DebugName
    ) const
    {
        ABYTEK_ENGINE_RHI_ASSERT(StrideInBytes);
        Sz ActualNumInArguments = NumInArguments;
        if (ActualNumInArguments == 0)
        {
            ActualNumInArguments = GetNumInArguments();
        }
        auto Data = SubmissionItemContainer->GetProcess()->GetArena()->AllocateData(ActualNumInArguments * StrideInBytes);
        const auto& IndirectUtilities = A_RHIIndirectUtilities::GetInstance();
        for (U32 Idx = 0; Idx < ActualNumInArguments; ++Idx)
        {
            IndirectUtilities->WriteArgument_DispatchMesh(((U8*)Data) + Idx * StrideInBytes, Argument);
        }
        H_RHISubmissionUtilities::UploadBuffer(
            SubmissionItemContainer,
            TF_Span<const U8>(
                (const U8*)Data,    
                ((const U8*)Data) + ActualNumInArguments * StrideInBytes   
            ),
            GetUAVBuffer(),
            BeginOffsetInBytes + IndexInArguments * StrideInBytes,
            DebugName
        );
    }
    void F_RHIIndirectArgumentList::Upload_DispatchCompute(
        const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer, 
        const F_Vector3_U32& Argument,
        U32 IndexInArguments,
        U32 NumInArguments,
        const F_DebugName& DebugName
    ) const
    {
        ABYTEK_ENGINE_RHI_ASSERT(StrideInBytes);
        Sz ActualNumInArguments = NumInArguments;
        if (ActualNumInArguments == 0)
        {
            ActualNumInArguments = GetNumInArguments();
        }
        auto Data = SubmissionItemContainer->GetProcess()->GetArena()->AllocateData(ActualNumInArguments * StrideInBytes);
        const auto& IndirectUtilities = A_RHIIndirectUtilities::GetInstance();
        for (U32 Idx = 0; Idx < ActualNumInArguments; ++Idx)
        {
            IndirectUtilities->WriteArgument_DispatchCompute(((U8*)Data) + Idx * StrideInBytes, Argument);
        }
        H_RHISubmissionUtilities::UploadBuffer(
            SubmissionItemContainer,
            TF_Span<const U8>(
                (const U8*)Data,    
                ((const U8*)Data) + ActualNumInArguments * StrideInBytes   
            ),
            GetUAVBuffer(),
            BeginOffsetInBytes + IndexInArguments * StrideInBytes,
            DebugName
        );
    }

    F_RHIIndirectConfig F_RHIIndirectArgumentList::GetIndirectConfig(U32 IndexInArguments) const
    {
        return F_RHIIndirectConfig::Make(GetIndirectArgumentBuffer(), BeginOffsetInBytes + IndexInArguments * StrideInBytes);
    }

    void F_RHIIndirectArgumentList::BindUAV(const TS<A_RHIBindGroup>& BindGroup, U32 SlotIndex) const
    {
        F_RHIBufferViewBuildParams ViewBuildParams;
        ViewBuildParams.Context = Page->GetContext();
        ViewBuildParams.BufferViewAspect.SizeInBytes = GetSizeInBytes();
        ViewBuildParams.BufferViewAspect.StrideInBytes = StrideInBytes;
        ViewBuildParams.Resource = GetUAVBuffer();
        ViewBuildParams.Access = F_RHIResourceAccess::MakeUAV();
        auto View = RACreateAndBuildShared<A_RHIResourceView>(ViewBuildParams);
        BindGroup->BindResourceView(SlotIndex, View);
    }
    void F_RHIIndirectArgumentList::BindUAV(const TS<A_RHIBindGroup>& BindGroup, const F_Name& SlotName) const
    {
        F_RHIBufferViewBuildParams ViewBuildParams;
        ViewBuildParams.Context = Page->GetContext();
        ViewBuildParams.BufferViewAspect.SizeInBytes = GetSizeInBytes();
        ViewBuildParams.BufferViewAspect.StrideInBytes = StrideInBytes;
        ViewBuildParams.Resource = GetUAVBuffer();
        ViewBuildParams.Access = F_RHIResourceAccess::MakeUAV();
        auto View = RACreateAndBuildShared<A_RHIResourceView>(ViewBuildParams);
        BindGroup->BindResourceView(SlotName, View);
    }

    ABYTEK_RA_OBJECT_DEFAULT(F_RHIIndirectArgumentAllocatorPage);
    void F_RHIIndirectArgumentAllocatorPage::Build(const F_RHIIndirectArgumentAllocatorPageBuildParams& BuildParams)
    {
        A_RHIContextChild::Build(BuildParams);
        _Allocator = BuildParams.Allocator;
        _Index = BuildParams.Index;
        _SizeInBytes = BuildParams.SizeInBytes;
        _UsageInBytes = 0;
         
        {
            F_RHIBufferBuildParams BufferBuildParams;
            BufferBuildParams.Context = GetContext();
            BufferBuildParams.AccessCapabilities = F_RHIResourceAccess::MakeUAVCapabilities();
            BufferBuildParams.BufferAspect.SizeInBytes = _SizeInBytes;
            BufferBuildParams.AdditionalFlags = _Allocator->GetResourceAdditionalFlags();
            _UAVBuffer = RACreateAndBuildShared<A_RHIResource>(BufferBuildParams);
        }
        {
            F_RHIBufferBuildParams BufferBuildParams;
            BufferBuildParams.Context = GetContext();
            BufferBuildParams.AccessCapabilities = F_RHIResourceAccess::MakeIndirectArgumentCapabilities();
            BufferBuildParams.BufferAspect.SizeInBytes = _SizeInBytes;
            BufferBuildParams.AdditionalFlags = _Allocator->GetResourceAdditionalFlags();
            _IndirectArgumentBuffer = RACreateAndBuildShared<A_RHIResource>(BufferBuildParams);
        }
    }
    void F_RHIIndirectArgumentAllocatorPage::Release()
    {
        _IndirectArgumentBuffer = {};
        _UAVBuffer = {};
        
        _UsageInBytes = 0;
        _SizeInBytes = 0;
        _Index = 0;
        _Allocator = {};
        A_RHIContextChild::Release();
    }

    TF_Optional<F_RHIIndirectArgumentAllocation> F_RHIIndirectArgumentAllocatorPage::Allocate(Sz SizeInBytes, Sz AlignmentInBytes)
    {
        Sz BeginOffsetInBytes = AlignAddress_PO2(_UsageInBytes, AlignmentInBytes);
        if ((BeginOffsetInBytes + SizeInBytes) > _SizeInBytes)
        {
            return {};
        }
        
        F_RHIIndirectArgumentAllocation Result;
        Result.Page = ABYTEK_WTHIS();
        Result.BeginOffsetInBytes = BeginOffsetInBytes;
        Result.EndOffsetInBytes = BeginOffsetInBytes + SizeInBytes;
        _UsageInBytes = BeginOffsetInBytes + SizeInBytes;
        return Result;
    }
    TF_Optional<F_RHIIndirectArgumentList> F_RHIIndirectArgumentAllocatorPage::Allocate(E_RHIIndirectArgumentType Type)
    {
        auto StrideInBytes = A_RHIIndirectUtilities::GetInstance()->GetArgumentStride(Type);
        if (auto Allocation = Allocate(StrideInBytes, StrideInBytes))
        {
            F_RHIIndirectArgumentList Result;
            static_cast<F_RHIIndirectArgumentAllocation&>(Result) = static_cast<const F_RHIIndirectArgumentAllocation&>(*Allocation);
            Result.StrideInBytes = StrideInBytes;
            Result.Type = Type;
        }
        return {};
    }

#ifdef ABYTEK_DEBUG_INFO
    void F_RHIIndirectArgumentAllocatorPage::SetDebugName(const F_DebugName& Value) noexcept
    {
        A_RHIContextChild::SetDebugName(Value);
        _UAVBuffer->SetDebugName(*Value + ABYTEK_TEXT(".UAVBuffer"));
        _IndirectArgumentBuffer->SetDebugName(*Value + ABYTEK_TEXT(".IndirectArgumentBuffer"));
    }
#endif

    ABYTEK_RA_OBJECT_DEFAULT(F_RHIIndirectArgumentAllocator);
    void F_RHIIndirectArgumentAllocator::Build(const F_RHIIndirectArgumentAllocatorBuildParams& BuildParams)
    {
        A_RHIContextChild::Build(BuildParams);
        _MinPageSizeInBytes = BuildParams.MinPageSizeInBytes;
        _MaxPageSizeInBytes = BuildParams.MaxPageSizeInBytes;
        _ResourceAdditionalFlags = BuildParams.ResourceAdditionalFlags;
    }
    void F_RHIIndirectArgumentAllocator::Release()
    {
        _Pages = {};
        _ResourceAdditionalFlags = E_RHIResourceAdditionalFlag::NONE;
        _MaxPageSizeInBytes = 0;
        _MinPageSizeInBytes = 0;
        A_RHIContextChild::Release();
    }

    F_RHIIndirectArgumentAllocation F_RHIIndirectArgumentAllocator::Allocate(Sz SizeInBytes, Sz AlignmentInBytes)
    {
        if (_Pages.size() > 0)
        {
            if (auto Allocation = _Pages.back()->Allocate(SizeInBytes, AlignmentInBytes))
            {
                return *Allocation;
            }
        }
        AddNewPage(SizeInBytes + AlignmentInBytes - 1);
        return *(_Pages.back()->Allocate(SizeInBytes, AlignmentInBytes));
    }
    void F_RHIIndirectArgumentAllocator::AddNewPage(Sz SizeInBytes)
    {
        Sz MinPageSizeInBytes = Max<Sz>(_MinPageSizeInBytes, SizeInBytes);
        if (!_Pages.empty())
        {
            MinPageSizeInBytes = Max<Sz>(
                MinPageSizeInBytes,
                _Pages.back()->GetSizeInBytes() * 2
            );
        }
        Sz ActualSizeInBytes = Min<Sz>(
            RoundUpToPowerOfTwo(MinPageSizeInBytes),
            _MaxPageSizeInBytes
        );
        ABYTEK_ENGINE_RHI_ASSERT(ActualSizeInBytes >= SizeInBytes);
        F_RHIIndirectArgumentAllocatorPageBuildParams PageBuildParams;
        PageBuildParams.Context = GetContext();
        PageBuildParams.Allocator = ABYTEK_WTHIS();
        PageBuildParams.Index = static_cast<U32>(_Pages.size());
        PageBuildParams.SizeInBytes = ActualSizeInBytes;
        auto Page = RACreateAndBuildShared<F_RHIIndirectArgumentAllocatorPage>(PageBuildParams);
#ifdef ABYTEK_DEBUG_INFO
        Page->SetDebugName(
            *GetDebugName()
            + ABYTEK_TEXT(".Pages[") + ToText(PageBuildParams.Index) + ABYTEK_TEXT("]")
        );
#endif
        _Pages.push_back(Page);
    }

    void F_RHIIndirectArgumentAllocator::Clear()
    {
        _Pages = {};
    }

#ifdef ABYTEK_DEBUG_INFO
    void F_RHIIndirectArgumentAllocator::SetDebugName(const F_DebugName& Value) noexcept
    {
        A_RHIContextChild::SetDebugName(Value);
        for (const auto& Page : _Pages)
        {
            Page->SetDebugName(
                *GetDebugName()
                + ABYTEK_TEXT(".Pages[") + ToText(Page->GetIndex()) + ABYTEK_TEXT("]")
            );
        }
    }
#endif
}
