#include "Abytek/World/WorldContext.hpp"
#include "Abytek/World/World.hpp"


namespace Abytek
{
    ABYTEK_REFLECT(A_WorldContext)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::A_WorldContext"));
    }

    A_WorldContext::A_WorldContext(const F_SerializableObjectInitParams& InitParams) :
        A_SerializableObject(InitParams)
    {
    }
    A_WorldContext::~A_WorldContext()
    {
    }
    
    TW_Valid<F_World> A_WorldContext::GetWorld() const
    {
        return GetEnvironment()->GetOwner().FastCast<F_World>();
    }
}
