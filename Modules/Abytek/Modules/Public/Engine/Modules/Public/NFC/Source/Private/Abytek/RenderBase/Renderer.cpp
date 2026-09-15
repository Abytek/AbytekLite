#include "Abytek/RenderBase/Renderer.hpp"
#include "Abytek/RenderBase/RenderViewFamily.hpp"
#include "Abytek/RenderBase/RenderView.hpp"
#include "Abytek/ActorComponents/Render/CanvasRenderProxy.hpp"


namespace Abytek
{
    A_Renderer::A_Renderer(const TW_Valid<F_WorldRenderResource>& WorldRenderResource, const F_RendererBuildParams& BuildParams) :
        A_RenderObject(WorldRenderResource),
        _Scene(BuildParams.Scene),
        _ViewFamily(BuildParams.ViewFamily)
    {
    }
    A_Renderer::~A_Renderer()
    {
    }

    void A_Renderer::OnRender()
    {
    }

    void A_Renderer::Render()
    {
        {
            TF_Function<void()> Command;
            while (PreRenderQueue.TryPop(Command))
            {
                Command();
            }
        }
        OnRender();
        {
            TF_Function<void()> Command;
            while (PostRenderQueue.TryPop(Command))
            {
                Command();
            }
        }
    }
}
