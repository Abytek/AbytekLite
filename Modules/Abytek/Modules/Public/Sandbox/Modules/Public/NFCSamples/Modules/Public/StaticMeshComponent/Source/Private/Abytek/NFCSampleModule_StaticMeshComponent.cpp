#include "Abytek/NFCSampleModule_StaticMeshComponent.hpp"
#include "Abytek/StaticMeshComponentSampleLevel.hpp"
#include "Abytek/SampleSpectatorComponent.hpp"
#include "Abytek/SampleStaticMeshComponent.hpp"


namespace Abytek
{
    ABYTEK_REFLECT(F_NFCSampleModule_StaticMeshComponent)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::F_NFCSampleModule_StaticMeshComponent"));
    }
    
    ABYTEK_DEFINE_STATIC_APPLICATION_MODULE(F_NFCSampleModule_StaticMeshComponent)
    
    F_NFCSampleModule_StaticMeshComponent::F_NFCSampleModule_StaticMeshComponent(const F_ProgramUnitBuildParams& BuildParams) :
        A_ApplicationModule(BuildParams)
    {
        ABYTEK_BIND_STATIC_APPLICATION_MODULE();
    }
    F_NFCSampleModule_StaticMeshComponent::~F_NFCSampleModule_StaticMeshComponent()
    {
    }

    void F_NFCSampleModule_StaticMeshComponent::OnReflect()
    {
        RegisterStaticType<F_StaticMeshComponentSampleLevel>();
        RegisterStaticType<F_SampleSpectatorComponent>();
        RegisterStaticType<F_SampleStaticMeshComponent>();
    }

    void F_NFCSampleModule_StaticMeshComponent::OnStartup()
    {
    }
}
