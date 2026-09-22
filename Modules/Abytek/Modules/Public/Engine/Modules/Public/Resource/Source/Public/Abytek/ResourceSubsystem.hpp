#pragma once

#include "Abytek/Engine.Resource.prerequisites.hpp"


namespace Abytek
{
    class F_RootResource;
    class F_Resource;

    class ABYTEK_ENGINE_RESOURCE_API F_ResourceManager final : public A_Object
    {
    public:
        friend class F_Resource;
        
    public:
        ABYTEK_BEGIN_REFLECTOR()
        ABYTEK_END_REFLECTOR(F_ResourceManager);
        
    private:
        E_FrameParamType _FrameParamType = E_FrameParamType::NUM;
        TS<F_RootResource> _RootResource;
        TF_Map<F_Name, TW<F_Resource>> _ShortcutFullNameToResource;
        
    public:
        ABYTEK_FORCE_INLINE auto GetFrameParamType() const noexcept
        {
            return _FrameParamType;
        }
        ABYTEK_FORCE_INLINE const auto& GetRootResource() const noexcept
        {
            return _RootResource;
        }
        
    public:
        F_ResourceManager(E_FrameParamType FrameParamType);
        ~F_ResourceManager() override;

    public:
        TS<F_Resource> FindByShortcut(const F_Name& ShortcutFullName);

    private:
        void _RegisterShortcut(const F_Name& ShortcutFullName, const TW_Valid<F_Resource>& Resource);
        void _UnregisterShortcut(const F_Name& ShortcutFullName);
    };
    
    class ABYTEK_ENGINE_RESOURCE_API F_ResourceSubsystem final : public A_ApplicationSubsystem
    {
    public:
        ABYTEK_BEGIN_REFLECTOR(A_ApplicationSubsystem)
        ABYTEK_END_REFLECTOR(F_ResourceSubsystem);

    public:
        ABYTEK_DECLARE_STATIC_APPLICATION_SUBSYSTEM(F_ResourceSubsystem);

    private:
        TF_Vector<TS<F_ResourceManager>> _Managers;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetManagers() const noexcept
        {
            return _Managers;
        }

    public:
        F_ResourceSubsystem(const F_ProgramUnitBuildParams& BuildParams);
        ~F_ResourceSubsystem() override;

    protected:
        void OnInit() override;
        void OnRelease() override;
        
    public:
        TW_Valid<F_ResourceManager> GetManager(E_FrameParamType FrameParamType);
    };
}
