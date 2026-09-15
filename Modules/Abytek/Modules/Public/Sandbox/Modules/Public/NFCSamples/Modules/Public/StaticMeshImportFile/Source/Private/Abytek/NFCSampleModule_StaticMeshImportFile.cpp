#include "Abytek/NFCSampleModule_StaticMeshImportFile.hpp"
#include "Abytek/Assets/StaticMesh.hpp"
#include "Abytek/Frame/FrameHelper.hpp"


namespace Abytek
{
    ABYTEK_REFLECT(F_NFCSampleModule_StaticMeshImportFile)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::F_NFCSampleModule_StaticMeshImportFile"));
    }
    
    ABYTEK_DEFINE_STATIC_APPLICATION_MODULE(F_NFCSampleModule_StaticMeshImportFile)
    
    F_NFCSampleModule_StaticMeshImportFile::F_NFCSampleModule_StaticMeshImportFile(const F_ProgramUnitBuildParams& BuildParams) :
        A_ApplicationModule(BuildParams)
    {
        ABYTEK_BIND_STATIC_APPLICATION_MODULE();
    }
    F_NFCSampleModule_StaticMeshImportFile::~F_NFCSampleModule_StaticMeshImportFile()
    {
    }

    void F_NFCSampleModule_StaticMeshImportFile::OnReflect()
    {
    }

    void F_NFCSampleModule_StaticMeshImportFile::OnStartup()
    {
        {
            TS<F_StaticMesh> StaticMesh;
            if (
                H_Serializable::PopulateObject<F_StaticMesh>(
                    StaticMesh,
                    ABYTEK_NAME("Cube"),
                    ABYTEK_NAME("@Abytek.Sandbox.NFCSamples.StaticMeshImportFile::Assets:/.IgnoreSVC/Cube")
                )
            )
            {
                StaticMesh->Import(ABYTEK_TEXT("@Abytek.Sandbox.NFCSamples.StaticMeshImportFile::Assets:/Cube.fbx"));
                StaticMesh->GetPackage()->Save();
            }
        }
        
        H_Frame::EnqueueCommand<E_FrameParamType::RENDER>(
            []
            {
                H_RHI::SignalCapture();
            }
        );
    }
}
