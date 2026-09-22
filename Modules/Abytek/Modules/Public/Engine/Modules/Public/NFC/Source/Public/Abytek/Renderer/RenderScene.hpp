#pragma once

#include "Abytek/Renderer/RenderObject.hpp"


namespace Abytek
{
    class A_WorldContext;
    class F_RenderGeometryStorage;
    class F_RenderPrimitiveManager;
    
    struct F_RenderSceneBuildParams
    {
    };
    class ABYTEK_ENGINE_NFC_API A_RenderScene : public A_RenderObject
    {
    public:
        friend struct F_RenderSceneUpdateRange;
        friend struct F_RenderScenePostUpdateRange;
        
    private:
        TS<F_RenderGeometryStorage> _GeometryStorage;
        TS<F_RenderPrimitiveManager> _PrimitiveManager;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetGeometryStorage() const noexcept
        {
            return _GeometryStorage;
        }
        ABYTEK_FORCE_INLINE const auto& GetPrimitiveManager() const noexcept
        {
            return _PrimitiveManager;
        }
        
    public:
        ABYTEK_RENDER_OBJECT(A_RenderScene, A_RenderObject);
        
    public:
        virtual void Init(
            const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer, 
            const F_RenderSceneBuildParams& BuildParams
        );
        void Release(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer) override;
        
    protected:
        virtual void OnBeginUpdate(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer);
        virtual void OnEndUpdate(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer);
        virtual void OnBeginPostUpdate(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer);
        virtual void OnEndPostUpdate(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer);
    };
}
