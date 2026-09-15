#include "Abytek/DirectX12/RHIResourceView.hpp"
#include "Abytek/RHIContext.hpp"
#include "Abytek/DirectX12/RHIDevice.hpp"
#include "Abytek/DirectX12/RHIContext.hpp"
#include "Abytek/RHISubsystem.hpp"
#include "Abytek/DirectX12/RHIFormat.hpp"
#include "Abytek/DirectX12/RHISubsystemProxy.hpp"
#include "Abytek/DirectX12/RHIResourceViewProxy.hpp"
#include "Abytek/DirectX12/RHIProcess.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    void F_DirectX12RHIResourceView::Release()
    {
        D3D12DeallocateDescriptor();
        A_RHIResourceView::Release();
    }

    void F_DirectX12RHIResourceView::Build(const F_RHIBufferViewBuildParams& BuildParams)
    {
        A_RHIResourceView::Build(BuildParams);
        GetProxy().FastCast<F_DirectX12RHIResourceViewProxy>()->LateBuildBufferView_DirectX12();
        D3D12InitDescriptor();
    }
    void F_DirectX12RHIResourceView::Build(const F_RHITextureViewBuildParams& BuildParams)
    {
        A_RHIResourceView::Build(BuildParams);
        GetProxy().FastCast<F_DirectX12RHIResourceViewProxy>()->LateBuildTextureView_DirectX12();
        D3D12InitDescriptor();
    }
    void F_DirectX12RHIResourceView::Build(const F_RHIRTASViewBuildParams& BuildParams)
    {
        A_RHIResourceView::Build(BuildParams);
        GetProxy().FastCast<F_DirectX12RHIResourceViewProxy>()->LateBuildRTASView_DirectX12();
        D3D12InitDescriptor();
    }

    void F_DirectX12RHIResourceView::BuildWithoutDescriptor(const F_RHIBufferViewBuildParams& BuildParams)
    {
        A_RHIResourceView::Build(BuildParams);
        GetProxy().FastCast<F_DirectX12RHIResourceViewProxy>()->LateBuildBufferView_DirectX12();
    }
    void F_DirectX12RHIResourceView::BuildWithoutDescriptor(const F_RHITextureViewBuildParams& BuildParams)
    {
        A_RHIResourceView::Build(BuildParams);
        GetProxy().FastCast<F_DirectX12RHIResourceViewProxy>()->LateBuildTextureView_DirectX12();
    }
    void F_DirectX12RHIResourceView::BuildWithoutDescriptor(const F_RHIRTASViewBuildParams& BuildParams)
    {
        A_RHIResourceView::Build(BuildParams);
        GetProxy().FastCast<F_DirectX12RHIResourceViewProxy>()->LateBuildRTASView_DirectX12();
    }

    void F_DirectX12RHIResourceView::Build(const F_RHIResourceViewBuildParamsBase& BuildParams)
    {
        A_RHIResourceView::Build(BuildParams);
        GetProxy().FastCast<F_DirectX12RHIResourceViewProxy>()->LateBuildResourceView_DirectX12();
    }

    void F_DirectX12RHIResourceView::D3D12AllocateDescriptor()
    {
        ABYTEK_ENGINE_RHI_CONTEXT_CHILD_VALIDATE();
        
        const auto& Access = GetAccess();
        
        DirectX12SharedAPIWrapper::E_DescriptorHeapType DescriptorHeapType = DirectX12SharedAPIWrapper::Conversions::RHIResourceGPUAccessToDescriptorHeapType(
            Access.GPU
        );
        
        B8 NeedGPUDescriptorHeap = GetAllowBindless();
        if (FlagHas(GetArchetype(), E_RHIResourceViewArchetype::TEXTURE_VIEW))
        {
            NeedGPUDescriptorHeap |= GetTextureViewAspect().UAVClearable;
        }
        
        auto DescriptorManager = GetContext().FastCast<F_DirectX12RHIContext>()->QueryDescriptorManager(
            DescriptorHeapType,
            NeedGPUDescriptorHeap
        );
        _DescriptorAllocation = DescriptorManager->Allocate();
        InjectBindlessId() = _DescriptorAllocation.BeginOffset;
    }
    void F_DirectX12RHIResourceView::D3D12InitDescriptor(B8 ShouldReallocate)
    {
        ABYTEK_ENGINE_RHI_CONTEXT_CHILD_VALIDATE();
        
        if (ShouldReallocate)
        {
            if (_DescriptorAllocation)
            {
                D3D12DeallocateDescriptor();
            }
            D3D12AllocateDescriptor();
        }
        
        DirectX12RHIProcessQueries::Execution::F_InitResourceViewDescriptor Query;
        Query.ResourceViewProxy = GetProxy().FastCast<A_RHIResourceViewProxy>();
        Query.DescriptorAllocation = _DescriptorAllocation;
        GetContext()->GetCurrentProcess().FastCast<F_DirectX12RHIProcess>()->Queues.Execution.InitResourceViewDescriptor.Push(Query);
    }
    void F_DirectX12RHIResourceView::D3D12DeallocateDescriptor()
    {
        ABYTEK_ENGINE_RHI_CONTEXT_CHILD_VALIDATE();
        
        if (_DescriptorAllocation)
        {
            DirectX12RHIProcessQueries::Compile::F_DeallocateDescriptors Query;
            Query.DescriptorAllocation = _DescriptorAllocation;
            GetContext()->GetCurrentProcess().FastCast<F_DirectX12RHIProcess>()->Queues.Compile.DeallocateDescriptors.Push(Query);
        }
        _DescriptorAllocation = {};
    }

    void F_DirectX12RHIResourceView::AppendSubresourceBindingSet(F_DirectX12RHISubresourceBindingSet& Result)
    {
        auto CastedResource = GetResource().FastCast<F_DirectX12RHIResource>();
        
        const auto& Access = GetAccess();

        auto Archetype = CastedResource->GetArchetype();
        if (FlagHas(Archetype, E_RHIResourceViewArchetype::BUFFER_VIEW))
        {
            Result.push_back(
                F_DirectX12RHISubresourceBinding::MakeCore(
                    F_DirectX12RHISubresourceReference::Make(CastedResource.Weak(), 0),
                    Access
                )
            );
        }
        if (FlagHas(Archetype, E_RHIResourceViewArchetype::TEXTURE_VIEW))
        {
            const auto& TextureViewAspect = GetTextureViewAspect();
            const auto& TextureAspect = CastedResource->GetTextureAspect();
            
            const U32 MipBegin   = TextureViewAspect.FirstMipLevel;
            const U32 MipEnd     = MipBegin + TextureViewAspect.MipLevelCount;

            const U32 SliceBegin = TextureViewAspect.FirstArraySize;
            const U32 SliceEnd   = SliceBegin + TextureViewAspect.ArraySize;

            for (U32 Slice = SliceBegin; Slice < SliceEnd; ++Slice)
            {
                for (U32 Mip = MipBegin; Mip < MipEnd; ++Mip)
                {
                    const U32 SubresourceIndex = D3D12CalcSubresource(
                            Mip,
                            Slice,
                            TextureViewAspect.PlaneIndex,
                            TextureAspect.MipLevelCount,
                            TextureAspect.ArraySize);
                    
                    Result.push_back(
                        F_DirectX12RHISubresourceBinding::MakeCore(
                            F_DirectX12RHISubresourceReference::Make(CastedResource.Weak(), SubresourceIndex),
                            Access
                        )
                    );
                }
            }
        }
        if (FlagHas(Archetype, E_RHIResourceViewArchetype::RTAS_VIEW))
        {
            ABYTEK_ENGINE_RHI_ASSERT(false) << "Not supported";
        }
    }
}
#endif