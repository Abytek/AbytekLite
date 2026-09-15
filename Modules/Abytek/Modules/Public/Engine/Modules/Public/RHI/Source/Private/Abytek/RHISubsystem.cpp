#include "Abytek/RHISubsystem.hpp"
#include "Abytek/RHISubsystemProxy.hpp"
#include "Abytek/RHIDeviceManager.hpp"
#include "Abytek/RHIBindGroup.hpp"
#include "Abytek/RHIBindGroupProxy.hpp"
#include "Abytek/RHIBindGroupTemplateRuntime.hpp"
#include "Abytek/RHIBindGroupTemplateRuntimeProxy.hpp"
#include "Abytek/RHIClearDSVPass.hpp"
#include "Abytek/RHIClearDSVPassProxy.hpp"
#include "Abytek/RHIClearRTVPass.hpp"
#include "Abytek/RHIClearRTVPassProxy.hpp"
#include "Abytek/RHICopyBufferPass.hpp"
#include "Abytek/RHICopyBufferPassProxy.hpp"
#include "Abytek/RHICopyTexturePass.hpp"
#include "Abytek/RHICopyTexturePassProxy.hpp" 
#include "Abytek/RHIReadbackBufferPass.hpp"
#include "Abytek/RHIReadbackBufferPassProxy.hpp"
#include "Abytek/RHIUploadBufferPass.hpp"
#include "Abytek/RHIUploadBufferPassProxy.hpp"
#include "Abytek/RHIReadbackTexturePass.hpp" 
#include "Abytek/RHIReadbackTexturePassProxy.hpp"
#include "Abytek/RHIUploadTexturePass.hpp"
#include "Abytek/RHIUploadTexturePassProxy.hpp"
#include "Abytek/RHIDispatchComputePassProxy.hpp"
#include "Abytek/RHIDrawPassProxy.hpp"
#include "Abytek/RHISubmissionList.hpp"
#include "Abytek/RHIFeature.hpp"
#include "Abytek/RHIIndirectUtilities.hpp"
#include "Abytek/RHIPipelineState.hpp"
#include "Abytek/RHIPipelineStateProxy.hpp"
#include "Abytek/RHIPipelineStateTemplateRuntime.hpp"
#include "Abytek/RHIPipelineStateTemplateRuntimeProxy.hpp"
#include "Abytek/RHIProcess.hpp"
#include "Abytek/RHIRayTracePass.hpp"
#include "Abytek/RHIViewport.hpp"
#include "Abytek/RHIResource.hpp"
#include "Abytek/RHIResourceView.hpp"
#include "Abytek/RHIViewportProxy.hpp"
#include "Abytek/RHIWorkGraphPass.hpp"
#include "Abytek/ApplicationUpdateFunction.hpp"
#include "Abytek/RHIClearUAVFloatPass.hpp"
#include "Abytek/RHIClearUAVFloatPassProxy.hpp"
#include "Abytek/RHIClearUAVUIntPass.hpp"
#include "Abytek/RHIClearUAVUIntPassProxy.hpp"
#include "Abytek/Frame/FrameHelper.hpp"
#include "Abytek/WindowManager.hpp"
#include "Abytek/CoreUpdateGraph/HighLevelUpdateRange.hpp"
#include "Abytek/RHICompiler.hpp"
#include "Abytek/RHIResourceSyncPass.hpp"
#include "Abytek/RHIResourceSyncPassProxy.hpp"
#include "Abytek/RHITemplateSerializer.hpp"


