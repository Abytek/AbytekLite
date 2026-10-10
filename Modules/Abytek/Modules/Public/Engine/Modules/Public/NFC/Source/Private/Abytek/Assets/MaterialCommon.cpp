#include "Abytek/Assets/MaterialCommon.hpp"


namespace Abytek
{
    ABYTEK_REFLECT(F_MaterialShaderSource_Slang)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::F_MaterialShaderSource_Slang"));
        
        ABYTEK_REFLECT_PROPERTY_SERIALIZABLE(ModuleName);
    }
    
    ABYTEK_REFLECT(F_MaterialShaderSource)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::F_MaterialShaderSource"));
        
        ABYTEK_REFLECT_PROPERTY_SERIALIZABLE(Type);
        ABYTEK_REFLECT_PROPERTY_SERIALIZABLE(Slang);
    }
}