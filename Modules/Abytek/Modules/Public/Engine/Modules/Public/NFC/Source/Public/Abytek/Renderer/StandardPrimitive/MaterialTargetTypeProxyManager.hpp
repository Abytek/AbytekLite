#pragma once

#include "Abytek/Renderer/RenderObject.hpp"
#include "Abytek/Renderer/StandardPrimitive/MaterialTargetType.hpp"


namespace Abytek
{
    class A_RenderScene;
    
    struct F_MaterialTargetTypeProxyManagerBuildParams
    {
        TW<A_RenderScene> Scene;
    };
    class ABYTEK_ENGINE_NFC_API F_MaterialTargetTypeProxyManager final : public A_RenderObject
    {
    public:
        friend class A_MaterialTargetTypeProxy;
        
    private:
        TW<A_RenderScene> _Scene;
        
        F_MaterialTargetTypeId _NextMaterialTargetTypeId = 0;
        TF_Vector<F_MaterialTargetTypeId> _FreeMaterialTargetTypeIds;
        TF_Vector<TW<A_MaterialTargetTypeProxy>> _TypeProxies;
        TF_Map<F_MaterialTargetTypeHashCode, TW<A_MaterialTargetTypeProxy>> _HashCodeToTypeProxies;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetScene() const noexcept
        {
            return _Scene;
        }
        
        ABYTEK_FORCE_INLINE const auto& GetTypeProxies() const noexcept
        {
            return _TypeProxies;
        }
        ABYTEK_FORCE_INLINE const auto& GetHashCodeToTypeProxies() const noexcept
        {
            return _HashCodeToTypeProxies;
        }
        
    public:
        ABYTEK_RENDER_OBJECT_CREATABLE(F_MaterialTargetTypeProxyManager, A_RenderObject);
        
    public:
        void Init(
            const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer, 
            const F_MaterialTargetTypeProxyManagerBuildParams& BuildParams
        );
        void Release(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer) override;
        
    private:
        void _RegisterTypeProxy(const TW_Valid<A_MaterialTargetTypeProxy>& Type);
        void _UnregisterTypeProxy(const TW_Valid<A_MaterialTargetTypeProxy>& Type);
        
    public:
        B8 HasTypeProxy(F_MaterialTargetTypeHashCode HashCode) const;
        TW<A_MaterialTargetTypeProxy> FindTypeProxy(F_MaterialTargetTypeHashCode HashCode) const;
        TW_Valid<A_MaterialTargetTypeProxy> GetTypeProxy(F_MaterialTargetTypeHashCode HashCode) const;
    };
}
