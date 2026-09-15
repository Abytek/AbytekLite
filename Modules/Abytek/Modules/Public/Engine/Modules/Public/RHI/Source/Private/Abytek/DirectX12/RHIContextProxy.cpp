#include "Abytek/DirectX12/RHIContextProxy.hpp"
#include "Abytek/DirectX12/RHICommandListManager.hpp"
#include "Abytek/DirectX12/RHISynchronizer.hpp"
#include "Abytek/DirectX12/RHIContext.hpp"
#include "Abytek/DirectX12/RHIDescriptorManager.hpp"
#include "Abytek/DirectX12/RHIDescriptorManagerProxy.hpp"
#include "Abytek/DirectX12/RHICommandQueue.hpp"
#include "Abytek/DirectX12/RHICommandQueueProxy.hpp"
#include "Abytek/DirectX12/RHICommandSignature.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    void F_DirectX12RHIContextProxy::LateBuildContext_DirectX12()
    {
        auto CastedContext = GetContext().FastCast<F_DirectX12RHIContext>();
        
        {
            _DescriptorManagerProxy_GPU_CBV_SRV_UAV = CastedContext->GetDescriptorManager_GPU_CBV_SRV_UAV()->GetProxy().FastCast<F_DirectX12RHIDescriptorManagerProxy>();
            _DescriptorManagerProxy_GPU_SAMPLER = CastedContext->GetDescriptorManager_GPU_SAMPLER()->GetProxy().FastCast<F_DirectX12RHIDescriptorManagerProxy>();
            _DescriptorManagerProxy_CPU_CBV_SRV_UAV = CastedContext->GetDescriptorManager_CPU_CBV_SRV_UAV()->GetProxy().FastCast<F_DirectX12RHIDescriptorManagerProxy>();
            _DescriptorManagerProxy_CPU_RTV = CastedContext->GetDescriptorManager_CPU_RTV()->GetProxy().FastCast<F_DirectX12RHIDescriptorManagerProxy>();
            _DescriptorManagerProxy_CPU_DSV = CastedContext->GetDescriptorManager_CPU_DSV()->GetProxy().FastCast<F_DirectX12RHIDescriptorManagerProxy>();
            _DescriptorManagerProxy_CPU_SAMPLER = CastedContext->GetDescriptorManager_CPU_SAMPLER()->GetProxy().FastCast<F_DirectX12RHIDescriptorManagerProxy>();
        }
        
        {
            _CommandQueueProxy_DIRECT = CastedContext->GetCommandQueue_DIRECT()->GetProxy().FastCast<F_DirectX12RHICommandQueueProxy>();
            _CommandQueueProxy_COMPUTE = CastedContext->GetCommandQueue_COMPUTE()->GetProxy().FastCast<F_DirectX12RHICommandQueueProxy>();
            _CommandQueueProxy_COPY = CastedContext->GetCommandQueue_COPY()->GetProxy().FastCast<F_DirectX12RHICommandQueueProxy>();
        }
        
        {
            {
                F_DirectX12RHICommandListManagerBuildParams CommandListManagerBuildParams;
                CommandListManagerBuildParams.Device = GetDevice();
                CommandListManagerBuildParams.CommandListType = DirectX12SharedAPIWrapper::E_CommandListType::DIRECT;
                _CommandListManager_DIRECT = RACreateAndBuildShared<F_DirectX12RHICommandListManager>(CommandListManagerBuildParams);
            }
            {
                F_DirectX12RHICommandListManagerBuildParams CommandListManagerBuildParams;
                CommandListManagerBuildParams.Device = GetDevice();
                CommandListManagerBuildParams.CommandListType = DirectX12SharedAPIWrapper::E_CommandListType::COMPUTE;
                _CommandListManager_COMPUTE = RACreateAndBuildShared<F_DirectX12RHICommandListManager>(CommandListManagerBuildParams);
            }
            {
                F_DirectX12RHICommandListManagerBuildParams CommandListManagerBuildParams;
                CommandListManagerBuildParams.Device = GetDevice();
                CommandListManagerBuildParams.CommandListType = DirectX12SharedAPIWrapper::E_CommandListType::COPY;
                _CommandListManager_COPY = RACreateAndBuildShared<F_DirectX12RHICommandListManager>(CommandListManagerBuildParams);
            }
        }
        
        {
            {
                F_DirectX12RHISynchronizerBuildParams SynchronizerBuildParams;
                SynchronizerBuildParams.Device = GetDevice();
                _Synchronizer_DIRECT = RACreateAndBuildShared<F_DirectX12RHISynchronizer>(SynchronizerBuildParams);
            }
            {
                F_DirectX12RHISynchronizerBuildParams SynchronizerBuildParams;
                SynchronizerBuildParams.Device = GetDevice();
                _Synchronizer_COMPUTE = RACreateAndBuildShared<F_DirectX12RHISynchronizer>(SynchronizerBuildParams);
            }
            {
                F_DirectX12RHISynchronizerBuildParams SynchronizerBuildParams;
                SynchronizerBuildParams.Device = GetDevice();
                _Synchronizer_COPY = RACreateAndBuildShared<F_DirectX12RHISynchronizer>(SynchronizerBuildParams);
            }
        }
        
        {
            {
                F_DirectX12RHICommandSignatureBuildParams CommandSignatureBuildParams;
                CommandSignatureBuildParams.Device = GetDevice();
                CommandSignatureBuildParams.IndirectArgumentType = E_RHIIndirectArgumentType::DRAW_NON_INDEXED;
                _CommandSignature_DrawNonIndexed = RACreateAndBuildShared<F_DirectX12RHICommandSignature>(CommandSignatureBuildParams);
            }
            {
                F_DirectX12RHICommandSignatureBuildParams CommandSignatureBuildParams;
                CommandSignatureBuildParams.Device = GetDevice();
                CommandSignatureBuildParams.IndirectArgumentType = E_RHIIndirectArgumentType::DRAW_INDEXED;
                _CommandSignature_DrawIndexed = RACreateAndBuildShared<F_DirectX12RHICommandSignature>(CommandSignatureBuildParams);
            }
            {
                F_DirectX12RHICommandSignatureBuildParams CommandSignatureBuildParams;
                CommandSignatureBuildParams.Device = GetDevice();
                CommandSignatureBuildParams.IndirectArgumentType = E_RHIIndirectArgumentType::DISPATCH_COMPUTE;
                _CommandSignature_DispatchCompute = RACreateAndBuildShared<F_DirectX12RHICommandSignature>(CommandSignatureBuildParams);
            }
            {
                F_DirectX12RHICommandSignatureBuildParams CommandSignatureBuildParams;
                CommandSignatureBuildParams.Device = GetDevice();
                CommandSignatureBuildParams.IndirectArgumentType = E_RHIIndirectArgumentType::DISPATCH_MESH;
                _CommandSignature_DispatchMesh = RACreateAndBuildShared<F_DirectX12RHICommandSignature>(CommandSignatureBuildParams);
            }
        }
    }
    void F_DirectX12RHIContextProxy::Release()
    {
        _CommandSignature_DispatchMesh = {};
        _CommandSignature_DispatchCompute = {};
        _CommandSignature_DrawIndexed = {};
        _CommandSignature_DrawNonIndexed = {};
        
        _Synchronizer_COPY = {};
        _Synchronizer_COMPUTE = {};
        _Synchronizer_DIRECT = {};
        
        _CommandListManager_COPY = {};
        _CommandListManager_COMPUTE = {};
        _CommandListManager_DIRECT = {};
        
        _CommandQueueProxy_COPY = {};
        _CommandQueueProxy_COMPUTE = {};
        _CommandQueueProxy_DIRECT = {};
        
        _DescriptorManagerProxy_GPU_CBV_SRV_UAV = {};
        _DescriptorManagerProxy_GPU_SAMPLER = {};
        _DescriptorManagerProxy_CPU_CBV_SRV_UAV = {};
        _DescriptorManagerProxy_CPU_RTV = {};
        _DescriptorManagerProxy_CPU_DSV = {};
        _DescriptorManagerProxy_CPU_SAMPLER = {};
        A_RHIContextProxy::Release();
    }
}
#endif