#include "Abytek/MathRenderer2D.hpp"
#include "Abytek/World/WorldContextHelper.hpp"
#include "Abytek/RenderCoreManager.hpp"
#include "Abytek/ImGuiRenderData.hpp"
#include "Abytek/RenderCoreHelper.hpp"


namespace Abytek
{
    namespace MathRenderer2D
    {
        ABYTEK_DEFINE_GLOBAL_RENDER_BINDING(F_DrawLineBinding)
        {
            ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::F_DrawLineBinding"));
        }
        ABYTEK_DEFINE_GLOBAL_RENDER_PIPELINE(F_DrawLinePipeline)
        {
            ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::F_DrawLinePipeline"));
        }
    }
    
    F_MathRenderer2DProxy::F_MathRenderer2DProxy(const TW_Valid<F_MathRenderer2D>& MathRenderer2D) :
        _MathRenderer2D(MathRenderer2D),
        _ModuleName(MathRenderer2D->GetModuleName()),
        _Window(MathRenderer2D->GetWindow())
    {
    }
    F_MathRenderer2DProxy::~F_MathRenderer2DProxy()
    {
    }

    void F_MathRenderer2DProxy::Init(const TS<F_RenderRegistryRuntime>& RenderRegistryRuntime)
    {
        _RenderRegistryRuntime = RenderRegistryRuntime;
        
        F_RHIViewportBuildParams ViewportBuildParams;
        ViewportBuildParams.Context = H_RHI::GetMainContext().Weak();
        ViewportBuildParams.Window = _Window;
        ViewportBuildParams.Size = _Window->GetScaledClientSize();
        _Viewport = RACreateAndBuildShared<A_RHIViewport>(ViewportBuildParams);
    }
    void F_MathRenderer2DProxy::Shutdown()
    {
        _Viewport = {};
        
        _RenderRegistryRuntime = {};
    }
    void F_MathRenderer2DProxy::Render(const MathRenderer2D::F_FrameData& FrameData)
    {
        _FrameData = FrameData;
        
        if (_LastFrameData.FrameBufferSize != _FrameData.FrameBufferSize)
        {
            _Viewport->Resize(FrameData.FrameBufferSize);
        }
        
        auto SubmissionList = RACreateAndBuildShared<A_RHISubmissionList>(H_RHI::GetMainProcess().Weak());
#ifdef ABYTEK_DEBUG_INFO
        SubmissionList->SetDebugName(
            F_Name(
                ToText("MathRenderer2DProxy.Frame(")
                + ToText(H_Frame::GetIndex(E_FrameParamType::RENDER))
                + ToText(")")
            )
        );
#endif
        
        UpdateSpace();
        UpdateView();
        ClearBackgroundColor(*SubmissionList);
        DrawAxisLines(*SubmissionList);
        for (const auto& Line : FrameData.Lines)
        {
            DrawLine(*SubmissionList, Line);
        }
        
        H_RHI::GetMainProcess()->AddSubmissionItem(SubmissionList);
        H_RHI::GetMainProcess()->AddViewport(_Viewport);
        
        _LastFrameData = _FrameData;
    }

    void F_MathRenderer2DProxy::UpdateSpace()
    {
        _SpaceUniformData.Transform[0] = F_Vector4_F32 { _FrameData.SpaceTransform[0], 0.0f, 0.0f };
        _SpaceUniformData.Transform[1] = F_Vector4_F32 { _FrameData.SpaceTransform[1], 0.0f, 0.0f };
        _SpaceUniformData.Transform[2] = F_Vector4_F32::Forward();
        _SpaceUniformData.Transform[3] = F_Vector4_F32::Future();
        _SpaceUniformData.InvTransform = Inverse(_SpaceUniformData.Transform);
    }
    void F_MathRenderer2DProxy::UpdateView()
    {
        _ViewUniformData.FrameBufferSize = _FrameData.FrameBufferSize; 
        _ViewUniformData.AspectRatio = (
            static_cast<F32>(_FrameData.FrameBufferSize.X) 
            / static_cast<F32>(_FrameData.FrameBufferSize.Y)
        );
        
        _ViewUniformData.Transform[0] = F_Vector4_F32 { _FrameData.ViewRight, 0.0f, 0.0f } * _FrameData.ViewScale;
        _ViewUniformData.Transform[1] = F_Vector4_F32 { -_FrameData.ViewRight.Y, _FrameData.ViewRight.X, 0.0f, 0.0f } * _FrameData.ViewScale / _ViewUniformData.AspectRatio;
        _ViewUniformData.Transform[2] = F_Vector4_F32 { _FrameData.ViewPosition, 1.0f, 0.0f };
        _ViewUniformData.Transform[3] = F_Vector4_F32::Future();
        _ViewUniformData.InvTransform = Inverse(_ViewUniformData.Transform);
        _ViewUniformData.Scale = _FrameData.ViewScale;
    }
    void F_MathRenderer2DProxy::ClearBackgroundColor(I_RHISubmissionItemContainer& SubmissionItemContainer)
    {
        F_RHIClearRTVPassBuildParams ClearRTVPassBuildParams;
        ClearRTVPassBuildParams.Context = H_RHI::GetMainContext().Weak();
        ClearRTVPassBuildParams.RTV = _Viewport->GetCurrentBackRenderTargetView();
        ClearRTVPassBuildParams.Color = { _FrameData.BackgroundColor, 1.0f };
        auto ClearRTVPass = RACreateAndBuildShared<A_RHIClearRTVPass>(ClearRTVPassBuildParams);
#ifdef ABYTEK_DEBUG
        ClearRTVPass->SetDebugName(ABYTEK_DEBUG_NAME("ClearBackgroundColor"));
#endif
        SubmissionItemContainer.AddSubmissionItem(ClearRTVPass);
    }
    void F_MathRenderer2DProxy::DrawAxisLines(I_RHISubmissionItemContainer& SubmissionItemContainer)
    {
        F32 LineDistance = 999999.0f;
        {
            MathRenderer2D::F_Line Line;
            Line.BeginPosition = -_FrameData.SpaceTransform[0] * LineDistance;
            Line.EndPosition = _FrameData.SpaceTransform[0] * LineDistance;
            Line.ColorAndThickness = { _FrameData.AxisLineColor, _FrameData.AxisLineThickness };
            DrawLine(SubmissionItemContainer, Line);
        }
        {
            MathRenderer2D::F_Line Line;
            Line.BeginPosition = -_FrameData.SpaceTransform[1] * LineDistance;
            Line.EndPosition = _FrameData.SpaceTransform[1] * LineDistance;
            Line.ColorAndThickness = { _FrameData.AxisLineColor, _FrameData.AxisLineThickness };
            DrawLine(SubmissionItemContainer, Line);
        }
    }
    void F_MathRenderer2DProxy::DrawLine(I_RHISubmissionItemContainer& SubmissionItemContainer, const MathRenderer2D::F_Line& Line)
    {
        auto BindGroup = MathRenderer2D::F_DrawLineBinding::Instantiate(_RenderRegistryRuntime).CreateBindGroup();
        BindGroup->BindRTV(
            ABYTEK_NAME("RTV"),
            _Viewport->GetCurrentBackRenderTargetView()
        );
        {
            MathRenderer2D::F_DrawLineUniformData UniformData;
            UniformData.Line = Line;
            UniformData.Space = _SpaceUniformData;
            UniformData.View = _ViewUniformData;
            BindGroup->BindUniformData(
                ABYTEK_NAME("UniformData"),
                UniformData
            );
        }
        BindGroup->Commit();
        
        H_RHIPassUtilities::DrawNonIndexed(
            SubmissionItemContainer,
            MathRenderer2D::F_DrawLinePipeline::Instantiate(_RenderRegistryRuntime).AcquirePipelineState(),
            { BindGroup },
            _Viewport->GetDefaultViewportScissorConfig(),
            F_RHIDrawNonIndexedConfig::Make(6)
        );
    }

