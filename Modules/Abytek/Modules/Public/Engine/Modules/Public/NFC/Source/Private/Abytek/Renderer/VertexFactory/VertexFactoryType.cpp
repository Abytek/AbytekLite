#include "Abytek/Renderer/VertexFactory/VertexFactoryType.hpp"

#include "Abytek/Development/Cook/CookProfile.hpp"


namespace Abytek
{
    ABYTEK_REFLECT(A_VertexFactoryType)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::A_VertexFactoryType"));
    }
    
    A_VertexFactoryType::A_VertexFactoryType(const F_SerializableObjectInitParams& InitParams) :
        A_WorldContext(InitParams)
    {
        if (!HasSerializableFlags(E_SerializableObjectFlag::CDO))
        {
            ABYTEK_ENGINE_NFC_ASSERT(GetPackageName()) << "Requires package name for non-CDO vertex factory type objects";
        }
#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
        _WorldContextDevelopmentData = CreateSerializableSubobjectDelayLoading<F_WorldContextDevelopmentData>(
            ABYTEK_NAME("WorldContextDevelopmentData")
        );
#endif
    }
    A_VertexFactoryType::~A_VertexFactoryType()
    {
    }

    void A_VertexFactoryType::OnLoad()
    {
#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
        PrepareDevelopmentItems(GetEnvironment());
#endif
        SetupRenderable();
    }
    void A_VertexFactoryType::OnUnload()
    {
        CleanUpRenderable();
    }

#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
    void A_VertexFactoryType::PrepareDevelopmentItems(const TW_Valid<F_SerializableEnvironment>& SerializableEnvironment)
    {
    }
#endif

#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
    void A_VertexFactoryType::PrepareForCooking()
    {
    }
    void A_VertexFactoryType::Cook()
    {
        auto CookProfile = F_CookProfile::GetMain();
        auto SerializableEnvironment = CookProfile->GetSerializableEnvironment();
        PrepareDevelopmentItems(
            SerializableEnvironment.Weak()
        );
    }
    void A_VertexFactoryType::CleanUpAfterCooking()
    {
    }
#endif
}
