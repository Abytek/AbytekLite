#pragma once

#include "Abytek/SubsystemContainer.hpp"


namespace Abytek
{
    class F_World;

    class ABYTEK_ENGINE_CORE_API F_WorldSubsystemContainer : public F_SubsystemContainer
    {
    public:
        friend class A_WorldSubsystem;
        
    public:
        ABYTEK_BEGIN_REFLECTOR(F_SubsystemContainer)
        ABYTEK_END_REFLECTOR(F_WorldSubsystemContainer);
        
    private:
        TW<F_World> _World;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetWorld() const noexcept
        {
            return _World;
        }
        
    public:
        F_WorldSubsystemContainer(const TW_Valid<F_World>& World);
        ~F_WorldSubsystemContainer() override;
        
    protected:
        F_FeedbackStatus ValidateUnit(const TW_Valid<F_ProgramUnit>& Unit) override;
        
    private:
        void _TickUnits();
        void _PostTickUnits();
    };
}
