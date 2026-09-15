#pragma once

#include "Abytek/Engine.RenderCore.prerequisites.hpp"
#include "Abytek/World/WorldContext.hpp"
#include "Abytek/GPUData/GlobalGPUDataComponent.hpp"
#include "Abytek/Renderable.hpp"


namespace Abytek
{
    class F_RenderRegistry;
    
    class ABYTEK_ENGINE_RENDER_CORE_API F_GlobalGPUDataPack : public A_WorldContext, public A_Renderable
    {
    public:
        ABYTEK_BEGIN_REFLECTOR(A_WorldContext)
        ABYTEK_END_REFLECTOR(F_GlobalGPUDataPack)
        
    private:
        TF_Vector<I_GlobalGPUDataComponent::F_CreateFunction> _ComponentTypeCreateFunctions;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetComponentTypeCreateFunctions() const
        {
            return _ComponentTypeCreateFunctions;
        }
        
    public:
        F_GlobalGPUDataPack(const F_SerializableObjectInitParams& InitParams);
        ~F_GlobalGPUDataPack() override;
        
    protected:
        void OnLoad() override;
        void OnUnload() override;
        
    protected:
        void OnCreateRenderState() override;
        void OnDestroyRenderState() override;
        TS<A_RenderProxy> CreateRenderProxy() override;
        
    private:
        void _Populate();
    };
}