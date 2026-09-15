#include "Abytek/DirectX12/RHIViewport.hpp"
#include "Abytek/DirectX12/RHIViewportProxy.hpp"
#include "Abytek/RHIProcess.hpp"
#include "Abytek/RHIResourceView.hpp"
#include "Abytek/DirectX12/RHIDevice.hpp"
#include "Abytek/DirectX12/RHICommandQueue.hpp"
#include "Abytek/DirectX12/RHIContext.hpp"
#include "Abytek/DirectX12/RHIProcess.hpp"
#include "Abytek/DirectX12/RHIResource.hpp"
#include "Abytek/DirectX12/RHISubsystemProxy.hpp"
#include "Abytek/Windows/Window.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    void F_DirectX12RHIViewport::Build(const F_RHIViewportBuildParams& BuildParams)
    {
        A_RHIViewport::Build(BuildParams);

        auto Size = GetSize();
        
        auto& Frames = InjectFrames();
        auto& RTVs = InjectRenderTargetViews();
        U32 NumFrames = GetNumFrames();
        for (U32 FrameIndex = 0; FrameIndex < NumFrames; ++FrameIndex)
        {
            F_RHITextureBuildParams FrameBuildParams;
            FrameBuildParams.Context = GetContext();
            FrameBuildParams.TextureAspect.Width = Size.X;
            FrameBuildParams.TextureAspect.Height = Size.Y;
            FrameBuildParams.TextureAspect.DimensionCount = 2;
            FrameBuildParams.Format = GetFormat();
            FrameBuildParams.AccessCapabilities = F_RHIResourceAccess::MakeRTVCapabilities();
            auto Frame = RACreateShared<A_RHIResource>();
            auto CastedFrame = Frame.FastCast<F_DirectX12RHIResource>(); 
            CastedFrame->BuildWithoutD3D12Resource(FrameBuildParams, FrameBuildParams.TextureAspect);
            
            F_RHITextureViewBuildParams RTVBuildParams;
            RTVBuildParams.Context = GetContext();
            RTVBuildParams.Format = GetFormat();
            RTVBuildParams.Access = F_RHIResourceAccess::MakeRTV(); 
            RTVBuildParams.Resource = Frame;
            auto RTV = RACreateAndBuildShared<A_RHIResourceView>(RTVBuildParams);
            
            Frames.push_back(Frame);
            RTVs.push_back(RTV);
        }
        
        GetProxy().FastCast<F_DirectX12RHIViewportProxy>()->LateBuildViewport_DirectX12();
        
        DirectX12RHIProcessQueries::Execution::F_InitViewport Query;
        Query.ViewportProxy = GetProxy().FastCast<A_RHIViewportProxy>();
        GetContext()->GetCurrentProcess().FastCast<F_DirectX12RHIProcess>()->Queues.Execution.InitViewport.Push(Query);
    }
    void F_DirectX12RHIViewport::Release()
    {
        A_RHIViewport::Release();
    }

    void F_DirectX12RHIViewport::Update()
    {
        ABYTEK_ENGINE_RHI_CONTEXT_CHILD_VALIDATE();
        
        auto& CurrentBackFrameIndex = InjectCurrentBackFrameIndex();
        CurrentBackFrameIndex = (CurrentBackFrameIndex + 1) % GetFrames().size();
    }

    void F_DirectX12RHIViewport::Resize(const F_Vector2_U32& NewSize)
    {
        A_RHIViewport::Resize(NewSize);
         
        DirectX12RHIProcessQueries::Execution::F_ResizeViewport Query;
        Query.ViewportProxy = GetProxy().FastCast<A_RHIViewportProxy>();
        Query.NewSize = NewSize;
        GetContext()->GetCurrentProcess().FastCast<F_DirectX12RHIProcess>()->Queues.Execution.ResizeViewport.Push(Query);
        
        auto& RTVs = InjectRenderTargetViews();
        U32 NumFrames = GetNumFrames();
        for (U32 FrameIndex = 0; FrameIndex < NumFrames; ++FrameIndex)
        {
            auto RTV = RTVs[FrameIndex].FastCast<F_DirectX12RHIResourceView>();
            RTV->D3D12InitDescriptor(false);
        }
        
        InjectCurrentBackFrameIndex() = 0;
    }
}
#endif