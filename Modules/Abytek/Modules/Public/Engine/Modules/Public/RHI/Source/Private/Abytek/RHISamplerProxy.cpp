#include "Abytek/RHISamplerProxy.hpp"
#include "Abytek/RHISubsystem.hpp"
#include "Abytek/RHISampler.hpp"
#include "Abytek/RHIResource.hpp"
#include "Abytek/DirectX12/RHISamplerProxy.hpp"


namespace Abytek
{
    ABYTEK_RA_BEGIN_OBJECT(A_RHISamplerProxy)
        auto RHISubsystem = F_RHISubsystem::GetInstance();
        switch (RHISubsystem->GetActiveAPI())
        {
#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
        case E_RHIAPI::DIRECTX12:
            return TU<F_DirectX12RHISamplerProxy>()();
#endif 
        }
    ABYTEK_RA_END_OBJECT()

    void A_RHISamplerProxy::LateBuildSampler()
    {
        auto Sampler = GetSampler();
            
        _Access = Sampler->GetAccess();
    
        _MinFilter = Sampler->GetMinFilter();
        _MagFilter = Sampler->GetMagFilter();
        _MipFilter = Sampler->GetMipFilter();
    
        _FilterReductionType = Sampler->GetFilterReductionType();
    
        _AddressModeU = Sampler->GetAddressModeU();
        _AddressModeV = Sampler->GetAddressModeV();
        _AddressModeW = Sampler->GetAddressModeW();
    
        _LODBias = Sampler->GetLODBias();
        _MinLOD = Sampler->GetMinLOD();
        _MaxLOD = Sampler->GetMaxLOD();
    
        _MaxAnisotropy = Sampler->GetMaxAnisotropy();
    
        _CompareOperation = Sampler->GetCompareOperation();
    
        _BorderColor = Sampler->GetBorderColor();
    
        _AllowBindless = Sampler->GetAllowBindless();
    }
    void A_RHISamplerProxy::Release()
    {
        _Access = {};

        _MinFilter = E_RHITextureFilter::DEFAULT;
        _MagFilter = E_RHITextureFilter::DEFAULT;
        _MipFilter = E_RHITextureFilter::DEFAULT;
    
        _FilterReductionType = E_RHITextureFilterReductionType::DEFAULT;

        _AddressModeU = E_RHITextureAddressMode::DEFAULT;
        _AddressModeV = E_RHITextureAddressMode::DEFAULT;
        _AddressModeW = E_RHITextureAddressMode::DEFAULT;

        _LODBias = 0.0f;
        _MinLOD = 0.0f;
        _MaxLOD = 9999.0f;

        _MaxAnisotropy = 1;

        _CompareOperation = E_RHICompareOperation::NONE;

        _BorderColor = F_Vector4_F32::Zero();
    
        _AllowBindless = false;
            
        A_RHIContextChildProxy::Release();
    }
}