    F_MathRenderer2D::F_MathRenderer2D(const F_MathRenderer2DBuildParams& BuildParams) :
        _ModuleName(BuildParams.ModuleName),
        _Window(BuildParams.Window)
    {
        _Proxy = TS<F_MathRenderer2DProxy>()(ABYTEK_WTHIS());
        
        H_Frame::EnqueueCommand<E_FrameParamType::RENDER>(
            [Proxy = _Proxy, RenderRegistry = H_WorldContext::GetUnit<F_RenderCoreManager>(BuildParams.WorldContext)->GetMainRegistry()]()
            {
                Proxy->Init(RenderRegistry->GetOrActiveRuntime(H_RHI::GetMainContext()));
            }
        );
         
        F_ImGuiContextBuildParams ImGuiContextBuildParams;
        ImGuiContextBuildParams.Window = _Window;
        _ImGuiContext = TS<F_ImGuiContext>()(ImGuiContextBuildParams);
    }
    F_MathRenderer2D::~F_MathRenderer2D()
    {
    }

    void F_MathRenderer2D::Shutdown()
    {
        _ImGuiContext->Release();
        
        H_Frame::EnqueueCommand<E_FrameParamType::RENDER>(
            [Proxy = _Proxy]()
            {
                Proxy->Shutdown();
            }
        );
        
        _ImGuiContext = {};
        _Proxy = {};
    }
    B8 F_MathRenderer2D::BeginFrame()
    {
        return _ImGuiContext->BeginFrame();
    }
    void F_MathRenderer2D::EndFrame()
    {
        auto ImGuiRenderData = _ImGuiContext->EndFrame();
        
        _FrameData.FrameBufferSize = _Window->GetScaledClientSize();
        H_Frame::EnqueueCommand<E_FrameParamType::RENDER>(
            [Proxy = _Proxy, FrameData = ABYTEK_MOVE(_FrameData)]()
            {
                Proxy->Render(FrameData);
            }
        );
        
        ImGuiRenderData->Submit();
    }

    void F_MathRenderer2D::SetBackgroundColor(const F_Vector3_F32& Value)
    {
        _FrameData.BackgroundColor = Value;
    }
    void F_MathRenderer2D::SetAxisLineColor(const F_Vector3_F32& Value)
    {
        _FrameData.AxisLineColor = Value;
    }
    void F_MathRenderer2D::SetAxisLineThickness(F32 Value)
    {
        _FrameData.AxisLineThickness = Value;
    }
    void F_MathRenderer2D::MoveView(const F_Vector2_F32& Value)
    {
        _FrameData.ViewPosition += Value;
    }
    void F_MathRenderer2D::ScaleViewSize(F32 Value)
    {
        _FrameData.ViewScale *= Max(1.0f + Value, 0.1f);
    }
    void F_MathRenderer2D::SetViewPosition(const F_Vector2_F32& Value)
    {
        _FrameData.ViewPosition = Value;
    }
    void F_MathRenderer2D::SetViewRight(const F_Vector2_F32& Value)
    {
        _FrameData.ViewRight = Value;
    }
    void F_MathRenderer2D::SetViewScale(F32 Value)
    {
        _FrameData.ViewScale = Value;
    }
    void F_MathRenderer2D::DrawLine(const MathRenderer2D::F_Line& Line)
    {
        _FrameData.Lines.push_back(Line);
    }
}
