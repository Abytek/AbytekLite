#pragma once

#include "Abytek/ActorComponents/RenderableComponent.hpp"
#include "Abytek/ActorComponents/SceneComponent.hpp"


namespace Abytek
{
    class ABYTEK_ENGINE_NFC_API F_PrimitiveSceneComponent : public F_SceneComponent
    {
    public:
        ABYTEK_BEGIN_REFLECTOR(F_SceneComponent)
        ABYTEK_END_REFLECTOR(F_PrimitiveSceneComponent);
    
    public:
        friend class A_PrimitiveComponent;
        
    private:
        TW<A_PrimitiveComponent> _PrimitiveComponent;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetPrimitiveComponent() const noexcept
        {
            return _PrimitiveComponent;
        }
        
    public:
        F_PrimitiveSceneComponent(const F_SerializableObjectInitParams& InitParams);
        ~F_PrimitiveSceneComponent() override;
        
    protected:
        void OnTransformChanged() override;
    };
    
    class ABYTEK_ENGINE_NFC_API A_PrimitiveComponent : public A_RenderableComponent
    {
    public:
        ABYTEK_BEGIN_REFLECTOR(A_RenderableComponent)
        ABYTEK_END_REFLECTOR(A_PrimitiveComponent);
        
    public:
        friend class F_PrimitiveSceneComponent;
    
    private:
        B8 _IsEnabled = true;
        TS<F_PrimitiveSceneComponent> _SceneComponent;
        
    public:
        ABYTEK_FORCE_INLINE B8 IsEnabled() const noexcept
        {
            return _IsEnabled;
        }
        ABYTEK_FORCE_INLINE const auto& GetSceneComponent() const noexcept
        {
            return _SceneComponent;
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
        
    protected:
        virtual void OnTransformChanged();
    };
}