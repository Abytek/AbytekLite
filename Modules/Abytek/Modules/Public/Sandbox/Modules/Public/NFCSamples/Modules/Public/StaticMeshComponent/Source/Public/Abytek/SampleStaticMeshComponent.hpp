#pragma once

#include "Abytek/Sandbox.NFCSamples.StaticMeshComponent.prerequisites.pch.hpp"
#include "Abytek/Actor/ActorComponent.hpp"
#include "Abytek/ActorComponents/StaticMeshComponent.hpp"


namespace Abytek
{
    class ABYTEK_SANDBOX_NFC_SAMPLES_STATIC_MESH_COMPONENT_API F_SampleStaticMeshComponent final : public A_ActorComponent
    {
    public:
        ABYTEK_BEGIN_REFLECTOR(A_ActorComponent)
        ABYTEK_END_REFLECTOR(F_SampleStaticMeshComponent);
        
    private:
        TS<F_StaticMeshComponent> _StaticMeshComponent;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetStaticMeshComponent() const noexcept
        {
            return _StaticMeshComponent;
        }
        
    public:
        F_SampleStaticMeshComponent(const F_SerializableObjectInitParams& InitParams);
        ~F_SampleStaticMeshComponent() override;
        
    protected:
        void OnTick() override;
    };
}
