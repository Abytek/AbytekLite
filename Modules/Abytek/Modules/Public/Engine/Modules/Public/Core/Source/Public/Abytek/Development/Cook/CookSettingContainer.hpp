#pragma once

#include "Abytek/ProgramUtilities/ProgramUnitContainer.hpp"


#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
namespace Abytek
{
    class F_CookProfile;
    
    class ABYTEK_ENGINE_CORE_API F_CookSettingContainer : public F_ProgramUnitContainer
    {
    public:
        ABYTEK_BEGIN_REFLECTOR(F_ProgramUnitContainer)
        ABYTEK_END_REFLECTOR(F_CookSettingContainer)
        
    public:
        friend class F_CookProfile;
        
    public:
        static TW<F_CookSettingContainer> GetMain();

    private:
        TW<F_CookProfile> _Profile;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetProfile() const noexcept
        {
            return _Profile;
        }
        
    public:
        F_CookSettingContainer(const F_ProgramUnitContainerBuildParams& BuildParams);
        ~F_CookSettingContainer() override;
        
    protected:
        F_FeedbackStatus ValidateUnit(const TW_Valid<F_ProgramUnit>& Unit) override;
    };
}
#endif