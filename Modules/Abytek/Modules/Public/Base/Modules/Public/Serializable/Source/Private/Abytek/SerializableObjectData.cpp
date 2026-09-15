#include "Abytek/SerializableObjectData.hpp"
#include "Abytek/SerializableObject.hpp"


namespace Abytek
{
    TS<A_SerializableObject> F_SerializableObjectHeader::CreateObject(
        const TW_Valid<F_SerializableEnvironment>& Environment,
        const TS<F_SerializablePackage>& Package    
    ) const
    {
        F_Name Name;
        F_Name PackageName;
        ABYTEK_FEEDBACK_STATUS_CHECK_HARD(
            F_SerializableEnvironment::ParseObjectPath(
                Path,
                Name, 
                PackageName
            ) 
        );
        
        F_SerializableObjectInitParams InitParams;
        InitParams.Type = Type;
        InitParams.Environment = Environment;
        InitParams.Package = Package;
        InitParams.Name = Name;
        InitParams.PackageName = PackageName;
        
        const auto& Metadata = Type->GetMetadata();
        const auto& MetadataElement = Metadata.Get(A_SerializableObject::GetMetadataElementName_Creator());
        const auto& Creator = AnyCast<A_SerializableObject::F_Creator>(MetadataElement);
        return Creator(InitParams);
    }
}
