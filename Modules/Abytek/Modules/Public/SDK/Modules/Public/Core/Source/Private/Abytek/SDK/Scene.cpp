#include "Abytek/SDK/Scene.hpp"


namespace Abytek::SDK
{
    F_Scene::F_Scene()
    {
        _ECSWorld.component<F_PrimitiveExternalId>();
        _ECSWorld.component<F_PrimitiveTag>();
    }
    F_Scene::~F_Scene()
    {
    }

    flecs::entity F_Scene::CreatePrimitive(const F_PrimitiveBuildParams& BuildParams)
    {
        auto Result = _ECSWorld.entity();
        ABYTEK_SDK_CORE_ASSERT(BuildParams.ExternalId) << "Requires a valid external id";
        Result.set<F_PrimitiveExternalId>(BuildParams.ExternalId);
        Result.add<F_PrimitiveTag>();
        return Result;
    }
}
