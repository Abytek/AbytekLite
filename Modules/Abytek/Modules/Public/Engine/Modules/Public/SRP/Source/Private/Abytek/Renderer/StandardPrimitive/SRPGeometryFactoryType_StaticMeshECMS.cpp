#include "Abytek/Renderer/StandardPrimitive/SRPGeometryFactoryType_StaticMeshECMS.hpp"
#include "Abytek/Renderer/StandardPrimitive/SRPGeometryFactoryTypeProxy_StaticMeshECMS.hpp"


namespace Abytek
{
    ABYTEK_REFLECT(F_SRPGeometryFactoryType_StaticMeshECMS)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::F_SRPGeometryFactoryType_StaticMeshECMS"));
    }

    F_SRPGeometryFactoryType_StaticMeshECMS::F_SRPGeometryFactoryType_StaticMeshECMS(const F_SerializableObjectInitParams& InitParams) :
        A_GeometryFactoryType_StaticMeshECMS(InitParams)
    {
    }
    F_SRPGeometryFactoryType_StaticMeshECMS::~F_SRPGeometryFactoryType_StaticMeshECMS()
    {
    }

    TS<A_RenderProxy> F_SRPGeometryFactoryType_StaticMeshECMS::CreateRenderProxy()
    {
        return TS<F_SRPGeometryFactoryTypeProxy_StaticMeshECMS>()(ABYTEK_WTHIS());
    }

#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
    F_Name F_SRPGeometryFactoryType_StaticMeshECMS::GetMainShaderModuleName()
    {
        return ABYTEK_NAME("Ábytek/Renderer/StandardPrimitive/SRPGeometryFactoryType_StaticMeshECMS.slangh");
    }
#endif
}
