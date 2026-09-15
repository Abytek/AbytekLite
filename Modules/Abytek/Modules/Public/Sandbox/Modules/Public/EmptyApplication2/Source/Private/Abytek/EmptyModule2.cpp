#include "Abytek/EmptyModule2.hpp"


namespace Abytek
{
    ABYTEK_REFLECT(F_EmptyModule2)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::F_EmptyModule2"));
    }
    
    ABYTEK_DEFINE_STATIC_APPLICATION_MODULE(F_EmptyModule2)
    
    F_EmptyModule2::F_EmptyModule2(const F_ProgramUnitBuildParams& BuildParams) :
        A_ApplicationModule(BuildParams)
    {
        ABYTEK_BIND_STATIC_APPLICATION_MODULE();
    }
    F_EmptyModule2::~F_EmptyModule2()
    {
    }

    void F_EmptyModule2::OnReflect()
    {
    }
}