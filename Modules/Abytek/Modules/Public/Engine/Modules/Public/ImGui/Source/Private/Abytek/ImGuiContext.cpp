#include "Abytek/ImGuiContext.hpp"
#include "Abytek/ImGuiRenderData.hpp"
#include "Abytek/DirectX12/RHICommandQueueProxy.hpp"
#include "Abytek/DirectX12/RHIContextProxy.hpp"
#include "Abytek/DirectX12/RHIDevice.hpp"
#include "Abytek/DirectX12/RHIEventStack.hpp"
#include "Abytek/Windows/Window.hpp"
#include "Abytek/Frame/FrameHelper.hpp"


#ifdef ABYTEK_ENGINE_IMGUI_ENABLE_WIN32
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);     
#endif           

namespace Abytek
{
    F_ImGuiContext::F_ImGuiContext(const F_ImGuiContextBuildParams& BuildParams)
    {
        H_Frame::Flush();
        
        _Window = BuildParams.Window;
        _ImGuiContext = ImGui::CreateContext();
        H_Frame::EnqueueCommand<E_FrameParamType::RENDER>(
            [this, SThis = ABYTEK_STHIS()]
            {
                ImGui::SetCurrentContext(_ImGuiContext);
                
                _RHIContextProxy = H_RHI::GetMainContext()->GetProxy().FastCast<A_RHIContextProxy>();
                
                ImGui::StyleColorsDark();
        
#ifdef ABYTEK_ENGINE_IMGUI_ENABLE_WIN32
                {
                    auto CastedWindow = _Window.FastCast<F_WindowsWindow>();
                    ImGui_ImplWin32_Init(CastedWindow->GetWinAPIHandle());
                                
                    auto WindowsLLEvents = A_LLEvents::GetInstance().FastCast<F_WindowsLLEvents>();
                    _OnWinAPIProcessMsgEventListenerHandle = WindowsLLEvents->GetOnWinAPIProcessMsgEvent().AddBackListener(
                        [this](HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, TF_Optional<HRESULT>& ResultOpt)
                        {
                            ImGui::SetCurrentContext(_ImGuiContext);
                            ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam);
                        }
                    );
                }
#endif
        
#ifdef ABYTEK_ENGINE_IMGUI_ENABLE_DIRECTX12
                if (F_RHISubsystem::GetInstance()->GetActiveAPI() == E_RHIAPI::DIRECTX12)
                {
                    auto Device = A_RHIDeviceManager::GetInstance()->GetMainDevice();
                    const auto& D3D12Device = Device.FastCast<F_DirectX12RHIDevice>()->GetD3D12Device();
                            
                    auto CastedRHIContextProxy = _RHIContextProxy.FastCast<F_DirectX12RHIContextProxy>();
                            
                    HRESULT HR;
                    
                    // Get d3d12 command queue
                    {
                        _D3D12CommandQueue = CastedRHIContextProxy->QueryCommandQueueProxy(
                            DirectX12SharedAPIWrapper::E_CommandListType::DIRECT    
                        )->GetD3D12CommandQueue();
                    }
                         
                    // Create SRV descriptor heap   
                    {
                        U32 SRVDescriptorHeapCapacity = 512;
                        
                        D3D12_DESCRIPTOR_HEAP_DESC D3D12DescriptorHeapDesc = {};
                        D3D12DescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
                        D3D12DescriptorHeapDesc.NumDescriptors = SRVDescriptorHeapCapacity;
                        D3D12DescriptorHeapDesc.NodeMask = 1;
                        D3D12DescriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
                        HR = D3D12Device->CreateDescriptorHeap(
                            &D3D12DescriptorHeapDesc,
                            IID_PPV_ARGS(&_D3D12DescriptorHeap_SRV)
                        );
                        ABYTEK_ENGINE_IMGUI_ASSERT(SUCCEEDED(HR)) << "Cannot create d3d12 SRV descriptor heap";
                        
                        auto CPUDescriptorHandleStart = _D3D12DescriptorHeap_SRV->GetCPUDescriptorHandleForHeapStart();
                        auto GPUDescriptorHandleStart = _D3D12DescriptorHeap_SRV->GetGPUDescriptorHandleForHeapStart();
                        auto DescriptorStride = D3D12Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
                        for (U32 SRVDescriptorIndex = 0; SRVDescriptorIndex < SRVDescriptorHeapCapacity; ++SRVDescriptorIndex)
                        {
                            F_DirectX12ImGuiSRVDescriptor Descriptor;
                            Descriptor.CPUHandle = {
                                CPUDescriptorHandleStart.ptr
                                + Sz(SRVDescriptorIndex) * Sz(DescriptorStride)
                            };
                            Descriptor.GPUHandle = {
                                GPUDescriptorHandleStart.ptr
                                + Sz(SRVDescriptorIndex) * Sz(DescriptorStride)
                            };
                            _DirectX12SRVDescriptors.Push(Descriptor);
                        }
                    }
                    
                    ImGui_ImplDX12_InitInfo ImGuiDX12InitInfo = {};
                    ImGuiDX12InitInfo.Device = D3D12Device.Get();
                    ImGuiDX12InitInfo.CommandQueue = _D3D12CommandQueue.Get();
                    ImGuiDX12InitInfo.NumFramesInFlight = static_cast<int>(
                        H_Frame::GetNumFramesInFlight(E_FrameParamType::RENDER)
                    );
                    ImGuiDX12InitInfo.RTVFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
                    ImGuiDX12InitInfo.DSVFormat = DXGI_FORMAT_UNKNOWN;
                    ImGuiDX12InitInfo.UserData = this;
                    ImGuiDX12InitInfo.SrvDescriptorHeap = _D3D12DescriptorHeap_SRV.Get();
                    ImGuiDX12InitInfo.SrvDescriptorAllocFn = [](
                        ImGui_ImplDX12_InitInfo* ImGuiDX12InitInfo, 
                        D3D12_CPU_DESCRIPTOR_HANDLE* OutCPUHandle, 
                        D3D12_GPU_DESCRIPTOR_HANDLE* OutGPUHandle
                    )
                    {
                        F_ImGuiContext* Context = (F_ImGuiContext*)ImGuiDX12InitInfo->UserData;
                        
                        auto& SRVDescriptors = Context->GetDirectX12SRVDescriptors();
                        
                        F_DirectX12ImGuiSRVDescriptor SRVDescriptor;
                        B8 PopStatus = SRVDescriptors.TryPop(SRVDescriptor);
                        ABYTEK_ENGINE_IMGUI_ASSERT(PopStatus);
                        *OutCPUHandle = SRVDescriptor.CPUHandle;
                        *OutGPUHandle = SRVDescriptor.GPUHandle;
                    };
                    ImGuiDX12InitInfo.SrvDescriptorFreeFn = [](
                        ImGui_ImplDX12_InitInfo* ImGuiDX12InitInfo, 
                        D3D12_CPU_DESCRIPTOR_HANDLE CPUHandle, 
                        D3D12_GPU_DESCRIPTOR_HANDLE GPUHandle
                    )
                    {
                        F_ImGuiContext* Context = (F_ImGuiContext*)ImGuiDX12InitInfo->UserData;
                        
                        auto& SRVDescriptors = Context->GetDirectX12SRVDescriptors();
                        
                        F_DirectX12ImGuiSRVDescriptor SRVDescriptor;
                        SRVDescriptor.CPUHandle = CPUHandle;
                        SRVDescriptor.GPUHandle = GPUHandle;
                        SRVDescriptors.Push(SRVDescriptor);
                    };
                    ImGui_ImplDX12_Init(&ImGuiDX12InitInfo);
                }
#endif
            }  
        );
    }
    F_ImGuiContext::~F_ImGuiContext()
    {
        H_Frame::Flush();
        
        ABYTEK_ENGINE_IMGUI_ASSERT(!_RHIContextProxy);
        ImGui::SetCurrentContext(_ImGuiContext);

#ifdef ABYTEK_ENGINE_IMGUI_ENABLE_WIN32
        {
            auto WindowsLLEvents = A_LLEvents::GetInstance().FastCast<F_WindowsLLEvents>();
            WindowsLLEvents->GetOnWinAPIProcessMsgEvent().RemoveListener(_OnWinAPIProcessMsgEventListenerHandle);
        }
#endif
        
#ifdef ABYTEK_ENGINE_IMGUI_ENABLE_DIRECTX12
        if (F_RHISubsystem::GetInstance()->GetActiveAPI() == E_RHIAPI::DIRECTX12)
        {
            ImGui_ImplDX12_Shutdown();
        }
#endif
        
#ifdef ABYTEK_ENGINE_IMGUI_ENABLE_WIN32
        {
            ImGui_ImplWin32_Shutdown();
        }
#endif
        
        ImGui::DestroyContext(_ImGuiContext);
    }

    void F_ImGuiContext::Release()
    {
        _RHIContextProxy = {};
    }

    B8 F_ImGuiContext::BeginFrame()
    {
        F_Vector2_U32 Size(_Window->GetScaledClientSize());
        if (
            (Size.X == 0)    
            || (Size.Y == 0)    
        )
        {
            return false;
        }
        
        _FrameLock.Lock();
        
        ImGui::SetCurrentContext(_ImGuiContext);
        
        // Cache frame data
        {
            FrameData.Size = Size;
        }
        
        ImGuiIO& IO = ImGui::GetIO();
        IO.DisplaySize.x = Size.X;
        IO.DisplaySize.y = Size.Y;
        
#ifdef ABYTEK_ENGINE_IMGUI_ENABLE_DIRECTX12
        if (F_RHISubsystem::GetInstance()->GetActiveAPI() == E_RHIAPI::DIRECTX12)
        {
            ImGui_ImplDX12_NewFrame();
        }
#endif
        
#ifdef ABYTEK_ENGINE_IMGUI_ENABLE_WIN32
        {
            ImGui_ImplWin32_NewFrame();
        }
#endif
        
        ImGui::NewFrame();
        return true;
    }
    TS<F_ImGuiRenderData> F_ImGuiContext::EndFrame()
    {
        F_ImGuiRenderDataBuildParams RenderDataBuildParams;
        RenderDataBuildParams.Context = ABYTEK_STHIS();
        auto RenderData = TS<F_ImGuiRenderData>()(RenderDataBuildParams);
        
        ImGui::Render();
        
#ifdef ABYTEK_ENGINE_IMGUI_ENABLE_DIRECTX12
        if (F_RHISubsystem::GetInstance()->GetActiveAPI() == E_RHIAPI::DIRECTX12)
        {
            const auto& DirectX12Pack = RenderData->GetDirectX12Pack();
#ifdef ABYTEK_ENGINE_RHI_ENABLE_PROFILER
            F_DirectX12RHIEventStack EventStack;
            EventStack.D3D12CommandList = DirectX12Pack.D3D12CommandList;
#endif
#ifdef ABYTEK_ENGINE_RHI_ENABLE_PROFILER
            {
                EventStack.Push(ABYTEK_NAME("ImGui"));
                EventStack.Push(ABYTEK_NAME("Core"));
            }
#endif
            {
                ID3D12DescriptorHeap* D3D12DescriptorHeap_SRV = _D3D12DescriptorHeap_SRV.Get();
                DirectX12Pack.D3D12CommandList->SetDescriptorHeaps(1, &D3D12DescriptorHeap_SRV);
            }
            {
                D3D12_RESOURCE_BARRIER D3D12ResourceBarrier = {};
                D3D12ResourceBarrier.Type                   = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
                D3D12ResourceBarrier.Flags                  = D3D12_RESOURCE_BARRIER_FLAG_NONE;
                D3D12ResourceBarrier.Transition.pResource   = DirectX12Pack.D3D12Resource.Get();
                D3D12ResourceBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
                D3D12ResourceBarrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COMMON;
                D3D12ResourceBarrier.Transition.StateAfter  = D3D12_RESOURCE_STATE_RENDER_TARGET;
                DirectX12Pack.D3D12CommandList->ResourceBarrier(1, &D3D12ResourceBarrier);
            }
            {
                D3D12_CPU_DESCRIPTOR_HANDLE D3D12RTVCPUHandle = DirectX12Pack.D3D12CPUDescriptorHandle;
                DirectX12Pack.D3D12CommandList->OMSetRenderTargets(
                    1,
                    &D3D12RTVCPUHandle,
                    false,
                    nullptr
                );
            }
            ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), DirectX12Pack.D3D12CommandList.Get());
            {
                D3D12_RESOURCE_BARRIER D3D12ResourceBarrier = {};
                D3D12ResourceBarrier.Type                   = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
                D3D12ResourceBarrier.Flags                  = D3D12_RESOURCE_BARRIER_FLAG_NONE;
                D3D12ResourceBarrier.Transition.pResource   = DirectX12Pack.D3D12Resource.Get();
                D3D12ResourceBarrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
                D3D12ResourceBarrier.Transition.StateBefore  = D3D12_RESOURCE_STATE_RENDER_TARGET;
                D3D12ResourceBarrier.Transition.StateAfter = D3D12_RESOURCE_STATE_COMMON;
                DirectX12Pack.D3D12CommandList->ResourceBarrier(1, &D3D12ResourceBarrier);
            }
