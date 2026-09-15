#include "Abytek/AssetsModule.hpp"
#include "Abytek/AssetSubsystem.hpp"
#include "Abytek/Asset.hpp"
#include "Abytek/AssetCooker.hpp"
#include "Abytek/AssetFactory.hpp"
#include "Abytek/TextAsset.hpp"
#include "Abytek/TextAssetFactory.hpp"


namespace Abytek
{
    ABYTEK_REFLECT(F_AssetsModule)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::F_AssetsModule"));
    }
    
    ABYTEK_DEFINE_STATIC_APPLICATION_MODULE(F_AssetsModule)
    
    F_AssetsModule::F_AssetsModule(const F_ProgramUnitBuildParams& BuildParams) :
        A_ApplicationModule(BuildParams)
    {
        ABYTEK_BIND_STATIC_APPLICATION_MODULE();
        
        AddDependency<F_CoreModule>();
    }
    F_AssetsModule::~F_AssetsModule()
    {
    }

    void F_AssetsModule::OnReflect()
    {
        RegisterStaticType<F_AssetSubsystem>();
        RegisterStaticType<A_Asset>();
        RegisterStaticType<A_AssetFactory>();
        RegisterStaticType<F_TextAsset>();
        RegisterStaticType<F_TextAssetFactory>();
        RegisterStaticType<A_AssetCooker>();
    }
}
