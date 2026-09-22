#pragma once

#include "Abytek/RHIContextChild.hpp"
#include "Abytek/DirectX12/RHIDescriptor.hpp"
#include "Abytek/DirectX12/RHIDescriptorManager.hpp"
#include "Abytek/DirectX12/RHISubmissionContext.hpp"
#include "Abytek/DirectX12Shared/APIWrapper.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    namespace DirectX12RHIDescriptorManagerQueries
    {
        struct F_Init
        {
            F_DirectX12RHIDescriptorIndex DescriptorIndex;
            DirectX12SharedAPIWrapper::F_D3D12DescriptorDesc D3D12DescriptorDesc;
        };
        
        struct F_Copy
        {
            F_DirectX12RHIDescriptorAllocationProxy SrcDescriptorRange;
            F_DirectX12RHIDescriptorAllocationProxy DstDescriptorRange;
            B8 SrcIsShaderVisible = false;
            B8 DstIsShaderVisible = false;
        };
    }
    
    struct ABYTEK_ENGINE_RHI_API H_DirectX12RHIDescriptorManagerProxy
    {
        static void PopulateCommandList(
            F_DirectX12RHISubmissionContext& SubmissionContext,
            const TF_Span<TW<F_DirectX12RHIDescriptorManagerProxy>>& DescriptorManagerProxies
        );
        static void PopulateCommandList(
            F_DirectX12RHISubmissionContext& SubmissionContext,
            const TW_Valid<A_RHIContextProxy>& ContextProxy
        );
        template<typename __F_DescriptorManagerProxies>
        ABYTEK_FORCE_INLINE static void PopulateCommandList(
            F_DirectX12RHISubmissionContext& SubmissionContext,
            const __F_DescriptorManagerProxies& DescriptorManagerProxies
        )
        {
            PopulateCommandList(
                SubmissionContext,
                TF_Span<TW<F_DirectX12RHIDescriptorManagerProxy>>(
                    (TW<F_DirectX12RHIDescriptorManagerProxy>*)DescriptorManagerProxies.data(),
                    DescriptorManagerProxies.size()
                )
            );
        }
    };
    
    struct F_DirectX12RHIDescriptorManagerProxyBuildParams
    {
        TW<A_RHIDevice> Device;
        DirectX12SharedAPIWrapper::E_DescriptorHeapType DescriptorHeapType = DirectX12SharedAPIWrapper::E_DescriptorHeapType::NONE;
        B8 IsShaderVisible = false;
        F_DirectX12RHIDescriptorIndex Size = 256;
    };
    
    class ABYTEK_ENGINE_RHI_API F_DirectX12RHIDescriptorManagerProxy : public A_RHIContextChildProxy
    {
    private:
        DirectX12SharedAPIWrapper::E_DescriptorHeapType _DescriptorHeapType;
        B8 _IsShaderVisible = false;
        F_DirectX12RHIDescriptorIndex _Size = 256;
        F_DirectX12RHIDescriptorIndex _D3D12DescriptorHandleStride = 0;
        
        Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> _D3D12DescriptorHeap_ShaderVisible;
        Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> _D3D12DescriptorHeap_NonShaderVisible;
        
    public:
        struct F_Queues
        {
            TF_ConcurrentQueue<DirectX12RHIDescriptorManagerQueries::F_Init> Init;
            TF_ConcurrentQueue<DirectX12RHIDescriptorManagerQueries::F_Copy> Copy;
        } Queues;
        
    public:
        ABYTEK_FORCE_INLINE auto GetDescriptorManager() const noexcept
        {
            return GetContextChild().FastCast<F_DirectX12RHIDescriptorManager>();
        }
        
        ABYTEK_FORCE_INLINE auto GetDescriptorHeapType() const noexcept
        {
            return _DescriptorHeapType;
        }
        ABYTEK_FORCE_INLINE auto& InjectDescriptorHeapType() noexcept
        {
            return _DescriptorHeapType;
        }
        ABYTEK_FORCE_INLINE auto IsShaderVisible() const noexcept
        {
            return _IsShaderVisible;
        }
        ABYTEK_FORCE_INLINE auto& InjectIsShaderVisible() noexcept
        {
            return _IsShaderVisible;
        }
        ABYTEK_FORCE_INLINE auto GetSize() const noexcept
        {
            return _Size;
        }
        ABYTEK_FORCE_INLINE auto& InjectSize() noexcept
        {
            return _Size;
        }
        ABYTEK_FORCE_INLINE auto GetD3D12DescriptorHandleStride() const noexcept
        {
            return _D3D12DescriptorHandleStride;
        }
        
        ABYTEK_FORCE_INLINE const auto& GetD3D12DescriptorHeap_ShaderVisible() const noexcept
        {
            return _D3D12DescriptorHeap_ShaderVisible;
        }
        ABYTEK_FORCE_INLINE auto& InjectD3D12DescriptorHeap_ShaderVisible() noexcept
        {
            return _D3D12DescriptorHeap_ShaderVisible;
        }
        ABYTEK_FORCE_INLINE const auto& GetD3D12DescriptorHeap_NonShaderVisible() const noexcept
        {
            return _D3D12DescriptorHeap_NonShaderVisible;
        }
        ABYTEK_FORCE_INLINE auto& InjectD3D12DescriptorHeap_NonShaderVisible() noexcept
        {
            return _D3D12DescriptorHeap_NonShaderVisible;
        }
        
    public:
        ABYTEK_RA_DECLARE_OBJECT_CREATABLE(F_DirectX12RHIDescriptorManagerProxy);
        void LateBuildDescriptorManager_DirectX12();
        virtual void Release() override;
        
    public:
        void Resize(F_DirectX12RHIDescriptorIndex NewSize);
        
    public:
        void Flush();
        void FlushInit(const DirectX12RHIDescriptorManagerQueries::F_Init& Query);
        void FlushCopies();
    };
}
#endif