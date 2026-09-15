#pragma once

#include "Abytek/Engine.IHI.prerequisites.hpp"
#include "Abytek/IHIMouseCommon.hpp"


namespace Abytek
{
    class ABYTEK_ENGINE_IHI_API A_IHIMouse : public A_RAObject
    {
    public:
        friend class A_IHIMouseManager;
        
    private:
        F_IHIMouseState _CurrentState;
        F_IHIMouseState _NextState;
        
    public:
        F_IHIMouseEvents Events;

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
        ABYTEK_FORCE_INLINE I64 GetDeltaWheel() const noexcept
        {
            return _CurrentState.DeltaWheel;
        }
        ABYTEK_FORCE_INLINE const auto& GetDeltaPosition() const noexcept
        {
            return _CurrentState.DeltaPosition;
        }

    public:
        ABYTEK_RA_DECLARE_OBJECT_CREATABLE(A_IHIMouse);
        virtual void Build();
        virtual void Release() override;
        
    public:
        virtual void FinalizeNextState();
        virtual void UpdateCurrentState();
    };
}
