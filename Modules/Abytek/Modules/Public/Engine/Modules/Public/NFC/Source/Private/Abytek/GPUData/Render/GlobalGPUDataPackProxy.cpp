#include "Abytek/GPUData/Render/GlobalGPUDataPackProxy.hpp"


namespace Abytek
{
    F_GlobalGPUDataPackProxy::F_GlobalGPUDataPackProxy(const TW_Valid<F_GlobalGPUDataPack>& Owner) :
        A_RenderProxy(Owner)
    {
    }
    F_GlobalGPUDataPackProxy::~F_GlobalGPUDataPackProxy()
    {
    }

    void F_GlobalGPUDataPackProxy::OnCreateRenderState_RenderTask()
    {
    }
    void F_GlobalGPUDataPackProxy::OnDestroyRenderState_RenderTask()
    {
        _ComponentTypeCreateFunctions = {};
    }
}
