#pragma once

#include "Abytek/Base.Serializable.prerequisites.pch.hpp"


namespace Abytek
{
    enum class E_SerializableObjectFlag : U8
    {
        NONE = 0x0,
        CDO = 0x1,
        DEFAULT = NONE
    };
    ABYTEK_DEFINE_FLAG_OPERATORS(E_SerializableObjectFlag);
    ABYTEK_ENUM_REFLECTOR_LOCALNS(E_SerializableObjectFlag)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::E_SerializableObjectFlag"));
        ABYTEK_REFLECT_ENUM_VALUE(NONE);
        ABYTEK_REFLECT_ENUM_VALUE(CDO);
        ABYTEK_REFLECT_ENUM_VALUE(DEFAULT);
    }
}