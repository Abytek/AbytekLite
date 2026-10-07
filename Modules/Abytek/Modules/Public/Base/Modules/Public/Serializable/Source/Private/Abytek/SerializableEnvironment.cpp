#include "Abytek/SerializableEnvironment.hpp"
#include "Abytek/SerializableObject.hpp"
#include "Abytek/SerializablePackage.hpp"


namespace Abytek
{
    ABYTEK_REFLECT(F_SerializableEnvironment)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::F_SerializableEnvironment"));
    }
    
    F_SerializableEnvironment::F_SerializableEnvironment(const F_SerializableEnvironmentBuildParams& BuildParams) :
        _Owner(BuildParams.Owner),
        _HasDevelopmentBuild(BuildParams.HasDevelopmentBuild)
    {
    }
    F_SerializableEnvironment::~F_SerializableEnvironment()
    {
    }

    void F_SerializableEnvironment::_OnAddCDOType(const TF_ReflectionTypeHandle<A_SerializableObject>& Type)
    {
        _CDOs[Type] = CreateCDO(Type);
    }
    void F_SerializableEnvironment::_OnRemoveCDOType(const TF_ReflectionTypeHandle<A_SerializableObject>& Type)
    {
        _CDOs.erase(_CDOs.find(Type));
    }

    void F_SerializableEnvironment::_RegisterObject(const TW_Valid<A_SerializableObject>& Object)
    {
        _Objects.insert({ Object->GetName(), Object });
    }
    void F_SerializableEnvironment::_UnregisterObject(const TW_Valid<A_SerializableObject>& Object)
    {
        _Objects.erase(_Objects.find(Object->GetName()));
    }

    void F_SerializableEnvironment::_RegisterPackage(const TW_Valid<F_SerializablePackage>& Package)
    {
        _Packages.insert({ Package->GetName(), Package });
    }
    void F_SerializableEnvironment::_UnregisterPackage(const TW_Valid<F_SerializablePackage>& Package)
    {
        _Packages.erase(_Packages.find(Package->GetName()));
    }

    F_FeedbackStatus F_SerializableEnvironment::ParseMountablePath(const F_Text& Raw, F_Name& OutModuleName, F_Text& OutPath)
    {
        if (Raw.empty() || Raw[0] != ABYTEK_TEXT('@'))
        {
            OutModuleName = {};
            OutPath = Raw;
            return F_FeedbackStatus::MakeSucceeded();
        }

        const auto SeparatorPos = Raw.find(ABYTEK_TEXT(":/"));
        if (SeparatorPos == F_Text::npos || SeparatorPos <= 1)
        {
            OutModuleName = {};
            OutPath = Raw;
            return F_FeedbackStatus::MakeSucceeded();
        }

        OutModuleName = F_Name(Raw.substr(1, SeparatorPos - 1));
        OutPath = Raw.substr(SeparatorPos + 2);
        return F_FeedbackStatus::MakeSucceeded();
    }
    F_FeedbackStatus F_SerializableEnvironment::ParseObjectPath(
        const F_Name& Raw,
        F_Name& OutObjectName,
        F_Name& OutPackageName
    )
    {
        const auto& RawText = *Raw;
        OutPackageName = {};
        OutObjectName = {};

        const auto SeparatorPos = RawText.rfind(ABYTEK_TEXT("::"));
        if (SeparatorPos == F_Text::npos)
        {
            OutObjectName = Raw;
            OutPackageName = {};
            return F_FeedbackStatus::MakeSucceeded();
        }

        if (SeparatorPos + 2 >= RawText.size())
        {
            return F_FeedbackStatus::MakeFailed(
                ABYTEK_TEXT("Object name is empty."));
        }

        OutPackageName = F_Name(H_Path::Normalize(RawText.substr(0, SeparatorPos)));
        OutObjectName = F_Name(RawText.substr(SeparatorPos + 2));

        return F_FeedbackStatus::MakeSucceeded();
    }
    F_Name F_SerializableEnvironment::MakeObjectPath(const F_Name& ObjectName, const F_Name& PackageName)
    {
        return *PackageName + ABYTEK_TEXT("::") + *ObjectName;
    }
    
    F_FeedbackStatus F_SerializableEnvironment::ResolveAbsolutePath(const F_Text& Raw, F_Text& OutAbsolutePath)
    {
        F_Name ModuleName;
        F_Text Path;
        ABYTEK_FEEDBACK_STATUS_CHECK(ParseMountablePath(Raw, ModuleName, Path));
        TF_Optional<F_Text> BaseDirectoryPath;
        if (ModuleName)
        {
            if (!HasMount(ModuleName))
            {
                return F_FeedbackStatus::MakeFailed(
                    ABYTEK_TEXT("Not found module: ")
                    + *ModuleName
                );
            }
            BaseDirectoryPath = GetMount(ModuleName);
        }
        if (BaseDirectoryPath)
        {
            OutAbsolutePath = *BaseDirectoryPath + ABYTEK_TEXT("/") + Path;
        }
        else
        {
            OutAbsolutePath = Path;
        }
        return F_FeedbackStatus::MakeSucceeded();
    }
    void F_SerializableEnvironment::AnalyzeObjectPaths(
        const TF_SmallVector<F_Name, 1>& InObjectPaths,
        TF_SmallVector<F_Name, 1>& OutOrderedObjectPaths,
        TF_Set<F_Name>& OutObjectPathSet,
        TF_Map<F_Name, TS<F_SerializablePackage>>& OutPackages
    )
    {
        
        TF_SmallVector<F_Name,  1> OrderedObjectPaths;
        TF_Set<F_Name> ObjectPathSet;
        TF_Map<F_Name, TS<F_SerializablePackage>> Packages;
        
        //
        TF_Set<F_Name> ObjectPathsToIterate;
        for (const auto& ObjectPath : InObjectPaths)
        {
            if (ObjectPathsToIterate.find(ObjectPath) != ObjectPathsToIterate.end())
            {
                continue;
            }
            
            F_Name ObjectName;
            F_Name PackageName;
            ABYTEK_FEEDBACK_STATUS_CHECK_HARD(
                  ParseObjectPath(ObjectPath, ObjectName, PackageName)
            );
            ABYTEK_BASE_SERIALIZABLE_ASSERT(PackageName) << "Cannot load objects having invalid package";
                    
            TS<F_SerializablePackage> Package;
            {
                auto It = Packages.find(PackageName);
                if (It == Packages.end())
                {
                    Package = EnsurePackage(PackageName);
                    Packages.insert({ PackageName, Package });
                }
                else
                {
                    Package = It->second;
                }
            }
            
            F_SerializableObjectHeader ObjectHeader;
            if (Package->SearchLastObjectHeader(ObjectPath, ObjectHeader))
            {
                ObjectPathsToIterate.insert(ObjectPath);
            }
        }
        
        //
        while (ObjectPathsToIterate.size() > 0)
        {
            auto CachedObjectPathsToIterate = ABYTEK_MOVE(ObjectPathsToIterate);
            for (const auto& ObjectPath : CachedObjectPathsToIterate)
            {
                if (ObjectPathSet.find(ObjectPath) != ObjectPathSet.end())
                {
                    continue;
                }
                
                F_SerializableObjectHeader ObjectHeader;
                {
                    F_Name ObjectName;
                    F_Name PackageName;
                    ABYTEK_FEEDBACK_STATUS_CHECK_HARD(
                          ParseObjectPath(ObjectPath, ObjectName, PackageName)
                    );
                    ABYTEK_BASE_SERIALIZABLE_ASSERT(PackageName) << "Cannot load objects having invalid package: " << ObjectPath;
                    
                    TS<F_SerializablePackage> Package;
                    {
                        auto It = Packages.find(PackageName);
                        if (It == Packages.end())
                        {
                            Package = EnsurePackage(PackageName);
                            Packages.insert({ PackageName, Package });
                        }
                        else
                        {
                            Package = It->second;
                        }
                    }
                    
                    B8 FoundObjectHeader = Package->SearchLastObjectHeader(ObjectPath, ObjectHeader);
                    ABYTEK_BASE_SERIALIZABLE_ASSERT(FoundObjectHeader) << "Not found object with path: " << ObjectPath;
                }
                ObjectPathSet.insert(ObjectPath);
                OrderedObjectPaths.push_back(ObjectPath);
                
                for (const auto& ReferencedObjectPath : ObjectHeader.ReferencePaths)
                {
                    if (ObjectPathsToIterate.find(ReferencedObjectPath) != ObjectPathsToIterate.end())
                    {
                        continue;
                    }
                    ObjectPathsToIterate.insert(ReferencedObjectPath);
                }
            }
        }
        
        //
        OutOrderedObjectPaths = ABYTEK_MOVE(OrderedObjectPaths);
        OutObjectPathSet = ABYTEK_MOVE(ObjectPathSet);
        OutPackages = ABYTEK_MOVE(Packages);
    }

    void F_SerializableEnvironment::CreateObjectsWithoutLoading(
        const TF_SmallVector<F_SerializableObjectCreationParams, 1>& CreationParamsList,
        TF_SmallVector<TS<A_SerializableObject>, 1>& OutObjects
    )
    {        
        TF_SmallVector<TS<A_SerializableObject>, 1> ObjectsCreatedFromPackage;
        TF_SmallVector<F_Name, 1> ObjectPathsToLoad;
        for (const auto& CreationParams : CreationParamsList)
        {
            if (CreationParams.Name && CreationParams.PackageName && !FindObject(CreationParams.Name))
            {
                ABYTEK_BASE_SERIALIZABLE_ASSERT(CreationParams.Name);
                ObjectPathsToLoad.push_back(
                    MakeObjectPath(CreationParams.Name, CreationParams.PackageName)    
                );
            }
        }
        if (ObjectPathsToLoad.size() > 0)
        {
            TF_SmallVector<F_Name, 1> AnalyzedOrderedObjectPaths;
            TF_Set<F_Name> AnalyzedObjectPathSet;
            TF_Map<F_Name, TS<F_SerializablePackage>> AnalyzedPackages;
            AnalyzeObjectPaths(
                ObjectPathsToLoad,
                AnalyzedOrderedObjectPaths,
                AnalyzedObjectPathSet,
                AnalyzedPackages
            );
            
            for (const auto& AnalyzedObjectPath : AnalyzedOrderedObjectPaths)
            {
                F_Name ObjectName;
                F_Name PackageName;
                ABYTEK_FEEDBACK_STATUS_CHECK_HARD(
                    ParseObjectPath(AnalyzedObjectPath, ObjectName, PackageName)
                );
         
                if (auto Object = FindObject(ObjectName))
                {
                    ObjectsCreatedFromPackage.push_back(ShareObject(Object));
                    continue;
                }
            
                TS<F_SerializablePackage> Package;
                {
                    auto It = AnalyzedPackages.find(PackageName);
                    ABYTEK_BASE_SERIALIZABLE_ASSERT(It != AnalyzedPackages.end()) << "Not found package: " << PackageName;
                    Package = It->second;
                }
            
                F_SerializableObjectHeader ObjectHeader;
                B8 FoundObjectHeader = Package->SearchLastObjectHeader(AnalyzedObjectPath, ObjectHeader);
                ABYTEK_BASE_SERIALIZABLE_ASSERT(FoundObjectHeader) << "Not found object with path: " << AnalyzedObjectPath;
                auto Object = ForceCreateObjectDelayLoading(
                    ObjectName,
                    PackageName,
                    ObjectHeader.Type
                );
                Object->_IsLoadedFromPackage = true;
                ObjectsCreatedFromPackage.push_back(Object);
            }
        }
        
        for (const auto& CreationParams : CreationParamsList)
        {
            if (CreationParams.Name)
            {
                if (auto FoundObject = FindObject(CreationParams.Name))
                {
                    ABYTEK_BASE_SERIALIZABLE_ASSERT(FoundObject->GetType()->SupportImplicitPolymorphismCast(CreationParams.Type) || !CreationParams.Type) << "Invalid type";
                    OutObjects.push_back(ShareObject(FoundObject));
                    continue;
                }
            }
            
            auto Object = ForceCreateObjectDelayLoading(
                CreationParams.Name,    
                CreationParams.PackageName,    
                CreationParams.Type    
            );
            if (CreationParams.PackageName)
            {
                Object->GetPackage()->MarkDirty();
            }
            OutObjects.push_back(Object);
        }
    }
    void F_SerializableEnvironment::CreateObjects(
        const TF_SmallVector<F_SerializableObjectCreationParams, 1>& CreationParamsList,
        TF_SmallVector<TS<A_SerializableObject>, 1>& OutObjects
    )
    {
        ABYTEK_BASE_SERIALIZABLE_ASSERT(IsEnabledObjectLoading()) << "Not enabled object loading";
        DisableObjectLoading();
        CreateObjectsWithoutLoading(
            CreationParamsList,
            OutObjects
        );
        EnableObjectLoading();
        LoadEnqueuedObjects();
    }
    TS<A_SerializableObject> F_SerializableEnvironment::ForceCreateObjectDelayLoading(
        const F_Name& Name, 
        const F_Name& PackageName,
        const TF_ReflectionTypeHandle<A_SerializableObject>& Type,
        E_SerializableObjectFlag Flags
    )
    {
        ABYTEK_BASE_SERIALIZABLE_ASSERT(Type) << "Invalid type for object: " << Name;
            
        F_SerializableObjectInitParams InitParams;
        InitParams.Type = Type;
        InitParams.Environment = ABYTEK_WTHIS();
        InitParams.Name = Name;
        InitParams.Flags = Flags;
        if (PackageName)
        {
            InitParams.PackageName = PackageName;
            InitParams.Package = EnsurePackage(PackageName);
        }
    
        const auto& TypeMetadata = Type->GetMetadata();
        ABYTEK_BASE_SERIALIZABLE_ASSERT(TypeMetadata.HasElement(A_SerializableObject::GetMetadataElementName_Creator())) 
            << "Type is missing serializable object creator: " << Type->GetFullName();
        const auto& MetadataElement = TypeMetadata.Get(A_SerializableObject::GetMetadataElementName_Creator());
        const auto& Creator = AnyCast<A_SerializableObject::F_Creator>(MetadataElement);
        auto Object = Creator(InitParams);
        Object->OnPostConstruct();
        _ObjectsToLoad.Push(Object);
        return Object;
    }
    void F_SerializableEnvironment::LoadEnqueuedObjects()
    {
        ABYTEK_BASE_SERIALIZABLE_ASSERT(IsEnabledObjectLoading()) << "Not enabled object loading";
        TS<A_SerializableObject> Object;
        while (_ObjectsToLoad.TryPop(Object))
        {
            if (Object->IsLoaded())
            {
                continue;
            }
            Object->CallLoad();
        }
    }
    TS<A_SerializableObject> F_SerializableEnvironment::CreateObjectDelayLoading(
        const F_Name& Name,
        const F_Name& PackageName, 
        const TF_ReflectionTypeHandle<A_SerializableObject>& Type
    )
    {
        ABYTEK_BASE_SERIALIZABLE_ASSERT(!HasObject(Name)) << "Object already created, name: " << Name;
        
        // Fast creation mode
        if (
            (!Name)    
            && (!PackageName)    
        )
        {
            auto Object = ForceCreateObjectDelayLoading(
                Name,    
                PackageName,    
                Type    
            );
            return Object;
        }
        
        TF_SmallVector<TS<A_SerializableObject>, 1> CreatedObjects;
        F_SerializableObjectCreationParams CreationParams;
        CreationParams.Type = Type;
        CreationParams.Name = Name;
        CreationParams.PackageName = PackageName;
        CreateObjectsWithoutLoading(
            { CreationParams },
            CreatedObjects
        );
        for (const auto& Object : CreatedObjects)
        {
            if (Object->GetName() == Name)
            {
                return Object;
            }
        }
        
        ABYTEK_LOG_FATAL() << "Failed to create object: " << Name;
        return {};
    }
    TS<A_SerializableObject> F_SerializableEnvironment::CreateObject(
        const F_Name& Name, 
        const F_Name& PackageName,
        const TF_ReflectionTypeHandle<A_SerializableObject>& Type
    )
    {
        ABYTEK_BASE_SERIALIZABLE_ASSERT(IsEnabledObjectLoading()) << "Not enabled object loading";
        DisableObjectLoading();
        auto Object = CreateObjectDelayLoading(
            Name,
            PackageName,
            Type
        );
        EnableObjectLoading();
        LoadEnqueuedObjects();
        return Object;
    }
    B8 F_SerializableEnvironment::PopulateObjectDelayLoading(
        TS<A_SerializableObject>& OutObject, 
        const F_Name& Name,
        const F_Name& PackageName, 
        const TF_ReflectionTypeHandle<A_SerializableObject>& Type
    )
    {
        auto Object = CreateObjectDelayLoading(
            Name,
            PackageName,
            Type
        );
        OutObject = Object;
        return !Object->IsLoadedFromPackage();
    }
    B8 F_SerializableEnvironment::PopulateObject(
        TS<A_SerializableObject>& OutObject, 
        const F_Name& Name,
        const F_Name& PackageName, 
        const TF_ReflectionTypeHandle<A_SerializableObject>& Type
    )
    {
        auto Object = CreateObject(
            Name,
            PackageName,
            Type
        );
        OutObject = Object;
        return !Object->IsLoadedFromPackage();
    }
    TS<A_SerializableObject> F_SerializableEnvironment::FindOrCreateObjectDelayLoading(
        const F_Name& Name,
        const F_Name& PackageName, 
        const TF_ReflectionTypeHandle<A_SerializableObject>& Type
    )
    {
        // Fast access mode
        if (auto Object = FindObject(Name))
        {
            return ShareObject(Object);
        }
        
        return CreateObjectDelayLoading(
            Name, 
            PackageName, 
            Type
        );
    }
    TS<A_SerializableObject> F_SerializableEnvironment::FindOrCreateObject(
        const F_Name& Name, 
        const F_Name& PackageName,
        const TF_ReflectionTypeHandle<A_SerializableObject>& Type
    )
    {
        // Fast access mode
        if (auto Object = FindObject(Name))
        {
            return ShareObject(Object);
        }
        
        return CreateObject(
            Name, 
            PackageName, 
            Type
        );
    }
    B8 F_SerializableEnvironment::FindOrPopulateObjectDelayLoading(
        TS<A_SerializableObject>& OutObject, 
        const F_Name& Name,
        const F_Name& PackageName, 
        const TF_ReflectionTypeHandle<A_SerializableObject>& Type
    )
    {
        // Fast access mode
        if (auto Object = FindObject(Name))
        {
            OutObject = ShareObject(Object);
            return !Object->IsLoadedFromPackage();
        }
        
        return PopulateObjectDelayLoading(
            OutObject,
            Name, 
            PackageName, 
            Type
        );
    }
    B8 F_SerializableEnvironment::FindOrPopulateObject(
        TS<A_SerializableObject>& OutObject, 
        const F_Name& Name,
        const F_Name& PackageName, 
        const TF_ReflectionTypeHandle<A_SerializableObject>& Type
    )
    {
        // Fast access mode
        if (auto Object = FindObject(Name))
        {
            OutObject = ShareObject(Object);
            return !Object->IsLoadedFromPackage();
        }
        
        return PopulateObject(
            OutObject,
            Name, 
            PackageName, 
            Type
        );
    }

    TS<A_SerializableObject> F_SerializableEnvironment::CreateCDO(const TF_ReflectionTypeHandle<A_SerializableObject>& Type)
    {
        return ForceCreateObjectDelayLoading(
            {},
            {},
            Type,
            E_SerializableObjectFlag::CDO
        );
    }

    TS<F_SerializablePackage> F_SerializableEnvironment::EnsurePackage(const F_Name& PackageName)
    {
        auto It = _Packages.find(PackageName);
        if (It != _Packages.end())
        {
            return ShareObject(It->second);
        }
        return TS<F_SerializablePackage>()(ABYTEK_WTHIS(), PackageName);
    }

    F_Name F_SerializableEnvironment::GenerateAnonymousObjectName()
    {
        U32 Index = _NextAnonymousObjectIndex.fetch_add(1);
        return ABYTEK_TEXT("Abytek::SerializableObject::Anonymous_") + ToText(Index);
    }

    TS<F_SerializableEnvironment> F_SerializableEnvironment::Clone() const
    {
        F_SerializableEnvironmentBuildParams BuildParams;
        BuildParams.HasDevelopmentBuild = HasDevelopmentBuild();
        auto Result = TS<F_SerializableEnvironment>()(BuildParams);
        for (const auto& [Name, Value] : _Mounts)
        {
            Result->AddMount(Name, Value);
        }
        for (const auto& CDOType : _CDOTypes)
        {
            Result->AddCDOType(CDOType);
        }
        return Result;
    }

    void F_SerializableEnvironment::EnableObjectLoading()
    {
        ABYTEK_BASE_SERIALIZABLE_ASSERT(!_IsEnabledObjectLoading) << "Already enabled object loading";
        _IsEnabledObjectLoading = true;
    }
    void F_SerializableEnvironment::DisableObjectLoading()
    {
        ABYTEK_BASE_SERIALIZABLE_ASSERT(_IsEnabledObjectLoading) << "Not enabled object loading";
        _IsEnabledObjectLoading = false;
    }
}
