#include "Abytek/DirectX12/RHISubsystemProxy.hpp"

#include "Abytek/RHIModule.hpp"
#include "Abytek/RHISubsystem.hpp"
#include "Abytek/DirectX12/RHIFormat.hpp"
#include "Abytek/DirectX12/RHIConversionUtilities.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12_AGILITY
extern "C"
{
    __declspec(dllexport) extern const UINT D3D12SDKVersion = ABYTEK_DIRECTX12_AGILITY_SDK_VERSION_MAJOR;
}
extern "C"
{
    __declspec(dllexport) extern const char* D3D12SDKPath = ABYTEK_DIRECTX12_AGILITY_SDK_RUNTIME_RELATIVE_DIR;
}
#endif

namespace Abytek
{
    void F_DirectX12RHISubsystemProxy::Build()
    {
        A_RHISubsystemProxy::Build();

        UINT DXGIFactoryFlags = 0;
#ifdef ABYTEK_ENGINE_RHI_ENABLE_DRIVER_DEBUGGER
        if (F_RHISubsystem::GetInstance()->GetConsoleVariable_EnableDriverDebugger()->GetValue())
        {
            DXGIFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
        }
#endif

        HRESULT HR;

        HR = CreateDXGIFactory2(DXGIFactoryFlags, IID_PPV_ARGS(&_DXGIFactory));
        ABYTEK_ENGINE_RHI_ASSERT(!FAILED(HR)) << "can't create d3d12 factory";

#ifdef ABYTEK_ENGINE_RHI_ENABLE_DRIVER_DEBUGGER
        if (F_RHISubsystem::GetInstance()->GetConsoleVariable_EnableDriverDebugger()->GetValue())
        {
            ID3D12Debug* D3D12DebugControllerP = 0;
            if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&D3D12DebugControllerP))))
            {
                D3D12DebugControllerP->EnableDebugLayer();
            }
        }
#endif

        Internal::DirectX12RHISetupFormatTables();

        _ConversionUtilities = RACreateAndBuild<F_DirectX12RHIConversionUtilities>();
        
#ifdef ABYTEK_ENGINE_RHI_ENABLE_PIX
        if (F_RHIModule::GetInstance()->GetShouldEnablePIX())
        {
            _WinPixGpuCapturerLibrary = PIXLoadLatestWinPixGpuCapturerLibrary();
        }
#endif
    }
    void F_DirectX12RHISubsystemProxy::Release()
    {
#ifdef ABYTEK_ENGINE_RHI_ENABLE_PIX
        if (F_RHIModule::GetInstance()->GetShouldEnablePIX())
        {
            if (_WinPixGpuCapturerLibrary)
            {
                FreeLibrary(_WinPixGpuCapturerLibrary);
            }
        }
#endif
        
        _ConversionUtilities = {};

        if(_DXGIFactory)
        {
            _DXGIFactory->Release();
            _DXGIFactory = 0;
        }
        
        A_RHISubsystemProxy::Release();
    }

    E_RHIAPI F_DirectX12RHISubsystemProxy::GetAPI()
    {
        return E_RHIAPI::DIRECTX12;
    }

    TU<A_RHISubsystemProxy> F_DirectX12RHISubsystemProxy::TryCreate()
    {
        return TU<F_DirectX12RHISubsystemProxy>()();
    }

    namespace Internal::DirectX12RHISubsystemProxy
    {
        std::string GenerateCaptureFileName(const std::string& prefix = "")
        {
            // Get current time
            std::time_t now = std::time(nullptr);
            std::tm localTime{};

#ifdef _WIN32
            localtime_s(&localTime, &now);   // Windows
#else
            localtime_r(&now, &localTime);   // Linux/macOS
#endif

            std::ostringstream oss;

            oss << prefix
                << std::put_time(&localTime, "%Y-%m-%d_%H-%M-%S");

            return oss.str();
        }
    }
    void F_DirectX12RHISubsystemProxy::OnBeginFrameParam(E_FrameParamType FrameParamType)
    {
#ifdef ABYTEK_ENGINE_RHI_ENABLE_PIX
        if (FrameParamType == E_FrameParamType::GPU)
        {
            auto CaptureFlags = F_RHISubsystem::GetInstance()->GetCaptureFlags_GPU();
            if (
                (CaptureFlags != E_RHICaptureFlag::NONE)
                && _WinPixGpuCapturerLibrary
            )
            {
                F_Text CaptureDirectoryPath = (
                    *A_PathManager::GetInstance()->GetCommonDirectoryPath(E_CommonDirectory::CACHE)
                    + ABYTEK_TEXT("/Abytek/RHICaptures/PIX/")
                    + ToText(Internal::DirectX12RHISubsystemProxy::GenerateCaptureFileName().c_str())
                );
                H_FSUtilities::EnsureDirectory(CaptureDirectoryPath);
            
                F_Text CaptureFilePath_GPU = (
                    CaptureDirectoryPath
                    + ABYTEK_TEXT("/GPU.wpix")
                );
                F_Text CaptureFilePath_Timing = (
                    CaptureDirectoryPath
                    + ABYTEK_TEXT("/Timing.wpix")
                );
            
                PIXCaptureParameters Params = {};
                DWORD PIXCaptureFlags = 0;
                if (FlagHas(CaptureFlags, E_RHICaptureFlag::GPU))
                {
                    ABYTEK_ENGINE_RHI_ASSERT(PIXCaptureFlags == 0) << "Multiple RHI capture modes are not allowed";
                    PIXCaptureFlags |= PIX_CAPTURE_GPU;
                    Params.GpuCaptureParameters.FileName = CaptureFilePath_GPU.c_str();
                }
                if (FlagHas(CaptureFlags, E_RHICaptureFlag::TIMING))
                {
                    ABYTEK_ENGINE_RHI_ASSERT(PIXCaptureFlags == 0) << "Multiple RHI capture modes are not allowed";
                    PIXCaptureFlags |= PIX_CAPTURE_TIMING;
                    Params.TimingCaptureParameters.FileName = CaptureFilePath_Timing.c_str();
                }
                HRESULT HR = PIXBeginCapture(PIXCaptureFlags, &Params);
                ABYTEK_ENGINE_RHI_ASSERT(!FAILED(HR));
            }
        }
#endif
    }
    void F_DirectX12RHISubsystemProxy::OnEndFrameParam(E_FrameParamType FrameParamType)
    {
#ifdef ABYTEK_ENGINE_RHI_ENABLE_PIX
        if (FrameParamType == E_FrameParamType::GPU)
        {
            auto CaptureFlags = F_RHISubsystem::GetInstance()->GetCaptureFlags_GPU();
            if (
                (CaptureFlags != E_RHICaptureFlag::NONE)
                && _WinPixGpuCapturerLibrary
            )
            {
                HRESULT HR = PIXEndCapture(false);
                ABYTEK_ENGINE_RHI_ASSERT(!FAILED(HR));
            }
        }
#endif
    }
}
#endif