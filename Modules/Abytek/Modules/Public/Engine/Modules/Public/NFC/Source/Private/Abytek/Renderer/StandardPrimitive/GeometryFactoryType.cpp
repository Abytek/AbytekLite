#include "Abytek/Renderer/StandardPrimitive/GeometryFactoryType.hpp"
#include "Abytek/Renderer/StandardPrimitive/GeometryFactoryTypeProxy.hpp"
#include "Abytek/Development/Cook/CookProfile.hpp"
#include "Abytek/Assets/StandardMaterial.hpp"


namespace Abytek
{
    ABYTEK_REFLECT(A_GeometryFactoryType)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::A_GeometryFactoryType"));
    }
    
    A_GeometryFactoryType::A_GeometryFactoryType(const F_SerializableObjectInitParams& InitParams) :
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
    A_GeometryFactoryType::~A_GeometryFactoryType()
    {
    }

    void A_GeometryFactoryType::OnLoad()
    {
#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
        PrepareDevelopmentItems(GetEnvironment());
#endif
        SetupRenderable();
    }
    void A_GeometryFactoryType::OnUnload()
    {
        CleanUpRenderable();
    }

#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
    void A_GeometryFactoryType::PrepareDevelopmentItems(const TW_Valid<F_SerializableEnvironment>& SerializableEnvironment)
    {
    }
#endif

#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
    void A_GeometryFactoryType::PrepareForCooking()
    {
    }
    void A_GeometryFactoryType::Cook()
    {
        auto CookProfile = F_CookProfile::GetMain();
        auto SerializableEnvironment = CookProfile->GetSerializableEnvironment();
        PrepareDevelopmentItems(
            SerializableEnvironment.Weak()
        );
    }
    void A_GeometryFactoryType::CleanUpAfterCooking()
    {
    }
#endif

    void A_GeometryFactoryType::OnCreateRenderState()
    {
        H_Frame::EnqueueCommand<E_FrameParamType::RENDER>(
            [
                CachedRenderProxy = GetRenderProxy().FastCast<A_GeometryFactoryTypeProxy>(),
                HashCode = GetHashCode()
            ]
            {
                CachedRenderProxy->_HashCode = HashCode;
            }
        );
    }
    void A_GeometryFactoryType::OnDestroyRenderState()
    {
    }

    F_GeometryFactoryTypeHashCode A_GeometryFactoryType::GetHashCode() const
    {
        return GetType()->GetHashCode();
    }

#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
    B8 A_GeometryFactoryType::ShouldCompilePermutation(
        const TS<F_RenderRegistry>& RenderRegistry,
        const TW_Valid<A_RenderPackTemplateMap>& RenderPackTemplateMap, 
        const TS<F_StandardMaterial>& Material,
        const F_MaterialPropertyInstanceList& MaterialPermutation,
        const F_StandardMaterialCompileTarget& MaterialCompileTarget
        )
    {
        return true;
    }
    void A_GeometryFactoryType::SetupCompilePermutation(
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
