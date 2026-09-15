#pragma once

#include "Abytek/Engine.NFC.prerequisites.hpp"
#include "Abytek/UpdateBase/UpdateUtilities.hpp"
#include "Abytek/IHIMouseCommon.hpp"
#include "Abytek/IHIKeyboardCommon.hpp"


namespace Abytek
{
    class F_InputComponent;
    
    class A_IHIMouse;
    class A_IHIKeyboard;

    namespace InputComponentManagerEvents
    {
        using F_OnRegisterComponent = TF_Event<void(const TW_Valid<F_InputComponent>& Component)>;
        using F_OnUnregisterComponent = TF_Event<void(const TW_Valid<F_InputComponent>& Component)>;
    }
    struct F_InputComponentManagerEvents
    {
        InputComponentManagerEvents::F_OnRegisterComponent OnRegisterComponent;
        InputComponentManagerEvents::F_OnUnregisterComponent OnUnregisterComponent;
    };
    
    class ABYTEK_ENGINE_NFC_API F_InputComponentManager final : public A_ApplicationSubsystem
    {
    public:
        friend class F_InputComponent;
        
    public:
        ABYTEK_BEGIN_REFLECTOR(A_ApplicationSubsystem)
        ABYTEK_END_REFLECTOR(F_InputComponentManager)
        
    public:
        ABYTEK_DECLARE_STATIC_APPLICATION_SUBSYSTEM(F_InputComponentManager);

    private:
        TF_Set<TW<F_InputComponent>> _InputComponents;
        
        TW<A_IHIMouse> _Mouse;
        IHIMouseEvents::F_OnButtonDown::F_ListenerHandle _MouseEventListener_OnButtonDown;
        IHIMouseEvents::F_OnButtonUp::F_ListenerHandle _MouseEventListener_OnButtonUp;
        IHIMouseEvents::F_OnMove::F_ListenerHandle _MouseEventListener_OnMove;
        IHIMouseEvents::F_OnWheel::F_ListenerHandle _MouseEventListener_OnWheel;
        
        TW<A_IHIKeyboard> _Keyboard;
        IHIKeyboardEvents::F_OnButtonDown::F_ListenerHandle _KeyboardEventListener_OnButtonDown;
        IHIKeyboardEvents::F_OnButtonUp::F_ListenerHandle _KeyboardEventListener_OnButtonUp;
        
    public:
        F_InputComponentManagerEvents Events;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetInputComponents() const noexcept
        {
            return _InputComponents;
        }
        
    public:
        F_InputComponentManager(const F_ProgramUnitBuildParams& BuildParams);
        ~F_InputComponentManager() override;

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
        void _RegisterInputComponent(const TW_Valid<F_InputComponent>& Component);
        void _UnregisterInputComponent(const TW_Valid<F_InputComponent>& Component);
        
    public:
        void UpdateStates();
        void PostUpdateStates();
    };
}
