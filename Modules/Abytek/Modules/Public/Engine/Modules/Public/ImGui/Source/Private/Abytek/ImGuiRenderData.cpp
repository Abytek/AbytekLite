#include "Abytek/ImGuiRenderData.hpp"
#include "Abytek/DirectX12/RHIEventStack.hpp"
#include "Abytek/DirectX12/RHIResourceProxy.hpp"


namespace Abytek
{
#ifdef ABYTEK_ENGINE_IMGUI_ENABLE_DIRECTX12
    ABYTEK_RA_OBJECT_DEFAULT(F_DirectX12ImGuiPass);
    void F_DirectX12ImGuiPass::Build(const F_DirectX12ImGuiPassBuildParams& BuildParams)
    {
        A_RHIPass::Build(BuildParams);
        A_DirectX12RHIPassExtension::Build();
        _RenderData = BuildParams.RenderData;
        _BackFrame = _RenderData->GetViewport()->GetCurrentBackFrame();
    }
    void F_DirectX12ImGuiPass::Release()
    {
        _BackFrame = {};
        _RenderData = {};
        A_DirectX12RHIPassExtension::Release();
        A_RHIPass::Release();
    }

    TS_Valid<A_RHIPassProxy> F_DirectX12ImGuiPass::CreateProxy()
    {
        return RACreateAndBuildShared<F_DirectX12ImGuiPassProxy>(ABYTEK_WTHIS());
    }

    void F_DirectX12ImGuiPass::AppendSubresourceBindings(F_DirectX12RHISubresourceBindingSet& SubresourceBindingSet)
    {
        A_DirectX12RHIPassExtension::AppendSubresourceBindings(SubresourceBindingSet);
        SubresourceBindingSet.push_back(
            F_DirectX12RHISubresourceBinding::MakeCore(
                F_DirectX12RHISubresourceReference::Make(
                    _BackFrame.Weak(),
                    0
                ),
                F_RHIResourceAccess::MakeCopyDest()
            )  
        );
    }

    E_DirectX12RHIPassBatchType F_DirectX12ImGuiPass::GetPassBatchType()
    {
        return E_DirectX12RHIPassBatchType::GPU;
    }
    E_DirectX12RHIExecutionRangeType F_DirectX12ImGuiPass::GetExecutionRangeType()
    {
        return E_DirectX12RHIExecutionRangeType::USE_COMMAND_QUEUE;
    }

    E_RHIPassClass F_DirectX12ImGuiPass::GetPassClass()
    {
        return E_RHIPassClass::GRAPHICS;
    }

    ABYTEK_RA_OBJECT_DEFAULT(F_DirectX12ImGuiPassProxy);
    void F_DirectX12ImGuiPassProxy::Build(const TW_Valid<A_RHIPass>& Pass)
    {
        A_RHIPassProxy::Build(Pass);
        A_DirectX12RHIPassProxyExtension::Build(Pass);
        auto CastedPass = Pass.FastCast<F_DirectX12ImGuiPass>();
        _RenderData = CastedPass->GetRenderData();
        _BackFrameProxy = CastedPass->GetBackFrame()->GetProxy().FastCast<A_RHIResourceProxy>();
    }
    void F_DirectX12ImGuiPassProxy::Release()
    {
        _BackFrameProxy = {};
        _RenderData = {};
        A_DirectX12RHIPassProxyExtension::Release();
        A_RHIPassProxy::Release();
    }

