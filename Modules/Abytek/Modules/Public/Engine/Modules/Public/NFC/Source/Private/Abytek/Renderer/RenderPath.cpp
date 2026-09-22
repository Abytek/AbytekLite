#include "Abytek/Renderer/RenderPath.hpp"
#include "Abytek/Renderer/RendererManager.hpp"
#include "Abytek/World/WorldContextHelper.hpp"


namespace Abytek
{
    ABYTEK_REFLECT(A_RenderPath)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::A_RenderPath"));
    }

    TS<A_RenderPath> A_RenderPath::Get(const TW_Valid<A_WorldContext>& WorldContext)
    {
        return H_WorldContext::GetUnit<F_RendererManager>(WorldContext)->GetRenderPath();
    };

    A_RenderPath::A_RenderPath(const F_SerializableObjectInitParams& InitParams) : 
        A_WorldContext(InitParams)
    {
    }
    A_RenderPath::~A_RenderPath()
    {
    }

    void A_RenderPath::OnLoad()
    {
    }
    void A_RenderPath::OnUnload()
    {
    }
}
