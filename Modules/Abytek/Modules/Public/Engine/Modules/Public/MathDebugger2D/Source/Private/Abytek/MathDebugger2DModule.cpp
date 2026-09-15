#include "Abytek/MathDebugger2DModule.hpp"
#include "Abytek/MathRenderer2D.hpp"


namespace Abytek
{
    ABYTEK_REFLECT(F_MathDebugger2DModule)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::F_MathDebugger2DModule"));
    }

    ABYTEK_DEFINE_STATIC_APPLICATION_MODULE(F_MathDebugger2DModule)

    F_MathDebugger2DModule::F_MathDebugger2DModule(const F_ProgramUnitBuildParams& BuildParams) :
        A_ApplicationModule(BuildParams)
    {
        ABYTEK_BIND_STATIC_APPLICATION_MODULE();
        
        AddDependency<F_CoreModule>();
        AddDependency<F_IHIModule>();
        AddDependency<F_RHIModule>();
        AddDependency<F_RenderCoreModule>();
    }
    F_MathDebugger2DModule::~F_MathDebugger2DModule()
    {
    }

    void F_MathDebugger2DModule::OnReflect()
    {
        RegisterStaticType<MathRenderer2D::F_DrawLineBinding>();
        RegisterStaticType<MathRenderer2D::F_DrawLinePipeline>();
    }
}