    void F_DirectX12ImGuiPassProxy::Execute(F_DirectX12RHIPassProxyExtensionExecuteParams& ExecuteParams)
    {
        A_DirectX12RHIPassProxyExtension::Execute(ExecuteParams);
        
        const auto& DirectX12Pack = _RenderData->GetDirectX12Pack();
        auto D3D12BackResource = _BackFrameProxy.FastCast<F_DirectX12RHIResourceProxy>()->GetD3D12Resource();
        
        auto CastedContextProxy = GetContextProxy().FastCast<F_DirectX12RHIContextProxy>();
        auto CommandListManager = CastedContextProxy->QueryCommandListManager(
            DirectX12SharedAPIWrapper::E_CommandListType::DIRECT    
        );
        
        // 
        auto D3D12CommandAllocator = CommandListManager->PopAllocator();
        
        // Record D3D12CommandListBegin
        Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> D3D12CommandListBegin;
        {
            D3D12CommandListBegin = CommandListManager->PopList(D3D12CommandAllocator);
#ifdef ABYTEK_ENGINE_RHI_ENABLE_PROFILER
            F_DirectX12RHIEventStack EventStack;
            EventStack.D3D12CommandList = D3D12CommandListBegin;
#endif
#ifdef ABYTEK_ENGINE_RHI_ENABLE_PROFILER
            {
                EventStack.Push(GetProfilerEventStatesToBegin());
                EventStack.Push(ABYTEK_NAME("Begin"));
            }
#endif
            {
                D3D12_RESOURCE_BARRIER D3D12ResourceBarrier = {};
                D3D12ResourceBarrier.Type                   = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
                D3D12ResourceBarrier.Flags                  = D3D12_RESOURCE_BARRIER_FLAG_NONE;
                D3D12ResourceBarrier.Transition.pResource   = D3D12BackResource.Get();
                D3D12ResourceBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
                D3D12ResourceBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
                D3D12ResourceBarrier.Transition.StateAfter  = D3D12_RESOURCE_STATE_COPY_SOURCE;
                D3D12CommandListBegin->ResourceBarrier(1, &D3D12ResourceBarrier);
            }
            {
                D3D12_RESOURCE_BARRIER D3D12ResourceBarrier = {};
                D3D12ResourceBarrier.Type                   = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
                D3D12ResourceBarrier.Flags                  = D3D12_RESOURCE_BARRIER_FLAG_NONE;
                D3D12ResourceBarrier.Transition.pResource   = DirectX12Pack.D3D12Resource.Get();
                D3D12ResourceBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
                D3D12ResourceBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COMMON;
                D3D12ResourceBarrier.Transition.StateAfter  = D3D12_RESOURCE_STATE_COPY_DEST;
                D3D12CommandListBegin->ResourceBarrier(1, &D3D12ResourceBarrier);
            }
            {
                D3D12_TEXTURE_COPY_LOCATION D3D12DestCopyLocation = {};
                {
                    D3D12DestCopyLocation.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
                    D3D12DestCopyLocation.SubresourceIndex = 0;
                    D3D12DestCopyLocation.pResource = DirectX12Pack.D3D12Resource.Get();
                }
                D3D12_TEXTURE_COPY_LOCATION D3D12SrcCopyLocation = {};
                {
                    D3D12SrcCopyLocation.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
                    D3D12SrcCopyLocation.SubresourceIndex = 0;
                    D3D12SrcCopyLocation.pResource = D3D12BackResource.Get();
                }
                D3D12CommandListBegin->CopyTextureRegion(
                    &D3D12DestCopyLocation,
                    0,
                    0,
                    0,
                    &D3D12SrcCopyLocation,
                    nullptr
                );
            }
            {
                D3D12_RESOURCE_BARRIER D3D12ResourceBarrier = {};
                D3D12ResourceBarrier.Type                   = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
                D3D12ResourceBarrier.Flags                  = D3D12_RESOURCE_BARRIER_FLAG_NONE;
                D3D12ResourceBarrier.Transition.pResource   = DirectX12Pack.D3D12Resource.Get();
                D3D12ResourceBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
                D3D12ResourceBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
                D3D12ResourceBarrier.Transition.StateAfter  = D3D12_RESOURCE_STATE_COMMON;
                D3D12CommandListBegin->ResourceBarrier(1, &D3D12ResourceBarrier);
            }
            {
                D3D12_RESOURCE_BARRIER D3D12ResourceBarrier = {};
                D3D12ResourceBarrier.Type                   = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
                D3D12ResourceBarrier.Flags                  = D3D12_RESOURCE_BARRIER_FLAG_NONE;
                D3D12ResourceBarrier.Transition.pResource   = D3D12BackResource.Get();
                D3D12ResourceBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
                D3D12ResourceBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_SOURCE;
                D3D12ResourceBarrier.Transition.StateAfter  = D3D12_RESOURCE_STATE_COPY_DEST;
                D3D12CommandListBegin->ResourceBarrier(1, &D3D12ResourceBarrier);
            }
#ifdef ABYTEK_ENGINE_RHI_ENABLE_PROFILER
            {
                EventStack.Pop();
                EventStack.Pop(GetProfilerEventStatesToEnd());
            }
#endif
            D3D12CommandListBegin->Close();
            CommandListManager->PushList(D3D12CommandListBegin);
        }
        
        // Record D3D12CommandListEnd
        Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> D3D12CommandListEnd;
        {
            D3D12CommandListEnd = CommandListManager->PopList(D3D12CommandAllocator);
#ifdef ABYTEK_ENGINE_RHI_ENABLE_PROFILER
            F_DirectX12RHIEventStack EventStack;
            EventStack.D3D12CommandList = D3D12CommandListEnd;
#endif
#ifdef ABYTEK_ENGINE_RHI_ENABLE_PROFILER
            {
                EventStack.Push(GetProfilerEventStatesToBegin());
                EventStack.Push(ABYTEK_NAME("End"));
            }
#endif
            {
                D3D12_RESOURCE_BARRIER D3D12ResourceBarrier = {};
                D3D12ResourceBarrier.Type                   = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
                D3D12ResourceBarrier.Flags                  = D3D12_RESOURCE_BARRIER_FLAG_NONE;
                D3D12ResourceBarrier.Transition.pResource   = DirectX12Pack.D3D12Resource.Get();
                D3D12ResourceBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
                D3D12ResourceBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COMMON;
                D3D12ResourceBarrier.Transition.StateAfter  = D3D12_RESOURCE_STATE_COPY_SOURCE;
                D3D12CommandListEnd->ResourceBarrier(1, &D3D12ResourceBarrier);
            }
            {
                D3D12_TEXTURE_COPY_LOCATION D3D12DestCopyLocation = {};
                {
                    D3D12DestCopyLocation.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
                    D3D12DestCopyLocation.SubresourceIndex = 0;
                    D3D12DestCopyLocation.pResource = D3D12BackResource.Get();
                }
                D3D12_TEXTURE_COPY_LOCATION D3D12SrcCopyLocation = {};
                {
                    D3D12SrcCopyLocation.Type = D3D12_TEXTURE_COPY_TYPE_SUBRESOURCE_INDEX;
                    D3D12SrcCopyLocation.SubresourceIndex = 0;
                    D3D12SrcCopyLocation.pResource = DirectX12Pack.D3D12Resource.Get();
                }
                D3D12CommandListEnd->CopyTextureRegion(
                    &D3D12DestCopyLocation,
                    0,
                    0,
                    0,
                    &D3D12SrcCopyLocation,
                    nullptr
                );
            }
            {
                D3D12_RESOURCE_BARRIER D3D12ResourceBarrier = {};
                D3D12ResourceBarrier.Type                   = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
                D3D12ResourceBarrier.Flags                  = D3D12_RESOURCE_BARRIER_FLAG_NONE;
                D3D12ResourceBarrier.Transition.pResource   = DirectX12Pack.D3D12Resource.Get();
                D3D12ResourceBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
                D3D12ResourceBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_SOURCE;
                D3D12ResourceBarrier.Transition.StateAfter  = D3D12_RESOURCE_STATE_COMMON;
                D3D12CommandListEnd->ResourceBarrier(1, &D3D12ResourceBarrier);
            }
#ifdef ABYTEK_ENGINE_RHI_ENABLE_PROFILER
            {
                EventStack.Pop();
                EventStack.Pop(GetProfilerEventStatesToEnd());
            }
#endif
            D3D12CommandListEnd->Close();
            CommandListManager->PushList(D3D12CommandListEnd);
        }
        
        // Submit
        ID3D12CommandList* D3D12CommandLists[] 
        {
            D3D12CommandListBegin.Get(),
            DirectX12Pack.D3D12CommandList.Get(),
            D3D12CommandListEnd.Get()
        };
        ExecuteParams.CommandQueueProxy->GetD3D12CommandQueue()->ExecuteCommandLists(
            GetStaticArraySize(D3D12CommandLists), 
            D3D12CommandLists    
        );
        
        //
        CommandListManager->PushAllocator(D3D12CommandAllocator);
    }
#endif

