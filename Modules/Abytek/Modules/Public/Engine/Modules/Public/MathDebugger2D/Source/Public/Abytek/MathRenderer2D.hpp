#pragma once

#include "Abytek/Engine.MathDebugger2D.prerequisites.hpp"
#include "Abytek/GlobalRenderBinding.hpp"
#include "Abytek/GlobalRenderPipeline.hpp"


namespace Abytek
{
    class F_MathRenderer2D;
    class F_ImGuiContext;
    
    namespace MathRenderer2D
    {
        struct F_Line
        {
            F_Vector2_F32 BeginPosition;
            F_Vector2_F32 EndPosition;
            F_Vector4_F32 ColorAndThickness = F_Vector4_F32 { 1.0f, 1.0f, 1.0f, 0.002f };
        };
        struct F_FrameData
        {
            F_Vector2_U32 FrameBufferSize = F_Vector2_U32::Zero();
            TF_Vector<F_Line> Lines;
            F_Vector3_F32 BackgroundColor = F_Vector3_F32 { 0.33f, 0.33f, 0.33f };
            F_Vector3_F32 AxisLineColor = F_Vector3_F32 { 0.66f, 0.66f, 0.66f };
            F32 AxisLineThickness = 0.002f;
            F_Matrix2x2_F32 SpaceTransform = Identity<F_Matrix2x2_F32>();
            F_Vector2_F32 ViewPosition = F_Vector2_F32::Zero();
            F_Vector2_F32 ViewRight = F_Vector2_F32 { 1.0f, 0.0f };
            F32 ViewScale = 10.0f; // units per screen height
        };
        
        struct ABYTEK_ALIGN(16) F_SpaceUniformData
        {
            F_Matrix4x4_F32 Transform = Identity<F_Matrix4x4_F32>();
            F_Matrix4x4_F32 InvTransform = Identity<F_Matrix4x4_F32>();
        };
        struct ABYTEK_ALIGN(16) F_ViewUniformData
        {
            F_Matrix4x4_F32 Transform = Identity<F_Matrix4x4_F32>();
            F_Matrix4x4_F32 InvTransform = Identity<F_Matrix4x4_F32>();
            F_Vector2_F32 FrameBufferSize = F_Vector2_F32::Zero();
            F32 AspectRatio = 1.0f;
            F32 Scale = 0.0f;
        };
        struct ABYTEK_ALIGN(16) F_DrawLineUniformData
        {
            F_SpaceUniformData Space;
            F_ViewUniformData View;
            F_Line Line;
        };
        struct ABYTEK_ENGINE_MATH_DEBUGGER_2D_API F_DrawLineBinding : TF_GlobalRenderBinding<F_DrawLineBinding>
        {
            ABYTEK_DECLARE_GLOBAL_RENDER_BINDING(F_DrawLineBinding);

            static F_FeedbackStatus Build(F_Config& Config)
            {
                Config.Slots.push_back(
                    F_RHIBindGroupTemplateSlot::MakeRTV(
                        ABYTEK_NAME("RTV"),
                        E_RHIFormat::R8G8B8A8_UNORM
                    ) 
                );
                Config.Slots.push_back(
                    F_RHIBindGroupTemplateSlot::MakeUniformData<F_DrawLineUniformData>(
                        ABYTEK_NAME("UniformData")
                    ) 
                );
                return F_FeedbackStatus::MakeSucceeded();
            }
        };
        struct ABYTEK_ENGINE_MATH_DEBUGGER_2D_API F_DrawLinePipeline : TF_GlobalRenderPipeline<F_DrawLinePipeline>
        {
            ABYTEK_DEFINE_PERMUTATION(
                F_FillMode, 
                TF_Permutation_Set<
                    E_RHIFillMode::SOLID,
                    E_RHIFillMode::WIREFRAME
                >
            );
            ABYTEK_OVERRIDE_PERMUTATION_DOMAIN(F_FillMode);
            
            ABYTEK_DECLARE_GLOBAL_RENDER_PIPELINE(F_DrawLinePipeline);

            static F_FeedbackStatus Build(F_Config& Config)
            {
                Config.Type = E_RHIPipelineStateType::GRAPHICS;
                Config.Rasterizer.FillMode = Config.PermutationVector.Get<F_FillMode>();
                Config.Rasterizer.CullMode = E_RHICullMode::NONE;
                Config.BindGroups.push_back(
                    F_RHIPipelineStateTemplateBindGroup::Make(
                        F_DrawLineBinding::GetTemplateHashCode()
                    )
                );
                Config.VertexShader = ABYTEK_GLOBAL_SHADER("MainVS", "MathRenderer2D/DrawLineVS", E_RHIShaderFrequency::VERTEX);
                Config.PixelShader = ABYTEK_GLOBAL_SHADER("MainPS", "MathRenderer2D/DrawLinePS", E_RHIShaderFrequency::PIXEL);
                return F_FeedbackStatus::MakeSucceeded();
            }
        };
    }
    
