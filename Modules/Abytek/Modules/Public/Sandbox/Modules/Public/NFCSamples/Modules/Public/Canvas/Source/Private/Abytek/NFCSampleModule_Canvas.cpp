#include "Abytek/NFCSampleModule_Canvas.hpp"
#include "Abytek/CanvasSampleLevel.hpp"


namespace Abytek
{
    ABYTEK_REFLECT(F_NFCSampleModule_Canvas)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::F_NFCSampleModule_Canvas"));
    }
    
    ABYTEK_DEFINE_STATIC_APPLICATION_MODULE(F_NFCSampleModule_Canvas)
    
    F_NFCSampleModule_Canvas::F_NFCSampleModule_Canvas(const F_ProgramUnitBuildParams& BuildParams) :
        A_ApplicationModule(BuildParams)
    {
        ABYTEK_BIND_STATIC_APPLICATION_MODULE();
    }
    F_NFCSampleModule_Canvas::~F_NFCSampleModule_Canvas()
    {
    }

    void F_NFCSampleModule_Canvas::OnReflect()
    {
        RegisterStaticType<F_CanvasSampleLevel>();
    }

    void F_NFCSampleModule_Canvas::OnStartup()
    {
    }
}
