#include "Abytek/SerializablePackage.hpp"
#include "Abytek/SerializableObject.hpp"


namespace Abytek
{
    ABYTEK_REFLECT(F_SerializablePackage)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::F_SerializablePackage"));    
    }

    F_Name F_SerializablePackage::GetFileExtension()
    {
        return ABYTEK_NAME("apkg");
    }

    F_SerializablePackage::F_SerializablePackage(const TW_Valid<F_SerializableEnvironment>& Environment, const F_Name& Name) :
        _Environment(Environment),
        _Name(Name)
    {
        ABYTEK_BASE_SERIALIZABLE_ASSERT(_Name) << "Invalid package name";
        ABYTEK_FEEDBACK_STATUS_CHECK_HARD(
            ResolveAbsolutePath(_Environment, _Name, _AbsolutePath)    
        );
        
        _Environment->_RegisterPackage(ABYTEK_WTHIS());
        
        CriticalSection(
            [this]
            {
                _InitialLoad();
            }
        );
    }
    F_SerializablePackage::~F_SerializablePackage()
    {
        _Environment->_UnregisterPackage(ABYTEK_WTHIS());
    }

    void F_SerializablePackage::_RegisterObject(const TW_Valid<A_SerializableObject>& Object)
    {
        _Objects.insert({ Object->GetName(), Object });
    }
    void F_SerializablePackage::_UnregisterObject(const TW_Valid<A_SerializableObject>& Object)
    {
        _Objects.erase(_Objects.find(Object->GetName()));
    }

    TS<A_FSFile> F_SerializablePackage::_OpenFile(B8 EnableWrite, const TW<F_SerializableEnvironment>& Environment)
    {
        auto ActualEnvironment = Environment;
        if (!ActualEnvironment)
        {
            ActualEnvironment = _Environment;
        }
        
        F_Text AbsolutePath;
        ABYTEK_FEEDBACK_STATUS_CHECK_HARD(
            ResolveAbsolutePath(ActualEnvironment, _Name, AbsolutePath)    
        );
        
        if (EnableWrite)
        {
            F_Text DirectoryPath = H_Path::GetBaseName(AbsolutePath);
            H_FSUtilities::EnsureDirectory(DirectoryPath);
        }
        
        F_FSFileOpenParams Desc;
        Desc.Path = AbsolutePath;
        Desc.AutoCreate = EnableWrite;
        Desc.ShareFlags = EnableWrite ? E_FSFileShareFlag::WRITE : E_FSFileShareFlag::READ;
        TS<A_FSFile> File;
        if (!H_FSUtilities::OpenFile(Desc, File))
        {
            return {};
        }
        return File;
    }
    void F_SerializablePackage::_InitialLoad()
    {
        _LastObjectHeaders = {};
        
        if (!H_FSUtilities::Exists(_AbsolutePath, E_FSEntryType::FILE))
        {
            return;
        }
        
        auto File = _OpenFile(false);
        ABYTEK_BASE_SERIALIZABLE_ASSERT(File) << "Cannot open file for package: " << _Name;

        File->Read<F_SerializablePackageHeader>(_LastHeader);

        TF_Vector<U8> ObjectHeaderBytes;
        ObjectHeaderBytes.resize(_LastHeader.ObjectHeaderRange_SizeInBytes);
        ABYTEK_FEEDBACK_STATUS_CHECK_HARD(File->Seek(_LastHeader.ObjectHeaderRange_OffsetInBytes));
        ABYTEK_FEEDBACK_STATUS_CHECK_HARD(File->ReadBinary(ObjectHeaderBytes));
        
        F_Archive ObjectHeaderArchive = F_Archive::From(ObjectHeaderBytes);
        F_ArchiveReadOnlyView ObjectHeaderArchiveView = F_ArchiveReadOnlyView::From(ObjectHeaderArchive);
        ObjectHeaderArchiveView.HasDevelopmentBuild = _Environment->HasDevelopmentBuild();
        
        ABYTEK_FEEDBACK_STATUS_CHECK_HARD(ObjectHeaderArchiveView >> _LastObjectHeaders);
    }

    void F_SerializablePackage::CriticalSection(TF_Function<void()>&& Callback)
    {
        _CriticalSection(ABYTEK_FORWARD(Callback));
    }
    void F_SerializablePackage::LoadObjectPayload(const F_SerializableObjectHeader& ObjectHeader, TF_Vector<U8>& OutPayload)
    {
        return LoadMainPayload(
            ObjectHeader.PayloadOffsetInBytes,
            ObjectHeader.PayloadSizeInBytes,
            OutPayload
        );
    }
    void F_SerializablePackage::LoadMainPayload(U64 OffsetInBytes, U64 SizeInBytes, TF_Vector<U8>& OutPayload)
    {
        auto File = _OpenFile(false);
        ABYTEK_BASE_SERIALIZABLE_ASSERT(File) << "Cannot open package: " << _Name;
        
        TF_Vector<U8> Bytes;
        Bytes.resize(SizeInBytes);
        ABYTEK_FEEDBACK_STATUS_CHECK_HARD(File->Seek(_LastHeader.MainPayloadRange_OffsetInBytes + OffsetInBytes));
        ABYTEK_FEEDBACK_STATUS_CHECK_HARD(File->ReadBinary(Bytes));
        
        OutPayload = ABYTEK_MOVE(Bytes);
    }
    void F_SerializablePackage::LoadBulkPayload(U64 OffsetInBytes, U64 SizeInBytes, TF_Vector<U8>& OutPayload)
    {
        auto File = _OpenFile(false);
        ABYTEK_BASE_SERIALIZABLE_ASSERT(File) << "Cannot open package: " << _Name;
        
        TF_Vector<U8> Bytes;
        Bytes.resize(SizeInBytes);
        ABYTEK_FEEDBACK_STATUS_CHECK_HARD(File->Seek(_LastHeader.BulkPayloadRange_OffsetInBytes + OffsetInBytes));
        ABYTEK_FEEDBACK_STATUS_CHECK_HARD(File->ReadBinary(Bytes));
        
        OutPayload = ABYTEK_MOVE(Bytes);
    }
    void F_SerializablePackage::Save(const TW<F_SerializableEnvironment>& Environment)
    {
        auto ActualEnvironment = Environment;
        if (!ActualEnvironment)
        {
            ActualEnvironment = _Environment;
        }
        
        auto Objects = GetOrLoadAllObjects();
        
        for (const auto& Object : Objects)
        {
            Object->OnPrepareForSaving(ActualEnvironment);
        }
            
        TF_Set<F_SerializableObjectHeader> ObjectHeaders;
        
        F_Archive MainPayloadArchive;
        F_Archive BulkPayloadArchive;
        F_SerializableObjectSerializeParams ObjectSerializeParams;
        ObjectSerializeParams.MainView = F_ArchiveReadWriteView::From(MainPayloadArchive);
        ObjectSerializeParams.MainView.HasDevelopmentBuild = ActualEnvironment->HasDevelopmentBuild();
        ObjectSerializeParams.BulkView = F_ArchiveReadWriteView::From(BulkPayloadArchive);
        ObjectSerializeParams.BulkView.HasDevelopmentBuild = ActualEnvironment->HasDevelopmentBuild();
        ObjectSerializeParams.Environment = _Environment;
        for (const auto& Object : Objects)
        {
            const auto& ObjectType = Object->GetType();
            const auto& Metadata = ObjectType->GetMetadata();
            const auto& MetadataElement = Metadata.Get(A_SerializableObject::GetMetadataElementName_GenerateHeaderFunction());
            const auto& GenerateHeaderFunction = AnyCast<A_SerializableObject::F_GenerateHeaderFunction>(MetadataElement);
            
            F_SerializableObjectHeader ObjectHeader;
            ObjectHeader = GenerateHeaderFunction(Object.Weak());
            ObjectSerializeParams.MainView.Shift<F_ArchiveData>(0); // Ensure object payload alignment.
            ObjectHeader.PayloadOffsetInBytes =  ObjectSerializeParams.MainView.Offset;
            ABYTEK_FEEDBACK_STATUS_CHECK_HARD(
                Object->Serialize( ObjectSerializeParams)
            );
            ObjectHeader.PayloadSizeInBytes =  ObjectSerializeParams.MainView.Offset - ObjectHeader.PayloadOffsetInBytes;
            
            ObjectHeaders.insert(ObjectHeader);
        }
        
        F_Archive ObjectHeaderArchive;
        F_ArchiveReadWriteView ObjectHeaderArchiveView = F_ArchiveReadWriteView::From(ObjectHeaderArchive);
        ObjectHeaderArchiveView.HasDevelopmentBuild = ActualEnvironment->HasDevelopmentBuild();
        ABYTEK_FEEDBACK_STATUS_CHECK_HARD(ObjectHeaderArchiveView << ObjectHeaders);
        
        auto File = _OpenFile(true, ActualEnvironment);
        ABYTEK_BASE_SERIALIZABLE_ASSERT(File) << "Cannot open package: " << _Name;
        
        F_SerializablePackageHeader PackageHeader;
        PackageHeader.ObjectHeaderRange_OffsetInBytes = sizeof(F_SerializablePackageHeader);
        PackageHeader.ObjectHeaderRange_SizeInBytes = ObjectHeaderArchiveView.GetSize();
        PackageHeader.MainPayloadRange_OffsetInBytes = PackageHeader.ObjectHeaderRange_OffsetInBytes + PackageHeader.ObjectHeaderRange_SizeInBytes;
        PackageHeader.MainPayloadRange_SizeInBytes = MainPayloadArchive.GetSize();
        PackageHeader.BulkPayloadRange_OffsetInBytes = PackageHeader.MainPayloadRange_OffsetInBytes + PackageHeader.MainPayloadRange_SizeInBytes;
        PackageHeader.BulkPayloadRange_SizeInBytes = BulkPayloadArchive.GetSize();
        Sz PackageSizeInBytes = PackageHeader.BulkPayloadRange_OffsetInBytes + PackageHeader.BulkPayloadRange_SizeInBytes;
        
        ABYTEK_FEEDBACK_STATUS_CHECK_HARD(File->Truncate(PackageSizeInBytes));
        ABYTEK_FEEDBACK_STATUS_CHECK_HARD(File->Seek(0));
        
        ABYTEK_FEEDBACK_STATUS_CHECK_HARD(
            File->Write<F_SerializablePackageHeader>(PackageHeader)
        );
        ABYTEK_FEEDBACK_STATUS_CHECK_HARD(
            File->WriteBinary(ObjectHeaderArchiveView.GetSpan<const U8>())    
        )
        ABYTEK_FEEDBACK_STATUS_CHECK_HARD(
            File->WriteBinary(ObjectSerializeParams.MainView.GetSpan<const U8>())    
        )
        ABYTEK_FEEDBACK_STATUS_CHECK_HARD(
            File->WriteBinary(ObjectSerializeParams.BulkView.GetSpan<const U8>())    
        )
        
        for (const auto& Object : Objects)
        {
            Object->OnCleanUpAfterSaving(ActualEnvironment);
        }
        
        if (Environment == GetEnvironment())
        {
            _LastHeader = ABYTEK_MOVE(PackageHeader);
            _LastObjectHeaders = ABYTEK_MOVE(ObjectHeaders);
        
            _IsDirty = false;
        }
    }

    B8 F_SerializablePackage::SearchLastObjectHeader(const F_Name& ObjectPath, F_SerializableObjectHeader& OutObjectHeader) const
    {
        F_SerializableObjectHeader X;
        X.Path = ObjectPath;
        auto It = _LastObjectHeaders.find(X);
        if (It != _LastObjectHeaders.end())
        {
            OutObjectHeader = *It;
            return true;
        }
        return false;
    }

    F_FeedbackStatus F_SerializablePackage::ResolveAbsolutePath(const TW_Valid<F_SerializableEnvironment>& Environment, const F_Name& Name, F_Text& OutAbsolutePath)
    {
        if (!Name)
        {
            return F_FeedbackStatus::MakeFailed(ABYTEK_TEXT("Invalid name"));
        }
        F_Text Result;
        ABYTEK_FEEDBACK_STATUS_CHECK(
            Environment->ResolveAbsolutePath(*Name, Result)    
        );
        Result += ABYTEK_TEXT(".") + *GetFileExtension();
        OutAbsolutePath = ABYTEK_MOVE(Result);
        return F_FeedbackStatus::MakeSucceeded();
    }

    B8 F_SerializablePackage::MarkDirty()
    {
        auto Result = _IsDirty;
        _IsDirty = true;
        return Result;
    }
    void F_SerializablePackage::SaveIfDirty()
    {
        if (IsDirty())
        {
            Save();       
        }
    }

    void F_SerializablePackage::GetOrLoadAllObjects(TF_Map<F_Name, TS<A_SerializableObject>>& OutObjects)
    {
        for (const auto& [ ObjectName, Object ] : _Objects)
        {
            OutObjects.insert({ ObjectName, ShareObject(Object) });
        }
        {
            TF_SmallVector<F_SerializableObjectCreationParams, 1> ObjectCreationParamsList;
            for (const auto& LastObjectHeader : _LastObjectHeaders)
            {
                F_Name ObjectName;
                F_Name PackageName;
                ABYTEK_FEEDBACK_STATUS_CHECK_HARD(
                    F_SerializableEnvironment::ParseObjectPath(LastObjectHeader.Path, ObjectName, PackageName)  
                );
                
                if (OutObjects.find(ObjectName) != OutObjects.end())
                {
                    continue;
                }
                
                F_SerializableObjectCreationParams ObjectCreationParams;
                ObjectCreationParams.Name = ObjectName;
                ObjectCreationParams.PackageName = PackageName;
                ObjectCreationParams.Type = LastObjectHeader.Type;
                ObjectCreationParamsList.push_back(ObjectCreationParams);
            }
            TF_SmallVector<TS<A_SerializableObject>, 1> LastObjects;
            _Environment->CreateObjects(
                ObjectCreationParamsList,
                LastObjects
            );
            for (const auto& LastObjectHeader : _LastObjectHeaders)
            {
                F_Name ObjectName;
                F_Name PackageName;
                ABYTEK_FEEDBACK_STATUS_CHECK_HARD(
                    F_SerializableEnvironment::ParseObjectPath(LastObjectHeader.Path, ObjectName, PackageName)  
                );
                
                if (OutObjects.find(ObjectName) != OutObjects.end())
                {
                    continue;
                }
                
                OutObjects.insert({
                    ObjectName,
                    ShareObject(_Environment->FindObject(ObjectName))
                });
            }
        }
    }
    TF_Vector<TS<A_SerializableObject>> F_SerializablePackage::GetOrLoadAllObjects()
    {
        TF_Vector<TS<A_SerializableObject>> Result;
        TF_Map<F_Name, TS<A_SerializableObject>> Objects; 
        GetOrLoadAllObjects(Objects);
        for (const auto& [ ObjectName, Object ] : Objects)
        {
            Result.push_back(Object);
        }
        return ABYTEK_MOVE(Result);
    }
}
