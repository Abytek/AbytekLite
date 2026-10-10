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
    
    B8 F_WorldContextDevelopmentData::CheckGeneratedTextFile(
        const TW_Valid<F_SerializableEnvironment>& SerializableEnvironment,
        const F_Name& FileName
    )
    {
        F_Text AbsoluteFilePath;
        if (
            !GetGeneratedTextFileAbsolutePath(
                SerializableEnvironment,
                FileName, 
                AbsoluteFilePath
            )
        )
        {
            return false;
        }
        return H_FSUtilities::Exists(AbsoluteFilePath, E_FSEntryType::FILE);
    }
    F_FeedbackStatus F_WorldContextDevelopmentData::GetGeneratedTextFileAbsolutePath(
        const TW_Valid<F_SerializableEnvironment>& SerializableEnvironment, 
        const F_Name& FileName,
        F_Text& OutAbsolutePath
    )
    {
        auto FilePath = _DirectoryPath + ABYTEK_TEXT("/") + *FileName;
        ABYTEK_FEEDBACK_STATUS_CHECK(
            SerializableEnvironment->ResolveAbsolutePath(FilePath, OutAbsolutePath)
        );
        return F_FeedbackStatus::MakeSucceeded();
    }
    F_FeedbackStatus F_WorldContextDevelopmentData::WriteGeneratedTextFile(
        const TW_Valid<F_SerializableEnvironment>& SerializableEnvironment,
        const F_Name& FileName, 
        const F_Text& Content
    )
    {
        F_Text AbsoluteFilePath;
        ABYTEK_FEEDBACK_STATUS_CHECK(
            GetGeneratedTextFileAbsolutePath(
                SerializableEnvironment,
                FileName, 
                AbsoluteFilePath
            )
        );
        F_Text DirectoryPath = H_Path::GetBaseName(AbsoluteFilePath);
        ABYTEK_FEEDBACK_STATUS_CHECK(
            H_FSUtilities::EnsureDirectory(DirectoryPath)
        );
        ABYTEK_FEEDBACK_STATUS_CHECK(
            H_FSUtilities::WriteFileText(AbsoluteFilePath, Content)
        );
        return F_FeedbackStatus::MakeSucceeded();
    }
    F_FeedbackStatus F_WorldContextDevelopmentData::ReadGeneratedTextFile(
        const TW_Valid<F_SerializableEnvironment>& SerializableEnvironment, 
        const F_Name& FileName,
        F_Text& OutContent
    )
    {
        F_Text AbsoluteFilePath;
        ABYTEK_FEEDBACK_STATUS_CHECK(
            GetGeneratedTextFileAbsolutePath(
                SerializableEnvironment,
                FileName, 
                AbsoluteFilePath
            )
        );
        ABYTEK_FEEDBACK_STATUS_CHECK(
            H_FSUtilities::ReadFileText(AbsoluteFilePath, OutContent)
        );
        return F_FeedbackStatus::MakeSucceeded();
    }
}
#endif