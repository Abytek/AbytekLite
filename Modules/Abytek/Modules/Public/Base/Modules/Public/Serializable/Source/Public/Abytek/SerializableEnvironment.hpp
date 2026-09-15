#pragma once

#include "Abytek/Base.Serializable.prerequisites.pch.hpp"
#include "Abytek/SerializableObjectData.hpp"


namespace Abytek
{
    class A_SerializableObject;
    class F_SerializablePackage;
    
    struct F_SerializableEnvironmentBuildParams
    {
        TW<A_Object> Owner;
        
#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
        B8 HasDevelopmentBuild = true;
#else
        B8 HasDevelopmentBuild = false;
#endif
    };
    
    class ABYTEK_BASE_SERIALIZABLE_API F_SerializableEnvironment final : public A_Object
    {
    public:
        friend class A_SerializableObject;
        
    public:
        ABYTEK_BEGIN_REFLECTOR()
        ABYTEK_END_REFLECTOR(F_SerializableEnvironment)
        
    private:
        TW<A_Object> _Owner;
#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
        B8 _HasDevelopmentBuild = true;
#else
        B8 _HasDevelopmentBuild = false;
#endif
        TF_Set<TF_ReflectionTypeHandle<A_SerializableObject>> _CDOTypes;
        TF_Map<F_Name, F_Text> _Mounts;
        TF_Map<F_Name, TW<A_SerializableObject>> _Objects;
        TF_Map<F_Name, TW<F_SerializablePackage>> _Packages;
        
        TF_Queue<TS<A_SerializableObject>> _ObjectsToLoad;
        
        AU32 _NextAnonymousObjectIndex = 0;
        
        B8 _AllowCreateObjectWithLoading = true;
        
        TF_Map<TF_ReflectionTypeHandle<A_SerializableObject>, TS<A_SerializableObject>> _CDOs;
        
    public:
        TF_Map<F_Name, F_Any> Metadata;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetOwner() const noexcept
        {
            return _Owner;
        }
        ABYTEK_FORCE_INLINE void AssignOwner(const TW<A_Object>& Value) noexcept
        {
            _Owner = Value;
        }
        ABYTEK_FORCE_INLINE auto HasDevelopmentBuild() const noexcept
        {
            return _HasDevelopmentBuild;
        }
        ABYTEK_FORCE_INLINE const auto& GetCDOTypes() const noexcept
        {
            return _CDOTypes;
        }
        ABYTEK_FORCE_INLINE const auto& GetMounts() const noexcept
        {
            return _Mounts;
        }
        ABYTEK_FORCE_INLINE const auto& GetObjects() const noexcept
        {
            return _Objects;
        }
        ABYTEK_FORCE_INLINE B8 HasObject(const F_Name& Name) const noexcept
        {
            return _Objects.find(Name) != _Objects.end();
        }
        ABYTEK_FORCE_INLINE const auto& GetObject_(const F_Name& Name) const noexcept
        {
            ABYTEK_BASE_SERIALIZABLE_ASSERT(HasObject(Name)) << "Not found object: " << Name;
            return _Objects.find(Name)->second;
        }
        ABYTEK_FORCE_INLINE TW<A_SerializableObject> FindObject(const F_Name& Name) const noexcept
        {
            auto It = _Objects.find(Name);
            if (It == _Objects.end())
            {
                return {};
            }
            return It->second;
        }
        template<typename __F_Object>
        ABYTEK_FORCE_INLINE TW<__F_Object> FindObject(const F_Name& Name) const noexcept
        {
            if (auto Object = FindObject(Name))
            {
                TW<__F_Object> Result;
                Object.TryDynamicCast<__F_Object>(Result);
                return Result;
            }
            return {};
        }
        ABYTEK_FORCE_INLINE B8 HasPackage(const F_Name& Name) const noexcept
        {
            return _Packages.find(Name) != _Packages.end();
        }
        ABYTEK_FORCE_INLINE const auto& GetPackage_(const F_Name& Name) const noexcept
        {
            ABYTEK_BASE_SERIALIZABLE_ASSERT(HasPackage(Name)) << "Not found object: " << Name;
            return _Packages.find(Name)->second;
        }
        ABYTEK_FORCE_INLINE TW<F_SerializablePackage> FindPackage(const F_Name& Name) const noexcept
        {
            auto It = _Packages.find(Name);
            if (It == _Packages.end())
            {
                return {};
            }
            return It->second;
        }
        
