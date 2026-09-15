#include "Abytek/Resource.hpp"
#include "Abytek/ResourceSubsystem.hpp"


namespace Abytek
{
    ABYTEK_REFLECT(F_Resource)
    {
    }
    
    F_Resource::F_Resource()
    {
    }
    F_Resource::~F_Resource()
    {
    }

    void F_Resource::Activate(const F_Name& Name, const TW<F_Resource>& Parent)
    {
        ABYTEK_ENGINE_RESOURCE_ASSERT(!_IsActivated) << "This resource was already activated";
        _Name = Name;
        _Parent = Parent;
        if (auto OverridedManager = GetOverridedManager())
        {
            _Manager = OverridedManager;
        }
        else
        {
            ABYTEK_ENGINE_RESOURCE_ASSERT(_Parent);
            _Manager = _Parent->GetManager();
        }
        if (Parent)
        {
            _Path = Parent->GetPath() + Name;
        }
        else
        {
            if (Name)
            {
                _Path = Name;
            }
            else
            {
                _Path = {};
            }
        }
        {
            _ShortcutFullName = _Path.ToText();
            _Manager->_RegisterShortcut(_ShortcutFullName, ABYTEK_WTHIS());
        }
        _IsActivated = true;
    }
    void F_Resource::Deactivate()
    {
        ABYTEK_ENGINE_RESOURCE_ASSERT(_IsActivated) << "This resource was not activated";
        _IsActivated = false;
        _Manager->_UnregisterShortcut(_ShortcutFullName);
        _Manager = {};
    }

    B8 F_Resource::HasChild(const TS_Valid<F_Resource>& Resource)
    {
        return HasChild(Resource->GetName());
    }
    B8 F_Resource::HasChild(const F_Name& Name)
    {
       return (_ChildIteratorMap.find(Name) != _ChildIteratorMap.end());
    }
    void F_Resource::AddChild(const F_Name& InName, const TS_Valid<F_Resource>& Resource)
    {
        ABYTEK_ENGINE_RESOURCE_ASSERT(!HasChild(InName)) << "A resource was already registered as child: " << *InName;
        _Childs.push_back(Resource);
        _ChildIteratorMap[InName] = --_Childs.end();
        Resource->Activate(InName, ABYTEK_WTHIS());
    }
    void F_Resource::RemoveChild(const TS_Valid<F_Resource>& Resource)
    {
        RemoveChild(Resource->GetName());
    }
    void F_Resource::RemoveChild(const F_Name& InName)
    {
        ABYTEK_ENGINE_RESOURCE_ASSERT(HasChild(InName)) << "Unknown resource to unregister: " << *InName;
        auto ChildIteratorIt = _ChildIteratorMap.find(InName);
        _Childs.erase(ChildIteratorIt->second);
        _ChildIteratorMap.erase(ChildIteratorIt);
    }
    TS<F_Resource> F_Resource::FindChild(const F_Name& InName)
    {
        auto It = _ChildIteratorMap.find(InName);
        if (It != _ChildIteratorMap.end())
        {
            return *It->second;
        }
        return {};
    }

    TS<F_Resource> F_Resource::Query(const F_TaggedPath& Path)
    {
        if (!Path)
        {
            return {};
        }
        auto CoreChild = FindChild(Path.GetCoreTag());
        if (!CoreChild)
        {
            return {};
        }
        if (Path.IsCoreOnly())
        {
            return CoreChild;
        }
        return CoreChild->Query(Path.GetSubPath());
    }

    TW<F_ResourceManager> F_Resource::GetOverridedManager()
    {
        return {};
    }
}
