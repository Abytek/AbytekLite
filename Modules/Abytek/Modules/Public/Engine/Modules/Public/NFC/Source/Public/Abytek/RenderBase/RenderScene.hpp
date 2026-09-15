#pragma once

#include "Abytek/RenderBase/RenderObject.hpp"


namespace Abytek
{
    class A_WorldContext;
    class F_RenderGeometryManager;
    class F_GPUData;
    
    struct F_RenderSceneBuildParams
    {
    };
    class ABYTEK_ENGINE_NFC_API A_RenderScene : public A_RenderObject
    {
    public:
        friend struct F_RenderSceneUpdateRange;
        friend struct F_RenderScenePostUpdateRange;
        
    private:
        TS<F_RenderGeometryManager> _GeometryManager;
        TS<F_GPUData> _GPUData;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetGeometryManager() const noexcept
        {
            return _GeometryManager;
        }
        ABYTEK_FORCE_INLINE const auto& GetGPUData() const noexcept
        {
            return _GPUData;
        }
        
    public:
        A_RenderScene(const TW_Valid<F_WorldRenderResource>& WorldRenderResource, const F_RenderSceneBuildParams& BuildParams);
        ~A_RenderScene() override;
        
    protected:
        virtual void OnBeginUpdate();
        virtual void OnEndUpdate();
        virtual void OnBeginPostUpdate();
        virtual void OnEndPostUpdate();
    };
}
