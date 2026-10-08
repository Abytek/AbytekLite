#pragma once

#include "Abytek/Engine.NFC.prerequisites.hpp"
#include "Abytek/Renderer/VertexFactory/VertexFactoryType.hpp"
#include "Abytek/Renderer/WorldRenderResourceChild.hpp"


namespace Abytek
{
    class ABYTEK_ENGINE_NFC_API A_VertexFactoryTypeProxy : public A_WorldRenderResourceChild
    {
    public:
        friend class F_VertexFactoryTypeManager;
        
    private:
        F_VertexFactoryTypeId _Id = INVALID_VERTEX_FACTORY_TYPE_ID;
        
    public:
        ABYTEK_FORCE_INLINE auto GetId() const noexcept
        {
            return _Id;
        }
        
    protected:
        A_VertexFactoryTypeProxy(const TW_Valid<A_VertexFactoryType>& VertexFactoryType);
        
    public:
        ~A_VertexFactoryTypeProxy() override;
        
    protected:
        void OnCreateRenderState_RenderTask(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer) override;
        void OnDestroyRenderState_RenderTask(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer) override;
    };
}
