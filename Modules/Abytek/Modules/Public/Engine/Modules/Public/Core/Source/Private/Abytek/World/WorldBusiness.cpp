#include "Abytek/World/WorldBusiness.hpp"
#include "Abytek/World/World.hpp"


namespace Abytek
{
    ABYTEK_REFLECT(F_WorldBusiness)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::F_WorldBusiness"));
    }

    F_WorldBusiness::F_WorldBusiness(const F_SerializableObjectInitParams& InitParams) :
        A_WorldContext(InitParams)
    {
    }
    F_WorldBusiness::~F_WorldBusiness()
    {
    }

    void F_WorldBusiness::OnBeginPlay()
    {
    }
    void F_WorldBusiness::OnEndPlay()
    {
    }

    void F_WorldBusiness::BeginPlay()
    {
        ABYTEK_ENGINE_NFC_ASSERT(!_BeganPlay);
        OnBeginPlay();
    }
    void F_WorldBusiness::EndPlay()
    {
        ABYTEK_ENGINE_NFC_ASSERT(_BeganPlay);
        OnEndPlay();
    }
}