    F_ImGuiRenderData::F_ImGuiRenderData(const F_ImGuiRenderDataBuildParams& BuildParams) :
        _Context(BuildParams.Context)
    {
        const auto& FrameData = BuildParams.Context->FrameData;
        
#ifdef ABYTEK_ENGINE_IMGUI_ENABLE_DIRECTX12
        if (F_RHISubsystem::GetInstance()->GetActiveAPI() == E_RHIAPI::DIRECTX12)
        {
            F_DirectX12ImGuiPackPopParams PopParams;
            PopParams.Size = FrameData.Size;
            _DirectX12Pack = _Context->PopDirectX12Pack(PopParams);
        }
#endif
    }
    F_ImGuiRenderData::~F_ImGuiRenderData()
    {
#ifdef ABYTEK_ENGINE_IMGUI_ENABLE_DIRECTX12
        if (F_RHISubsystem::GetInstance()->GetActiveAPI() == E_RHIAPI::DIRECTX12)
        {
            _Context->PushDirectX12Pack(ABYTEK_MOVE(_DirectX12Pack));
        }
#endif
    }

    TS<A_RHISubmissionItem> F_ImGuiRenderData::_CreateSubmissionItem()
    {
        auto Window = _Context->GetWindow();
        if (!Window)
        {
            return {};
        }
        
        TS<A_RHIViewport> Viewport;
        for (const auto& ViewportToIterate : H_RHI::GetMainProcess()->GetViewports())
        {
            if (ViewportToIterate->GetWindow() == Window)
            {
                Viewport = ViewportToIterate;
                break;
            }
        }
        if (!Viewport)
        {
            return {};
        }
        
        _Viewport = Viewport;
        _ViewportProxy = Viewport->GetProxy().FastCast<A_RHIViewportProxy>();
        
        auto RHIContext = H_RHI::GetMainContext();
        auto RHIContextProxy = _Context->GetRHIContextProxy();
        
#ifdef ABYTEK_ENGINE_IMGUI_ENABLE_DIRECTX12
        if (F_RHISubsystem::GetInstance()->GetActiveAPI() == E_RHIAPI::DIRECTX12)
        {
            F_DirectX12ImGuiPassBuildParams PassBuildParams;
            PassBuildParams.Context = RHIContext.Weak();
            PassBuildParams.RenderData = ABYTEK_STHIS();
            auto Pass = RACreateAndBuildShared<F_DirectX12ImGuiPass>(PassBuildParams);
#ifdef ABYTEK_DEBUG_INFO
            Pass->SetDebugName(ABYTEK_DEBUG_NAME("ImGui"));
#endif
            return ABYTEK_MOVE(Pass);
        }
#endif
        return {};
    }

    void F_ImGuiRenderData::Submit()
    {
        H_Frame::EnqueueCommand<E_FrameParamType::RENDER>(
            [this, SThis = ABYTEK_STHIS()]()
            {
                auto SubmissionItem = _CreateSubmissionItem();
                if (!SubmissionItem)
                {
                    return;
                }
                H_RHI::GetMainProcess()->AddSubmissionItem(SubmissionItem);
            }
        );
    }
}