        ABYTEK_FORCE_INLINE auto GetAllowCreateObjectWithLoading() const noexcept
        {
            return _AllowCreateObjectWithLoading;
        }
        
        ABYTEK_FORCE_INLINE const auto& GetCDOs() const noexcept
        {
            return _CDOs;
        }
        ABYTEK_FORCE_INLINE B8 HasCDO(const TF_ReflectionTypeHandle<A_SerializableObject>& Type) const noexcept
        {
            return _CDOs.find(Type) != _CDOs.end();
        }
        template<typename __F_Object>
        ABYTEK_FORCE_INLINE B8 HasCDO(const TF_ReflectionTypeHandle<__F_Object>& Type = TF_ReflectionTypeHandle<__F_Object>(F_ReflectionContext::GetGlobal())) const noexcept
        {
            return _CDOs.find(Type) != _CDOs.end();
        }
        ABYTEK_FORCE_INLINE const auto& GetCDO(const TF_ReflectionTypeHandle<A_SerializableObject>& Type) const noexcept
        {
            ABYTEK_BASE_SERIALIZABLE_ASSERT(Type) << "Invalid type";
            ABYTEK_BASE_SERIALIZABLE_ASSERT(HasCDO(Type)) << "Not found default object: " << Type->GetFullName();
            return _CDOs.find(Type)->second;
        }
        template<typename __F_Object>
        ABYTEK_FORCE_INLINE const auto& GetCDO(const TF_ReflectionTypeHandle<__F_Object>& Type = TF_ReflectionTypeHandle<__F_Object>(F_ReflectionContext::GetGlobal())) const noexcept
        {
            ABYTEK_BASE_SERIALIZABLE_ASSERT(Type) << "Invalid type";
            ABYTEK_BASE_SERIALIZABLE_ASSERT(HasCDO<__F_Object>(Type)) << "Not found default object: " << Type->GetFullName();
            return _CDOs.find(Type)->second;
        }
        ABYTEK_FORCE_INLINE TS<A_SerializableObject> FindCDO(const TF_ReflectionTypeHandle<A_SerializableObject>& Type) const noexcept
        {
            auto It = _CDOs.find(Type);
            if (It == _CDOs.end())
            {
                return {};
            }
            return It->second;
        }
        template<typename __F_Object>
        ABYTEK_FORCE_INLINE TS<__F_Object> FindCDO(const TF_ReflectionTypeHandle<__F_Object>& Type = TF_ReflectionTypeHandle<__F_Object>(F_ReflectionContext::GetGlobal())) const noexcept
        {
            if (auto Object = FindCDO(Type))
            {
                if (!Object.template CheckPolymorphism<__F_Object>())
                {
                    return {};
                }
                return Object.template FastCast<__F_Object>();
            }
            return {};
        }
        
    public:
        F_SerializableEnvironment(const F_SerializableEnvironmentBuildParams& BuildParams = {});
        ~F_SerializableEnvironment() override;
        
    public:
        ABYTEK_FORCE_INLINE B8 HasMount(const F_Name& Name) const noexcept
        {
            return _Mounts.find(Name) != _Mounts.end();
        }
        ABYTEK_FORCE_INLINE const auto& GetMount(const F_Name& Name) const noexcept
        {
            ABYTEK_BASE_SERIALIZABLE_ASSERT(HasMount(Name)) << "Not found mount " << Name;
            return _Mounts.find(Name)->second;
        }
        ABYTEK_FORCE_INLINE void AddMount(const F_Name& Name, const F_Text& Value) noexcept
        {
            ABYTEK_BASE_SERIALIZABLE_ASSERT(!HasMount(Name)) << "Already added mount " << Name;
            _Mounts[Name] = Value;
        }
        ABYTEK_FORCE_INLINE void RemoveMount(const F_Name& Name) noexcept
        {
            ABYTEK_BASE_SERIALIZABLE_ASSERT(!HasMount(Name)) << "Not found mount " << Name;
            _Mounts.erase(_Mounts.find(Name));
        }
        
