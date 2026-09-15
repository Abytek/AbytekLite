#pragma once

#include "Abytek/RenderBase/WorldContextRenderProxy.hpp"
#include "Abytek/GPUData/GlobalGPUDataPack.hpp"


namespace Abytek
{
    class ABYTEK_ENGINE_RENDER_CORE_API F_GlobalGPUDataPackProxy : public A_RenderProxy
    {
    public:
        friend class F_GlobalGPUDataPack;
        
    private:
        TF_Vector<I_GlobalGPUDataComponent::F_CreateFunction> _ComponentTypeCreateFunctions;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetComponentTypeCreateFunctions() const
        {
            return _ComponentTypeCreateFunctions;
        }
        
    public:
        F_GlobalGPUDataPackProxy(const TW_Valid<F_GlobalGPUDataPack>& Owner);
        ~F_GlobalGPUDataPackProxy() override;
        
    protected:
        void OnCreateRenderState_RenderTask() override;
        void OnDestroyRenderState_RenderTask() override;
    };
}