#pragma once

#include "Abytek/Engine.NFC.prerequisites.hpp"
#include "Abytek/UpdateBase/UpdateUtilities.hpp"


namespace Abytek
{
    class F_CanvasComponent;
    
    namespace CanvasComponentManagerEvents
    {
        using F_OnRegisterComponent = TF_Event<void(const TW_Valid<F_CanvasComponent>& Component)>;
        using F_OnUnregisterComponent = TF_Event<void(const TW_Valid<F_CanvasComponent>& Component)>;
    }
    struct F_CanvasComponentManagerEvents
    {
        CanvasComponentManagerEvents::F_OnRegisterComponent OnRegisterComponent;
        CanvasComponentManagerEvents::F_OnUnregisterComponent OnUnregisterComponent;
    };
    
    class ABYTEK_ENGINE_NFC_API F_CanvasComponentManager final : public A_ApplicationSubsystem
    {
    public:
        friend class F_CanvasComponent;
        
    public:
        ABYTEK_BEGIN_REFLECTOR(A_ApplicationSubsystem)
        ABYTEK_END_REFLECTOR(F_CanvasComponentManager);
        
    public:
        ABYTEK_DECLARE_STATIC_APPLICATION_SUBSYSTEM(F_CanvasComponentManager);

    private:
        TF_Set<TW<F_CanvasComponent>> _CanvasComponents;
        
    public:
        F_CanvasComponentManagerEvents Events;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetCanvasComponents() const noexcept
        {
            return _CanvasComponents;
        }
        
    public:
        F_CanvasComponentManager(const F_ProgramUnitBuildParams& BuildParams);
        ~F_CanvasComponentManager() override;

    protected:
        void OnConfig() override;
        void OnPostConfig() override;
        
    protected:
        void OnInit() override;
        void OnStartup() override;
        void OnTick() override;
        void OnShutdown() override;
        void OnRelease() override;
        
    public:
        void DrawCanvasComponents();
        
    private:
        void _RegisterCanvasComponent(const TW_Valid<F_CanvasComponent>& Component);
        void _UnregisterCanvasComponent(const TW_Valid<F_CanvasComponent>& Component);
    };
}
