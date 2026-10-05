#include "Abytek/Renderable.hpp"
#include "Abytek/RenderProxy.hpp"
#include "Abytek/RenderRegistry.hpp"
#include "Abytek/RenderCoreManager.hpp"
#include "Abytek/RHISubmissionQueue.hpp"
#include "Abytek/World/WorldContext.hpp"
#include "Abytek/Frame/FrameHelper.hpp"
#include "Abytek/World/WorldContextHelper.hpp"


namespace Abytek
{
    A_Renderable::A_Renderable()
    {
    }
    A_Renderable::~A_Renderable()
    {
    }

    void A_Renderable::SetupRenderable()
    {
        _RenderRegistryPort = FindRenderRegistryPort();
        if (_RenderRegistryPort)
        {
            _RenderProxy = CreateRenderProxy();
            _RenderProxy->Init(_RenderRegistryPort);
            if (IsRenderable())
            {
                CreateRenderState();
            }
        }
    }
    void A_Renderable::CleanUpRenderable()
    {
        if (_RenderRegistryPort)
        {
            if (CreatedRenderState())
            {
                DestroyRenderState();
            }
            _RenderProxy->Release(_RenderRegistryPort); 
            _RenderProxy = {};
        }
        _RenderRegistryPort = {};
    }

    B8 A_Renderable::IsRenderable() const
    {
        return static_cast<B8>(_RenderRegistryPort);
    }

    void A_Renderable::OnCreateRenderState()
    {
    }
    void A_Renderable::OnDestroyRenderState()
    {
    }

    void A_Renderable::CreateRenderState()
    {
        _CreatedRenderState = true;
#ifdef ABYTEK_DEBUG_INFO
        _RenderRegistryPort->EnqueueCommand(
            [RenderProxy = _RenderProxy, DebugName = ABYTEK_WTHIS().DynamicCast<A_Object>()->GetDebugName()]
            {
                RenderProxy->SetDebugName(DebugName);
            }
        );
#endif
        OnCreateRenderState();
        _RenderRegistryPort->EnqueueCommand(
            [RenderProxy = _RenderProxy, RenderRegistryPortData = _RenderRegistryPort->GetData()]
            {
                RenderProxy->OnCreateRenderState_RenderTask(
                    RenderRegistryPortData->GetSubmissionItemContainer()
                );
            }
        );
    }
    void A_Renderable::DestroyRenderState()
    {
        OnDestroyRenderState();
        _RenderRegistryPort->EnqueueCommand(
            [RenderProxy = _RenderProxy, RenderRegistryPortData = _RenderRegistryPort->GetData()]
            {
                RenderProxy->OnDestroyRenderState_RenderTask(
                    RenderRegistryPortData->GetSubmissionItemContainer()
                );
            }
        );
#ifdef ABYTEK_DEBUG_INFO
        _RenderRegistryPort->EnqueueCommand(
            [RenderProxy = _RenderProxy]
            {
                RenderProxy->SetDebugName({});
            }
        );
#endif
        _CreatedRenderState = false;
    }
    void A_Renderable::RecreateRenderState()
    {
        if (CreatedRenderState())
        {
            DestroyRenderState();
        }
        CreateRenderState();
    }

    TS<F_RenderRegistry> A_Renderable::GetRenderRegistry() const
    {
        return H_WorldContext::GetUnit<F_RenderCoreManager>(
            ABYTEK_WTHIS().DynamicCast<I_GetWorld>()->GetWorld()    
        )->GetMainRegistry();
    }

    TS<A_RenderRegistryPort> A_Renderable::FindRenderRegistryPort() const
    {
        auto RenderRegistry = GetRenderRegistry();
        return RenderRegistry->GetMainPort();
    }
}
