#include "Abytek/Renderer/StandardPrimitive/GeometryFactoryType_StaticMeshECMS.hpp"
#include "Abytek/Renderer/StandardPrimitive/GeometryFactoryTypeProxy_StaticMeshECMS.hpp"


namespace Abytek
{
    ABYTEK_REFLECT(A_GeometryFactoryType_StaticMeshECMS)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::A_GeometryFactoryType_StaticMeshECMS"));
    }

    A_GeometryFactoryType_StaticMeshECMS::A_GeometryFactoryType_StaticMeshECMS(const F_SerializableObjectInitParams& InitParams) :
        A_GeometryFactoryType(InitParams)
    {
    }
    A_GeometryFactoryType_StaticMeshECMS::~A_GeometryFactoryType_StaticMeshECMS()
    {
    }
}
