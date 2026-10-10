#pragma once

#include "Abytek/Renderer/RenderObject.hpp"
#include "Abytek/Renderer/StandardPrimitive/GeometryFactoryType.hpp"


namespace Abytek
{
    class A_RenderScene;
    
    struct F_GeometryFactoryTypeProxyManagerBuildParams
    {
        TW<A_RenderScene> Scene;
    };
    class ABYTEK_ENGINE_NFC_API F_GeometryFactoryTypeProxyManager final : public A_RenderObject
    {
    public:
        friend class A_GeometryFactoryTypeProxy;
        
    private:
        TW<A_RenderScene> _Scene;
        
        F_GeometryFactoryTypeId _NextGeometryFactoryTypeId = 0;
        TF_Vector<F_GeometryFactoryTypeId> _FreeGeometryFactoryTypeIds;
        TF_Vector<TW<A_GeometryFactoryTypeProxy>> _TypeProxies;
        TF_Map<F_GeometryFactoryTypeHashCode, TW<A_GeometryFactoryTypeProxy>> _HashCodeToTypeProxies;
        
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
        ABYTEK_RENDER_OBJECT_CREATABLE(F_GeometryFactoryTypeProxyManager, A_RenderObject);
        
    public:
        void Init(
            const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer, 
            const F_GeometryFactoryTypeProxyManagerBuildParams& BuildParams
        );
        void Release(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer) override;
        
    private:
        void _RegisterTypeProxy(const TW_Valid<A_GeometryFactoryTypeProxy>& Type);
        void _UnregisterTypeProxy(const TW_Valid<A_GeometryFactoryTypeProxy>& Type);
        
    public:
        B8 HasTypeProxy(F_GeometryFactoryTypeHashCode HashCode) const;
        TW<A_GeometryFactoryTypeProxy> FindTypeProxy(F_GeometryFactoryTypeHashCode HashCode) const;
        TW_Valid<A_GeometryFactoryTypeProxy> GetTypeProxy(F_GeometryFactoryTypeHashCode HashCode) const;
    };
}
