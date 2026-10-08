#pragma once

#include "Abytek/Engine.Core.prerequisites.hpp"


#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
namespace Abytek
{
    class A_CookSetting;
    class F_CookSettingContainer;

    enum class E_CookPackageExportFlag : U8
    {
        NONE = 0x0,
        USER_DATA,
        DEFAULT = USER_DATA
    };
    struct F_CookPackageExport
    {
        F_Name Name;
        E_CookPackageExportFlag Flags = E_CookPackageExportFlag::DEFAULT;
    };
    
    class ABYTEK_ENGINE_CORE_API F_CookProfile final : public A_Object
    {
    public:
        ABYTEK_BEGIN_REFLECTOR()
        ABYTEK_END_REFLECTOR(F_CookProfile);
        
    public:
        static TW<F_CookProfile> _Main;
        static ABYTEK_FORCE_INLINE const auto& GetMain() noexcept
        {
            return _Main;
        }
        static ABYTEK_FORCE_INLINE void SetMain(const TW<F_CookProfile>& X) noexcept
        {
            _Main = X;
        }

    private:
        F_Name _Name;
        F_Name _ModuleName;
        TS<F_SerializableEnvironment> _SerializableEnvironment;
        TU<F_Console> _Console;
        TF_Set<F_Name> _ParsedNames;
        TU<F_CookSettingContainer> _SettingContainer;
        TF_Vector<TF_ReflectionTypeHandle<A_CookSetting>> _CookSettingTypes;
        
        F_Text _IntermediateDataDirectoryPath;
        F_Text _IntermediateConfigsDirectoryPath;
        F_Text _IntermediateModulesDirectoryPath;
        
        TF_Map<F_Name, F_CookPackageExport> _PackagesToExport;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetName() const noexcept
        {
            return _Name;
        }
        ABYTEK_FORCE_INLINE const auto& GetModuleName() const noexcept
        {
            return _ModuleName;
        }
        ABYTEK_FORCE_INLINE const auto& GetSerializableEnvironment() const noexcept
        {
            return _SerializableEnvironment;
        }
        ABYTEK_FORCE_INLINE auto GetConsole() const noexcept
        {
            return _Console.Weak();
        }
        ABYTEK_FORCE_INLINE auto GetSettingContainer() const noexcept
        {
            return _SettingContainer.Weak();
        }
        ABYTEK_FORCE_INLINE const auto& GetCookSettingTypes() const noexcept
        {
            return _CookSettingTypes;
        }
        
        ABYTEK_FORCE_INLINE const auto& GetIntermediateDataDirectoryPath() const noexcept
        {
            return _IntermediateDataDirectoryPath;
        }
        ABYTEK_FORCE_INLINE const auto& GetIntermediateConfigsDirectoryPath() const noexcept
        {
            return _IntermediateConfigsDirectoryPath;
        }
        ABYTEK_FORCE_INLINE const auto& GetIntermediateModulesDirectoryPath() const noexcept
        {
            return _IntermediateModulesDirectoryPath;
        }
        
        ABYTEK_FORCE_INLINE const auto& GetPackagesToExport() const noexcept
        {
            return _PackagesToExport;
        }
        
    public:
        F_CookProfile(const F_Name& Name, const F_Name& ModuleName);
        ~F_CookProfile() override;
        
    private:
        void _SetupConsole(const F_Name& Name);
        
    public:
        void AddPackageToExport(const F_Name& PackageName, E_CookPackageExportFlag Flags = E_CookPackageExportFlag::DEFAULT);
        void ExportPackages(const F_Text& DstDirectoryPath, E_CookPackageExportFlag Flags = E_CookPackageExportFlag::DEFAULT);
        
    public:
        void ExportConfigs(const F_Text& DstDirectoryPath);
    };
}
#endif