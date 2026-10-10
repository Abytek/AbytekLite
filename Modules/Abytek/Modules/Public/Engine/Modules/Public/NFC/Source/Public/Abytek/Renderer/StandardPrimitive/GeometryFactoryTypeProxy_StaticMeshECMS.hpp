#pragma once

#include "Abytek/Engine.NFC.prerequisites.hpp"
#include "Abytek/Renderer/StandardPrimitive/GeometryFactoryTypeProxy.hpp"


namespace Abytek
{
    class ABYTEK_ENGINE_NFC_API A_GeometryFactoryTypeProxy_StaticMeshECMS : public A_GeometryFactoryTypeProxy
    {
    private:
        
    public:
        
    protected:
        A_GeometryFactoryTypeProxy_StaticMeshECMS(const TW_Valid<A_GeometryFactoryType>& GeometryFactoryType);
        
    public:
        ~A_GeometryFactoryTypeProxy_StaticMeshECMS() override;
        
    protected:
        void OnCreateRenderState_RenderTask(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer) override;
        void OnDestroyRenderState_RenderTask(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer) override;
    };
}
