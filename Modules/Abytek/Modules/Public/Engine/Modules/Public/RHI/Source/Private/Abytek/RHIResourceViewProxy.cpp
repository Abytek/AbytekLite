#include "Abytek/RHIResourceViewProxy.hpp"
#include "Abytek/RHISubsystem.hpp"
#include "Abytek/RHIResourceView.hpp"
#include "Abytek/RHIResource.hpp"
#include "Abytek/RHIResourceProxy.hpp"
#include "Abytek/DirectX12/RHIResourceViewProxy.hpp"


namespace Abytek
{
    ABYTEK_RA_BEGIN_OBJECT(A_RHIResourceViewProxy)
        auto RHISubsystem = F_RHISubsystem::GetInstance();
        switch (RHISubsystem->GetActiveAPI())
        {
#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
        case E_RHIAPI::DIRECTX12:
            return TU<F_DirectX12RHIResourceViewProxy>()();
#endif 
        }
    ABYTEK_RA_END_OBJECT()

    void A_RHIResourceViewProxy::LateBuildResourceView()
    {
        auto ResourceView = GetResourceView();
            
        _ResourceProxy = GetContextChild()
            .FastCast<A_RHIResourceView>()
            ->GetResource()
            ->GetProxy()
            .FastCast<A_RHIResourceProxy>();
            
        _Format = ResourceView->GetFormat();
        _Access = ResourceView->GetAccess();
        _AllowBindless = ResourceView->GetAllowBindless();
    }
    void A_RHIResourceViewProxy::LateBuildBufferView()
    {
        _Archetype |= E_RHIResourceViewArchetype::BUFFER_VIEW;
        _BufferViewAspect = GetResourceView()->GetBufferViewAspect();
    }
    void A_RHIResourceViewProxy::LateBuildTextureView()
    {
        _Archetype |= E_RHIResourceViewArchetype::TEXTURE_VIEW;
        _TextureViewAspect = GetResourceView()->GetTextureViewAspect();
    }
    void A_RHIResourceViewProxy::LateBuildRTASView()
    {
        _Archetype |= E_RHIResourceViewArchetype::RTAS_VIEW;
        _RTASViewAspect = GetResourceView()->GetRTASViewAspect();
    }
    void A_RHIResourceViewProxy::Release()
    {
        _Archetype = E_RHIResourceViewArchetype::NONE;
        _BufferViewAspect = {};
        _TextureViewAspect = {};
        _RTASViewAspect = {};
        
        _AllowBindless = false;
        _Access = {};
        _Format = E_RHIFormat::NONE;
        _ResourceProxy = {};
        A_RHIContextChildProxy::Release();
    }
}
