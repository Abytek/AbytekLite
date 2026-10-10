#pragma once

#include "Abytek/Engine.NFC.prerequisites.hpp"
#include "Abytek/World/WorldSubsystem.hpp"
#include "Abytek/Renderable.hpp"


namespace Abytek
{
    class F_RendererManager;
    class A_RenderPath;
    class A_RenderScene;
    
    class ABYTEK_ENGINE_NFC_API F_WorldRenderResourceOwner : public A_WorldContext, public A_Renderable
    {
        ABYTEK_BEGIN_REFLECTOR(A_WorldContext)
        ABYTEK_END_REFLECTOR(F_WorldRenderResourceOwner);
        
    private:
        TW<F_RendererManager> _Manager;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetManager() const noexcept
        {
            return _Manager;
        }
        
    public:
        F_WorldRenderResourceOwner(const F_SerializableObjectInitParams& InitParams);
        ~F_WorldRenderResourceOwner() override;
        
    protected:
        void OnLoad() override;
        void OnUnload() override;
        
    protected:
        TS<A_RenderProxy> CreateRenderProxy() override;
    };
    
    class ABYTEK_ENGINE_NFC_API F_RendererManager : public A_WorldSubsystem
    {
    public:
        ABYTEK_BEGIN_REFLECTOR(A_WorldSubsystem)
        ABYTEK_END_REFLECTOR(F_RendererManager);
        
    public:
        ABYTEK_DECLARE_STATIC_SUBSYSTEM(F_RendererManager);
        
    private:
        TW<TF_ConsoleVariable<TF_ReflectionTypeHandle<A_RenderPath>>> _ConsoleVariable_RenderPathType;
        
        TS<A_RenderPath> _RenderPath;
        TS<F_WorldRenderResourceOwner> _WorldRenderResourceOwner;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetConsoleVariable_RenderPathType() const noexcept
        {
            return _ConsoleVariable_RenderPathType;
        }
        
        ABYTEK_FORCE_INLINE const auto& GetRenderPath() const noexcept
        {
            return _RenderPath;
        }
        ABYTEK_FORCE_INLINE const auto& GetWorldRenderResourceOwner() const noexcept
        {
            return _WorldRenderResourceOwner;
        }
    
    public:
        F_RendererManager(const F_ProgramUnitBuildParams& BuildParams);
        ~F_RendererManager() override;
        
    protected:
        void OnConfig() override;
        
    protected:
        void OnInit() override;
        void OnStartup() override;
        void OnShutdown() override;
        void OnRelease() override;
    };
}
