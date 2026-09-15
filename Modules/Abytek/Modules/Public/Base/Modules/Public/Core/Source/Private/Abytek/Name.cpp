#include "Abytek/Name.hpp"


namespace Abytek
{
    ABYTEK_DEFINE_STRING_DATABASE(F_NameDatabase);
    
    F_NameDatabase::F_NameDatabase() :
        TA_StringDatabase(
            ABYTEK_BASE_CORE_NAME_DATABASE_HASH_SIZE
        )
    { 
        ABYTEK_BIND_STRING_DATABASE();
    }
    F_NameDatabase::~F_NameDatabase()
    {
        ABYTEK_UNBIND_STRING_DATABASE();
    }
}