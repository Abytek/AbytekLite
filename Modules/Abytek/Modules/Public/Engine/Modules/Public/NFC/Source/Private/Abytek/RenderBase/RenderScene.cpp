#include "Abytek/RenderBase/RenderScene.hpp"
#include "Abytek/RenderCoreManager.hpp"
#include "Abytek/RenderRegistry.hpp"
#include "Abytek/World/WorldContextHelper.hpp"
#include "Abytek/World/WorldSubsystemContainer.hpp"
#include "Abytek/RenderBase/RenderManager.hpp"
#include "Abytek/Geometries/Render/RenderGeometryManager.hpp"
#include "Abytek/GPUData/Render/GPUData.hpp"


namespace Abytek
{
    A_RenderScene::A_RenderScene(const TW_Valid<F_WorldRenderResource>& WorldRenderResource, const F_RenderSceneBuildParams& BuildParams) :
        A_RenderObject(WorldRenderResource)
    {
        F_RenderGeometryManagerBuildParams GeometryManagerBuildParams;
        GeometryManagerBuildParams.Scene = ABYTEK_WTHIS();
        _GeometryManager = TS<F_RenderGeometryManager>()(
            GetWorldRenderResource(),
            GeometryManagerBuildParams
        );
        
        F_GPUDataBuildParams GPUDataBuildParams;
        GPUDataBuildParams.Scene = ABYTEK_WTHIS();
        _GPUData = TS<F_GPUData>()(
            GetWorldRenderResource(),
            GPUDataBuildParams
        );
    }
    A_RenderScene::~A_RenderScene()
    {
        _GPUData = {};
        _GeometryManager = {};
    }

    void A_RenderScene::OnBeginUpdate()
    {
        _GeometryManager->BeginUpdate();
        _GPUData->BeginUpdate();
    }
    void A_RenderScene::OnEndUpdate()
    {
        _GPUData->EndUpdate();
        _GeometryManager->EndUpdate();
    }
    void A_RenderScene::OnBeginPostUpdate()
    {
        _GeometryManager->BeginPostUpdate();
        _GPUData->BeginPostUpdate();
    }
    void A_RenderScene::OnEndPostUpdate()
    {
        _GPUData->EndPostUpdate();
        _GeometryManager->EndPostUpdate();
    }
}
