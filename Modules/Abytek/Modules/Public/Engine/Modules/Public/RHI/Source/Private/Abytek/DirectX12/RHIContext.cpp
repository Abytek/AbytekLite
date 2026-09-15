#include "Abytek/DirectX12/RHIContext.hpp"
#include "Abytek/DirectX12/RHIContextProxy.hpp"
#include "Abytek/DirectX12/RHICommandQueue.hpp"
#include "Abytek/DirectX12/RHIDescriptorManager.hpp"
#include "Abytek/DirectX12/RHIPlacedResourceManager.hpp"
#include "Abytek/RHITransientUploadBufferManager.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    void F_DirectX12RHIContext::FirstCompile()
    {
        A_RHIContext::FirstCompile();
        
        {
            F_DirectX12RHIDescriptorManagerBuildParams DescriptorManagerBuildParams;
            DescriptorManagerBuildParams.Context = ABYTEK_WTHIS();
            DescriptorManagerBuildParams.DescriptorHeapType = DirectX12SharedAPIWrapper::E_DescriptorHeapType::CBV_SRV_UAV;
            DescriptorManagerBuildParams.IsShaderVisible = true;
            _DescriptorManager_GPU_CBV_SRV_UAV = RACreateAndBuildShared<F_DirectX12RHIDescriptorManager>(DescriptorManagerBuildParams);
        }
        {
            F_DirectX12RHIDescriptorManagerBuildParams DescriptorManagerBuildParams;
            DescriptorManagerBuildParams.Context = ABYTEK_WTHIS();
            DescriptorManagerBuildParams.DescriptorHeapType = DirectX12SharedAPIWrapper::E_DescriptorHeapType::SAMPLER;
            DescriptorManagerBuildParams.IsShaderVisible = true;
            _DescriptorManager_GPU_SAMPLER = RACreateAndBuildShared<F_DirectX12RHIDescriptorManager>(DescriptorManagerBuildParams);
        }
        {
            F_DirectX12RHIDescriptorManagerBuildParams DescriptorManagerBuildParams;
            DescriptorManagerBuildParams.Context = ABYTEK_WTHIS();
            DescriptorManagerBuildParams.DescriptorHeapType = DirectX12SharedAPIWrapper::E_DescriptorHeapType::CBV_SRV_UAV;
            DescriptorManagerBuildParams.IsShaderVisible = false;
            _DescriptorManager_CPU_CBV_SRV_UAV = RACreateAndBuildShared<F_DirectX12RHIDescriptorManager>(DescriptorManagerBuildParams);
        }
        {
            F_DirectX12RHIDescriptorManagerBuildParams DescriptorManagerBuildParams;
            DescriptorManagerBuildParams.Context = ABYTEK_WTHIS();
            DescriptorManagerBuildParams.DescriptorHeapType = DirectX12SharedAPIWrapper::E_DescriptorHeapType::RTV;
            DescriptorManagerBuildParams.IsShaderVisible = false;
            _DescriptorManager_CPU_RTV = RACreateAndBuildShared<F_DirectX12RHIDescriptorManager>(DescriptorManagerBuildParams);
        }
        {
            F_DirectX12RHIDescriptorManagerBuildParams DescriptorManagerBuildParams;
            DescriptorManagerBuildParams.Context = ABYTEK_WTHIS();
            DescriptorManagerBuildParams.DescriptorHeapType = DirectX12SharedAPIWrapper::E_DescriptorHeapType::DSV;
            DescriptorManagerBuildParams.IsShaderVisible = false;
            _DescriptorManager_CPU_DSV = RACreateAndBuildShared<F_DirectX12RHIDescriptorManager>(DescriptorManagerBuildParams);
        }
        {
            F_DirectX12RHIDescriptorManagerBuildParams DescriptorManagerBuildParams;
            DescriptorManagerBuildParams.Context = ABYTEK_WTHIS();
            DescriptorManagerBuildParams.DescriptorHeapType = DirectX12SharedAPIWrapper::E_DescriptorHeapType::SAMPLER;
            DescriptorManagerBuildParams.IsShaderVisible = false;
            _DescriptorManager_CPU_SAMPLER = RACreateAndBuildShared<F_DirectX12RHIDescriptorManager>(DescriptorManagerBuildParams);
        }
        
        {
            F_DirectX12RHICommandQueueBuildParams CommandQueueBuildParams;
            CommandQueueBuildParams.Context = ABYTEK_WTHIS();
            CommandQueueBuildParams.CommandListType = DirectX12SharedAPIWrapper::E_CommandListType::DIRECT;
            CommandQueueBuildParams.Flags = DirectX12SharedAPIWrapper::E_CommandQueueFlag::DEFAULT;
            _CommandQueue_DIRECT = RACreateAndBuildShared<F_DirectX12RHICommandQueue>(CommandQueueBuildParams);
        }
        {
            F_DirectX12RHICommandQueueBuildParams CommandQueueBuildParams;
            CommandQueueBuildParams.Context = ABYTEK_WTHIS();
            CommandQueueBuildParams.CommandListType = DirectX12SharedAPIWrapper::E_CommandListType::COMPUTE;
            CommandQueueBuildParams.Flags = DirectX12SharedAPIWrapper::E_CommandQueueFlag::DEFAULT;
            _CommandQueue_COMPUTE = RACreateAndBuildShared<F_DirectX12RHICommandQueue>(CommandQueueBuildParams);
        }
        {
            F_DirectX12RHICommandQueueBuildParams CommandQueueBuildParams;
            CommandQueueBuildParams.Context = ABYTEK_WTHIS();
            CommandQueueBuildParams.CommandListType = DirectX12SharedAPIWrapper::E_CommandListType::COPY;
            CommandQueueBuildParams.Flags = DirectX12SharedAPIWrapper::E_CommandQueueFlag::DEFAULT;
            _CommandQueue_COPY = RACreateAndBuildShared<F_DirectX12RHICommandQueue>(CommandQueueBuildParams);
        }
        {
            F_DirectX12RHIPlacedResourceManagerBuildParams PlacedResourceManagerBuildParams;
            PlacedResourceManagerBuildParams.Context = ABYTEK_WTHIS();
            PlacedResourceManagerBuildParams.AlignmentInBytes = D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT;
            PlacedResourceManagerBuildParams.Access = F_RHIResourceAccess::MakeDefaultCapabilities();
            PlacedResourceManagerBuildParams.ResourceArchetype = E_RHIResourceArchetype::BUFFER;
            _PlacedResourceManager_AllowOnlyBuffers_Default = RACreateAndBuildShared<F_DirectX12RHIPlacedResourceManager>(PlacedResourceManagerBuildParams);
        }
        {
            F_DirectX12RHIPlacedResourceManagerBuildParams PlacedResourceManagerBuildParams;
            PlacedResourceManagerBuildParams.Context = ABYTEK_WTHIS();
            PlacedResourceManagerBuildParams.AlignmentInBytes = D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT;
            PlacedResourceManagerBuildParams.Access = F_RHIResourceAccess::MakeDefaultCapabilities();
            PlacedResourceManagerBuildParams.ResourceArchetype = E_RHIResourceArchetype::TEXTURE;
            _PlacedResourceManager_AllowOnlyNonRTDSTextures_Default = RACreateAndBuildShared<F_DirectX12RHIPlacedResourceManager>(PlacedResourceManagerBuildParams);
        }
        {
            F_DirectX12RHIPlacedResourceManagerBuildParams PlacedResourceManagerBuildParams;
            PlacedResourceManagerBuildParams.Context = ABYTEK_WTHIS();
            PlacedResourceManagerBuildParams.AlignmentInBytes = D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT;
            PlacedResourceManagerBuildParams.Access = (
                F_RHIResourceAccess::MakeDefaultCapabilities() 
                | F_RHIResourceAccess::MakeRTVCapabilities()
                | F_RHIResourceAccess::MakeDSVCapabilities()
            );
            PlacedResourceManagerBuildParams.ResourceArchetype = E_RHIResourceArchetype::TEXTURE;
            _PlacedResourceManager_AllowOnlyRTDSTextures_Default = RACreateAndBuildShared<F_DirectX12RHIPlacedResourceManager>(PlacedResourceManagerBuildParams);
        }
        {
            F_DirectX12RHIPlacedResourceManagerBuildParams PlacedResourceManagerBuildParams;
            PlacedResourceManagerBuildParams.Context = ABYTEK_WTHIS();
            PlacedResourceManagerBuildParams.AlignmentInBytes = D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT;
            PlacedResourceManagerBuildParams.Access = F_RHIResourceAccess::MakeReadbackCapabilities();
            PlacedResourceManagerBuildParams.ResourceArchetype = E_RHIResourceArchetype::BUFFER;
            _PlacedResourceManager_AllowOnlyBuffers_Readback = RACreateAndBuildShared<F_DirectX12RHIPlacedResourceManager>(PlacedResourceManagerBuildParams);
        }
        {
            F_DirectX12RHIPlacedResourceManagerBuildParams PlacedResourceManagerBuildParams;
            PlacedResourceManagerBuildParams.Context = ABYTEK_WTHIS();
            PlacedResourceManagerBuildParams.AlignmentInBytes = D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT;
            PlacedResourceManagerBuildParams.Access = F_RHIResourceAccess::MakeReadbackCapabilities();
            PlacedResourceManagerBuildParams.ResourceArchetype = E_RHIResourceArchetype::TEXTURE;
            _PlacedResourceManager_AllowOnlyNonRTDSTextures_Readback = RACreateAndBuildShared<F_DirectX12RHIPlacedResourceManager>(PlacedResourceManagerBuildParams);
        }
        {
            F_DirectX12RHIPlacedResourceManagerBuildParams PlacedResourceManagerBuildParams;
            PlacedResourceManagerBuildParams.Context = ABYTEK_WTHIS();
            PlacedResourceManagerBuildParams.AlignmentInBytes = D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT;
            PlacedResourceManagerBuildParams.Access = (
                F_RHIResourceAccess::MakeReadbackCapabilities() 
                | F_RHIResourceAccess::MakeRTVCapabilities()
                | F_RHIResourceAccess::MakeDSVCapabilities()
            );
            PlacedResourceManagerBuildParams.ResourceArchetype = E_RHIResourceArchetype::TEXTURE;
            _PlacedResourceManager_AllowOnlyRTDSTextures_Readback = RACreateAndBuildShared<F_DirectX12RHIPlacedResourceManager>(PlacedResourceManagerBuildParams);
        }
        {
            F_DirectX12RHIPlacedResourceManagerBuildParams PlacedResourceManagerBuildParams;
            PlacedResourceManagerBuildParams.Context = ABYTEK_WTHIS();
            PlacedResourceManagerBuildParams.AlignmentInBytes = D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT;
            PlacedResourceManagerBuildParams.Access = F_RHIResourceAccess::MakeUploadCapabilities();
            PlacedResourceManagerBuildParams.ResourceArchetype = E_RHIResourceArchetype::BUFFER;
            _PlacedResourceManager_AllowOnlyBuffers_Upload = RACreateAndBuildShared<F_DirectX12RHIPlacedResourceManager>(PlacedResourceManagerBuildParams);
        }
        {
            F_DirectX12RHIPlacedResourceManagerBuildParams PlacedResourceManagerBuildParams;
            PlacedResourceManagerBuildParams.Context = ABYTEK_WTHIS();
            PlacedResourceManagerBuildParams.AlignmentInBytes = D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT;
            PlacedResourceManagerBuildParams.Access = F_RHIResourceAccess::MakeUploadCapabilities();
            PlacedResourceManagerBuildParams.ResourceArchetype = E_RHIResourceArchetype::TEXTURE;
            _PlacedResourceManager_AllowOnlyNonRTDSTextures_Upload = RACreateAndBuildShared<F_DirectX12RHIPlacedResourceManager>(PlacedResourceManagerBuildParams);
        }
        {
            F_DirectX12RHIPlacedResourceManagerBuildParams PlacedResourceManagerBuildParams;
            PlacedResourceManagerBuildParams.Context = ABYTEK_WTHIS();
            PlacedResourceManagerBuildParams.AlignmentInBytes = D3D12_DEFAULT_RESOURCE_PLACEMENT_ALIGNMENT;
            PlacedResourceManagerBuildParams.Access = (
                F_RHIResourceAccess::MakeUploadCapabilities() 
                | F_RHIResourceAccess::MakeRTVCapabilities()
                | F_RHIResourceAccess::MakeDSVCapabilities()
            );
            PlacedResourceManagerBuildParams.ResourceArchetype = E_RHIResourceArchetype::TEXTURE;
            _PlacedResourceManager_AllowOnlyRTDSTextures_Upload = RACreateAndBuildShared<F_DirectX12RHIPlacedResourceManager>(PlacedResourceManagerBuildParams);
        }
        
        GetProxy().FastCast<F_DirectX12RHIContextProxy>()->LateBuildContext_DirectX12();
    }
    void F_DirectX12RHIContext::FinalizeRelease()
    {
        ABYTEK_ENGINE_RHI_ASSERT(Queues.ResourcePlacementsToDeallocate.GetSize() == 0) << "ResourcePlacementsToDeallocate was not flushed";
        ABYTEK_ENGINE_RHI_ASSERT(Queues.DeallocateDescriptors.GetSize() == 0) << "DeallocateDescriptors was not flushed";
        
        _PlacedResourceManager_AllowOnlyRTDSTextures_Upload = {};
        _PlacedResourceManager_AllowOnlyNonRTDSTextures_Upload = {};
        _PlacedResourceManager_AllowOnlyBuffers_Upload = {};
        
        _PlacedResourceManager_AllowOnlyRTDSTextures_Readback = {};
        _PlacedResourceManager_AllowOnlyNonRTDSTextures_Readback = {};
        _PlacedResourceManager_AllowOnlyBuffers_Readback = {};
        
        _PlacedResourceManager_AllowOnlyRTDSTextures_Default = {};
        _PlacedResourceManager_AllowOnlyNonRTDSTextures_Default = {};
        _PlacedResourceManager_AllowOnlyBuffers_Default = {};
        
        _DescriptorManager_GPU_CBV_SRV_UAV = {};
        _DescriptorManager_GPU_SAMPLER = {};
        _DescriptorManager_CPU_CBV_SRV_UAV = {};
        _DescriptorManager_CPU_RTV = {};
        _DescriptorManager_CPU_DSV = {};
        _DescriptorManager_CPU_SAMPLER = {};
        
        _CommandQueue_COPY = {};
        _CommandQueue_COMPUTE = {};
        _CommandQueue_DIRECT = {};
        A_RHIContext::FinalizeRelease();
    }

    void F_DirectX12RHIContext::HighLevelInitialize()
    {
        A_RHIContext::HighLevelInitialize();
    }
    void F_DirectX12RHIContext::HighLevelDeinitialize()
    {
        A_RHIContext::HighLevelDeinitialize();
    }
}
#endif