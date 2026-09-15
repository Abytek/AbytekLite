#include "Abytek/ActorComponents/InputComponentManager.hpp"
#include "Abytek/ActorComponents/InputComponent.hpp"
#include "Abytek/IHIMouse.hpp"
#include "Abytek/IHIMouseManager.hpp"
#include "Abytek/IHIKeyboard.hpp"
#include "Abytek/IHIKeyboardManager.hpp" 


namespace Abytek
{
    ABYTEK_REFLECT(F_InputComponentManager)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::F_InputComponentManager"));
    }
    
    ABYTEK_DEFINE_STATIC_APPLICATION_SUBSYSTEM(F_InputComponentManager)

    F_InputComponentManager::F_InputComponentManager(const F_ProgramUnitBuildParams& BuildParams) :
        A_ApplicationSubsystem(BuildParams)
    {
        ABYTEK_BIND_STATIC_APPLICATION_SUBSYSTEM();
        
        AddDependency<F_WorldManager>();
    }
    F_InputComponentManager::~F_InputComponentManager()
    {
    }

    void F_InputComponentManager::OnConfig()
    {
    }
    void F_InputComponentManager::OnPostConfig()
    {
    }

    void F_InputComponentManager::OnInit() 
    {
        F_InputComponent::GlobalInit();
    }
    void F_InputComponentManager::OnStartup() 
    {
        _Mouse = A_IHIMouseManager::GetInstance()->GetMainMouse();
        _Keyboard = A_IHIKeyboardManager::GetInstance()->GetMainKeyboard();
        
        _MouseEventListener_OnButtonDown = _Mouse->Events.OnButtonDown.AddListener(
            [this](const F_IHIButton& Button, const F_IHIMouseState& CurrentState, const F_IHIMouseState& NewState)
            {
                auto CachedList = _InputComponents;
                for (const auto& InputComponent : CachedList)
                {
                    auto& Buttons_Down = InputComponent->_Buttons_Down;
                    if (
                        auto It = std::find(
                            Buttons_Down.begin(),
                            Buttons_Down.end(),
                            Button
                        );
                        It == Buttons_Down.end()
                    )
                    {
                        Buttons_Down.push_back(Button);
                    }
                }
            }
        );
        _MouseEventListener_OnButtonUp = _Mouse->Events.OnButtonUp.AddListener(
            [this](const F_IHIButton& Button, const F_IHIMouseState& CurrentState, const F_IHIMouseState& NewState)
            {
                auto CachedList = _InputComponents;
                for (const auto& InputComponent : CachedList)
                {
                    auto& Buttons_Up = InputComponent->_Buttons_Up;
                    if (
                        auto It = std::find(
                            Buttons_Up.begin(),
                            Buttons_Up.end(),
                            Button
                        );
                        It == Buttons_Up.end()
                    )
                    {
                        Buttons_Up.push_back(Button);
                    }
                }
            }
        );
        _MouseEventListener_OnMove = _Mouse->Events.OnMove.AddListener(
            [this](const F_Vector2_I32& DeltaPosition, const F_IHIMouseState& CurrentState, const F_IHIMouseState& NewState)
            {
                auto CachedList = _InputComponents;
                for (const auto& InputComponent : CachedList)
                {
                    InputComponent->_DeltaMousePosition = DeltaPosition;
                }
            }
        );
        _MouseEventListener_OnWheel = _Mouse->Events.OnWheel.AddListener(
            [this](I64 Value, const F_IHIMouseState& CurrentState, const F_IHIMouseState& NewState)
            {
                auto CachedList = _InputComponents;
                for (const auto& InputComponent : CachedList)
                {
                    InputComponent->_DeltaMouseWheel = Value;
                }
            }
        );
        
        _KeyboardEventListener_OnButtonDown = _Keyboard->Events.OnButtonDown.AddListener(
            [this](const F_IHIButton& Button, const F_IHIKeyboardState& CurrentState, const F_IHIKeyboardState& NewState)
            {
                auto CachedList = _InputComponents;
                for (const auto& InputComponent : CachedList)
                {
                    auto& Buttons_Down = InputComponent->_Buttons_Down;
                    if (
                        auto It = std::find(
                            Buttons_Down.begin(),
                            Buttons_Down.end(),
                            Button
                        );
                        It == Buttons_Down.end()
                    )
                    {
                        Buttons_Down.push_back(Button);
                    }
                }
            }
        );
        _KeyboardEventListener_OnButtonUp = _Keyboard->Events.OnButtonUp.AddListener(
            [this](const F_IHIButton& Button, const F_IHIKeyboardState& CurrentState, const F_IHIKeyboardState& NewState)
            {
                auto CachedList = _InputComponents;
                for (const auto& InputComponent : CachedList)
                {
                    auto& Buttons_Up = InputComponent->_Buttons_Up;
                    if (
                        auto It = std::find(
                            Buttons_Up.begin(),
                            Buttons_Up.end(),
                            Button
                        );
                        It == Buttons_Up.end()
                    )
                    {
                        Buttons_Up.push_back(Button);
                    }
                }
            }
        );
    }
    void F_InputComponentManager::OnTick()
    {
    }
    void F_InputComponentManager::OnShutdown()
    {
        _Keyboard->Events.OnButtonUp.RemoveListener(_KeyboardEventListener_OnButtonUp);
        _Keyboard->Events.OnButtonDown.RemoveListener(_KeyboardEventListener_OnButtonDown);
        _Mouse->Events.OnButtonUp.RemoveListener(_MouseEventListener_OnButtonUp);
        _Mouse->Events.OnButtonDown.RemoveListener(_MouseEventListener_OnButtonDown);
        
        _Keyboard = {};
        _Mouse = {};
    }
    void F_InputComponentManager::OnRelease() 
    {
        F_InputComponent::GlobalRelease();
    }

    void F_InputComponentManager::_RegisterInputComponent(const TW_Valid<F_InputComponent>& Component)
    {
        _InputComponents.insert(Component);
        Events.OnRegisterComponent.Broadcast(Component);
    }
    void F_InputComponentManager::_UnregisterInputComponent(const TW_Valid<F_InputComponent>& Component)
    {
        Events.OnUnregisterComponent.Broadcast(Component);
        _InputComponents.erase(_InputComponents.find(Component));
    }

    void F_InputComponentManager::UpdateStates()
    {
        auto CachedList = _InputComponents;
        for (const auto& InputComponent : CachedList)
        {
            auto& Buttons_Down = InputComponent->_Buttons_Down;
            auto& Buttons_Pressed = InputComponent->_Buttons_Pressed;
            auto& Buttons_Up = InputComponent->_Buttons_Up;
            
            for (const auto& Button : Buttons_Down)
            {
                if (
                    auto It = std::find(
                        Buttons_Pressed.begin(),
                        Buttons_Pressed.end(),
                        Button
                    );
                    It == Buttons_Pressed.end()
                )
                {
                    Buttons_Pressed.push_back(Button);
                }
            }
        }
    }
    void F_InputComponentManager::PostUpdateStates()
    {
        auto CachedList = _InputComponents;
        for (const auto& InputComponent : CachedList)
        {
            auto& Buttons_Down = InputComponent->_Buttons_Down;
            auto& Buttons_Pressed = InputComponent->_Buttons_Pressed;
            auto& Buttons_Up = InputComponent->_Buttons_Up;
            
            for (const auto& Button : Buttons_Up)
            {
                if (
                    auto It = std::find(
                        Buttons_Pressed.begin(),
                        Buttons_Pressed.end(),
                        Button
                    );
                    It != Buttons_Pressed.end()
                )
                {
                    Buttons_Pressed.erase(It);
                }
            }
            
            InputComponent->_Buttons_Down = {};
            InputComponent->_Buttons_Up = {};
            InputComponent->_DeltaMouseWheel = 0;
            InputComponent->_DeltaMousePosition = F_Vector2_I32::Zero();
        }
    }
}
