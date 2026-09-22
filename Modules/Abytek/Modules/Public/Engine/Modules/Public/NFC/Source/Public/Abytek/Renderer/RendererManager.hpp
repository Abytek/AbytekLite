#pragma once

#include "Abytek/World/WorldSubsystem.hpp"
#include "Abytek/Renderable.hpp"


namespace Abytek
{
    class A_RenderPath;
    class A_RenderScene;
    
    class ABYTEK_ENGINE_NFC_API F_RendererManager : public A_WorldSubsystem, public A_Renderable
    {
    public:
        ABYTEK_BEGIN_REFLECTOR(A_WorldSubsystem)
        ABYTEK_END_REFLECTOR(F_RendererManager);
        
    public:
        ABYTEK_DECLARE_STATIC_SUBSYSTEM(F_RendererManager);
        
    private:
        TW<TF_ConsoleVariable<TF_ReflectionTypeHandle<A_RenderPath>>> _ConsoleVariable_RenderPathType;
        
        TS<A_RenderPath> _RenderPath;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetConsoleVariable_RenderPathType() const noexcept
        {
            return _ConsoleVariable_RenderPathType;
        }
        
        ABYTEK_FORCE_INLINE const auto& GetRenderPath() const noexcept
        {
            return _RenderPath;
        }
    
    public:
        F_RendererManager(const F_ProgramUnitBuildParams& BuildParams);
        ~F_RendererManager() override;
        
    protected:
        void OnConfig() override;
        
    protected:
        void OnStartup() override;
        void OnShutdown() override;
        
    public:
        B8 IsRenderable() const override;
        
    protected:
        TS<A_RenderProxy> CreateRenderProxy() override;
    };
}
