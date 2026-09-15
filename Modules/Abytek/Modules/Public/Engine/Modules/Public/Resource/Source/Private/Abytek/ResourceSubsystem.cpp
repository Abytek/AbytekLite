#include "Abytek/ResourceSubsystem.hpp"
#include "Abytek/RootResource.hpp"


namespace Abytek
{
    ABYTEK_REFLECT(F_ResourceManager)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::F_ResourceManager"));
    }

    F_ResourceManager::F_ResourceManager(E_FrameParamType FrameParamType) :
        _FrameParamType(FrameParamType)
    {
        _RootResource = TS<F_RootResource>()(ABYTEK_WTHIS());
        _RootResource->Activate({}, {});
    }
    F_ResourceManager::~F_ResourceManager()
    {
        _RootResource = {};
    }

    TS<F_Resource> F_ResourceManager::FindByShortcut(const F_Name& ShortcutFullName)
    {
        auto It = _ShortcutFullNameToResource.find(ShortcutFullName);
        if (It != _ShortcutFullNameToResource.end())
        {
            return ShareObject(It->second);
        }
        return {};
    }

    void F_ResourceManager::_RegisterShortcut(const F_Name& ShortcutFullName, const TW_Valid<F_Resource>& Resource)
    {
        ABYTEK_ENGINE_RESOURCE_ASSERT(!FindByShortcut(ShortcutFullName));
        _ShortcutFullNameToResource[ShortcutFullName] = Resource;
    }
    void F_ResourceManager::_UnregisterShortcut(const F_Name& ShortcutFullName)
    {
        ABYTEK_ENGINE_RESOURCE_ASSERT(FindByShortcut(ShortcutFullName));
        _ShortcutFullNameToResource.erase(_ShortcutFullNameToResource.find(ShortcutFullName));
    }

    ABYTEK_REFLECT(F_ResourceSubsystem)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::F_ResourceSubsystem"));
    }

    ABYTEK_DEFINE_STATIC_APPLICATION_SUBSYSTEM(F_ResourceSubsystem);

    F_ResourceSubsystem::F_ResourceSubsystem(const F_ProgramUnitBuildParams& BuildParams) :
        A_ApplicationSubsystem(BuildParams)
    {
        ABYTEK_BIND_STATIC_APPLICATION_SUBSYSTEM();
    }
    F_ResourceSubsystem::~F_ResourceSubsystem()
    {
    }

    void F_ResourceSubsystem::OnInit()
    {
        for (F_FrameParamTypeIndex ParamTypeIndex = 0; ParamTypeIndex < static_cast<F_FrameParamTypeIndex>(E_FrameParamType::NUM); ++ParamTypeIndex)
        {
            _Managers.push_back(TS<F_ResourceManager>()(static_cast<E_FrameParamType>(ParamTypeIndex)));
        }
    }
    void F_ResourceSubsystem::OnRelease()
    {
        _Managers = {};
    }

    TW_Valid<F_ResourceManager> F_ResourceSubsystem::GetManager(E_FrameParamType FrameParamType)
    {
        return _Managers[static_cast<F_FrameParamTypeIndex>(FrameParamType)].Weak();
    }
}
