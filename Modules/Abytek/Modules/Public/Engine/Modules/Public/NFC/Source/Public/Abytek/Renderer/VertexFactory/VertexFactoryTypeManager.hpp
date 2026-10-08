#pragma once

#include "Abytek/Renderer/RenderObject.hpp"
#include "Abytek/Renderer/VertexFactory/VertexFactoryType.hpp"


namespace Abytek
{
    class A_RenderScene;
    
    struct F_VertexFactoryTypeManagerBuildParams
    {
        TW<A_RenderScene> Scene;
    };
    class ABYTEK_ENGINE_NFC_API F_VertexFactoryTypeManager final : public A_RenderObject
    {
    public:
        friend class A_VertexFactoryTypeProxy;
        
    private:
        TW<A_RenderScene> _Scene;
        
        F_VertexFactoryTypeId _NextVertexFactoryTypeId = 0;
        TF_Vector<F_VertexFactoryTypeId> _FreeVertexFactoryTypeIds;
        TF_Vector<TW<A_VertexFactoryTypeProxy>> _Types;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetScene() const noexcept
        {
            return _Scene;
        }
        
        ABYTEK_FORCE_INLINE const auto& GetTypes() const noexcept
        {
            return _Types;
        }
        
    public:
        ABYTEK_RENDER_OBJECT_CREATABLE(F_VertexFactoryTypeManager, A_RenderObject);
        
    public:
        void Init(
            const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer, 
            const F_VertexFactoryTypeManagerBuildParams& BuildParams
        );
        void Release(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer) override;
        
    private:
        void _RegisterType(const TW_Valid<A_VertexFactoryTypeProxy>& Type);
        void _UnregisterType(const TW_Valid<A_VertexFactoryTypeProxy>& Type);
    };
}
