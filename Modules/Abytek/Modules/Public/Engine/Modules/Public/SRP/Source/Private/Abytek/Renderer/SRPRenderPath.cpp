#include "Abytek/Renderer/SRPRenderPath.hpp"
#include "Abytek/Renderer/SRPRenderObjectFactory.hpp"
#include "Abytek/Renderer/SRPRenderScene.hpp"
#include "Abytek/Renderer/SRPRenderView.hpp"


namespace Abytek
{
    ABYTEK_REFLECT(F_SRPRenderPath)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::F_SRPRenderPath"));
    }

    F_SRPRenderPath::F_SRPRenderPath(const F_SerializableObjectInitParams& InitParams) :
        A_RenderPath(InitParams)
    {
    }
    F_SRPRenderPath::~F_SRPRenderPath()
    {
    }

    TS<A_RenderObjectFactory> F_SRPRenderPath::CreateObjectFactory(const TW_Valid<F_WorldRenderResource>& WorldRenderResource)
    {
        return TS<F_SRPRenderObjectFactory>()(WorldRenderResource);
    }
}
