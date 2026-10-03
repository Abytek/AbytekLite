#include "Abytek/SRPModule.hpp"
#include "Abytek/SRPSubsystem.hpp"
#include "Abytek/SRPRenderPath.hpp"
#include "Abytek/SRPBasicDrawers/Cube.hpp"
#include "Abytek/SRPBasicDrawers/StaticMesh.hpp"
#include "Abytek/SRPVisibilityBuffer.hpp"
#include "Abytek/SimplePrimitive/SRPVisibilityBufferPass_Simple.hpp"
#include "Abytek/SimplePrimitive/SRPColorPass_Simple.hpp"


namespace Abytek
{
    ABYTEK_REFLECT(F_SRPModule)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::F_SRPModule"));
    }
    
    ABYTEK_DEFINE_STATIC_APPLICATION_MODULE(F_SRPModule);
    
    F_SRPModule::F_SRPModule(const F_ProgramUnitBuildParams& BuildParams) :
        A_ApplicationModule(BuildParams)
    {
        ABYTEK_BIND_STATIC_APPLICATION_MODULE();
        
        AddDependency<F_CoreModule>();
        AddDependency<F_IHIModule>();
        AddDependency<F_RHIModule>();
        AddDependency<F_RenderCoreModule>();
        AddDependency<F_NFCModule>();
    }
    F_SRPModule::~F_SRPModule()
    {
    }

    void F_SRPModule::OnReflect()
    {
        RegisterStaticType<F_SRPSubsystem>();
        RegisterStaticType<F_SRPRenderPath>();
        RegisterStaticType<SRPBasicDrawers::F_CubeBinding>();
        RegisterStaticType<SRPBasicDrawers::F_CubePipeline>();
        RegisterStaticType<SRPBasicDrawers::F_StaticMeshBinding>();
        RegisterStaticType<SRPBasicDrawers::F_StaticMeshPipeline>();
        RegisterStaticType<SRP::VisibilityBuffer::F_DemoBinding>();
        RegisterStaticType<SRP::VisibilityBuffer::F_DemoPipeline>();
        RegisterStaticType<SRP::SimplePrimitive::VisibilityBufferPass::F_Binding>();
        RegisterStaticType<SRP::SimplePrimitive::VisibilityBufferPass::F_Pipeline>();
        RegisterStaticType<SRP::SimplePrimitive::ColorPass::F_Binding>();
        RegisterStaticType<SRP::SimplePrimitive::ColorPass::F_Pipeline>();
    }
}
