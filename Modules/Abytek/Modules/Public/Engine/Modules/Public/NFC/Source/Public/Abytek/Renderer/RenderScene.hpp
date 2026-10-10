#pragma once

#include "Abytek/Renderer/RenderObject.hpp"


namespace Abytek
{
    class A_RenderPrimitiveProcessor_Simple;
    class A_RenderPrimitiveProcessor_Standard;
    class A_WorldContext;
    class F_RenderGeometryStorage;
    class F_RenderPrimitiveManager;
    class F_GeometryFactoryTypeProxyManager;
    class F_MaterialTargetTypeProxyManager;
    
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
        TS<F_GeometryFactoryTypeProxyManager> _GeometryFactoryTypeProxyManager;
        TS<F_MaterialTargetTypeProxyManager> _MaterialTargetTypeProxyManager;
        
        TW<A_RenderPrimitiveProcessor_Simple> _PrimitiveProcessor_Simple;
        TW<A_RenderPrimitiveProcessor_Standard> _PrimitiveProcessor_Standard;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetGeometryStorage() const noexcept
        {
            return _GeometryStorage;
        }
        ABYTEK_FORCE_INLINE const auto& GetPrimitiveManager() const noexcept
        {
            return _PrimitiveManager;
        }
        ABYTEK_FORCE_INLINE const auto& GetGeometryFactoryTypeProxyManager() const noexcept
        {
            return _GeometryFactoryTypeProxyManager;
        }
        ABYTEK_FORCE_INLINE const auto& GetMaterialTargetTypeProxyManager() const noexcept
        {
            return _MaterialTargetTypeProxyManager;
        }
        
        ABYTEK_FORCE_INLINE const auto& GetPrimitiveProcessor_Simple() const noexcept
        {
            return _PrimitiveProcessor_Simple;
        }
        ABYTEK_FORCE_INLINE const auto& GetPrimitiveProcessor_Standard() const noexcept
        {
            return _PrimitiveProcessor_Standard;
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
