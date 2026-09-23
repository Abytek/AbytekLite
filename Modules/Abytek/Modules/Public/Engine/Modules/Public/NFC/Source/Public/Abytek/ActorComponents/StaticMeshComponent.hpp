#pragma once

#include "Abytek/ActorComponents/PrimitiveComponent.hpp"


namespace Abytek
{
    class F_StaticMesh;

    class ABYTEK_ENGINE_NFC_API F_StaticMeshComponent : public A_PrimitiveComponent
    {
    public:
        ABYTEK_BEGIN_REFLECTOR(A_PrimitiveComponent)
        ABYTEK_END_REFLECTOR(F_StaticMeshComponent);
    
    private:
        TS<F_StaticMesh> _StaticMesh;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetStaticMesh() const noexcept
        {
            return _StaticMesh;
        }
        
    public:
        F_StaticMeshComponent(const F_SerializableObjectInitParams& InitParams);
        ~F_StaticMeshComponent() override;
        
    protected:
        void OnRegisterComponent() override;
        void OnUnregisterComponent() override;
        
    public:
        B8 IsRenderable() const override;
        
    protected:
        void OnCreateRenderState() override;
        void OnDestroyRenderState() override;
        TS<A_RenderProxy> CreateRenderProxy() override;
    };
}