#pragma once

#include "Abytek/Actor/ActorComponent.hpp"
#include "Abytek/IHIButton.hpp"


namespace Abytek
{
    class ABYTEK_ENGINE_NFC_API F_InputComponent : public A_ActorComponent
    {
    public:
        ABYTEK_BEGIN_REFLECTOR(A_ActorComponent)
        ABYTEK_END_REFLECTOR(F_InputComponent);
        
    public:
        friend class F_InputComponentManager;
        
    public:
        static F_Name GetUpdateStatesUpdateFunctionName();
        static F_Name GetPostUpdateStatesUpdateFunctionName();
        static void GlobalInit();
        static void GlobalRelease();
    
    private:
        TF_Vector<F_IHIButton> _Buttons_Down;
        TF_Vector<F_IHIButton> _Buttons_Pressed;
        TF_Vector<F_IHIButton> _Buttons_Up;
        I64 _DeltaMouseWheel = 0;
        F_Vector2_I32 _DeltaMousePosition = F_Vector2_I32::Zero();
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetButtons_Down() const noexcept
        {
            return _Buttons_Down;
        }
        ABYTEK_FORCE_INLINE const auto& GetButtons_Pressed() const noexcept
        {
            return _Buttons_Pressed;
        }
        ABYTEK_FORCE_INLINE const auto& GetButtons_Up() const noexcept
        {
            return _Buttons_Up;
        }
        ABYTEK_FORCE_INLINE auto GetDeltaMouseWheel() const noexcept
        {
            return _DeltaMouseWheel;
        }
        ABYTEK_FORCE_INLINE const auto& GetDeltaMousePosition() const noexcept
        {
            return _DeltaMousePosition;
        }
        
    public:
        F_InputComponent(const F_SerializableObjectInitParams& InitParams);
        ~F_InputComponent() override;
        
    protected:
        void OnRegisterComponent() override;
        void OnUnregisterComponent() override;
        
    public:
        B8 IsButtonDown(const F_IHIButton& Button) const;
        B8 IsButtonPressed(const F_IHIButton& Button) const;
        B8 IsButtonUp(const F_IHIButton& Button) const;
    };
}