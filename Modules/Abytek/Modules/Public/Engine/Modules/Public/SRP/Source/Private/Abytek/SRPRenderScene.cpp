#include "Abytek/SRPRenderScene.hpp"


namespace Abytek
{
    F_SRPRenderScene::F_SRPRenderScene(const TW_Valid<F_WorldRenderResource>& WorldRenderResource, const F_RenderSceneBuildParams& BuildParams) :
        A_RenderScene(WorldRenderResource, BuildParams)
    {
    }
    F_SRPRenderScene::~F_SRPRenderScene()
    {
    }

    void F_SRPRenderScene::OnBeginUpdate()
    {
        A_RenderScene::OnBeginUpdate();
    }
    void F_SRPRenderScene::OnEndUpdate()
    {
        A_RenderScene::OnEndUpdate();
    }
}
