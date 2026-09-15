#pragma once

#include "Abytek/RHIContextProxy.hpp"
#include "Abytek/RHIIndirectConfig.hpp"
#include "Abytek/DirectX12Shared/APIWrapper.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    class F_DirectX12RHICommandQueueProxy;
    class F_DirectX12RHICommandListManager;
    class F_DirectX12RHISynchronizer;
    class F_DirectX12RHIDescriptorManagerProxy;
    class F_DirectX12RHICommandSignature;
    
    class ABYTEK_ENGINE_RHI_API F_DirectX12RHIContextProxy : public A_RHIContextProxy
    {
    private:
        TS<F_DirectX12RHICommandQueueProxy> _CommandQueueProxy_DIRECT;
        TS<F_DirectX12RHICommandQueueProxy> _CommandQueueProxy_COMPUTE;
        TS<F_DirectX12RHICommandQueueProxy> _CommandQueueProxy_COPY;
        
        TS<F_DirectX12RHICommandListManager> _CommandListManager_DIRECT;
        TS<F_DirectX12RHICommandListManager> _CommandListManager_COMPUTE;
        TS<F_DirectX12RHICommandListManager> _CommandListManager_COPY;
        
        TS<F_DirectX12RHISynchronizer> _Synchronizer_DIRECT;
        TS<F_DirectX12RHISynchronizer> _Synchronizer_COMPUTE;
        TS<F_DirectX12RHISynchronizer> _Synchronizer_COPY;
        
        TS<F_DirectX12RHIDescriptorManagerProxy> _DescriptorManagerProxy_GPU_CBV_SRV_UAV;
        TS<F_DirectX12RHIDescriptorManagerProxy> _DescriptorManagerProxy_GPU_SAMPLER;
        TS<F_DirectX12RHIDescriptorManagerProxy> _DescriptorManagerProxy_CPU_CBV_SRV_UAV;
        TS<F_DirectX12RHIDescriptorManagerProxy> _DescriptorManagerProxy_CPU_RTV;
        TS<F_DirectX12RHIDescriptorManagerProxy> _DescriptorManagerProxy_CPU_DSV;
        TS<F_DirectX12RHIDescriptorManagerProxy> _DescriptorManagerProxy_CPU_SAMPLER;
        
        TS<F_DirectX12RHICommandSignature> _CommandSignature_DrawNonIndexed;
        TS<F_DirectX12RHICommandSignature> _CommandSignature_DrawIndexed;
        TS<F_DirectX12RHICommandSignature> _CommandSignature_DispatchCompute;
        TS<F_DirectX12RHICommandSignature> _CommandSignature_DispatchMesh;
    
    public:
        ABYTEK_FORCE_INLINE auto GetCommandQueueProxy_DIRECT() const noexcept
        {
            return _CommandQueueProxy_DIRECT.Weak();
        }
        ABYTEK_FORCE_INLINE auto GetCommandQueueProxy_COMPUTE() const noexcept
        {
            return _CommandQueueProxy_COMPUTE.Weak();
        }
        ABYTEK_FORCE_INLINE auto GetCommandQueueProxy_COPY() const noexcept
        {
            return _CommandQueueProxy_COPY.Weak();
        }
        ABYTEK_FORCE_INLINE TW<F_DirectX12RHICommandQueueProxy> QueryCommandQueueProxy(DirectX12SharedAPIWrapper::E_CommandListType CommandListType)
        {
            switch (CommandListType)
            {
            case DirectX12SharedAPIWrapper::E_CommandListType::DIRECT:
                return GetCommandQueueProxy_DIRECT();
            case DirectX12SharedAPIWrapper::E_CommandListType::COMPUTE:
                return GetCommandQueueProxy_COMPUTE();
            case DirectX12SharedAPIWrapper::E_CommandListType::COPY:
                return GetCommandQueueProxy_COPY();
            default:
                ABYTEK_ENGINE_RHI_ASSERT(false) << "Invalid command list type";
                break;
            }
            return {};
        }
        template<typename __F_Callback>
        void ForEachCommandQueueProxy(__F_Callback&& Callback)
        {
            Callback(_CommandQueueProxy_DIRECT.Weak());
            Callback(_CommandQueueProxy_COMPUTE.Weak());
            Callback(_CommandQueueProxy_COPY.Weak());
        }
        
        ABYTEK_FORCE_INLINE auto GetCommandListManager_DIRECT() const noexcept
        {
            return _CommandListManager_DIRECT.Weak();
        }
        ABYTEK_FORCE_INLINE auto GetCommandListManager_COMPUTE() const noexcept
        {
            return _CommandListManager_COMPUTE.Weak();
        }
        ABYTEK_FORCE_INLINE auto GetCommandListManager_COPY() const noexcept
        {
            return _CommandListManager_COPY.Weak();
        }
        ABYTEK_FORCE_INLINE TW<F_DirectX12RHICommandListManager> QueryCommandListManager(DirectX12SharedAPIWrapper::E_CommandListType CommandListType)
        {
            switch (CommandListType)
            {
            case DirectX12SharedAPIWrapper::E_CommandListType::DIRECT:
                return GetCommandListManager_DIRECT();
            case DirectX12SharedAPIWrapper::E_CommandListType::COMPUTE:
                return GetCommandListManager_COMPUTE();
            case DirectX12SharedAPIWrapper::E_CommandListType::COPY:
                return GetCommandListManager_COPY();
            default:
                ABYTEK_ENGINE_RHI_ASSERT(false) << "Invalid command list type";
                break;
            }
            return {};
        }
        template<typename __F_Callback>
        void ForEachCommandListManager(__F_Callback&& Callback)
        {
            Callback(_CommandListManager_DIRECT.Weak());
            Callback(_CommandListManager_COMPUTE.Weak());
            Callback(_CommandListManager_COPY.Weak());
        }
        
        ABYTEK_FORCE_INLINE auto GetSynchronizer_DIRECT() const noexcept
        {
            return _Synchronizer_DIRECT.Weak();
        }
        ABYTEK_FORCE_INLINE auto GetSynchronizer_COMPUTE() const noexcept
        {
            return _Synchronizer_COMPUTE.Weak();
        }
        ABYTEK_FORCE_INLINE auto GetSynchronizer_COPY() const noexcept
        {
            return _Synchronizer_COPY.Weak();
        }
        ABYTEK_FORCE_INLINE TW<F_DirectX12RHISynchronizer> QuerySynchronizer(DirectX12SharedAPIWrapper::E_CommandListType CommandListType)
        {
            switch (CommandListType)
            {
            case DirectX12SharedAPIWrapper::E_CommandListType::DIRECT:
                return GetSynchronizer_DIRECT();
            case DirectX12SharedAPIWrapper::E_CommandListType::COMPUTE:
                return GetSynchronizer_COMPUTE();
            case DirectX12SharedAPIWrapper::E_CommandListType::COPY:
                return GetSynchronizer_COPY();
            default:
                ABYTEK_ENGINE_RHI_ASSERT(false) << "Invalid command list type";
                break;
            }
            return {};
        }
        template<typename __F_Callback>
        void ForEachSynchronizer(__F_Callback&& Callback)
        {
            Callback(_Synchronizer_DIRECT.Weak());
            Callback(_Synchronizer_COMPUTE.Weak());
            Callback(_Synchronizer_COPY.Weak());
        }
        
        ABYTEK_FORCE_INLINE auto GetDescriptorManagerProxy_GPU_CBV_SRV_UAV() const noexcept
        {
            return _DescriptorManagerProxy_GPU_CBV_SRV_UAV.Weak();
        }
        ABYTEK_FORCE_INLINE auto GetDescriptorManagerProxy_GPU_SAMPLER() const noexcept
        {
            return _DescriptorManagerProxy_GPU_SAMPLER.Weak();
        }
        ABYTEK_FORCE_INLINE auto GetDescriptorManagerProxy_CPU_CBV_SRV_UAV() const noexcept
        {
            return _DescriptorManagerProxy_CPU_CBV_SRV_UAV.Weak();
        }
        ABYTEK_FORCE_INLINE auto GetDescriptorManagerProxy_CPU_RTV() const noexcept
        {
            return _DescriptorManagerProxy_CPU_RTV.Weak();
        }
        ABYTEK_FORCE_INLINE auto GetDescriptorManagerProxy_CPU_DSV() const noexcept
        {
            return _DescriptorManagerProxy_CPU_DSV.Weak();
        }
        ABYTEK_FORCE_INLINE auto GetDescriptorManagerProxy_CPU_SAMPLER() const noexcept
        {
            return _DescriptorManagerProxy_CPU_SAMPLER.Weak();
        }
        ABYTEK_FORCE_INLINE TW_Valid<F_DirectX12RHIDescriptorManagerProxy> QueryDescriptorManagerProxy(DirectX12SharedAPIWrapper::E_DescriptorHeapType DescriptorHeapType, B8 IsShaderVisible) const noexcept
        {
            if (IsShaderVisible)
            {
                switch (DescriptorHeapType)
                {
                case DirectX12SharedAPIWrapper::E_DescriptorHeapType::CBV_SRV_UAV:
                    return _DescriptorManagerProxy_GPU_CBV_SRV_UAV.Weak();
                case DirectX12SharedAPIWrapper::E_DescriptorHeapType::SAMPLER:
                    return _DescriptorManagerProxy_GPU_SAMPLER.Weak();
                default:
                    ABYTEK_ENGINE_RHI_ASSERT(false) << "Invalid descriptor heap type";
                    break;
                }
            }
            else
            {
                switch (DescriptorHeapType)
                {
                case DirectX12SharedAPIWrapper::E_DescriptorHeapType::CBV_SRV_UAV:
                    return _DescriptorManagerProxy_CPU_CBV_SRV_UAV.Weak();
                case DirectX12SharedAPIWrapper::E_DescriptorHeapType::RTV:
                    return _DescriptorManagerProxy_CPU_RTV.Weak();
                case DirectX12SharedAPIWrapper::E_DescriptorHeapType::DSV:
                    return _DescriptorManagerProxy_CPU_DSV.Weak();
                case DirectX12SharedAPIWrapper::E_DescriptorHeapType::SAMPLER:
                    return _DescriptorManagerProxy_CPU_SAMPLER.Weak();
                default:
                    ABYTEK_ENGINE_RHI_ASSERT(false) << "Invalid descriptor heap type";
                    break;
                }
            }
            return {};
        }
        template<typename __F_Callback>
        void ForEachDescriptorManagerProxy(__F_Callback&& Callback)
        {
            Callback(_DescriptorManagerProxy_GPU_CBV_SRV_UAV.Weak());
            Callback(_DescriptorManagerProxy_GPU_SAMPLER.Weak());
            Callback(_DescriptorManagerProxy_CPU_CBV_SRV_UAV.Weak());
            Callback(_DescriptorManagerProxy_CPU_RTV.Weak());
            Callback(_DescriptorManagerProxy_CPU_DSV.Weak());
            Callback(_DescriptorManagerProxy_CPU_SAMPLER.Weak());
        }
        
        ABYTEK_FORCE_INLINE auto GetCommandSignature_DrawNonIndexed() const noexcept
        {
            return _CommandSignature_DrawNonIndexed.Weak();
        }
        ABYTEK_FORCE_INLINE auto GetCommandSignature_DrawIndexed() const noexcept
        {
            return _CommandSignature_DrawIndexed.Weak();
        }
        ABYTEK_FORCE_INLINE auto GetCommandSignature_DispatchCompute() const noexcept
        {
            return _CommandSignature_DispatchCompute.Weak();
        }
        ABYTEK_FORCE_INLINE auto GetCommandSignature_DispatchMesh() const noexcept
        {
            return _CommandSignature_DispatchMesh.Weak();
        }
        TW<F_DirectX12RHICommandSignature> GetCommandSignature(E_RHIIndirectArgumentType IndirectArgumentType)
        {
            switch (IndirectArgumentType)
            {
            case E_RHIIndirectArgumentType::DRAW_NON_INDEXED:
                return _CommandSignature_DrawNonIndexed.Weak();
            case E_RHIIndirectArgumentType::DRAW_INDEXED:
                return _CommandSignature_DrawIndexed.Weak();
            case E_RHIIndirectArgumentType::DISPATCH_COMPUTE:
                return _CommandSignature_DispatchCompute.Weak();
            case E_RHIIndirectArgumentType::DISPATCH_MESH:
                return _CommandSignature_DispatchMesh.Weak();
            default:
                ABYTEK_ENGINE_RHI_ASSERT(false) << "Unknown indirect argument type";
                break;
            }
            return {};
        }
        
    public:
        ABYTEK_RA_DECLARE_OBJECT(F_DirectX12RHIContextProxy);
        void LateBuildContext_DirectX12();
        virtual void Release() override;
    };
}
#endif