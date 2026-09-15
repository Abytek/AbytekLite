#include "Abytek/RHIBindGroup.hpp"
#include "Abytek/RHIBindGroupProxy.hpp"
#include "Abytek/RHISubsystem.hpp"
#include "Abytek/RHIBindGroupTemplate.hpp"
#include "Abytek/RHIBindGroupTemplateRuntime.hpp"
#include "Abytek/RHIResource.hpp"
#include "Abytek/RHIResourceView.hpp"
#include "Abytek/RHIContext.hpp"
#include "Abytek/DirectX12/RHIBindGroup.hpp"


namespace Abytek
{
    ABYTEK_RA_BEGIN_OBJECT(A_RHIBindGroup)
        auto RHISubsystem = F_RHISubsystem::GetInstance();
        switch (RHISubsystem->GetActiveAPI())
        {
#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
        case E_RHIAPI::DIRECTX12:
            return TU<F_DirectX12RHIBindGroup>()();
#endif 
        }
    ABYTEK_RA_END_OBJECT()
    void A_RHIBindGroup::Build(const F_RHIBindGroupBuildParams& BuildParams)
    {
        A_RHIContextChild::Build(BuildParams);
            
        _TemplateRuntime = BuildParams.TemplateRuntime;
            
        auto CastedTemplate = _TemplateRuntime->GetTemplate().FastCast<A_RHIBindGroupTemplate>();
        _EncodedDataSizeInBytes = CastedTemplate->GetEncodedDataSizeInBytes();
        _EncodedDataAlignmentInBytes = CastedTemplate->GetEncodedDataAlignmentInBytes();

        const auto& SlotTemplates = _TemplateRuntime->GetTemplate().FastCast<A_RHIBindGroupTemplate>()->GetSlots();
        for (const auto& SlotTemplate : SlotTemplates)
        {
            switch (SlotTemplate.Type)
            {
            case E_RHIBindGroupSlotType::RESOURCE_VIEW:
                _SlotData.AddResourceView();
                break;
            case E_RHIBindGroupSlotType::RESOURCE_VIEW_SET:
                _SlotData.AddResourceViewSet();
                break;
                
            case E_RHIBindGroupSlotType::SAMPLER:
                _SlotData.AddSampler();
                break;
            case E_RHIBindGroupSlotType::SAMPLER_SET:
                _SlotData.AddSamplerSet();
                break;
                
            case E_RHIBindGroupSlotType::RTV:
                _SlotData.AddRTV();
                break;
            case E_RHIBindGroupSlotType::DSV:
                _SlotData.AddDSV();
                break;
                
            case E_RHIBindGroupSlotType::VERTEX_BUFFER:
                _SlotData.AddVertexBuffer();
                break;
            case E_RHIBindGroupSlotType::INDEX_BUFFER:
                _SlotData.AddIndexBuffer();
                break;
                
            case E_RHIBindGroupSlotType::UNIFORM_DATA:
                _SlotData.AddUniformData(SlotTemplate.UniformDataSizeInBytes);
                break;
            }
        }
            
        GetProxy().FastCast<A_RHIBindGroupProxy>()->LateBuildBindGroup();
    }
    void A_RHIBindGroup::Release()
    {
        _EverCommitted.clear(boost::memory_order_release);
            
        _EncodedDataSizeInBytes = ~U32(0);
        _EncodedDataAlignmentInBytes = ~U32(0);
        _TemplateRuntime = {};
            
        _SlotData = {};
            
        A_RHIContextChild::Release();
    }

    TS<A_RHIContextChildProxy> A_RHIBindGroup::CreateProxy()
    {
        return RACreateAndBuildShared<A_RHIBindGroupProxy>(ABYTEK_WTHIS());
    }

