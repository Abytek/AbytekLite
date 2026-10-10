#pragma once

#include "Abytek/Engine.SRP.prerequisites.hpp"
#include "Abytek/Renderer/StandardPrimitive/GeometryFactoryTypeProxy_StaticMeshECMS.hpp"


namespace Abytek
{
    class ABYTEK_ENGINE_SRP_API F_SRPGeometryFactoryTypeProxy_StaticMeshECMS : public A_GeometryFactoryTypeProxy_StaticMeshECMS
    {
    private:
        
    public:
        
    public:
        F_SRPGeometryFactoryTypeProxy_StaticMeshECMS(const TW_Valid<A_GeometryFactoryType>& GeometryFactoryType);
        ~F_SRPGeometryFactoryTypeProxy_StaticMeshECMS() override;
        
    protected:
        void OnCreateRenderState_RenderTask(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer) override;
        void OnDestroyRenderState_RenderTask(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer) override;
    };
}
