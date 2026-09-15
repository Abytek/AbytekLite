#pragma once

#include "Abytek/Engine.Core.prerequisites.hpp"


namespace Abytek
{
    enum class E_ApplicationMountFlag : U8
    {
        NONE = 0x0,
        DEVELOPMENT_BUILD,
        NON_DEVELOPMENT_BUILD,
        DEFAULT = DEVELOPMENT_BUILD | NON_DEVELOPMENT_BUILD,
    };
    struct F_ApplicationMount
    {
        F_Name Name;
        F_Name ModuleName;
        F_Text RelativePath;
        E_ApplicationMountFlag Flags = E_ApplicationMountFlag::DEFAULT;
    };
    
    class ABYTEK_ENGINE_CORE_API F_ApplicationMountManager : public A_Object
    {
    public:
        ABYTEK_DECLARE_OBJECT_SINGLETON_CRT(F_ApplicationMountManager);
        
    private:
        TF_Vector<F_ApplicationMount> _Mounts;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetMounts() const noexcept
        {
            return _Mounts;
        }
        
    public:
        F_ApplicationMountManager();
        ~F_ApplicationMountManager() override;
    
    public:
        void AddMount(const F_Name& Name, const F_Name& ModuleName, const F_Text& RelativePath, E_ApplicationMountFlag Flags = E_ApplicationMountFlag::DEFAULT);
        void Export(const TW_Valid<F_SerializableEnvironment>& SerializableEnvironment, const F_Text& BaseDirectoryPath);
    };
}