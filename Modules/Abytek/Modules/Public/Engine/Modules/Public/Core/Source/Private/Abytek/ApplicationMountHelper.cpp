#include "Abytek/ApplicationMountHelper.hpp"


namespace Abytek
{
    void H_ApplicationMount::Add(const F_Name& Name, const F_Name& ModuleName, const F_Text& RelativePath, E_ApplicationMountFlag Flags)
    {
        F_ApplicationMountManager::GetInstance()->AddMount(Name, ModuleName, RelativePath, Flags);
    }
    void H_ApplicationMount::Export(const TW_Valid<F_SerializableEnvironment>& SerializableEnvironment, const F_Text& BaseDirectoryPath)
    {
        F_ApplicationMountManager::GetInstance()->Export(SerializableEnvironment, BaseDirectoryPath);
    }
}
