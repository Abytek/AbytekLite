#include "Abytek/DirectX12/RHIPlacedResourceManager.hpp"
#include "Abytek/DirectX12/RHIPlacedResourceManagerProxy.hpp"
#include "Abytek/DirectX12/RHIDevice.hpp"
#include "Abytek/RHISubsystem.hpp"
#include "Abytek/DirectX12/RHISubsystemProxy.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    namespace Internal::DirectX12RHIPlacedResourceManager
    {
        D3D12_HEAP_TYPE RHIResourceAccessToD3D12HeapType(const F_RHIResourceAccess& Access)
        {
            B8 HasCPUWrite = RHIHasCPUWriteAccess(Access.CPU);
            B8 HasCPUReadOnly = RHIHasCPUReadOnlyAccess(Access.CPU);
            B8 HasGPUWrite = RHIHasGPUWriteAccess(Access.GPU);
            B8 HasGPUReadOnly = RHIHasGPUReadOnlyAccess(Access.GPU);
            if (HasCPUReadOnly && HasGPUWrite)
            {
                return D3D12_HEAP_TYPE_READBACK;
            }
            if (HasCPUWrite && HasGPUReadOnly)
            {
                return D3D12_HEAP_TYPE_UPLOAD;
            }
            return D3D12_HEAP_TYPE_DEFAULT;
        }
    }
    
    ABYTEK_RA_OBJECT_DEFAULT(F_DirectX12RHIPlacedResourcePage);
    void F_DirectX12RHIPlacedResourcePage::Build(const F_DirectX12RHIPlacedResourcePageBuildParams& BuildParams)
    {
        A_RHIContextChild::Build(BuildParams);
        _Access = BuildParams.Access;
        _ResourceArchetype = BuildParams.ResourceArchetype;
        _SizeInBytes = BuildParams.SizeInBytes;
        _AlignmentInBytes = BuildParams.AlignmentInBytes;
        _Distributor.Extends(_SizeInBytes);
        GetProxy().FastCast<F_DirectX12RHIPlacedResourcePageProxy>()->LateBuildPlacedResourcePage();
    }
    void F_DirectX12RHIPlacedResourcePage::Release()
    {
        _Distributor = {};
        _AlignmentInBytes = 0;
        _SizeInBytes = 0;
        _ResourceArchetype = E_RHIResourceArchetype::NONE;
        _Access = {};
        A_RHIContextChild::Release();
    }

    TS<A_RHIContextChildProxy> F_DirectX12RHIPlacedResourcePage::CreateProxy()
    {
        return RACreateAndBuildShared<F_DirectX12RHIPlacedResourcePageProxy>(ABYTEK_WTHIS());
    }

    TF_Optional<F_DirectX12RHIResourcePlacement> F_DirectX12RHIPlacedResourcePage::Allocate(Sz ResourceSizeInBytes, Sz ResourceAlignmentInBytes)
    {
        if (auto PageOffset = _Distributor.Allocate(ResourceSizeInBytes, ResourceAlignmentInBytes))
        {
            F_DirectX12RHIResourcePlacement ResourcePlacement;
            ResourcePlacement.Page = ABYTEK_WTHIS();
            ResourcePlacement.BeginOffsetInBytes = *PageOffset;
            ResourcePlacement.EndOffsetInBytes = ResourcePlacement.BeginOffsetInBytes + ResourceSizeInBytes;
            return ResourcePlacement;
        }
        return {};
    }
    void F_DirectX12RHIPlacedResourcePage::Deallocate(const F_DirectX12RHIResourcePlacement& Placement)
    {
        _Distributor.Deallocate(Placement.BeginOffsetInBytes);
    }

    ABYTEK_RA_OBJECT_DEFAULT(F_DirectX12RHIPlacedResourceManager);
    void F_DirectX12RHIPlacedResourceManager::Build(const F_DirectX12RHIPlacedResourceManagerBuildParams& BuildParams)
    {
        A_RHIContextChild::Build(BuildParams);
        _Access = BuildParams.Access;
        _ResourceArchetype = BuildParams.ResourceArchetype;
        _MinPageSizeInBytes = BuildParams.MinPageSizeInBytes;
        _MaxPageSizeInBytes = BuildParams.MaxPageSizeInBytes;
        _AlignmentInBytes = BuildParams.AlignmentInBytes;
        _Pages = {};
        GetProxy().FastCast<F_DirectX12RHIPlacedResourceManagerProxy>()->LateBuildPlacedResourceManager();
    }
    void F_DirectX12RHIPlacedResourceManager::Release()
    {
        _Pages = {};
        _AlignmentInBytes = 0;
        _MaxPageSizeInBytes = 0;
        _MinPageSizeInBytes = 0;
        _ResourceArchetype = E_RHIResourceArchetype::NONE;
        _Access = {};
        A_RHIContextChild::Release();
    }

    TS<A_RHIContextChildProxy> F_DirectX12RHIPlacedResourceManager::CreateProxy()
    {
        return RACreateAndBuildShared<F_DirectX12RHIPlacedResourceManagerProxy>(ABYTEK_WTHIS());
    }

    F_DirectX12RHIResourcePlacement F_DirectX12RHIPlacedResourceManager::Allocate(Sz ResourceSizeInBytes, Sz ResourceAlignmentInBytes)
    {
        for (auto It = _Pages.rbegin(); It != _Pages.rend(); ++It)
        {
            if (auto Allocation = (*It)->Allocate(ResourceSizeInBytes, ResourceAlignmentInBytes))
            {
                return *Allocation;
            }
        }
        AddNewPage(ResourceSizeInBytes, ResourceAlignmentInBytes);
        return *(_Pages.back()->Allocate(ResourceSizeInBytes, ResourceAlignmentInBytes));
    }
    void F_DirectX12RHIPlacedResourceManager::AddNewPage(Sz ResourceSizeInBytes, Sz ResourceAlignmentInBytes)
    {
        Sz MinPageSizeInBytes = Max<Sz>(_MinPageSizeInBytes, ResourceSizeInBytes + ResourceAlignmentInBytes);
        if (_Pages.size() > 0)
        {
            MinPageSizeInBytes = Max<Sz>(
                MinPageSizeInBytes,
                _Pages.back()->GetSizeInBytes() * 2
            );
        }
        Sz SizeInBytes = Min<Sz>(
            RoundUpToPowerOfTwo(MinPageSizeInBytes),
            _MaxPageSizeInBytes
        );
        F_DirectX12RHIPlacedResourcePageBuildParams PageBuildParams;
        PageBuildParams.Context = GetContext();
        PageBuildParams.Access = _Access;
        PageBuildParams.ResourceArchetype = _ResourceArchetype;
        PageBuildParams.SizeInBytes = SizeInBytes;
        PageBuildParams.AlignmentInBytes = Max<Sz>(_AlignmentInBytes, ResourceAlignmentInBytes);
        _Pages.push_back(
            RACreateAndBuildShared<F_DirectX12RHIPlacedResourcePage>(PageBuildParams)  
        );
    }
}
#endif