    void A_RHIBindGroup::BindResourceView(
        U32 Index,
        const TS_Valid<A_RHIResource>& Resource,
        const F_RHIResourceAccess& Access
    )
    {
        ABYTEK_ENGINE_RHI_CONTEXT_CHILD_VALIDATE();
        auto Archetype = Resource->GetArchetype();
        if (FlagHas(Archetype, E_RHIResourceArchetype::RTAS))
        {
            F_RHIRTASViewBuildParams BuildParams;
            BuildParams.Context = GetContext();
            BuildParams.Resource = Resource;
            BuildParams.Access = Access;
            BindResourceView(
                Index,
                RACreateAndBuildShared<A_RHIResourceView>(BuildParams)
            );
        }
        else if (FlagHas(Archetype, E_RHIResourceArchetype::TEXTURE))
        {
            F_RHITextureViewBuildParams BuildParams;
            BuildParams.Context = GetContext();
            BuildParams.Resource = Resource;
            BuildParams.Access = Access;
            BindResourceView(
                Index,
                RACreateAndBuildShared<A_RHIResourceView>(BuildParams)
            );
        }
        else if (FlagHas(Archetype, E_RHIResourceArchetype::BUFFER))
        {
            F_RHIBufferViewBuildParams BuildParams;
            BuildParams.Context = GetContext();
            BuildParams.Resource = Resource;
            BuildParams.Access = Access;
            BindResourceView(
                Index,
                RACreateAndBuildShared<A_RHIResourceView>(BuildParams)
            );
        }
        else
        {
            ABYTEK_ENGINE_RHI_ASSERT(false) << "Cannot bind resource, unknown archetype";
        }
    }
    void A_RHIBindGroup::BindResourceView(U32 Index, const TS_Valid<A_RHIResource>& Resource)
    {
        ABYTEK_ENGINE_RHI_CONTEXT_CHILD_VALIDATE();
        auto Template = _TemplateRuntime->GetTemplate().FastCast<A_RHIBindGroupTemplate>();
        BindResourceView(Index, Resource, Template->GetSlots()[Index].ResourceAccess);
    }
    void A_RHIBindGroup::BindResourceView(U32 Index, const TS_Valid<A_RHIResourceView>& ResourceView)
    {
        ABYTEK_ENGINE_RHI_CONTEXT_CHILD_VALIDATE();
        ABYTEK_ENGINE_RHI_ASSERT(Index < _SlotData.GetSize()) << "Slot index out of bounds";
        ABYTEK_ENGINE_RHI_ASSERT(_SlotData.GetType(Index) == E_RHIBindGroupSlotType::RESOURCE_VIEW) << "Bind slot at index: " << Index << " is not resource view";
        ABYTEK_ENGINE_RHI_ASSERT(ResourceView->GetAccess().Has(_TemplateRuntime->GetTemplate().FastCast<A_RHIBindGroupTemplate>()->GetSlots()[Index].ResourceAccess)) << "Invalid resource access";
        _SlotData.GetResourceView(Index).ResourceView = ResourceView;
    }
    void A_RHIBindGroup::BindResourceViewSet(U32 Index, const F_RHIResourceViewSet& ResourceViewSet)
    {
        ABYTEK_ENGINE_RHI_CONTEXT_CHILD_VALIDATE();
        ABYTEK_ENGINE_RHI_ASSERT(Index < _SlotData.GetSize()) << "Slot index out of bounds";
        ABYTEK_ENGINE_RHI_ASSERT(_SlotData.GetType(Index) == E_RHIBindGroupSlotType::RESOURCE_VIEW_SET) << "Bind slot at index: " << Index << " is not resource view set";
#ifdef ABYTEK_ENGINE_RHI_ENABLE_ASSERTIONS
        for (const auto& ResourceView : ResourceViewSet)
        {
            ABYTEK_ENGINE_RHI_ASSERT(ResourceView->GetAccess().Has(_TemplateRuntime->GetTemplate().FastCast<A_RHIBindGroupTemplate>()->GetSlots()[Index].ResourceAccess)) << "Invalid resource access";
        }
#endif
        _SlotData.GetResourceViewSet(Index).ResourceViewSet = ResourceViewSet;
    }
    void A_RHIBindGroup::BindSampler(U32 Index, const TS_Valid<A_RHISampler>& Sampler)
    {
        ABYTEK_ENGINE_RHI_CONTEXT_CHILD_VALIDATE();
        ABYTEK_ENGINE_RHI_ASSERT(Index < _SlotData.GetSize()) << "Slot index out of bounds";
        ABYTEK_ENGINE_RHI_ASSERT(_SlotData.GetType(Index) == E_RHIBindGroupSlotType::SAMPLER) << "Bind slot at index: " << Index << " is not sampler";
        _SlotData.GetSampler(Index).Sampler = Sampler;
    }
    void A_RHIBindGroup::BindSamplerSet(U32 Index, const F_RHISamplerSet& SamplerSet)
    {
        ABYTEK_ENGINE_RHI_CONTEXT_CHILD_VALIDATE();
        ABYTEK_ENGINE_RHI_ASSERT(Index < _SlotData.GetSize()) << "Slot index out of bounds";
        ABYTEK_ENGINE_RHI_ASSERT(_SlotData.GetType(Index) == E_RHIBindGroupSlotType::SAMPLER_SET) << "Bind slot at index: " << Index << " is not sampler set";
        _SlotData.GetSamplerSet(Index).SamplerSet = SamplerSet;
    }
    void A_RHIBindGroup::BindRTV(U32 Index, const TS_Valid<A_RHIResource>& Resource)
    {
        ABYTEK_ENGINE_RHI_CONTEXT_CHILD_VALIDATE();
        auto Archetype = Resource->GetArchetype();
        if (FlagHas(Archetype, E_RHIResourceArchetype::TEXTURE))
        {
            F_RHITextureViewBuildParams BuildParams;
            BuildParams.Context = GetContext();
            BuildParams.Resource = Resource;
            BuildParams.Access = F_RHIResourceAccess::MakeRTV();
            BindRTV(
                Index,
                RACreateAndBuildShared<A_RHIResourceView>(BuildParams)
            );
        }
        else
        {
            ABYTEK_ENGINE_RHI_ASSERT(false) << "Cannot bind resource, lacks of texture archetype";
        }
    }
    void A_RHIBindGroup::BindRTV(U32 Index, const TS_Valid<A_RHIResourceView>& RTV)
    {
        ABYTEK_ENGINE_RHI_CONTEXT_CHILD_VALIDATE();
        ABYTEK_ENGINE_RHI_ASSERT(Index < _SlotData.GetSize()) << "Slot index out of bounds";
        ABYTEK_ENGINE_RHI_ASSERT(_SlotData.GetType(Index) == E_RHIBindGroupSlotType::RTV) << "Bind slot at index: " << Index << " is not RTV";
#ifdef ABYTEK_ENGINE_RHI_ENABLE_ASSERTIONS
        ABYTEK_ENGINE_RHI_ASSERT(RTV->GetAccess().Has(F_RHIResourceAccess::MakeRTV())) << "Invalid access, requires RTV";
#endif
        _SlotData.GetRTV(Index).RTV = RTV;
    }
    void A_RHIBindGroup::BindDSV(U32 Index, const TS_Valid<A_RHIResource>& Resource)
    {
        ABYTEK_ENGINE_RHI_CONTEXT_CHILD_VALIDATE();
        auto Archetype = Resource->GetArchetype();
        if (FlagHas(Archetype, E_RHIResourceArchetype::TEXTURE))
        {
            F_RHITextureViewBuildParams BuildParams;
            BuildParams.Context = GetContext();
            BuildParams.Resource = Resource;
            BuildParams.Access = F_RHIResourceAccess::MakeDSV();
            BindDSV(
                Index,
                RACreateAndBuildShared<A_RHIResourceView>(BuildParams)
            );
        }
        else
        {
            ABYTEK_ENGINE_RHI_ASSERT(false) << "Cannot bind resource, lacks of texture archetype";
        }
    }
    void A_RHIBindGroup::BindDSV(U32 Index, const TS_Valid<A_RHIResourceView>& DSV)
    {
        ABYTEK_ENGINE_RHI_CONTEXT_CHILD_VALIDATE();
        ABYTEK_ENGINE_RHI_ASSERT(Index < _SlotData.GetSize()) << "Slot index out of bounds";
        ABYTEK_ENGINE_RHI_ASSERT(_SlotData.GetType(Index) == E_RHIBindGroupSlotType::DSV) << "Bind slot at index: " << Index << " is not DSV";
#ifdef ABYTEK_ENGINE_RHI_ENABLE_ASSERTIONS
        ABYTEK_ENGINE_RHI_ASSERT(DSV->GetAccess().Has(F_RHIResourceAccess::MakeDSV())) << "Invalid access, requires DSV";
#endif
        _SlotData.GetDSV(Index).DSV = DSV;
    }
    void A_RHIBindGroup::BindVertexBuffer(U32 Index, const TS_Valid<A_RHIResource>& Resource)
    {
        ABYTEK_ENGINE_RHI_CONTEXT_CHILD_VALIDATE();
        auto Archetype = Resource->GetArchetype();
        if (FlagHas(Archetype, E_RHIResourceArchetype::BUFFER))
        {
            const auto& BufferAspect = Resource->GetBufferAspect();
            
            F_RHIVertexBufferBinding VertexBufferBinding;
            VertexBufferBinding.Resource = Resource;
            VertexBufferBinding.OffsetInBytes = 0;
            VertexBufferBinding.StrideInBytes = BufferAspect.StrideInBytes;
            VertexBufferBinding.SizeInBytes = BufferAspect.SizeInBytes;
            BindVertexBuffer(
                Index,
                VertexBufferBinding
            );
        }
        else
        {
            ABYTEK_ENGINE_RHI_ASSERT(false) << "Cannot bind resource, lacks of vertex buffer archetype";
        }
    }
    void A_RHIBindGroup::BindVertexBuffer(U32 Index, const F_RHIVertexBufferBinding& VertexBufferBinding)
    {
        ABYTEK_ENGINE_RHI_CONTEXT_CHILD_VALIDATE();
        ABYTEK_ENGINE_RHI_ASSERT(Index < _SlotData.GetSize()) << "Slot index out of bounds";
        ABYTEK_ENGINE_RHI_ASSERT(_SlotData.GetType(Index) == E_RHIBindGroupSlotType::VERTEX_BUFFER) << "Bind slot at index: " << Index << " is not vertex buffer";
        _SlotData.GetVertexBuffer(Index).VertexBufferBinding = VertexBufferBinding;
    }
    void A_RHIBindGroup::BindIndexBuffer(U32 Index, const TS_Valid<A_RHIResource>& Resource)
    {
        ABYTEK_ENGINE_RHI_CONTEXT_CHILD_VALIDATE();
        auto Archetype = Resource->GetArchetype();
        if (FlagHas(Archetype, E_RHIResourceArchetype::BUFFER))
        {
            const auto& BufferAspect = Resource->GetBufferAspect();
            
            F_RHIIndexBufferBinding IndexBufferBinding;
            IndexBufferBinding.Resource = Resource;
            IndexBufferBinding.OffsetInBytes = 0;
            IndexBufferBinding.Format = Resource->GetFormat();
            IndexBufferBinding.SizeInBytes = BufferAspect.SizeInBytes;
            BindIndexBuffer(
                Index,
                IndexBufferBinding
            );
        }
        else
        {
            ABYTEK_ENGINE_RHI_ASSERT(false) << "Cannot bind resource, lacks of index buffer archetype";
        }
    }
    void A_RHIBindGroup::BindIndexBuffer(U32 Index, const F_RHIIndexBufferBinding& IndexBufferBinding)
    {
        ABYTEK_ENGINE_RHI_CONTEXT_CHILD_VALIDATE();
        ABYTEK_ENGINE_RHI_ASSERT(Index < _SlotData.GetSize()) << "Slot index out of bounds";
        ABYTEK_ENGINE_RHI_ASSERT(_SlotData.GetType(Index) == E_RHIBindGroupSlotType::INDEX_BUFFER) << "Bind slot at index: " << Index << " is not index buffer";
        _SlotData.GetIndexBuffer(Index).IndexBufferBinding = IndexBufferBinding;
    }
    void A_RHIBindGroup::BindUniformData(U32 Index, const F_RHIUniformDataView& UniformDataView)
    {
        ABYTEK_ENGINE_RHI_CONTEXT_CHILD_VALIDATE();
        ABYTEK_ENGINE_RHI_ASSERT(Index < _SlotData.GetSize()) << "Slot index out of bounds";
        ABYTEK_ENGINE_RHI_ASSERT(_SlotData.GetType(Index) == E_RHIBindGroupSlotType::UNIFORM_DATA) << "Bind slot at index: " << Index << " is not uniform data";
        ABYTEK_ENGINE_RHI_ASSERT(
            _TemplateRuntime->GetTemplate()
            .FastCast<A_RHIBindGroupTemplate>()->GetSlots()[Index] 
            .UniformDataSizeInBytes
            == UniformDataView.size()
        ) << "Invalid uniform data view size in bytes ("
            << "template: " << _TemplateRuntime->GetTemplate()
                .FastCast<A_RHIBindGroupTemplate>()->GetSlots()[Index] 
                .UniformDataSizeInBytes      
            << ", passed uniform data: " << UniformDataView.size()
            << ")";
        memcpy(
            _SlotData.GetUniformData(Index).GetPayloadPtr(),
            UniformDataView.data(),
            UniformDataView.size()
        );
    }

