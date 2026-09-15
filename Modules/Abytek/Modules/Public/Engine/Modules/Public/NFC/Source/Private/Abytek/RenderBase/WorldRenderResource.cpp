#include "Abytek/RenderBase/WorldRenderResource.hpp"
#include "Abytek/GPUData/GlobalGPUDataPack.hpp"
#include "Abytek/GPUData/Render/GlobalGPUDataPackProxy.hpp"
#include "Abytek/RenderBase/RenderManager.hpp"
#include "Abytek/RenderBase/RenderPath.hpp"
#include "Abytek/RenderBase/RenderObjectFactory.hpp"
#include "Abytek/RenderBase/RenderScene.hpp"
#include "Abytek/World/WorldContextHelper.hpp"


namespace Abytek
{
    TS<F_WorldRenderResource> F_WorldRenderResource::Get_MainTask(const TW_Valid<A_WorldContext>& WorldContext)
    {
        return H_WorldContext::GetUnit<F_RenderManager>(WorldContext)->GetRenderProxy().FastCast<F_WorldRenderResource>();
    }

    F_WorldRenderResource::F_WorldRenderResource(const TW_Valid<F_RenderManager>& RenderManager) :
        A_RenderProxy(RenderManager)
    {
        auto RenderPath = RenderManager->GetRenderPath();
        _RenderObjectFactory = RenderPath->CreateObjectFactory(ABYTEK_WTHIS());
        _GlobalGPUDataPackProxy = RenderManager->GetGPUDataPack()->GetRenderProxy().StaticCast<F_GlobalGPUDataPackProxy>();
    }
    F_WorldRenderResource::~F_WorldRenderResource()
    {
        _GlobalGPUDataPackProxy = {};
        _RenderObjectFactory = {};
    }

    void F_WorldRenderResource::OnCreateRenderState_RenderTask()
    {
        F_RenderSceneBuildParams SceneBuildParams;
        _Scene = _RenderObjectFactory->CreateScene(SceneBuildParams);
    }
    void F_WorldRenderResource::OnDestroyRenderState_RenderTask()
    {
        _Scene = {};
    }
}
