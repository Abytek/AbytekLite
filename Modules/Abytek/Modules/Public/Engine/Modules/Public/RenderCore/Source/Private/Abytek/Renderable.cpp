#include "Abytek/Renderable.hpp"
#include "Abytek/RenderProxy.hpp"
#include "Abytek/RenderRegistry.hpp"
#include "Abytek/RenderCoreManager.hpp"
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
        TW<F_World> World = ABYTEK_WTHIS().DynamicCast<I_GetWorld>()->GetWorld();
        _ShouldEnableProxy = World->HasFlags(E_WorldFlag::CREATE_RENDER_SCENE);
        if (ShouldEnableProxy())
        {
            _RenderProxy = CreateRenderProxy();
            _RenderProxy->Init(GetRenderRegistry());
            if (IsRenderable())
            {
                CreateRenderState();
            }
        }
    }
    void A_Renderable::CleanUpRenderable()
    {
        if (ShouldEnableProxy())
        {
            if (CreatedRenderState())
            {
                DestroyRenderState();
            }
            _RenderProxy->Release(); 
            _RenderProxy = {};
        }
    }

    B8 A_Renderable::IsRenderable() const
    {
        return ShouldEnableProxy();
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
        OnCreateRenderState();
        H_Frame::EnqueueCommand<E_FrameParamType::RENDER>(
            [RenderProxy = _RenderProxy]
            {
                RenderProxy->OnCreateRenderState_RenderTask();
            }
        );
    }
    void A_Renderable::DestroyRenderState()
    {
        OnDestroyRenderState();
        H_Frame::EnqueueCommand<E_FrameParamType::RENDER>(
            [RenderProxy = _RenderProxy]
            {
                RenderProxy->OnDestroyRenderState_RenderTask();
            }
        );
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
}
