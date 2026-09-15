#include "Abytek/DirectX12/RHICommandSignature.hpp"
#include "Abytek/RHIIndirectUtilities.hpp"
#include "Abytek/DirectX12/RHIDevice.hpp"
#include "Abytek/RHISubsystem.hpp"
#include "Abytek/DirectX12/RHISubsystemProxy.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    ABYTEK_RA_OBJECT_DEFAULT(F_DirectX12RHICommandSignature);

    void F_DirectX12RHICommandSignature::Build(const F_DirectX12RHICommandSignatureBuildParams& BuildParams)
    {
        A_RHIDeviceChild::Build(BuildParams.Device);
        _IndirectArgumentType = BuildParams.IndirectArgumentType;
        {
            auto IndirectUtilities = A_RHIIndirectUtilities::GetInstance();
            
            D3D12_INDIRECT_ARGUMENT_DESC D3D12IndirectArgumentDesc;
            switch (_IndirectArgumentType)
            {
            case E_RHIIndirectArgumentType::DRAW_NON_INDEXED:
                D3D12IndirectArgumentDesc.Type = D3D12_INDIRECT_ARGUMENT_TYPE_DRAW;
                break;
            case E_RHIIndirectArgumentType::DRAW_INDEXED:
                D3D12IndirectArgumentDesc.Type = D3D12_INDIRECT_ARGUMENT_TYPE_DRAW_INDEXED;
                break;
            case E_RHIIndirectArgumentType::DISPATCH_COMPUTE:
                D3D12IndirectArgumentDesc.Type = D3D12_INDIRECT_ARGUMENT_TYPE_DISPATCH;
                break;
            case E_RHIIndirectArgumentType::DISPATCH_MESH:
                D3D12IndirectArgumentDesc.Type = D3D12_INDIRECT_ARGUMENT_TYPE_DISPATCH_MESH;
                break;
            }
            
            D3D12_COMMAND_SIGNATURE_DESC D3D12CommandSignatureDesc;
            D3D12CommandSignatureDesc.ByteStride = IndirectUtilities->GetArgumentStride(_IndirectArgumentType);
            D3D12CommandSignatureDesc.NodeMask = 1;
            D3D12CommandSignatureDesc.NumArgumentDescs = 1;
            D3D12CommandSignatureDesc.pArgumentDescs = &D3D12IndirectArgumentDesc;
      
            HRESULT HR = GetDevice().FastCast<F_DirectX12RHIDevice>()->GetD3D12Device()->CreateCommandSignature(
                &D3D12CommandSignatureDesc,
                nullptr,
                IID_PPV_ARGS(&_D3D12CommandSignature)
            );
            ABYTEK_ENGINE_RHI_ASSERT(SUCCEEDED(HR)) << "Cannot create command signature";
        }
    }
    void F_DirectX12RHICommandSignature::Release()
    {
        _D3D12CommandSignature = nullptr;
        _IndirectArgumentType = E_RHIIndirectArgumentType::NONE;
        A_RHIDeviceChild::Release();
    }
}
#endif