    class ABYTEK_ENGINE_MATH_DEBUGGER_2D_API F_MathRenderer2DProxy : public A_Object
    {
    private:
        TW<F_MathRenderer2D> _MathRenderer2D;
        F_Name _ModuleName;
        TW<A_Window> _Window;
        TS<F_RenderRegistryRuntime> _RenderRegistryRuntime;
        MathRenderer2D::F_FrameData _FrameData;
        MathRenderer2D::F_FrameData _LastFrameData;
        
        TS<A_RHIViewport> _Viewport;
        
        MathRenderer2D::F_SpaceUniformData _SpaceUniformData;
        MathRenderer2D::F_ViewUniformData _ViewUniformData;

    public:
        ABYTEK_FORCE_INLINE const auto& GetMathRenderer2D() const noexcept
        {
            return _MathRenderer2D;
        }
        ABYTEK_FORCE_INLINE const auto& GetModuleName() const noexcept
        {
            return _ModuleName;
        }
        ABYTEK_FORCE_INLINE const auto& GetWindow() const noexcept
        {
            return _Window;
        }
        ABYTEK_FORCE_INLINE const auto& GetRenderRegistryRuntime() const noexcept
        {
            return _RenderRegistryRuntime;
        }
        ABYTEK_FORCE_INLINE const auto& GetFrameData() const noexcept
        {
            return _FrameData;
        }
        ABYTEK_FORCE_INLINE const auto& GetLastFrameData() const noexcept
        {
            return _LastFrameData;
        }
        
        ABYTEK_FORCE_INLINE const auto& GetViewport() const noexcept
        {
            return _Viewport;
        }
        
        ABYTEK_FORCE_INLINE const auto& GetSpaceUniformData() const noexcept
        {
            return _SpaceUniformData;
        }
        ABYTEK_FORCE_INLINE const auto& GetViewUniformData() const noexcept
        {
            return _ViewUniformData;
        }
        
    public:
        F_MathRenderer2DProxy(const TW_Valid<F_MathRenderer2D>& MathRenderer2D);
        virtual ~F_MathRenderer2DProxy();
        
    public:
        void Init(const TS<F_RenderRegistryRuntime>& RenderRegistryRuntime);
        void Shutdown();
        void Render(const MathRenderer2D::F_FrameData& FrameData);
        
    public:
        void UpdateSpace();
        void UpdateView();
        void ClearBackgroundColor(I_RHISubmissionItemContainer& SubmissionItemContainer);
        void DrawAxisLines(I_RHISubmissionItemContainer& SubmissionItemContainer);
        void DrawLine(I_RHISubmissionItemContainer& SubmissionItemContainer, const MathRenderer2D::F_Line& Line);
    };
    
    struct F_MathRenderer2DBuildParams
    {
        F_Name ModuleName;
        TW<A_Window> Window;
        TW<A_WorldContext> WorldContext;
    };
    
    class ABYTEK_ENGINE_MATH_DEBUGGER_2D_API F_MathRenderer2D : public A_Object
    {
    private:
        F_Name _ModuleName;
        TW<A_Window> _Window;
        TS<F_MathRenderer2DProxy> _Proxy;
        MathRenderer2D::F_FrameData _FrameData;
        TS<F_ImGuiContext> _ImGuiContext;

    public:
        ABYTEK_FORCE_INLINE const auto& GetModuleName() const noexcept
        {
            return _ModuleName;
        }
        ABYTEK_FORCE_INLINE const auto& GetWindow() const noexcept
        {
            return _Window;
        }
        ABYTEK_FORCE_INLINE const auto& GetProxy() const noexcept
        {
            return _Proxy;
        }
        ABYTEK_FORCE_INLINE const auto& GetFrameData() const noexcept
        {
            return _FrameData;
        }
        ABYTEK_FORCE_INLINE const auto& GetImGuiContext() const noexcept
        {
            return _ImGuiContext;
        }
        
    public:
        F_MathRenderer2D(const F_MathRenderer2DBuildParams& BuildParams);
        ~F_MathRenderer2D() override;
        
    public:
        void Shutdown();
        B8 BeginFrame();
        void EndFrame();
        
    public:
        void SetBackgroundColor(const F_Vector3_F32& Value);
        void SetAxisLineColor(const F_Vector3_F32& Value);
        void SetAxisLineThickness(F32 Value);
        void MoveView(const F_Vector2_F32& Value);
        void ScaleViewSize(F32 Value);
        void SetViewPosition(const F_Vector2_F32& Value);
        void SetViewRight(const F_Vector2_F32& Value);
        void SetViewScale(F32 Value);
        void DrawLine(const MathRenderer2D::F_Line& Line);
    };
}