#pragma once

#include "Abytek/Renderer/RenderObject.hpp"
#include "Abytek/ActorComponents/CameraComponent.hpp"
#include "Abytek/GlobalRenderBinding.hpp"


namespace Abytek
{
    class A_RenderViewFamily;
    
    enum class E_RenderViewFlag : U8
    {
        NONE = 0x0,
        PRIMARY = 0x1,
        MONO = 0x2,
        STEREO_LEFT = 0x4,
        STEREO_RIGHT = 0x8
    };
    ABYTEK_DEFINE_FLAG_OPERATORS(E_RenderViewFlag);
    
    struct ABYTEK_ALIGN(16) F_RenderViewUniformData
    {
        F_Matrix4x4_F32 ViewMatrix = Identity<F_Matrix4x4_F32>();
        F_Matrix4x4_F32 ProjectionMatrix = Identity<F_Matrix4x4_F32>();
        F_Vector2_U32 Resolution = F_Vector2_U32::Zero();
        F32 AspectRatio = 0.0f;
    };
    struct ABYTEK_ENGINE_NFC_API F_RenderViewUniformDataBinding : F_GlobalRenderBinding
    {
        ABYTEK_GLOBAL_RENDER_BINDING(F_RenderViewUniformDataBinding, ABYTEK_NAME("Abytek::F_RenderViewUniformDataBinding"));
        
        static F_FeedbackStatus Build(F_Config& Config)
        {
            Config.Slots.push_back(
                F_RHIBindGroupTemplateSlot::MakeUniformData<F_RenderViewUniformData>(
                    ABYTEK_NAME("ViewUniformData")
                )  
            );
            return F_FeedbackStatus::MakeSucceeded();
        }
    };
    
    struct F_RenderViewBuildParams
    {
        TW<A_RenderViewFamily> Family;
        E_RenderViewFlag Flags = E_RenderViewFlag::NONE;
        F_CameraProjectionOptions ProjectionOptions;
        F_Vector2_U32 Resolution = F_Vector2_U32::Zero();
        F32 AspectRatio = 0.0f;
        F_Matrix4x4_F32 ViewMatrix = Identity<F_Matrix4x4_F32>();
        
        TS<A_RHIResourceView> RTV;
    };
    class ABYTEK_ENGINE_NFC_API A_RenderView : public A_RenderObject
    {
    private:
        TW<A_RenderViewFamily> _Family;
        E_RenderViewFlag _Flags = E_RenderViewFlag::NONE;
        F_CameraProjectionOptions _ProjectionOptions;
        F_RenderViewUniformData _UniformData;
        
        TS<A_RHIBindGroup> _UniformBindGroup;
        TS<A_RHIResourceView> _RTV;
        F_RHIViewportScissorConfig _DefaultViewportScissorConfig;
        
        TS<A_RHIResource> _DepthBuffer;
        TS<A_RHIResourceView> _DSV;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetFamily() const noexcept
        {
            return _Family;
        }
        ABYTEK_FORCE_INLINE auto GetFlags() const noexcept
        {
            return _Flags;
        }
        ABYTEK_FORCE_INLINE B8 HasFlags(E_RenderViewFlag X) const noexcept
        {
            return FlagHas(_Flags, X);
        }
        ABYTEK_FORCE_INLINE B8 HasFlagsAny(E_RenderViewFlag X) const noexcept
        {
            return FlagHasAny(_Flags, X);
        }
        ABYTEK_FORCE_INLINE const auto& GetProjectionOptions() const noexcept
        {
            return _ProjectionOptions;
        }
        ABYTEK_FORCE_INLINE const auto& GetUniformData() const noexcept
        {
            return _UniformData;
        }
        ABYTEK_FORCE_INLINE const auto& GetResolution() const noexcept
        {
            return _UniformData.Resolution;
        }
        void SetResolution(const F_Vector2_U32& X)
        {
            _UniformData.Resolution = X;
        }
        ABYTEK_FORCE_INLINE auto GetAspectRatio() const noexcept
        {
            return _UniformData.AspectRatio;
        }
        void SetAspectRatio(const F32 X)
        {
            _UniformData.AspectRatio = X;
        }
        ABYTEK_FORCE_INLINE const auto& GetViewMatrix() const noexcept
        {
            return _UniformData.ViewMatrix;
        }
        void SetViewMatrix(const F_Matrix4x4_F32 X)
        {
            _UniformData.ViewMatrix = X;
        }
        ABYTEK_FORCE_INLINE const auto& GetProjectionMatrix() const noexcept
        {
            return _UniformData.ProjectionMatrix;
        }
        
        ABYTEK_FORCE_INLINE const auto& GetUniformBindGroup() const noexcept
        {
            return _UniformBindGroup;
        }
        ABYTEK_FORCE_INLINE const auto& GetRTV() const noexcept
        {
            return _RTV;
        }
        void SetRTV(const TS<A_RHIResourceView>& X)
        {
            _RTV = X;
        }
        ABYTEK_FORCE_INLINE const auto& GetDefaultViewportScissorConfig() const noexcept
        {
            return _DefaultViewportScissorConfig;
        }
        
        ABYTEK_FORCE_INLINE const auto& GetDepthBuffer() const noexcept
        {
            return _DepthBuffer;
        }
        ABYTEK_FORCE_INLINE const auto& GetDSV() const noexcept
        {
            return _DSV;
        }
        
    public:
        ABYTEK_RENDER_OBJECT(A_RenderView, A_RenderObject);
        
    public:
        virtual void Init(
            const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer, 
            const F_RenderViewBuildParams& BuildParams
        );
        void Release(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer) override;
        
    protected:
        virtual void OnBeginFrame(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer);
        virtual void OnEndFrame(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer);
        
    public:
        void BeginFrame(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer);
        void EndFrame(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer);
        
    public:
        void UpdateProjectionMatrix(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer);
        void UpdateUniformBindGroup(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer);
        void UpdateDefaultViewportScissorConfig(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer);
        void UpdateDepthBufferAndDSV(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer);
    };
}
