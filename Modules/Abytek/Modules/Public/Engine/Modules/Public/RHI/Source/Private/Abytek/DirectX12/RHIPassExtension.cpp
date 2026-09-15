#include "Abytek/DirectX12/RHIPassExtension.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    A_DirectX12RHIPassExtension::A_DirectX12RHIPassExtension()
    {
    }
    A_DirectX12RHIPassExtension::~A_DirectX12RHIPassExtension()
    {
    }
    
    void A_DirectX12RHIPassExtension::Build()
    {
        A_DirectX12RHISubmissionItemExtension::Build();
    }
    void A_DirectX12RHIPassExtension::Release()
    {
        A_DirectX12RHISubmissionItemExtension::Release();
    }

    void A_DirectX12RHIPassExtension::AppendSubresourceBindings(F_DirectX12RHISubresourceBindingSet& SubresourceBindingSet)
    {
    }

    void A_DirectX12RHIPassExtension::GatherSubresourceBindings(F_DirectX12RHISubresourceBindingSet& SubresourceBindingSet)
    {
        U32 Offset = SubresourceBindingSet.size();
        
        F_DirectX12RHISubresourceBindingSet TempSubresourceBindingSet;
        AppendSubresourceBindings(TempSubresourceBindingSet);
        
        U32 NumNewSubresourceBindings = TempSubresourceBindingSet.size();
        
        TF_SmallVector<B8, 32> CanAdds(NumNewSubresourceBindings);
        memset(CanAdds.data(), 1, NumNewSubresourceBindings * sizeof(B8));
        
        for (U32 Idx = 0; Idx < NumNewSubresourceBindings; ++Idx)
        {
            if (!CanAdds[Idx])
            {
                continue;
            }
            
            auto& SubresourceBinding = TempSubresourceBindingSet[Idx];
            
            auto ResourceAdditionalFlags = SubresourceBinding.SubresourceReference.Resource->GetAdditionalFlags();
            if (FlagHas(ResourceAdditionalFlags, E_RHIResourceAdditionalFlag::STATIC))
            {
                ABYTEK_ENGINE_RHI_ASSERT(
                    SubresourceBinding.SubresourceReference.Resource->GetStaticAccess()
                    ->Has(SubresourceBinding.Access)
                ) << "Invalid resource access to use as static, write access in static-mode requires E_RHIResourceAdditionalFlag::STATIC_WITH_WRITE_ACCESS";
#ifdef ABYTEK_ENGINE_RHI_ENABLE_ASSERTIONS
                auto CompileData_Resource = SubresourceBinding.SubresourceReference.Resource.FastCast<F_DirectX12RHIResource>()->GetCompileData();
                CompileData_Resource->IsUsedAsStatic.test_and_set(boost::memory_order_release);
#endif
                continue;
            }
            
            for (U32 J = Idx + 1; J < NumNewSubresourceBindings; ++J)
            {
                const auto& SubresourceBinding_J = TempSubresourceBindingSet[J];
                if (SubresourceBinding_J.SubresourceReference == SubresourceBinding.SubresourceReference)
                {
                    ABYTEK_ENGINE_RHI_ASSERT(
                        RHIParseResourceAccessCategory(SubresourceBinding.Access)  
                        == RHIParseResourceAccessCategory(SubresourceBinding_J.Access)
                    ) << "Subresource binding access conflict";
                    SubresourceBinding.Access |= SubresourceBinding_J.Access;
                    CanAdds[J] = false;
                }
            }
            
            SubresourceBindingSet.push_back(ABYTEK_MOVE(SubresourceBinding));
        }
        
        U32 NumSubresourceBindings = SubresourceBindingSet.size();
        for (U32 Idx = Offset; Idx < NumSubresourceBindings; ++Idx)
        {
            auto& SubresourceBinding = SubresourceBindingSet[Idx];
            SubresourceBinding.AccessCategories = RHIParseResourceAccessCategory(SubresourceBinding.Access);
            SubresourceBinding.BaseD3D12State = Internal::DirectX12RHIResource::RHIResourceAccessToD3D12ResourceStates_Detailed(
                SubresourceBinding.Access
            );
        }
    }

    E_DirectX12RHIPassBatchType A_DirectX12RHIPassExtension::GetPassBatchType()
    {
        return E_DirectX12RHIPassBatchType::DEFAULT;
    }
    E_DirectX12RHIExecutionRangeType A_DirectX12RHIPassExtension::GetExecutionRangeType()
    {
        switch (GetPassBatchType())
        {
        case E_DirectX12RHIPassBatchType::GPU:
            return E_DirectX12RHIExecutionRangeType::USE_COMMAND_LIST;
            break;
        case E_DirectX12RHIPassBatchType::CPU_SYNC:
            return E_DirectX12RHIExecutionRangeType::CPU_ACCESS;
            break;
        default:
            ABYTEK_ENGINE_RHI_ASSERT(false) << "Invalid pass batch type";
            break;
        }
        return E_DirectX12RHIExecutionRangeType::NONE;
    }
}
#endif