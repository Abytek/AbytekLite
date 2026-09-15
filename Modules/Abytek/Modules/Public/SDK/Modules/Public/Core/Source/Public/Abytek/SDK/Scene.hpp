#pragma once

#include "Abytek/SDK/Core.prerequisites.pch.hpp"
#include "Abytek/SDK/Primitive.hpp"


namespace Abytek::SDK
{
    class ABYTEK_SDK_CORE_API F_Scene : public A_Object
    {
    private:
        flecs::world _ECSWorld;
        
    public:
        ABYTEK_FORCE_INLINE auto& GetECSWorld() noexcept
        {
            return _ECSWorld;
        }
        
    public:
        F_Scene();
        virtual ~F_Scene() override;
        
    public:
        flecs::entity CreatePrimitive(const F_PrimitiveBuildParams& BuildParams);
    };
}