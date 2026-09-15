#pragma once

#include "Abytek/Actor/ActorComponent.hpp"
#include "Abytek/Renderable.hpp"


namespace Abytek
{
    class ABYTEK_ENGINE_NFC_API A_RenderableComponent : public A_ActorComponent, public A_Renderable
    {
    public:
        friend class F_RenderableComponentManager;
        
    public:
        ABYTEK_BEGIN_REFLECTOR(A_ActorComponent)
        ABYTEK_END_REFLECTOR(A_RenderableComponent)
        
    public:
        static F_Name GetCreateAndDestroyRenderStateUpdateFunctionName();
        static F_Name GetUpdateRenderStateUpdateFunctionName();
        static void GlobalInit();
        static void GlobalRelease();
        
    private:
        B8 _IsRenderStateDirty = false;
        B8 _ShouldUpdateRenderStateEveryFrame = false;
        B8 _ShouldUpdateRenderStateThisFrame = false;
        
    public:
        ABYTEK_FORCE_INLINE auto IsRenderStateDirty() const noexcept
        {
            return _IsRenderStateDirty;
        }
        B8 MarkRenderStateDirty();
        ABYTEK_FORCE_INLINE auto GetShouldUpdateRenderStateEveryFrame() const noexcept
        {
            return _ShouldUpdateRenderStateEveryFrame;
        }
        ABYTEK_FORCE_INLINE auto GetShouldUpdateRenderStateThisFrame() const noexcept
        {
            return _ShouldUpdateRenderStateThisFrame;
        }
        
    protected:
        A_RenderableComponent(const F_SerializableObjectInitParams& InitParams);
        
    public:
        ~A_RenderableComponent() override;
        
    protected:
        void OnRegisterComponent() override;
        void OnUnregisterComponent() override;
        
    protected:
        virtual void OnUpdateRenderState();
        
    private:
        void _UpdateRenderState();
        
    public:
        void EnableUpdateRenderStateEveryFrame();
        void DisableUpdateRenderStateEveryFrame();
        void MarkUpdateRenderStateThisFrame();
        
    private:
        void _ActualMarkRenderStateDirty();
        void _ActualEnableUpdateRenderStateEveryFrame();
        void _ActualDisableUpdateRenderStateEveryFrame();
        void _ActualMarkUpdateRenderStateThisFrame();
    };
}