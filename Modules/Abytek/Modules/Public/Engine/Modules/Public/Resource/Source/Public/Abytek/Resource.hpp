#pragma once

#include "Abytek/Engine.Resource.prerequisites.hpp"
#include "Abytek/TaggedPath.hpp"


namespace Abytek
{
    class F_ResourceManager;

    // Not thread-safe
    class ABYTEK_ENGINE_RESOURCE_API F_Resource : public A_Object
    {
    public:
        ABYTEK_BEGIN_REFLECTOR()
        ABYTEK_END_REFLECTOR(F_Resource);

    private:
        F_Name _Name;
        TW<F_Resource> _Parent;
        TW<F_ResourceManager> _Manager;
        F_TaggedPath _Path;
        F_Name _ShortcutFullName;
        TF_List<TS<F_Resource>> _Childs;
        TF_Map<F_Name, TF_List<TS<F_Resource>>::iterator> _ChildIteratorMap;
        B8 _IsActivated = false;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetName() const noexcept
        {
            ABYTEK_ENGINE_RESOURCE_ASSERT(IsActivated()) << "Cannot get name on a resource that was not activated";
            return _Name;
        }
        ABYTEK_FORCE_INLINE const auto& GetParent() const noexcept
        {
            ABYTEK_ENGINE_RESOURCE_ASSERT(IsActivated()) << "Cannot get parent on a resource that was not activated";
            return _Parent;
        }
        ABYTEK_FORCE_INLINE const auto& GetManager() const noexcept
        {
            return _Manager;
        }
        ABYTEK_FORCE_INLINE const auto& GetPath() const noexcept
        {
            ABYTEK_ENGINE_RESOURCE_ASSERT(IsActivated()) << "Cannot get path on a resource that was not activated";
            return _Path;
        }
        ABYTEK_FORCE_INLINE const auto& GetChilds() const noexcept
        {
            ABYTEK_ENGINE_RESOURCE_ASSERT(IsActivated()) << "Cannot get childs on a resource that was not activated";
            return _Childs;
        }
        ABYTEK_FORCE_INLINE const auto& GetShortcutFullName() const noexcept
        {
            ABYTEK_ENGINE_RESOURCE_ASSERT(IsActivated()) << "Cannot get shortcut full name on a resource that was not activated";
            return _ShortcutFullName;
        }
        ABYTEK_FORCE_INLINE B8 IsActivated() const noexcept
        {
            return _IsActivated;
        }

    public:
        F_Resource();
        virtual ~F_Resource();

    public:
        virtual void Activate(const F_Name& Name, const TW<F_Resource>& Parent);
        virtual void Deactivate();

    public:
        B8 HasChild(const TS_Valid<F_Resource>& Resource);
        B8 HasChild(const F_Name& Name);
        void AddChild(const F_Name& InName, const TS_Valid<F_Resource>& Resource);
        void RemoveChild(const TS_Valid<F_Resource>& Resource);
        void RemoveChild(const F_Name& Name);
        TS<F_Resource> FindChild(const F_Name& Name);

    public:
        virtual TS<F_Resource> Query(const F_TaggedPath& Path);
        void RemoveFromParent()
        {
            _Parent->RemoveChild(_Name);
        }

    public:
        virtual F_TaggedPath ResolvePathToCreate(const F_TaggedPath& RawPath)
        {
            return RawPath;
        }
        
    protected:
        virtual TW<F_ResourceManager> GetOverridedManager(); 
    };
}
