#pragma once

#include "Abytek/ProgramUtilities/ProgramUnit.hpp"
#include "Abytek/ProgramUtilities/ProgramUnitContainer.hpp"


#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
namespace Abytek
{
#define ABYTEK_DECLARE_STATIC_COOK_SETTING(...) ABYTEK_DECLARE_STATIC_PROGRAM_UNIT(__VA_ARGS__)
#define ABYTEK_DEFINE_STATIC_COOK_SETTING(...) ABYTEK_DEFINE_STATIC_PROGRAM_UNIT(Abytek::ToText(Abytek::TypeFullName<__VA_ARGS__>()), __VA_ARGS__)
    
    class F_CookProfile;
    
    class ABYTEK_ENGINE_CORE_API A_CookSetting : public F_ProgramUnit
    {
    public:
        ABYTEK_BEGIN_REFLECTOR(F_ProgramUnit)
        ABYTEK_END_REFLECTOR(A_CookSetting);

    private:
        TW<F_CookProfile> _Profile;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetProfile() const noexcept
        {
            return _Profile;
        }
        
    protected:
        A_CookSetting(const F_ProgramUnitBuildParams& BuildParams);
        
    public:
        ~A_CookSetting() override;
    };
}
#endif