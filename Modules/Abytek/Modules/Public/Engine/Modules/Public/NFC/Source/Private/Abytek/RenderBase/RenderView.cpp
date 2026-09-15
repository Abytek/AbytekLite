#include "Abytek/RenderBase/RenderView.hpp"
#include "Abytek/ActorComponents/Render/CameraRenderProxy.hpp"


namespace Abytek
{
    ABYTEK_DEFINE_GLOBAL_RENDER_BINDING(F_RenderViewUniformDataBinding)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::F_RenderViewUniformDataBinding"))
    }
    
    A_RenderView::A_RenderView(const TW_Valid<F_WorldRenderResource>& WorldRenderResource, const F_RenderViewBuildParams& BuildParams) :
        A_RenderObject(WorldRenderResource),
        _Family(BuildParams.Family),
        _Flags(BuildParams.Flags),
        _ProjectionOptions(BuildParams.ProjectionOptions),
        _RTV(BuildParams.RTV)
    {
        _UniformData.Resolution = BuildParams.Resolution;
        _UniformData.AspectRatio = BuildParams.AspectRatio;
        _UniformData.ViewMatrix = BuildParams.ViewMatrix;
        UpdateProjectionMatrix();
        
        _UniformBindGroup = F_RenderViewUniformDataBinding::Instantiate(
            GetRenderRegistryRuntime()
        ).CreateBindGroup();
        UpdateUniformBindGroup();
        
        UpdateDefaultViewportScissorConfig();
        
        UpdateDepthBufferAndDSV();
    }
    A_RenderView::~A_RenderView()
    {
    }

    void A_RenderView::OnBeginFrame()
    {
        UpdateProjectionMatrix();
        UpdateUniformBindGroup();
        UpdateDefaultViewportScissorConfig();
        UpdateDepthBufferAndDSV();
        
        auto Process = H_RHI::GetMainProcess();
        auto SubmissionList = RACreateAndBuildShared<A_RHISubmissionList>(
            Process.Weak()
        );
        if (_DSV)
        {
            H_RHIPassUtilities::ClearDSV(
                *SubmissionList,
                _DSV
            );
        }
        Process->AddSubmissionItem(SubmissionList);
    }
    void A_RenderView::OnEndFrame()
    {
    }

    void A_RenderView::BeginFrame()
    {
        OnBeginFrame();
    }
    void A_RenderView::EndFrame()
    {
        OnEndFrame();
    }

    void A_RenderView::UpdateProjectionMatrix()
    {
        _UniformData.ProjectionMatrix = _ProjectionOptions.GetProjectionMatrix(_UniformData.AspectRatio);
    }
    void A_RenderView::UpdateUniformBindGroup()
    {
        _UniformBindGroup->BindUniformData(ABYTEK_NAME("ViewUniformData"), _UniformData);
        _UniformBindGroup->Commit();
    }
    void A_RenderView::UpdateDefaultViewportScissorConfig()
    {
        _DefaultViewportScissorConfig = F_RHIViewportScissorConfig::Make(
            F_Vector2_F32(_UniformData.Resolution)  
        );
    }
    void A_RenderView::UpdateDepthBufferAndDSV()
    {
        if (_DepthBuffer)
        {
            const auto& LastTextureAspect = _DepthBuffer->GetTextureAspect();
            if (
                (LastTextureAspect.Width == _UniformData.Resolution.X)
                && (LastTextureAspect.Height == _UniformData.Resolution.Y)
            )
            {
                return;
            }
        }
        
        F_RHITextureBuildParams DepthBufferBuildParams;
        DepthBufferBuildParams.Context = H_RHI::GetMainContext().Weak();
        DepthBufferBuildParams.Format = E_RHIFormat::R32_TYPELESS;
        DepthBufferBuildParams.TextureAspect.Width = _UniformData.Resolution.X;
        DepthBufferBuildParams.TextureAspect.Height = _UniformData.Resolution.Y;
        DepthBufferBuildParams.TextureAspect.DimensionCount = 2;
        DepthBufferBuildParams.AccessCapabilities = F_RHIResourceAccess::MakeDSVCapabilities();
        _DepthBuffer = RACreateAndBuildShared<A_RHIResource>(DepthBufferBuildParams);
        
        F_RHITextureViewBuildParams DSVBuildParams;
        DSVBuildParams.Context = H_RHI::GetMainContext().Weak();
        DSVBuildParams.Format = E_RHIFormat::D32_FLOAT;
        DSVBuildParams.Resource = _DepthBuffer;
        DSVBuildParams.Access = F_RHIResourceAccess::MakeDSV();
        _DSV = RACreateAndBuildShared<A_RHIResourceView>(DSVBuildParams);
    }
}