namespace Abytek
{
    ABYTEK_REFLECT(F_RHISubsystem)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::F_RHISubsystem"));
    }

    ABYTEK_DEFINE_STATIC_APPLICATION_SUBSYSTEM(F_RHISubsystem);

    F_Name F_RHISubsystem::GetBeginRenderUpdateFunctionName()
    {
        return ABYTEK_NAME("Abytek::F_RHISubsystem::BeginRender");
    }
    F_Name F_RHISubsystem::GetEndRenderUpdateFunctionName()
    {
        return ABYTEK_NAME("Abytek::F_RHISubsystem::EndRender");
    };

    struct F_RHIStaticObjectPoolRegisters : TF_RAScopedStaticObjectPoolRegister<
        A_RHIProcess,
        A_RHIBindGroup,
        A_RHIBindGroupProxy,
        A_RHIBindGroupTemplateRuntime,
        A_RHIBindGroupTemplateRuntimeProxy,
        A_RHIClearDSVPass,
        A_RHIClearDSVPassProxy,
        A_RHIClearRTVPass,
        A_RHIClearRTVPassProxy,
        A_RHIClearUAVFloatPass,
        A_RHIClearUAVFloatPassProxy,
        A_RHIClearUAVUIntPass,
        A_RHIClearUAVUIntPassProxy,
        A_RHIResourceSyncPass,
        A_RHIResourceSyncPassProxy,
        A_RHICopyBufferPass,
        A_RHICopyBufferPassProxy,
        A_RHICopyTexturePass,
        A_RHICopyTexturePassProxy,
        A_RHIReadbackBufferPass,
        A_RHIReadbackBufferPassProxy,
        A_RHIUploadBufferPass,
        A_RHIUploadBufferPassProxy,
        A_RHIDispatchComputePass,
        A_RHIDispatchComputePassProxy,
        A_RHIDrawPass,
        A_RHIDrawPassProxy,
        A_RHIPipelineStateTemplateRuntime,
        A_RHIPipelineStateTemplateRuntimeProxy,
        A_RHIRayTracePass,
        A_RHIResource,
        A_RHIResourceProxy,
        A_RHIResourceView,
        A_RHIResourceViewProxy,
        A_RHISampler,
        A_RHISamplerProxy,
        A_RHISubmissionList,
        A_RHIViewport,
        A_RHIViewportProxy,
        A_RHIWorkGraphPass
    >
    {
    };

    F_RHISubsystem::F_RHISubsystem(const F_ProgramUnitBuildParams& BuildParams) :
        A_ApplicationSubsystem(BuildParams)
    {
        ABYTEK_BIND_STATIC_APPLICATION_SUBSYSTEM();
    }
    F_RHISubsystem::~F_RHISubsystem()
    {
    }

    void F_RHISubsystem::OnConfig()
    {
        _ConsoleVariable_APINames = RegisterConsoleVariable(
            ABYTEK_NAME("Abytek.RHI.API"),
            ABYTEK_TEXT(""),
            RHIGetDefaultAPINames()
        );
        _ConsoleVariable_EnableDriverDebugger = RegisterConsoleVariable(
            ABYTEK_NAME("Abytek.RHI.EnableDriverDebugger"),
            ABYTEK_TEXT(""),
#if defined(ABYTEK_DEBUG) || defined(ABYTEK_ENGINE_RHI_ENABLE_DRIVER_DEBUGGER)
                true
#else
                false
#endif
        );
        _ConsoleVariable_CaptureFrameIndex = RegisterConsoleVariable(
            ABYTEK_NAME("Abytek.RHI.CaptureFrameIndex"),
            ABYTEK_TEXT(""),
            ~U64(0)
        );
    }
    
    void F_RHISubsystem::OnInit()
    {
        _StaticObjectPoolRegisters = TU_Unmanaged<F_RHIStaticObjectPoolRegisters>()();
        _Proxy = RACreate<A_RHISubsystemProxy>();
        if (_Proxy)
        {
            _Proxy->Build();
            _DeviceManager = RACreateAndBuild<A_RHIDeviceManager>();
            _IndirectUtilities = RACreateAndBuild<A_RHIIndirectUtilities>();
            
            F_RHIContextBuildParams ContextBuildParams;
            ContextBuildParams.Device = A_RHIDeviceManager::GetInstance()->GetMainDevice();
            _MainRHIContext = RACreateAndBuildShared<A_RHIContext>(ContextBuildParams);
            
            {
                auto UpdateFunction = H_ApplicationUpdateFunction::Register(
                    [this]
                    {
                        F_RHIProcessBuildParams RHIProcessBuildParams;
                        RHIProcessBuildParams.Contexts = { _MainRHIContext.Weak() };
                        auto RHIProcess = RACreateAndBuildShared<A_RHIProcess>(RHIProcessBuildParams);
                        
                        H_Frame::EnqueueCommand<E_FrameParamType::RENDER>(
                            [this, RHIProcess]
                            {
                                _MainRHIProcess_Render = RHIProcess;
                                _CaptureCriticalSection_Render(
                                    [this]
                                    {
                                        _CaptureFlagsPtr_Render = H_Frame::GetArena(E_FrameParamType::RENDER)->New<E_RHICaptureFlag>(
                                            E_RHICaptureFlag::NONE    
                                        );
                                    }
                                );
                                if (_ConsoleVariable_CaptureFrameIndex->GetValue() == H_Frame::GetIndex(E_FrameParamType::RENDER))
                                {
                                    AddCaptureFlag_Render(E_RHICaptureFlag::DEFAULT);
                                }
                                _Proxy->OnBeginFrameParam(E_FrameParamType::RENDER);
                                H_Frame::EnqueueCommand<E_FrameParamType::GPU, E_FrameParamType::RENDER>(
                                    [this, RHIProcess, CaptureFlagsPtr_Render = _CaptureFlagsPtr_Render]
                                    {
                                        _CaptureFlags_GPU = *CaptureFlagsPtr_Render;
                                         _Proxy->OnBeginFrameParam(E_FrameParamType::GPU);
                                        _MainRHIProcess_GPU = RHIProcess;
                                    }
                                );
                                H_Frame::EnqueueCommand<E_FrameParamType::DISPLAY, E_FrameParamType::RENDER>(
                                    [this, RHIProcess]
                                    {
                                        _Proxy->OnBeginFrameParam(E_FrameParamType::DISPLAY);
                                        _MainRHIProcess_Final = RHIProcess;
                                    }
                                );
                            }
                        );
                    },
                    GetBeginRenderUpdateFunctionName()
                );
                UpdateFunction->AddDependency(
                    A_WindowManager::GetUpdateUpdateFunctionName()
                );
                UpdateFunction->AddReverseDependency(
                    F_HighLevelUpdateRange::GetBeginFunctionName()  
                );
            }
            {
                auto UpdateFunction = H_ApplicationUpdateFunction::Register(
                    [this]
                    {
                        H_Frame::EnqueueCommand<E_FrameParamType::RENDER>(
                            [this]
                            {
                                auto RHIProcess = _MainRHIProcess_Render;
                                
                                RHIProcess->Flush();
                                ABYTEK_AWAIT RHIProcess->GetCompilePromise();
                                H_Frame::EnqueueCommand<E_FrameParamType::GPU, E_FrameParamType::RENDER>(
                                    [this, RHIProcess]
                                    {
                                        ABYTEK_AWAIT RHIProcess->GetMainPromise();
                                        _Proxy->OnEndFrameParam(E_FrameParamType::GPU);
                                        _CaptureFlags_GPU = {};
                                        _MainRHIProcess_GPU = {};
                                    }
                                );
                                H_Frame::EnqueueCommand<E_FrameParamType::DISPLAY, E_FrameParamType::RENDER>(
                                    [this, RHIProcess]
                                    {
                                        ABYTEK_AWAIT RHIProcess->GetFinalPromise();
                                        _Proxy->OnEndFrameParam(E_FrameParamType::DISPLAY);
                                        _MainRHIProcess_Final = {};
                                    }
                                );
                                _Proxy->OnEndFrameParam(E_FrameParamType::RENDER);
                                _CaptureCriticalSection_Render(
                                    [this]
                                    {
                                        _CaptureFlagsPtr_Render = nullptr;
                                    }
                                );
                                _MainRHIProcess_Render = {};
                            }
                        );
                    },
                    GetEndRenderUpdateFunctionName()
                );
                UpdateFunction->AddDependency(
                    F_HighLevelUpdateRange::GetEndFunctionName()  
                );
                UpdateFunction->AddReverseDependency(
                    A_WindowManager::GetPostUpdateUpdateFunctionName()
                );
            }
        }
    }
    void F_RHISubsystem::OnRelease()
    {
        if (_Proxy)
        {
            H_UpdateUtilities::UnregisterFunction(
                GetEndRenderUpdateFunctionName()
            );
            H_UpdateUtilities::UnregisterFunction(
                GetBeginRenderUpdateFunctionName()
            );
            
            _MainRHIContext.Reset();
        }
        
        _IndirectUtilities.Reset();
        _DeviceManager.Reset();
        _Proxy.Reset();
        _StaticObjectPoolRegisters.Reset();
    }
}