    void A_RHIBindGroup::BindResourceView(const F_Name& Name, const TS_Valid<A_RHIResource>& Resource, const F_RHIResourceAccess& Access)
    {
        BindResourceView(*_TemplateRuntime->GetTemplate().FastCast<A_RHIBindGroupTemplate>()->FindSlotIndex(Name), Resource, Access);
    }
    void A_RHIBindGroup::BindResourceView(const F_Name& Name, const TS_Valid<A_RHIResource>& Resource)
    {
        auto Template = _TemplateRuntime->GetTemplate().FastCast<A_RHIBindGroupTemplate>();
        auto SlotIndex = *Template->FindSlotIndex(Name);
        BindResourceView(SlotIndex, Resource, Template->GetSlots()[SlotIndex].ResourceAccess);
    }
    void A_RHIBindGroup::BindResourceView(const F_Name& Name, const TS_Valid<A_RHIResourceView>& ResourceView)
    {
        BindResourceView(*_TemplateRuntime->GetTemplate().FastCast<A_RHIBindGroupTemplate>()->FindSlotIndex(Name), ResourceView);
    }
    void A_RHIBindGroup::BindResourceViewSet(const F_Name& Name, const F_RHIResourceViewSet& ResourceViewSet)
    {
        BindResourceViewSet(*_TemplateRuntime->GetTemplate().FastCast<A_RHIBindGroupTemplate>()->FindSlotIndex(Name), ResourceViewSet);
    }
    void A_RHIBindGroup::BindSampler(const F_Name& Name, const TS_Valid<A_RHISampler>& Sampler)
    {
        BindSampler(*_TemplateRuntime->GetTemplate().FastCast<A_RHIBindGroupTemplate>()->FindSlotIndex(Name), Sampler);
    }
    void A_RHIBindGroup::BindSamplerSet(const F_Name& Name, const F_RHISamplerSet& SamplerSet)
    {
        BindSamplerSet(*_TemplateRuntime->GetTemplate().FastCast<A_RHIBindGroupTemplate>()->FindSlotIndex(Name), SamplerSet);
    }
    void A_RHIBindGroup::BindRTV(const F_Name& Name, const TS_Valid<A_RHIResource>& Resource)
    {
        BindRTV(*_TemplateRuntime->GetTemplate().FastCast<A_RHIBindGroupTemplate>()->FindSlotIndex(Name), Resource);
    }
    void A_RHIBindGroup::BindRTV(const F_Name& Name, const TS_Valid<A_RHIResourceView>& RTV)
    {
        BindRTV(*_TemplateRuntime->GetTemplate().FastCast<A_RHIBindGroupTemplate>()->FindSlotIndex(Name), RTV);
    }
    void A_RHIBindGroup::BindDSV(const F_Name& Name, const TS_Valid<A_RHIResource>& Resource)
    {
        BindDSV(*_TemplateRuntime->GetTemplate().FastCast<A_RHIBindGroupTemplate>()->FindSlotIndex(Name), Resource);
    }
    void A_RHIBindGroup::BindDSV(const F_Name& Name, const TS_Valid<A_RHIResourceView>& DSV)
    {
        BindDSV(*_TemplateRuntime->GetTemplate().FastCast<A_RHIBindGroupTemplate>()->FindSlotIndex(Name), DSV);
    }
    void A_RHIBindGroup::BindVertexBuffer(const F_Name& Name, const TS_Valid<A_RHIResource>& Resource)
    {
        BindVertexBuffer(*_TemplateRuntime->GetTemplate().FastCast<A_RHIBindGroupTemplate>()->FindSlotIndex(Name), Resource);
    }
    void A_RHIBindGroup::BindVertexBuffer(const F_Name& Name, const F_RHIVertexBufferBinding& VertexBufferBinding)
    {
        BindVertexBuffer(*_TemplateRuntime->GetTemplate().FastCast<A_RHIBindGroupTemplate>()->FindSlotIndex(Name), VertexBufferBinding);
    }
    void A_RHIBindGroup::BindIndexBuffer(const F_Name& Name, const TS_Valid<A_RHIResource>& Resource)
    {
        BindIndexBuffer(*_TemplateRuntime->GetTemplate().FastCast<A_RHIBindGroupTemplate>()->FindSlotIndex(Name), Resource);
    }
    void A_RHIBindGroup::BindIndexBuffer(const F_Name& Name, const F_RHIIndexBufferBinding& IndexBufferBinding)
    {
        BindIndexBuffer(*_TemplateRuntime->GetTemplate().FastCast<A_RHIBindGroupTemplate>()->FindSlotIndex(Name), IndexBufferBinding);
    }
    void A_RHIBindGroup::BindUniformData(const F_Name& Name, const F_RHIUniformDataView& UniformDataView)
    {
        BindUniformData(*_TemplateRuntime->GetTemplate().FastCast<A_RHIBindGroupTemplate>()->FindSlotIndex(Name), UniformDataView);
    }

