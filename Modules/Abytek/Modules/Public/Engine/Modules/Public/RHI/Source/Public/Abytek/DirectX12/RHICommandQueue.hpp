#pragma once

#include "Abytek/RHIContextChild.hpp"
#include "Abytek/DirectX12Shared/APIWrapper.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    struct F_DirectX12RHICommandQueueBuildParams : F_RHIContextChildBuildParams
    {
        DirectX12SharedAPIWrapper::E_CommandListType CommandListType = DirectX12SharedAPIWrapper::E_CommandListType::DEFAULT;
        DirectX12SharedAPIWrapper::E_CommandQueueFlag Flags = DirectX12SharedAPIWrapper::E_CommandQueueFlag::DEFAULT;
    };
    
    class ABYTEK_ENGINE_RHI_API F_DirectX12RHICommandQueue : public A_RHIContextChild
    {
    private:
        DirectX12SharedAPIWrapper::E_CommandListType _CommandListType = DirectX12SharedAPIWrapper::E_CommandListType::NONE;
        DirectX12SharedAPIWrapper::E_CommandQueueFlag _Flags = DirectX12SharedAPIWrapper::E_CommandQueueFlag::NONE;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetCommandListType() const noexcept
        {
            return _CommandListType;
        }
        ABYTEK_FORCE_INLINE auto& InjectCommandListType() noexcept
        {
            return _CommandListType;
        }
        ABYTEK_FORCE_INLINE const auto& GetFlags() const noexcept
        {
            return _Flags;
        }
        ABYTEK_FORCE_INLINE auto& InjectFlags() noexcept
        {
            return _Flags;
        }
        
    public:
        ABYTEK_RA_DECLARE_OBJECT_CREATABLE(F_DirectX12RHICommandQueue);
        virtual void Build(const F_DirectX12RHICommandQueueBuildParams& BuildParams);
        virtual void Release() override;
        
    protected:
        virtual TS<A_RHIContextChildProxy> CreateProxy() override;
        
    public:
        virtual B8 UseStrictMode() const override
        {
            return false;
        }
    };
}
#endif