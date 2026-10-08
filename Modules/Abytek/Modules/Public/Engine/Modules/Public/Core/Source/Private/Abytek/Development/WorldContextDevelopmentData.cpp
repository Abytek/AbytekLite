#include "Abytek/Development/WorldContextDevelopmentData.hpp"

#include "Abytek/ApplicationCore.hpp"


#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
namespace Abytek
{
    ABYTEK_REFLECT(F_WorldContextDevelopmentData)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::F_WorldContextDevelopmentData"));
        
        ABYTEK_REFLECT_PROPERTY_SERIALIZABLE_DEV(_GUID);
    }
    
    F_WorldContextDevelopmentData::F_WorldContextDevelopmentData(const F_SerializableObjectInitParams& InitParams) :
        A_WorldContext(InitParams)
    {
        if (!HasSerializableFlags(E_SerializableObjectFlag::CDO))
        {
            ABYTEK_ENGINE_NFC_ASSERT(GetPackageName()) << "Requires package name for non-CDO world context development data objects";
        }
    }
    F_WorldContextDevelopmentData::~F_WorldContextDevelopmentData()
    {
    }

    void F_WorldContextDevelopmentData::OnLoad()
    {
        if (!_GUID)
        {
            _GUID = H_UUID::Generate();
        }
        {
            F_Text DirectoryPath;
            DirectoryPath += ABYTEK_TEXT("@") + *A_ApplicationCore::GetInstance()->GetName();
            DirectoryPath += ABYTEK_TEXT("::Intermediate::DevelopmentData:/WorldContexts/");
            DirectoryPath += *_GUID;
            _DirectoryPath = ABYTEK_MOVE(DirectoryPath);
        }
    }
    void F_WorldContextDevelopmentData::OnUnload()
    {
    }

    B8 F_WorldContextDevelopmentData::CanSerialize(const TW_Valid<F_SerializableEnvironment>& Environment) const
    {
        return Environment->HasDevelopmentBuild();
    }
}
#endif