    B8 A_RHIBindGroup::HasSlot(const F_Name& Name) const
    {
        return _TemplateRuntime->GetTemplate().FastCast<A_RHIBindGroupTemplate>()->HasSlot(Name);
    }

    void A_RHIBindGroup::Commit()
    {
        ABYTEK_ENGINE_RHI_CONTEXT_CHILD_VALIDATE();
        _EverCommitted.test_and_set(boost::memory_order_release);
    }

    void A_RHIBindGroup::EncodeData(void* OutData)
    {
        ABYTEK_ENGINE_RHI_CONTEXT_CHILD_VALIDATE();
        ABYTEK_ENGINE_RHI_ASSERT(EverCommitted()) << "Cannot encode data without committing the bind group";
    }
    F_RHIBindGroupEncodedData A_RHIBindGroup::EncodeData()
    {
        F_RHIBindGroupEncodedData Result;
        Result.resize(_EncodedDataSizeInBytes);
        EncodeData(Result.data());
        return ABYTEK_MOVE(Result);
    }

    F_RHIResourceViewBindlessRange A_RHIBindGroup::GetResourceViewBindlessRange(U32 Index) const
    {
        ABYTEK_ENGINE_RHI_CONTEXT_CHILD_VALIDATE();
        ABYTEK_ENGINE_RHI_ASSERT(EverCommitted()) << "Cannot get resource view bindless range without committing the bind group";
        ABYTEK_ENGINE_RHI_ASSERT(Index < GetSlotData().GetSize()) << "Slot index out of bounds";
        ABYTEK_ENGINE_RHI_ASSERT(
            (_SlotData.GetType(Index) == E_RHIBindGroupSlotType::RESOURCE_VIEW)
            || (_SlotData.GetType(Index) == E_RHIBindGroupSlotType::RESOURCE_VIEW_SET)
        ) << "Bind slot at index: " << Index << " is not neither resource view nor resource view set";
        return {};
    }
    F_RHISamplerBindlessRange A_RHIBindGroup::GetSamplerBindlessRange(U32 Index) const
    {
        ABYTEK_ENGINE_RHI_CONTEXT_CHILD_VALIDATE();
        ABYTEK_ENGINE_RHI_ASSERT(EverCommitted()) << "Cannot get sampler bindless range without committing the bind group";
        ABYTEK_ENGINE_RHI_ASSERT(Index < GetSlotData().GetSize()) << "Slot index out of bounds";
        ABYTEK_ENGINE_RHI_ASSERT(
            (_SlotData.GetType(Index) == E_RHIBindGroupSlotType::SAMPLER)
            || (_SlotData.GetType(Index) == E_RHIBindGroupSlotType::SAMPLER_SET)
        ) << "Bind slot at index: " << Index << " is not neither sampler nor sampler set";
        return {};
    }
    F_RHIResourceViewBindlessRange A_RHIBindGroup::GetResourceViewBindlessRange(const F_Name& Name) const
    {
        return GetResourceViewBindlessRange(*_TemplateRuntime->GetTemplate().FastCast<A_RHIBindGroupTemplate>()->FindSlotIndex(Name));
    }
    F_RHISamplerBindlessRange A_RHIBindGroup::GetSamplerBindlessRange(const F_Name& Name) const
    {
        return GetSamplerBindlessRange(*_TemplateRuntime->GetTemplate().FastCast<A_RHIBindGroupTemplate>()->FindSlotIndex(Name));
    }
}
