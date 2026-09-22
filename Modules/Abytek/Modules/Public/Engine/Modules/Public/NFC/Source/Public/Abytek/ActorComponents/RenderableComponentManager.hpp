#pragma once

#include "Abytek/Engine.NFC.prerequisites.hpp"
#include "Abytek/UpdateBase/UpdateUtilities.hpp"


namespace Abytek
{
    class ABYTEK_ENGINE_NFC_API F_RenderableComponentManager final : public A_ApplicationSubsystem
    {
    public:
        friend class A_RenderableComponent;
        
    public:
        ABYTEK_BEGIN_REFLECTOR(A_ApplicationSubsystem)
        ABYTEK_END_REFLECTOR(F_RenderableComponentManager);
        
    public:
        ABYTEK_DECLARE_STATIC_APPLICATION_SUBSYSTEM(F_RenderableComponentManager);

    private:
        TF_Set<TW<A_RenderableComponent>> _RenderableComponents;
        TF_Set<TW<A_RenderableComponent>> _RenderableComponentsToUpdateRenderStateEveryFrame;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetRenderableComponents() const noexcept
        {
            return _RenderableComponents;
        }
        ABYTEK_FORCE_INLINE const auto& GetRenderableComponentsToUpdateRenderStateEveryFrame() const noexcept
        {
            return _RenderableComponentsToUpdateRenderStateEveryFrame;
        }
        
    public:
        F_RenderableComponentManager(const F_ProgramUnitBuildParams& BuildParams);
        ~F_RenderableComponentManager() override;

    protected:
        void OnConfig() override;
        void OnPostConfig() override;
        
    protected:
        void OnInit() override;
        void OnStartup() override;
        void OnTick() override;
        void OnShutdown() override;
        void OnRelease() override;
        
    private:
        void _RegisterRenderableComponent(const TW_Valid<A_RenderableComponent>& Component);
        void _UnregisterRenderableComponent(const TW_Valid<A_RenderableComponent>& Component);
        void _RegisterRenderableComponentToUpdateRenderStateEveryFrame(const TW_Valid<A_RenderableComponent>& Component);
        void _UnregisterRenderableComponentFromUpdateRenderStateEveryFrame(const TW_Valid<A_RenderableComponent>& Component);
        
    public:
        void UpdateRenderStates();
    };
}
