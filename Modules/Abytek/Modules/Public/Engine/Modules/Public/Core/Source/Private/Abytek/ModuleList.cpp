#include "Abytek/ModuleList.hpp"

namespace Abytek
{
    ABYTEK_REFLECT(F_ModuleList)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::F_ModuleList"));
    }
    
    ABYTEK_DEFINE_OBJECT_SINGLETON_CRT(F_ModuleList);
    
    F_ModuleList::F_ModuleList()
    {
        ABYTEK_BIND_OBJECT_SINGLETON_CRT();
        
        TF_Vector<F_String> ModuleNameStrs = {
#include "Abytek/Modules.inl"
            ""
        };
        Sz NumModules = ModuleNameStrs.size();
        for (Sz Idx = 0; Idx < (NumModules - 1); ++Idx)
        {
            _Names.insert(ToText(ModuleNameStrs[Idx]));
        }
    }
    F_ModuleList::~F_ModuleList()
    {
    }
}