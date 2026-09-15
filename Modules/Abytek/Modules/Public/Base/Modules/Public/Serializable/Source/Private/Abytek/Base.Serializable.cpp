#include "Abytek/Base.Serializable.prerequisites.pch.hpp"
#include "Abytek/JSONSerializable.hpp"
#include "Abytek/Serializable.hpp"
#include "Abytek/SerializableObject.hpp"
#include "Abytek/SerializablePackage.hpp"
#include "Abytek/SerializableEnvironment.hpp"


namespace Abytek
{
    void SetupReflectionSession_Base_Serializable(const TW_Valid<F_ReflectionSession>& Session)
    {
        Session->RegisterType<I_JSONSerializable>();
        Session->RegisterType<I_BinarySerializable>();
        Session->RegisterType<I_Serializable>();
        Session->RegisterType<E_SerializableObjectFlag>();
        Session->RegisterType<A_SerializableObject>();
        Session->RegisterType<F_SerializablePackage>();
        Session->RegisterType<F_SerializableEnvironment>();
    }
}