#ifdef ABYTEK_ENGINE_RHI_ENABLE_PROFILER
            {
                EventStack.Pop();
                EventStack.Pop();
            }
#endif
            DirectX12Pack.D3D12CommandList->Close();
        }
#endif
        
#ifdef ABYTEK_ENGINE_IMGUI_ENABLE_WIN32
        {
        }
#endif
            
        
        FrameData = {};
        _FrameLock.Unlock();
        
        return ABYTEK_MOVE(RenderData);
    }

#ifdef ABYTEK_ENGINE_IMGUI_ENABLE_DIRECTX12
    F_DirectX12ImGuiPack F_ImGuiContext::PopDirectX12Pack(const F_DirectX12ImGuiPackPopParams& PopParams)
    {
        ABYTEK_ENGINE_IMGUI_ASSERT((PopParams.Size.X != 0) && (PopParams.Size.Y != 0)) << "Size cannot be zero";
        
        auto Device = A_RHIDeviceManager::GetInstance()->GetMainDevice();
        const auto& D3D12Device = Device.FastCast<F_DirectX12RHIDevice>()->GetD3D12Device();
        
        HRESULT HR;
        
        F_DirectX12ImGuiPack Result;
        _DirectX12Packs.TryPop(Result);
        
        if (Result.D3D12CommandAllocator == nullptr)
        {
            HR = D3D12Device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&Result.D3D12CommandAllocator));
            ABYTEK_ENGINE_IMGUI_ASSERT(SUCCEEDED(HR)) << "Cannot create d3d12 command allocator";
            
            Result.D3D12CommandAllocator->SetName(ABYTEK_TEXT("ImGuiContext::CommandAllocator"));
        }
        else
        {
        }
        
        if (Result.D3D12CommandList == nullptr)
        {
            HR = D3D12Device->CreateCommandList(
                1, 
                D3D12_COMMAND_LIST_TYPE_DIRECT, 
                Result.D3D12CommandAllocator.Get(),
                nullptr,
                IID_PPV_ARGS(&Result.D3D12CommandList)
            );
            ABYTEK_ENGINE_IMGUI_ASSERT(SUCCEEDED(HR)) << "Cannot create d3d12 command list";
            
            Result.D3D12CommandAllocator->SetName(ABYTEK_TEXT("ImGuiContext::CommandList"));
        }
        else
        {
            Result.D3D12CommandList->Reset(Result.D3D12CommandAllocator.Get(), nullptr);
        }
        
        if ((Result.D3D12Resource == nullptr) || (Result.Size != PopParams.Size))
        {
            F_RHITextureAspect TextureAspect;
            TextureAspect.Width = PopParams.Size.X;
            TextureAspect.Height = PopParams.Size.Y;
            
            D3D12_HEAP_PROPERTIES D3D12HeapProperties = H_DirectX12RHIResource::GetD3D12HeapProperties(
                F_RHIResourceAccess::MakeRTVCapabilities()
            );
            D3D12_RESOURCE_DESC D3D12ResourceDesc = H_DirectX12RHIResource::GetD3D12ResourceDesc(
                F_RHIResourceAccess::MakeRTVCapabilities(),
                E_RHIFormat::R8G8B8A8_UNORM,
                TextureAspect
            );
            HR = D3D12Device->CreateCommittedResource(
                &D3D12HeapProperties,
                D3D12_HEAP_FLAG_NONE,
                &D3D12ResourceDesc,
                D3D12_RESOURCE_STATE_COMMON,
                nullptr,
                IID_PPV_ARGS(&Result.D3D12Resource)
            );
            ABYTEK_ENGINE_IMGUI_ASSERT(SUCCEEDED(HR)) << "Cannot create d3d12 command list";
            
            Result.D3D12CommandAllocator->SetName(ABYTEK_TEXT("ImGuiContext::Resource"));
        }
        else
        {
        }
        
        if (Result.D3D12DescriptorHeap == nullptr)
        {
            D3D12_DESCRIPTOR_HEAP_DESC D3D12DescriptorHeapDesc = {};
            D3D12DescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
            D3D12DescriptorHeapDesc.NumDescriptors = 1;
            D3D12DescriptorHeapDesc.NodeMask = 1;
            D3D12DescriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
            HR = D3D12Device->CreateDescriptorHeap(
                &D3D12DescriptorHeapDesc,
                IID_PPV_ARGS(&Result.D3D12DescriptorHeap)
            );
            ABYTEK_ENGINE_IMGUI_ASSERT(SUCCEEDED(HR)) << "Cannot create d3d12 RTV descriptor heap";
            
            Result.D3D12CommandAllocator->SetName(ABYTEK_TEXT("ImGuiContext::DescriptorHeap"));
        }
        
        {
            Result.D3D12CPUDescriptorHandle = Result.D3D12DescriptorHeap->GetCPUDescriptorHandleForHeapStart();
            
            D3D12_RENDER_TARGET_VIEW_DESC D3D12RTVDesc = {};
            D3D12RTVDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
            D3D12RTVDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
            D3D12RTVDesc.Texture2D.MipSlice = 0;
            D3D12RTVDesc.Texture2D.PlaneSlice = 0;
            D3D12Device->CreateRenderTargetView(Result.D3D12Resource.Get(), &D3D12RTVDesc, Result.D3D12CPUDescriptorHandle);
        }
        return Result;
    } 
    void F_ImGuiContext::PushDirectX12Pack(F_DirectX12ImGuiPack&& DirectX12Pack)
    {
        DirectX12Pack.D3D12CommandAllocator->Reset();
        _DirectX12Packs.Push(ABYTEK_MOVE(DirectX12Pack));
    }
#endif
}
