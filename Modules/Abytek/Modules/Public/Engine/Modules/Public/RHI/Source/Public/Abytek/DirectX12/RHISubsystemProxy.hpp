#pragma once

#include "Abytek/RHISubsystemProxy.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    class F_DirectX12RHIConversionUtilities;
    
    class ABYTEK_ENGINE_RHI_API F_DirectX12RHISubsystemProxy : public A_RHISubsystemProxy
    {
    private:
        IDXGIFactory* _DXGIFactory = 0;
        TU<F_DirectX12RHIConversionUtilities> _ConversionUtilities;
#ifdef ABYTEK_ENGINE_RHI_ENABLE_PIX
        HMODULE _WinPixGpuCapturerLibrary = nullptr;
#endif
        
    public:
        ABYTEK_FORCE_INLINE IDXGIFactory* GetDXGIFactory() const noexcept
        {
            return _DXGIFactory;
        }
        ABYTEK_FORCE_INLINE IDXGIFactory*& InjectDXGIFactory() noexcept
        {
            return _DXGIFactory;
        }
#ifdef ABYTEK_ENGINE_RHI_ENABLE_PIX
        ABYTEK_FORCE_INLINE auto GetWinPixGpuCapturerLibrary() const noexcept
        {
            return _WinPixGpuCapturerLibrary;
        }
#endif

    public:
        ABYTEK_RA_DECLARE_OBJECT(F_DirectX12RHISubsystemProxy);
        virtual void Build() override;
        virtual void Release() override;

    public:
        virtual E_RHIAPI GetAPI() override;

    public:
        static TU<A_RHISubsystemProxy> TryCreate();
        
    public:
        void OnBeginFrameParam(E_FrameParamType FrameParamType) override;
        void OnEndFrameParam(E_FrameParamType FrameParamType) override;
    };
}
#endif