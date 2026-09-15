#pragma once

#include "Abytek/Engine.IHI.prerequisites.hpp"
#include "Abytek/IHIKeyboardCommon.hpp"


namespace Abytek
{
    class ABYTEK_ENGINE_IHI_API A_IHIKeyboard : public A_RAObject
    {
    public:
        friend class A_IHIKeyboardManager;
        
    private:
        F_IHIKeyboardState _CurrentState;
        F_IHIKeyboardState _NextState;
        
    public:
        F_IHIKeyboardEvents Events;

    public:
        ABYTEK_FORCE_INLINE const auto& GetCurrentState() const noexcept
        {
            return _CurrentState;
        }
        ABYTEK_FORCE_INLINE auto& InjectCurrentState() noexcept
        {
            return _CurrentState;
        }
        ABYTEK_FORCE_INLINE const auto& GetNextState() const noexcept
        {
            return _NextState;
        }
        ABYTEK_FORCE_INLINE auto& InjectNextState() noexcept
        {
            return _NextState;
        }

        ABYTEK_FORCE_INLINE auto WillBeDestroyed() const noexcept
        {
            return _CurrentState.WillBeDestroyed;
        }
        ABYTEK_FORCE_INLINE const auto& GetPressedButtons() const noexcept
        {
            return _CurrentState.PressedButtons;
        }
        ABYTEK_FORCE_INLINE const auto& GetTextChars() const noexcept
        {
            return _CurrentState.TextChars;
        }

    public:
        ABYTEK_RA_DECLARE_OBJECT_CREATABLE(A_IHIKeyboard);
        virtual void Build();
        virtual void Release() override;
        
    public:
        virtual void FinalizeNextState();
        virtual void UpdateCurrentState();
    };
}
