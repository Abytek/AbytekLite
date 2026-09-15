#include "Abytek/RHISampler.hpp"
#include "Abytek/RHISamplerProxy.hpp"
#include "Abytek/RHISubsystem.hpp"
#include "Abytek/DirectX12/RHISampler.hpp"


namespace Abytek
{
    ABYTEK_RA_BEGIN_OBJECT(A_RHISampler)
        auto RHISubsystem = F_RHISubsystem::GetInstance();
        switch (RHISubsystem->GetActiveAPI())
        {
#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
        case E_RHIAPI::DIRECTX12:
            return TU<F_DirectX12RHISampler>()();
#endif 
        }
    ABYTEK_RA_END_OBJECT()
    void A_RHISampler::Build(const F_RHISamplerBuildParams& BuildParams)
    {
        A_RHIContextChild::Build(BuildParams);

        _Access = BuildParams.Access;
        
        _MinFilter = BuildParams.MinFilter;
        _MagFilter = BuildParams.MagFilter;
        _MipFilter = BuildParams.MipFilter;
        
        _FilterReductionType = BuildParams.FilterReductionType;
        
        _AddressModeU = BuildParams.AddressModeU;
        _AddressModeV = BuildParams.AddressModeV;
        _AddressModeW = BuildParams.AddressModeW;
        
        _LODBias = BuildParams.LODBias;
        _MinLOD = BuildParams.MinLOD;
        _MaxLOD = BuildParams.MaxLOD;
        
        _MaxAnisotropy = BuildParams.MaxAnisotropy;
        
        _CompareOperation = BuildParams.CompareOperation;
        
        _BorderColor = BuildParams.BorderColor;
        
        _AllowBindless = BuildParams.AllowBindless;
            
        GetProxy().FastCast<A_RHISamplerProxy>()->LateBuildSampler();
    }
    void A_RHISampler::Release()
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
            
        _BindlessId = INVALID_RHI_BINDLESS_ID;
            
        A_RHIContextChild::Release();
    }

    TS<A_RHIContextChildProxy> A_RHISampler::CreateProxy()
    {
        return RACreateAndBuildShared<A_RHISamplerProxy>(ABYTEK_WTHIS());
    }
}
