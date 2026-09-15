#include "Abytek/Development/Cook/CookSetting.hpp"
#include "Abytek/Development/Cook/CookSettingContainer.hpp"


#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
namespace Abytek
{
    ABYTEK_REFLECT(A_CookSetting)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::A_CookSetting"));
    }
    
    A_CookSetting::A_CookSetting(const F_ProgramUnitBuildParams& BuildParams) :
        F_ProgramUnit(BuildParams),
        _Profile(BuildParams.Container.FastCast<F_CookSettingContainer>()->GetProfile())
    {
    }
    A_CookSetting::~A_CookSetting()
    {
    }
}
#endif