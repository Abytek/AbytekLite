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
        const auto& TypeMedata = Type->GetMetadata();
        auto MetadataElementName = A_SerializableObject::GetMetadataElementName_Creator();
        ABYTEK_BASE_SERIALIZABLE_ASSERT(TypeMedata.HasElement(MetadataElementName)) << "Type not have serializable object creator metadata: " << Type->GetFullName();
        const auto& MetadataElement = TypeMedata.Get(MetadataElementName);
        const auto& Creator = AnyCast<A_SerializableObject::F_Creator>(MetadataElement);
        
        F_SerializableObjectInitParams ObjectInitParams;
        ObjectInitParams.Type = Type;
        ObjectInitParams.Environment = ABYTEK_WTHIS();
        ObjectInitParams.Flags = E_SerializableObjectFlag::CDO;
        auto Object = Creator(ObjectInitParams);
        
        _CDOs[Type] = Object;
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
    void F_SerializableEnvironment::LoadObjects(
        const TF_SmallVector<F_Name, 1>& ObjectPaths,
        TF_SmallVector<TS<A_SerializableObject>, 1>& OutObjects
    )
    {
        
        TF_SmallVector<F_Name, 1> AnalyzedOrderedObjectPaths;
        TF_Set<F_Name> AnalyzedObjectPathSet;
        TF_Map<F_Name, TS<F_SerializablePackage>> AnalyzedPackages;
        AnalyzeObjectPaths(
            ObjectPaths,
            AnalyzedOrderedObjectPaths,
            AnalyzedObjectPathSet,
            AnalyzedPackages
        );
        
        TF_SmallVector<std::pair<TS<A_SerializableObject>, F_SerializableObjectHeader>, 1> ObjectsAndHeaders;
        for (const auto& AnalyzedObjectPath : AnalyzedOrderedObjectPaths)
        {
            F_Name ObjectName;
            F_Name PackageName;
            ABYTEK_FEEDBACK_STATUS_CHECK_HARD(
                ParseObjectPath(AnalyzedObjectPath, ObjectName, PackageName)
            );
         
            if (auto Object = FindObject(ObjectName))
            {
                OutObjects.push_back(ShareObject(Object));
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
            auto Object = ObjectHeader.CreateObject(ABYTEK_WTHIS(), Package);
            ObjectsAndHeaders.push_back({
                Object,
                ObjectHeader
            });
        }
        for (const auto& [ Object, ObjectHeader ] : ObjectsAndHeaders)
        {
            if (!Object->IsLoaded())
            {
                Object->CallLoad();
            }
        }
        
        for (const auto& [ Object, ObjectHeader ] : ObjectsAndHeaders)
        {
            OutObjects.push_back(Object);
        }
    }

    void F_SerializableEnvironment::CreateObjectsWihtoutLoading(
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
                auto Object = ObjectHeader.CreateObject(ABYTEK_WTHIS(), Package);
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
            
            auto Object = ForceCreateObjectWithoutLoading(
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
        ABYTEK_BASE_SERIALIZABLE_ASSERT(_AllowCreateObjectWithLoading) << "Already in an object creation scope with loading";
        _AllowCreateObjectWithLoading = false;
        CreateObjectsWihtoutLoading(
            CreationParamsList,
            OutObjects
        );
        LoadEnqueuedObjects();
        _AllowCreateObjectWithLoading = true;
    }
    TS<A_SerializableObject> F_SerializableEnvironment::ForceCreateObjectWithoutLoading(
        const F_Name& Name, 
        const F_Name& PackageName,
        const TF_ReflectionTypeHandle<A_SerializableObject>& Type
    )
    {
        ABYTEK_BASE_SERIALIZABLE_ASSERT(Type) << "Invalid type for object: " << Name;
            
        F_SerializableObjectInitParams InitParams;
        InitParams.Type = Type;
        InitParams.Environment = ABYTEK_WTHIS();
        InitParams.Name = Name;
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
        return Object;
    }
    void F_SerializableEnvironment::LoadEnqueuedObjects()
    {
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
    TS<A_SerializableObject> F_SerializableEnvironment::CreateObjectWithoutLoading(
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
        
        // Fast creation mode
        if (
            (!Name)    
            && (!PackageName)    
        )
        {
            auto Object = ForceCreateObjectWithoutLoading(
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
        CreateObjectsWihtoutLoading(
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
        ABYTEK_BASE_SERIALIZABLE_ASSERT(_AllowCreateObjectWithLoading) << "Already in an object creation scope with loading";
        _AllowCreateObjectWithLoading = false;
        auto Object = CreateObjectWithoutLoading(
            Name,
            PackageName,
            Type
        );
        LoadEnqueuedObjects();
        _AllowCreateObjectWithLoading = true;
        return Object;
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
}
