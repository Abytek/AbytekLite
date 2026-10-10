#include "Abytek/Renderer/RenderScene.hpp"
#include "Abytek/RenderCoreManager.hpp"
#include "Abytek/RenderRegistry.hpp"
#include "Abytek/World/WorldContextHelper.hpp"
#include "Abytek/World/WorldSubsystemContainer.hpp"
#include "Abytek/Renderer/RendererManager.hpp"
#include "Abytek/Renderer/RenderObjectFactory.hpp"
#include "Abytek/Renderer/GPUData/GPUData.hpp"
#include "Abytek/Renderer/RenderGeometry/RenderGeometryStorage.hpp"
#include "Abytek/Renderer/RenderPrimitive/RenderPrimitiveManager.hpp"
#include "Abytek/Renderer/StandardPrimitive/GeometryFactoryTypeProxyManager.hpp"
#include "Abytek/Renderer/StandardPrimitive/MaterialTargetTypeProxyManager.hpp"
#include "Abytek/Renderer/RenderPrimitive/Archetypes/Processor_Simple.hpp"
#include "Abytek/Renderer/StandardPrimitive/RenderPrimitiveProcessor.hpp"


namespace Abytek
{
    void A_RenderScene::Init(
        const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer,
        const F_RenderSceneBuildParams& BuildParams
    )
    {
        InitMinimal(SubmissionItemContainer);
        
        F_RenderGeometryStorageBuildParams GeometryStorageBuildParams;
        GeometryStorageBuildParams.Scene = ABYTEK_WTHIS();
#ifdef ABYTEK_DEBUG_INFO
        _GeometryStorage = F_RenderGeometryStorage::CreateAndInit_WithDebugName(
            *GetDebugName()
            + ABYTEK_TEXT(".GeometryStorage"),
#else
        _GeometryStorage = F_RenderGeometryStorage::CreateAndInit(
#endif
            GetWorldRenderResource(),
            SubmissionItemContainer, 
            GeometryStorageBuildParams
        );
        
        {
            F_RenderPrimitiveManagerBuildParams PrimitiveManagerBuildParams;
            PrimitiveManagerBuildParams.Scene = ABYTEK_WTHIS();
#ifdef ABYTEK_DEBUG_INFO
            _PrimitiveManager = F_RenderPrimitiveManager::CreateAndInit_WithDebugName(
                *GetDebugName()
                + ABYTEK_TEXT(".PrimitiveManager"),
#else
            _PrimitiveManager = F_RenderPrimitiveManager::CreateAndInit(
#endif
                GetWorldRenderResource(),
                SubmissionItemContainer, 
                PrimitiveManagerBuildParams
            );
        }
        
        {
            F_GeometryFactoryTypeProxyManagerBuildParams GeometryFactoryTypeProxyManagerBuildParams;
            GeometryFactoryTypeProxyManagerBuildParams.Scene = ABYTEK_WTHIS();
#ifdef ABYTEK_DEBUG_INFO
            _GeometryFactoryTypeProxyManager = F_GeometryFactoryTypeProxyManager::CreateAndInit_WithDebugName(
                *GetDebugName()
                + ABYTEK_TEXT(".GeometryFactoryTypeProxyManager"),
#else
            _GeometryFactoryTypeProxyManager = F_GeometryFactoryTypeProxyManager::CreateAndInit(
#endif
                GetWorldRenderResource(),
                SubmissionItemContainer, 
                GeometryFactoryTypeProxyManagerBuildParams
            );
        }
        
        {
            F_MaterialTargetTypeProxyManagerBuildParams MaterialTargetTypeProxyManagerBuildParams;
            MaterialTargetTypeProxyManagerBuildParams.Scene = ABYTEK_WTHIS();
#ifdef ABYTEK_DEBUG_INFO
            _MaterialTargetTypeProxyManager = F_MaterialTargetTypeProxyManager::CreateAndInit_WithDebugName(
                *GetDebugName()
                + ABYTEK_TEXT(".MaterialTargetTypeProxyManager"),
#else
            _MaterialTargetTypeProxyManager = F_MaterialTargetTypeProxyManager::CreateAndInit(
#endif
                GetWorldRenderResource(),
                SubmissionItemContainer, 
                MaterialTargetTypeProxyManagerBuildParams
            );
        }
        
        {
            if (auto Processor = GetRenderObjectFactory()->CreatePrimitiveProcessor_Simple())
            {
#ifdef ABYTEK_DEBUG_INFO
                Processor->SetDebugName(
                    *GetDebugName()
                    + ABYTEK_TEXT(".PrimitiveProcessor_Simple")
                );
#endif
                _PrimitiveProcessor_Simple = _PrimitiveManager->AddProcessor(
                    SubmissionItemContainer,
                    Processor
                );
            }
        }
        
        {
            if (auto Processor = GetRenderObjectFactory()->CreatePrimitiveProcessor_Standard())
            {
#ifdef ABYTEK_DEBUG_INFO
                Processor->SetDebugName(
                    *GetDebugName()
                    + ABYTEK_TEXT(".PrimitiveProcessor_Standard")
                );
#endif
                _PrimitiveProcessor_Standard = _PrimitiveManager->AddProcessor(
                    SubmissionItemContainer,
                    Processor
                );
            }
        }
    }
    void A_RenderScene::Release(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
        _MaterialTargetTypeProxyManager->Release(SubmissionItemContainer);
        _MaterialTargetTypeProxyManager = {};
        
        _GeometryFactoryTypeProxyManager->Release(SubmissionItemContainer);
        _GeometryFactoryTypeProxyManager = {};
        
        _PrimitiveManager->Release(SubmissionItemContainer);
        _PrimitiveManager = {};
        
        _GeometryStorage->Release(SubmissionItemContainer);
        _GeometryStorage = {};
        
        A_RenderObject::Release(SubmissionItemContainer);
    }

    void A_RenderScene::OnBeginUpdate(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
        _GeometryStorage->BeginUpdate(SubmissionItemContainer);
        _PrimitiveManager->BeginUpdate(SubmissionItemContainer);
    }
    void A_RenderScene::OnEndUpdate(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
        _PrimitiveManager->EndUpdate(SubmissionItemContainer);
        _GeometryStorage->EndUpdate(SubmissionItemContainer);
    }
    void A_RenderScene::OnBeginPostUpdate(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
    }
    void A_RenderScene::OnEndPostUpdate(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
        _PrimitiveManager->FinalizeFrame(SubmissionItemContainer);
        _GeometryStorage->FinalizeFrame(SubmissionItemContainer);
    }
}
