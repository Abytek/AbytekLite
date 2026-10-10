#pragma once

#include "Abytek/Engine.SRP.prerequisites.hpp"
#include "Abytek/Renderer/StandardPrimitive/GeometryFactoryType_StaticMeshECMS.hpp"


namespace Abytek
{
    class ABYTEK_ENGINE_SRP_API F_SRPGeometryFactoryType_StaticMeshECMS : public A_GeometryFactoryType_StaticMeshECMS
    {
    public:
        ABYTEK_BEGIN_REFLECTOR(A_GeometryFactoryType_StaticMeshECMS)
        ABYTEK_END_REFLECTOR(F_SRPGeometryFactoryType_StaticMeshECMS);
        
    private:
        
    public:
        
    public:
        F_SRPGeometryFactoryType_StaticMeshECMS(const F_SerializableObjectInitParams& InitParams);
        ~F_SRPGeometryFactoryType_StaticMeshECMS() override;
        
    protected:
        TS<A_RenderProxy> CreateRenderProxy() override;
        
    public:
#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
        F_Name GetMainShaderModuleName() override;
#endif
    };
}