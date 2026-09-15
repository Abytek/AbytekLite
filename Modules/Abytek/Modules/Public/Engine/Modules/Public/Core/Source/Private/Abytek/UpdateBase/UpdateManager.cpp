#include "Abytek/UpdateBase/UpdateManager.hpp"


namespace Abytek
{
    ABYTEK_REFLECT(F_UpdateManager)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::F_UpdateManager"));
    }

    ABYTEK_DEFINE_OBJECT_SINGLETON_CRT(F_UpdateManager);
    
    F_UpdateManager::F_UpdateManager()
    {
        ABYTEK_BIND_OBJECT_SINGLETON_CRT();
    }
    F_UpdateManager::~F_UpdateManager()
    {
    }
}
