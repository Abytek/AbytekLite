#pragma once

#include "Abytek/Engine.ImGui.prerequisites.hpp"
#include "Abytek/DirectX12/RHIPassExtension.hpp"
#include "Abytek/DirectX12/RHIPassProxyExtension.hpp"
#include "Abytek/ImGuiContext.hpp"


namespace Abytek
{
    class F_ImGuiRenderData;
    
#ifdef ABYTEK_ENGINE_IMGUI_ENABLE_DIRECTX12
    struct F_DirectX12ImGuiPassBuildParams : F_RHIPassBuildParams
    {
        TS<F_ImGuiRenderData> RenderData;
    };
    
    class ABYTEK_ENGINE_IMGUI_API F_DirectX12ImGuiPass : public A_RHIPass, public A_DirectX12RHIPassExtension
    {
    private:
        TS<F_ImGuiRenderData> _RenderData;
        TS<A_RHIResource> _BackFrame;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetRenderData() const noexcept
        {
            return _RenderData;
        }
        ABYTEK_FORCE_INLINE const auto& GetBackFrame() const noexcept
        {
            return _BackFrame;
        }
        
    public:
        ABYTEK_RA_DECLARE_OBJECT_CREATABLE(F_DirectX12ImGuiPass);
        virtual void Build(const F_DirectX12ImGuiPassBuildParams& BuildParams);
        virtual void Release() override;
        
    protected:
        virtual TS_Valid<A_RHIPassProxy> CreateProxy() override;
        
    protected:
        virtual void AppendSubresourceBindings(F_DirectX12RHISubresourceBindingSet& SubresourceBindingSet) override;
        
    public:
        virtual E_DirectX12RHIPassBatchType GetPassBatchType() override;
        virtual E_DirectX12RHIExecutionRangeType GetExecutionRangeType() override;
        
    public:
        virtual E_RHIPassClass GetPassClass() override;
    };
    
    class ABYTEK_ENGINE_IMGUI_API F_DirectX12ImGuiPassProxy : public A_RHIPassProxy, public A_DirectX12RHIPassProxyExtension
    {
    private:
        TS<F_ImGuiRenderData> _RenderData;
        TS<A_RHIResourceProxy> _BackFrameProxy;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetRenderData() const noexcept
        {
            return _RenderData;
        }
        ABYTEK_FORCE_INLINE const auto& GetBackFrameProxy() const noexcept
        {
            return _BackFrameProxy;
        }
        
    public:
        ABYTEK_RA_DECLARE_OBJECT_CREATABLE(F_DirectX12ImGuiPassProxy);
        void Build(const TW_Valid<A_RHIPass>& Pass);
        virtual void Release() override;
        
    public:
        virtual void Execute(F_DirectX12RHIPassProxyExtensionExecuteParams& ExecuteParams) override;
    };
#endif
    
    struct F_ImGuiRenderDataBuildParams
    {
        TS<F_ImGuiContext> Context;
    };
    
    class ABYTEK_ENGINE_IMGUI_API F_ImGuiRenderData : public A_Object
    {
    private:
        TS<F_ImGuiContext> _Context;
        TS<A_RHIViewport> _Viewport;
        TS<A_RHIViewportProxy> _ViewportProxy;
        
#ifdef ABYTEK_ENGINE_IMGUI_ENABLE_DIRECTX12
        F_DirectX12ImGuiPack _DirectX12Pack;
#endif

    public:
        ABYTEK_FORCE_INLINE const auto& GetContext() const noexcept
        {
            return _Context;
        }
        ABYTEK_FORCE_INLINE const auto& GetViewport() const noexcept
        {
            return _Viewport;
        }
        ABYTEK_FORCE_INLINE const auto& GetViewportProxy() const noexcept
        {
            return _ViewportProxy;
        }
        
#ifdef ABYTEK_ENGINE_IMGUI_ENABLE_DIRECTX12
        ABYTEK_FORCE_INLINE const auto& GetDirectX12Pack() const noexcept
        {
            return _DirectX12Pack;
        }
#endif
        
    public:
        F_ImGuiRenderData(const F_ImGuiRenderDataBuildParams& BuildParams);
        virtual ~F_ImGuiRenderData();
        
    private:
        TS<A_RHISubmissionItem> _CreateSubmissionItem();
        
    public:
        void Submit();
    };
}
