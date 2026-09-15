#pragma once

#include "Abytek/RHIContext.hpp"
#include "Abytek/RHIResource.hpp"
#include "Abytek/DirectX12/RHIDescriptorAllocation.hpp"
#include "Abytek/DirectX12/RHIResourcePlacement.hpp"
#include "Abytek/DirectX12Shared/APIWrapper.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    class F_DirectX12RHICommandQueue;
    class F_DirectX12RHIDescriptorManager;
    class F_DirectX12RHIPlacedResourceManager;
    
    namespace DirectX12RHIContextQueries
    {
        struct F_DeallocateDescriptors
        {
            F_DirectX12RHIDescriptorAllocation DescriptorAllocation;
        };
    }
    
    class ABYTEK_ENGINE_RHI_API F_DirectX12RHIContext : public A_RHIContext
    {
    private:
        TS<F_DirectX12RHICommandQueue> _CommandQueue_DIRECT;
        TS<F_DirectX12RHICommandQueue> _CommandQueue_COMPUTE;
        TS<F_DirectX12RHICommandQueue> _CommandQueue_COPY;
        
        TS<F_DirectX12RHIDescriptorManager> _DescriptorManager_GPU_CBV_SRV_UAV;
        TS<F_DirectX12RHIDescriptorManager> _DescriptorManager_GPU_SAMPLER;
        TS<F_DirectX12RHIDescriptorManager> _DescriptorManager_CPU_CBV_SRV_UAV;
        TS<F_DirectX12RHIDescriptorManager> _DescriptorManager_CPU_RTV;
        TS<F_DirectX12RHIDescriptorManager> _DescriptorManager_CPU_DSV;
        TS<F_DirectX12RHIDescriptorManager> _DescriptorManager_CPU_SAMPLER;
        
        TS<F_DirectX12RHIPlacedResourceManager> _PlacedResourceManager_AllowOnlyBuffers_Default;
        TS<F_DirectX12RHIPlacedResourceManager> _PlacedResourceManager_AllowOnlyNonRTDSTextures_Default;
        TS<F_DirectX12RHIPlacedResourceManager> _PlacedResourceManager_AllowOnlyRTDSTextures_Default;
        TS<F_DirectX12RHIPlacedResourceManager> _PlacedResourceManager_AllowOnlyBuffers_Readback;
        TS<F_DirectX12RHIPlacedResourceManager> _PlacedResourceManager_AllowOnlyNonRTDSTextures_Readback;
        TS<F_DirectX12RHIPlacedResourceManager> _PlacedResourceManager_AllowOnlyRTDSTextures_Readback;
        TS<F_DirectX12RHIPlacedResourceManager> _PlacedResourceManager_AllowOnlyBuffers_Upload;
        TS<F_DirectX12RHIPlacedResourceManager> _PlacedResourceManager_AllowOnlyNonRTDSTextures_Upload;
        TS<F_DirectX12RHIPlacedResourceManager> _PlacedResourceManager_AllowOnlyRTDSTextures_Upload;
        
    public:
        struct F_Queues
        {
            TF_ConcurrentQueue<DirectX12RHIContextQueries::F_DeallocateDescriptors> DeallocateDescriptors;
                TF_ConcurrentQueue<F_DirectX12RHIResourcePlacement> ResourcePlacementsToDeallocate;
        } Queues;
        
    public:
        ABYTEK_FORCE_INLINE auto GetCommandQueue_DIRECT() const noexcept
        {
            return _CommandQueue_DIRECT.Weak();
        }
        ABYTEK_FORCE_INLINE auto GetCommandQueue_COMPUTE() const noexcept
        {
            return _CommandQueue_COMPUTE.Weak();
        }
        ABYTEK_FORCE_INLINE auto GetCommandQueue_COPY() const noexcept
        {
            return _CommandQueue_COPY.Weak();
        }
        ABYTEK_FORCE_INLINE TW<F_DirectX12RHICommandQueue> QueryCommandQueue(DirectX12SharedAPIWrapper::E_CommandListType CommandListType)
        {
            switch (CommandListType)
            {
            case DirectX12SharedAPIWrapper::E_CommandListType::DIRECT:
                return GetCommandQueue_DIRECT();
            case DirectX12SharedAPIWrapper::E_CommandListType::COMPUTE:
                return GetCommandQueue_COMPUTE();
            case DirectX12SharedAPIWrapper::E_CommandListType::COPY:
                return GetCommandQueue_COPY();
            default:
                ABYTEK_ENGINE_RHI_ASSERT(false) << "Invalid command list type";
                break;
            }
            return {};
        }
        template<typename __F_Callback>
        void ForEachCommandQueue(__F_Callback&& Callback)
        {
            Callback(_CommandQueue_DIRECT.Weak());
            Callback(_CommandQueue_COMPUTE.Weak());
            Callback(_CommandQueue_COPY.Weak());
        }
        
        ABYTEK_FORCE_INLINE auto GetDescriptorManager_GPU_CBV_SRV_UAV() const noexcept
        {
            return _DescriptorManager_GPU_CBV_SRV_UAV.Weak();
        }
        ABYTEK_FORCE_INLINE auto GetDescriptorManager_GPU_SAMPLER() const noexcept
        {
            return _DescriptorManager_GPU_SAMPLER.Weak();
        }
        ABYTEK_FORCE_INLINE auto GetDescriptorManager_CPU_CBV_SRV_UAV() const noexcept
        {
            return _DescriptorManager_CPU_CBV_SRV_UAV.Weak();
        }
        ABYTEK_FORCE_INLINE auto GetDescriptorManager_CPU_RTV() const noexcept
        {
            return _DescriptorManager_CPU_RTV.Weak();
        }
        ABYTEK_FORCE_INLINE auto GetDescriptorManager_CPU_DSV() const noexcept
        {
            return _DescriptorManager_CPU_DSV.Weak();
        }
        ABYTEK_FORCE_INLINE auto GetDescriptorManager_CPU_SAMPLER() const noexcept
        {
            return _DescriptorManager_CPU_SAMPLER.Weak();
        }
        ABYTEK_FORCE_INLINE TW_Valid<F_DirectX12RHIDescriptorManager> QueryDescriptorManager(DirectX12SharedAPIWrapper::E_DescriptorHeapType DescriptorHeapType, B8 IsShaderVisible) const noexcept
        {
            if (IsShaderVisible)
            {
                switch (DescriptorHeapType)
                {
                case DirectX12SharedAPIWrapper::E_DescriptorHeapType::CBV_SRV_UAV:
                    return _DescriptorManager_GPU_CBV_SRV_UAV.Weak();
                case DirectX12SharedAPIWrapper::E_DescriptorHeapType::SAMPLER:
                    return _DescriptorManager_GPU_SAMPLER.Weak();
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
                    return _DescriptorManager_CPU_CBV_SRV_UAV.Weak();
                case DirectX12SharedAPIWrapper::E_DescriptorHeapType::RTV:
                    return _DescriptorManager_CPU_RTV.Weak();
                case DirectX12SharedAPIWrapper::E_DescriptorHeapType::DSV:
                    return _DescriptorManager_CPU_DSV.Weak();
                case DirectX12SharedAPIWrapper::E_DescriptorHeapType::SAMPLER:
                    return _DescriptorManager_CPU_SAMPLER.Weak();
                default:
                    ABYTEK_ENGINE_RHI_ASSERT(false) << "Invalid descriptor heap type";
                    break;
                }
            }
            return {};
        }
        template<typename __F_Callback>
        void ForEachDescriptorManager(__F_Callback&& Callback)
        {
            Callback(_DescriptorManager_GPU_CBV_SRV_UAV.Weak());
            Callback(_DescriptorManager_GPU_SAMPLER.Weak());
            Callback(_DescriptorManager_CPU_CBV_SRV_UAV.Weak());
            Callback(_DescriptorManager_CPU_RTV.Weak());
            Callback(_DescriptorManager_CPU_DSV.Weak());
            Callback(_DescriptorManager_CPU_SAMPLER.Weak());
        }
        
        ABYTEK_FORCE_INLINE const auto& GetPlacedResourceManager_AllowOnlyBuffers_Default() const noexcept
        {
            return _PlacedResourceManager_AllowOnlyBuffers_Default;
        }
        ABYTEK_FORCE_INLINE const auto& GetPlacedResourceManager_AllowOnlyNonRTDSTextures_Default() const noexcept
        {
            return _PlacedResourceManager_AllowOnlyNonRTDSTextures_Default;
        }
        ABYTEK_FORCE_INLINE const auto& GetPlacedResourceManager_AllowOnlyRTDSTextures_Default() const noexcept
        {
            return _PlacedResourceManager_AllowOnlyRTDSTextures_Default;
        }
        ABYTEK_FORCE_INLINE const auto& GetPlacedResourceManager_AllowOnlyBuffers_Readback() const noexcept
        {
            return _PlacedResourceManager_AllowOnlyBuffers_Readback;
        }
        ABYTEK_FORCE_INLINE const auto& GetPlacedResourceManager_AllowOnlyNonRTDSTextures_Readback() const noexcept
        {
            return _PlacedResourceManager_AllowOnlyNonRTDSTextures_Readback;
        }
        ABYTEK_FORCE_INLINE const auto& GetPlacedResourceManager_AllowOnlyRTDSTextures_Readback() const noexcept
        {
            return _PlacedResourceManager_AllowOnlyRTDSTextures_Readback;
        }
        ABYTEK_FORCE_INLINE const auto& GetPlacedResourceManager_AllowOnlyBuffers_Upload() const noexcept
        {
            return _PlacedResourceManager_AllowOnlyBuffers_Upload;
        }
        ABYTEK_FORCE_INLINE const auto& GetPlacedResourceManager_AllowOnlyNonRTDSTextures_Upload() const noexcept
        {
            return _PlacedResourceManager_AllowOnlyNonRTDSTextures_Upload;
        }
        ABYTEK_FORCE_INLINE const auto& GetPlacedResourceManager_AllowOnlyRTDSTextures_Upload() const noexcept
        {
            return _PlacedResourceManager_AllowOnlyRTDSTextures_Upload;
        }
        TW<F_DirectX12RHIPlacedResourceManager> QueryPlacedResourceManager(const TW_Valid<A_RHIResource>& Resource)
        {
            auto Archetype = Resource->GetArchetype();
            const auto& ResourceAccessCapabilities = Resource->GetAccessCapabilities();
            if (FlagHas(Archetype, E_RHIResourceArchetype::BUFFER))
            {
                if (FlagHas(ResourceAccessCapabilities.CPU, E_RHIResourceCPUAccess::READ))
                {
                    return _PlacedResourceManager_AllowOnlyBuffers_Readback.Weak();
                }
                else if (FlagHas(ResourceAccessCapabilities.CPU, E_RHIResourceCPUAccess::WRITE))
                {
                    return _PlacedResourceManager_AllowOnlyBuffers_Upload.Weak();
                }
                else
                {
                    return _PlacedResourceManager_AllowOnlyBuffers_Default.Weak();
                }
                return {};
            }
            if (FlagHas(Archetype, E_RHIResourceArchetype::TEXTURE))
            {
                if (
                    FlagHas(ResourceAccessCapabilities.GPU, E_RHIResourceGPUAccess::RTV)
                    || FlagHas(ResourceAccessCapabilities.GPU, E_RHIResourceGPUAccess::DSV)
                )
                {
                    if (FlagHas(ResourceAccessCapabilities.CPU, E_RHIResourceCPUAccess::READ))
                    {
                        return _PlacedResourceManager_AllowOnlyRTDSTextures_Readback.Weak();
                    }
                    else if (FlagHas(ResourceAccessCapabilities.CPU, E_RHIResourceCPUAccess::WRITE))
                    {
                        return _PlacedResourceManager_AllowOnlyRTDSTextures_Upload.Weak();
                    }
                    else
                    {
                        return _PlacedResourceManager_AllowOnlyRTDSTextures_Default.Weak();
                    }
                    return {};
                }
                if (FlagHas(ResourceAccessCapabilities.CPU, E_RHIResourceCPUAccess::READ))
                {
                    return _PlacedResourceManager_AllowOnlyNonRTDSTextures_Readback.Weak();
                }
                else if (FlagHas(ResourceAccessCapabilities.CPU, E_RHIResourceCPUAccess::WRITE))
                {
                    return _PlacedResourceManager_AllowOnlyNonRTDSTextures_Upload.Weak();
                }
                else
                {
                    return _PlacedResourceManager_AllowOnlyNonRTDSTextures_Default.Weak();
                }
            }
            return {};
        }
        
    public:
        ABYTEK_RA_DECLARE_OBJECT(F_DirectX12RHIContext);

    public:
        virtual void FirstCompile() override;
        virtual void FinalizeRelease() override;
        
    public:
        virtual void HighLevelInitialize() override;
        virtual void HighLevelDeinitialize() override;
    };
}
#endif