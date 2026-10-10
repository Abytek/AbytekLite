#include "Abytek/Renderer/StandardPrimitive/MaterialTargetType.hpp"
#include "Abytek/Renderer/StandardPrimitive/MaterialTargetTypeProxy.hpp"
#include "Abytek/Development/Cook/CookProfile.hpp"
#include "Abytek/Assets/StandardMaterial.hpp"


namespace Abytek
{
    ABYTEK_REFLECT(A_MaterialTargetType)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::A_MaterialTargetType"));
    }
    
    A_MaterialTargetType::A_MaterialTargetType(const F_SerializableObjectInitParams& InitParams) :
        A_WorldContext(InitParams)
    {
        if (!HasSerializableFlags(E_SerializableObjectFlag::CDO))
        {
            ABYTEK_ENGINE_NFC_ASSERT(GetPackageName()) << "Requires package name for non-CDO vertex factory type objects";
#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
            _WorldContextDevelopmentData = CreateSerializableSubobjectDelayLoading<F_WorldContextDevelopmentData>(
                ABYTEK_NAME("WorldContextDevelopmentData")
            );
#endif
        }
    }
    A_MaterialTargetType::~A_MaterialTargetType()
    {
    }

    void A_MaterialTargetType::OnLoad()
    {
#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
        PrepareDevelopmentItems(GetEnvironment());
#endif
        SetupRenderable();
    }
    void A_MaterialTargetType::OnUnload()
    {
        CleanUpRenderable();
    }

#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
    void A_MaterialTargetType::PrepareDevelopmentItems(const TW_Valid<F_SerializableEnvironment>& SerializableEnvironment)
    {
    }
#endif

#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
    void A_MaterialTargetType::PrepareForCooking()
    {
    }
    void A_MaterialTargetType::Cook()
    {
        auto CookProfile = F_CookProfile::GetMain();
        auto SerializableEnvironment = CookProfile->GetSerializableEnvironment();
        PrepareDevelopmentItems(
            SerializableEnvironment.Weak()
        );
    }
    void A_MaterialTargetType::CleanUpAfterCooking()
    {
    }
#endif

    void A_MaterialTargetType::OnCreateRenderState()
    {
        H_Frame::EnqueueCommand<E_FrameParamType::RENDER>(
            [
                CachedRenderProxy = GetRenderProxy().FastCast<A_MaterialTargetTypeProxy>(),
                HashCode = GetHashCode()
            ]
            {
                CachedRenderProxy->_HashCode = HashCode;
            }
        );
    }
    void A_MaterialTargetType::OnDestroyRenderState()
    {
    }

    F_MaterialTargetTypeHashCode A_MaterialTargetType::GetHashCode() const
    {
        return GetType()->GetHashCode();
    }

#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
    B8 A_MaterialTargetType::ShouldCompilePermutation(
        const TS<F_RenderRegistry>& RenderRegistry,
        const TW_Valid<A_RenderPackTemplateMap>& RenderPackTemplateMap, 
        const TS<F_StandardMaterial>& Material,
        const F_MaterialPropertyInstanceList& MaterialPermutation,
        const F_StandardMaterialCompileTarget& MaterialCompileTarget
        )
    {
        return true;
    }
    void A_MaterialTargetType::SetupCompilePermutation(
        const TS<F_RenderRegistry>& RenderRegistry,
        const TW_Valid<A_RenderPackTemplateMap>& RenderPackTemplateMap, 
        const TS<F_StandardMaterial>& Material,
        const F_MaterialPropertyInstanceList& MaterialPermutation,
        const F_StandardMaterialCompileTarget& MaterialCompileTarget,
        F_MaterialCompilePermutation& CompilePermutation
    )
    {
    }
#endif
}