    public:
        ABYTEK_FORCE_INLINE B8 HasCDOType(const TF_ReflectionTypeHandle<A_SerializableObject>& Type) const noexcept
        {
            return _CDOTypes.find(Type) != _CDOTypes.end();
        }
        template<typename __F_Object>
        ABYTEK_FORCE_INLINE B8 HasCDOType(const TF_ReflectionTypeHandle<__F_Object>& Type = TF_ReflectionTypeHandle<__F_Object>(F_ReflectionContext::GetGlobal())) const noexcept
        {
            return _CDOTypes.find(Type) != _CDOTypes.end();
        }
        ABYTEK_FORCE_INLINE void AddCDOType(const TF_ReflectionTypeHandle<A_SerializableObject>& Type) noexcept
        {
            ABYTEK_BASE_SERIALIZABLE_ASSERT(Type) << "Invalid type";
            ABYTEK_BASE_SERIALIZABLE_ASSERT(!HasCDOType(Type)) << "Already added CDOType " << Type->GetFullName();
            _CDOTypes.insert(Type);
            _OnAddCDOType(Type);
        }
        template<typename __F_Object>
        ABYTEK_FORCE_INLINE void AddCDOType(const TF_ReflectionTypeHandle<__F_Object>& Type = TF_ReflectionTypeHandle<__F_Object>(F_ReflectionContext::GetGlobal())) noexcept
        {
            ABYTEK_BASE_SERIALIZABLE_ASSERT(Type) << "Invalid type";
            ABYTEK_BASE_SERIALIZABLE_ASSERT(!HasCDOType<__F_Object>(Type)) << "Already added CDOType " << Type->GetFullName();
            _CDOTypes.insert(Type);
            _OnAddCDOType(Type);
        }
        ABYTEK_FORCE_INLINE void RemoveCDOType(const TF_ReflectionTypeHandle<A_SerializableObject>& Type) noexcept
        {
            ABYTEK_BASE_SERIALIZABLE_ASSERT(Type) << "Invalid type";
            ABYTEK_BASE_SERIALIZABLE_ASSERT(HasCDOType(Type)) << "Not found CDOType " << Type->GetFullName();
            _OnRemoveCDOType(Type);
            _CDOTypes.erase(_CDOTypes.find(Type));
        }
        template<typename __F_Object>
        ABYTEK_FORCE_INLINE void RemoveCDOType(const TF_ReflectionTypeHandle<__F_Object>& Type = TF_ReflectionTypeHandle<__F_Object>(F_ReflectionContext::GetGlobal())) noexcept
        {
            ABYTEK_BASE_SERIALIZABLE_ASSERT(Type) << "Invalid type";
            ABYTEK_BASE_SERIALIZABLE_ASSERT(!HasCDOType<__F_Object>(Type)) << "Not found CDOType " << Type->GetFullName();
            _OnRemoveCDOType(Type);
            _CDOTypes.erase(_CDOTypes.find(Type));
        }
        
    private:
        void _OnAddCDOType(const TF_ReflectionTypeHandle<A_SerializableObject>& Type);
        void _OnRemoveCDOType(const TF_ReflectionTypeHandle<A_SerializableObject>& Type);
        
    public:
        void _RegisterObject(const TW_Valid<A_SerializableObject>& Object);
        void _UnregisterObject(const TW_Valid<A_SerializableObject>& Object);
        
    public:
        void _RegisterPackage(const TW_Valid<F_SerializablePackage>& Package);
        void _UnregisterPackage(const TW_Valid<F_SerializablePackage>& Package);
        
    public:
        static F_FeedbackStatus ParseMountablePath(const F_Text& Raw, F_Name& OutModuleName, F_Text& OutPath);
        static F_FeedbackStatus ParseObjectPath(const F_Name& Raw, F_Name& OutObjectName, F_Name& OutPackageName);
        static F_Name MakeObjectPath(const F_Name& ObjectName, const F_Name& PackageName);
        
