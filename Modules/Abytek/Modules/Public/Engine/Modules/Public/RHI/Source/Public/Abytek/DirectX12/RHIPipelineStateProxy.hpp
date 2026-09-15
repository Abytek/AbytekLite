#pragma once

#include "Abytek/RHIPipelineStateProxy.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    /*class ABYTEK_ENGINE_RHI_API F_DirectX12RHIPipelineStateProxy : public A_RHIPipelineStateProxy
    {
    private:
        Microsoft::WRL::ComPtr<ID3D12PipelineState> _D3D12PipelineState = nullptr;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetD3D12PipelineState() const noexcept
        {
            return _D3D12PipelineState;
        }
        ABYTEK_FORCE_INLINE auto& InjectD3D12PipelineState() noexcept
        {
            return _D3D12PipelineState;
        }
        
    public:
        ABYTEK_RA_DECLARE_OBJECT(F_DirectX12RHIPipelineStateProxy);
        void LateBuildPipelineState_DirectX12();
        virtual void Release() override;
        
    public:
        void InitD3D12PipelineState();
        
    private: 
        void _InitGraphics();
        void _InitCompute();
        void _InitWorkGraph();
    };*/
}
#endif