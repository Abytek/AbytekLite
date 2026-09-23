#pragma once

#include "Abytek/ActorComponents/RenderableComponent.hpp"


namespace Abytek
{
    class ABYTEK_ENGINE_NFC_API A_PrimitiveComponent : public A_RenderableComponent
    {
    public:
        ABYTEK_BEGIN_REFLECTOR(A_RenderableComponent)
        ABYTEK_END_REFLECTOR(A_PrimitiveComponent);
    
    private:
        B8 _IsEnabled = true;
        
    public:
        ABYTEK_FORCE_INLINE B8 IsEnabled() const noexcept
        {
            return _IsEnabled;
        }
        
    public:
        A_PrimitiveComponent(const F_SerializableObjectInitParams& InitParams);
        ~A_PrimitiveComponent() override;
        
    protected:
        void OnRegisterComponent() override;
        void OnUnregisterComponent() override;
        
    public:
        B8 IsRenderable() const override;
        
    protected:
        void OnCreateRenderState() override;
        void OnDestroyRenderState() override;
        TS<A_RenderProxy> CreateRenderProxy() override;
        
    protected:
        virtual void OnEnable();
        virtual void OnDisable();
        
    public:
        void Enable();
        void Disable();
        
    private:
        void _Enable_Impl();
        void _Disable_Impl();
    };
}