    public:
        F_FeedbackStatus ResolveAbsolutePath(const F_Text& Raw, F_Text& OutAbsolutePath);
        void AnalyzeObjectPaths(
            const TF_SmallVector<F_Name, 1>& InObjectPaths,
            TF_SmallVector<F_Name, 1>& OutOrderedObjectPaths,
            TF_Set<F_Name>& OutObjectPathSet,
            TF_Map<F_Name, TS<F_SerializablePackage>>& OutPackages
        );
        void LoadObjects(
            const TF_SmallVector<F_Name, 1>& ObjectPaths,
            TF_SmallVector<TS<A_SerializableObject>, 1>& OutObjects
        );
        void CreateObjectsWihtoutLoading(
            const TF_SmallVector<F_SerializableObjectCreationParams, 1>& CreationParamsList,
            TF_SmallVector<TS<A_SerializableObject>, 1>& OutObjects
        );
        void CreateObjects(
            const TF_SmallVector<F_SerializableObjectCreationParams, 1>& CreationParamsList,
            TF_SmallVector<TS<A_SerializableObject>, 1>& OutObjects
        );
        TS<A_SerializableObject> ForceCreateObjectWithoutLoading(
            const F_Name& Name = {},
            const F_Name& PackageName = {},
            const TF_ReflectionTypeHandle<A_SerializableObject>& Type = {}
        );
        void LoadEnqueuedObjects();
        template<typename __F_Object>
        TS<__F_Object> ForceCreateObject(
            const F_Name& Name = {},
            const F_Name& PackageName = {},
            const TF_ReflectionTypeHandle<__F_Object>& Type = TF_ReflectionTypeHandle<__F_Object>(F_ReflectionContext::GetGlobal())
        )
        {
            auto Object = ForceCreateObjectWithoutLoading(Name, PackageName, Type.template Cast<A_SerializableObject>());
            return Object.template FastCast<__F_Object>();
        }
        TS<A_SerializableObject> CreateObjectWithoutLoading(
            const F_Name& Name = {},
            const F_Name& PackageName = {},
            const TF_ReflectionTypeHandle<A_SerializableObject>& Type = {}
        );
        template<typename __F_Object>
        TS<__F_Object> CreateObjectWithoutLoading(
            const F_Name& Name = {},
            const F_Name& PackageName = {},
            const TF_ReflectionTypeHandle<__F_Object>& Type = TF_ReflectionTypeHandle<__F_Object>(F_ReflectionContext::GetGlobal())
        )
        {
            auto Object = CreateObjectWithoutLoading(Name, PackageName, Type.template Cast<A_SerializableObject>());
            return Object.template FastCast<__F_Object>();
        }
        TS<A_SerializableObject> CreateObject(
            const F_Name& Name = {},
            const F_Name& PackageName = {},
            const TF_ReflectionTypeHandle<A_SerializableObject>& Type = {}
        );
        template<typename __F_Object>
        TS<__F_Object> CreateObject(
            const F_Name& Name = {},
            const F_Name& PackageName = {},
            const TF_ReflectionTypeHandle<__F_Object>& Type = TF_ReflectionTypeHandle<__F_Object>(F_ReflectionContext::GetGlobal())
        )
        {
            auto Object = CreateObject(Name, PackageName, Type.template Cast<A_SerializableObject>());
            return Object.template FastCast<__F_Object>();
        }
        B8 PopulateObject(
            TS<A_SerializableObject>& OutObject,
            const F_Name& Name,
            const F_Name& PackageName,
            const TF_ReflectionTypeHandle<A_SerializableObject>& Type
        );
        template<typename __F_Object>
        B8 PopulateObject(
            TS<__F_Object>& OutObject,
            const F_Name& Name = {},
            const F_Name& PackageName = {},
            const TF_ReflectionTypeHandle<__F_Object>& Type = TF_ReflectionTypeHandle<__F_Object>(F_ReflectionContext::GetGlobal())
        )
        {
            TS<A_SerializableObject> Object;
            B8 Result = PopulateObject(
                Object,
                Name,
                PackageName,
                Type
            );
            OutObject = Object.FastCast<__F_Object>();
            return Result;
        }
        
    public:
        TS<F_SerializablePackage> EnsurePackage(const F_Name& PackageName);
        
    public:
        F_Name GenerateAnonymousObjectName();
        
    public:
        TS<F_SerializableEnvironment> Clone() const;
    };
}