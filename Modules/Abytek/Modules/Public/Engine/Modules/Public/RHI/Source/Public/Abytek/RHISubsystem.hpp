#pragma once

#include "Abytek/Engine.RHI.prerequisites.hpp"
#include "Abytek/ApplicationSubsystem.hpp"
#include "Abytek/RHISubsystemProxy.hpp"
#include "Abytek/RHICapture.hpp"


namespace Abytek
{
    class A_RHISubsystemProxy;
    class A_RHIDeviceManager;
    class A_RHIIndirectUtilities;
    class F_RHIPlatform;
    class A_RHIContext;
    class A_RHIProcess;

    struct F_RHIStaticObjectPoolRegisters;

    class ABYTEK_ENGINE_RHI_API F_RHISubsystem final : public A_ApplicationSubsystem
    {
    public:
        ABYTEK_BEGIN_REFLECTOR(A_ApplicationSubsystem)
        ABYTEK_END_REFLECTOR(F_RHISubsystem)

    public:
        ABYTEK_DECLARE_STATIC_APPLICATION_SUBSYSTEM(F_RHISubsystem);
        
    public:
        static F_Name GetBeginRenderUpdateFunctionName();
        static F_Name GetEndRenderUpdateFunctionName();

    private:
        TU_Unmanaged<F_RHIStaticObjectPoolRegisters> _StaticObjectPoolRegisters;
        
        TU<A_RHISubsystemProxy> _Proxy;
        TU<A_RHIDeviceManager> _DeviceManager;
        TU<A_RHIIndirectUtilities> _IndirectUtilities;
        
        TW<TF_ConsoleVariable<TF_Vector<F_Name>>> _ConsoleVariable_APINames;
        TW<TF_ConsoleVariable<B8>> _ConsoleVariable_EnableDriverDebugger;
        TW<TF_ConsoleVariable<U64>> _ConsoleVariable_CaptureFrameIndex;
        
        TS<A_RHIContext> _MainRHIContext;
        TS<A_RHIProcess> _MainRHIProcess_Render;
        TS<A_RHIProcess> _MainRHIProcess_GPU;
        TS<A_RHIProcess> _MainRHIProcess_Final;
        
        F_YieldCriticalSection _CaptureCriticalSection_Render;
        E_RHICaptureFlag* _CaptureFlagsPtr_Render = nullptr;
        E_RHICaptureFlag _CaptureFlags_GPU = E_RHICaptureFlag::NONE;
        
    public:
        ABYTEK_FORCE_INLINE E_RHIAPI GetActiveAPI() const noexcept
        {
            if (!_Proxy)
            {
                return E_RHIAPI::NONE;
            }
            return _Proxy->GetAPI();
        }
        ABYTEK_FORCE_INLINE B8 IsEnabled() const noexcept
        {
            return GetActiveAPI() != E_RHIAPI::NONE;
        }
        
        ABYTEK_FORCE_INLINE const auto& GetConsoleVariable_APINames() const noexcept
        {
            return _ConsoleVariable_APINames;
        }
        ABYTEK_FORCE_INLINE const auto& GetConsoleVariable_EnableDriverDebugger() const noexcept
        {
            return _ConsoleVariable_EnableDriverDebugger;
        }
        ABYTEK_FORCE_INLINE const auto& GetConsoleVariable_CaptureFrameIndex() const noexcept
        {
            return _ConsoleVariable_CaptureFrameIndex;
        }
        
        ABYTEK_FORCE_INLINE const auto& GetMainRHIContext() const noexcept
        {
            return _MainRHIContext;
        }
        ABYTEK_FORCE_INLINE const auto& GetMainRHIProcess_Render() const noexcept
        {
            return _MainRHIProcess_Render;
        }
        ABYTEK_FORCE_INLINE const auto& GetMainRHIProcess_GPU() const noexcept
        {
            return _MainRHIProcess_GPU;
        }
        ABYTEK_FORCE_INLINE const auto& GetMainRHIProcess_Final() const noexcept
        {
            return _MainRHIProcess_Final;
        }

        ABYTEK_FORCE_INLINE E_RHICaptureFlag GetCaptureFlags_Render() const noexcept
        {
            ABYTEK_ENGINE_RHI_ASSERT(
                H_TaskUtilities::HasTag(GetFrameParamTypeTaskTag(E_FrameParamType::RENDER))
                || H_TaskUtilities::HasTag(GetApplicationLowLevelTaskTag())
            );
            E_RHICaptureFlag Result;
            _CaptureCriticalSection_Render(
                [this, &Result]
                {
                    ABYTEK_ENGINE_RHI_ASSERT(_CaptureFlagsPtr_Render);
                    Result = *_CaptureFlagsPtr_Render;
                }
            );
            return Result;
        }
        ABYTEK_FORCE_INLINE auto GetCaptureFlags_GPU() const noexcept
        {
            ABYTEK_ENGINE_RHI_ASSERT(
                H_TaskUtilities::HasTag(GetFrameParamTypeTaskTag(E_FrameParamType::GPU))
                || H_TaskUtilities::HasTag(GetApplicationLowLevelTaskTag())
            );
            return _CaptureFlags_GPU;
        }
        ABYTEK_FORCE_INLINE void AddCaptureFlag_Render(E_RHICaptureFlag CaptureFlags) noexcept
        {
            ABYTEK_ENGINE_RHI_ASSERT(
                H_TaskUtilities::HasTag(GetFrameParamTypeTaskTag(E_FrameParamType::RENDER))
                || H_TaskUtilities::HasTag(GetApplicationLowLevelTaskTag())
            );
            _CaptureCriticalSection_Render(
                [this, CaptureFlags]
                {
                    ABYTEK_ENGINE_RHI_ASSERT(_CaptureFlagsPtr_Render);
                    *_CaptureFlagsPtr_Render = *_CaptureFlagsPtr_Render | CaptureFlags;
                }
            );
        }
        
    public:
        F_RHISubsystem(const F_ProgramUnitBuildParams& BuildParams);
        ~F_RHISubsystem() override;

    protected:
        void OnConfig() override;
        
    protected:
        void OnInit() override;
        void OnRelease() override;
    };
}
