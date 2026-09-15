#pragma once

#include "Abytek/Engine.NFC.prerequisites.hpp"
#include "Abytek/GPUData/GPUDataCommon.hpp"


namespace Abytek
{
    class F_GPUDataComponentStorage;

    class ABYTEK_ENGINE_NFC_API F_GPUDataComponentPage : public A_Object
    {
    private:
        TW<F_GPUDataComponentStorage> _Storage;
        U32 _Index = 0;
        U64 _SizeInBytes = 0;
        F_LinearAllocationDistributor _Distributor;
        TS<A_RHIResource> _RHIBuffer;
        TS<A_RHIResourceView> _SRV;
        TS<A_RHIResourceView> _UAV;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetStorage() const noexcept
        {
            return _Storage;
        }
        ABYTEK_FORCE_INLINE auto GetIndex() const noexcept
        {
            return _Index;
        }
        ABYTEK_FORCE_INLINE const auto& GetSizeInBytes() const noexcept
        {
            return _SizeInBytes;
        }
        ABYTEK_FORCE_INLINE const auto& GetDistributor() const noexcept
        {
            return _Distributor;
        }
        ABYTEK_FORCE_INLINE const auto& GetRHIBuffer() const noexcept
        {
            return _RHIBuffer;
        }
        ABYTEK_FORCE_INLINE const auto& GetSRV() const noexcept
        {
            return _SRV;
        }
        ABYTEK_FORCE_INLINE const auto& GetUAV() const noexcept
        {
            return _UAV;
        }
        
    public:
        F_GPUDataComponentPage(const TW<F_GPUDataComponentStorage>& Storage, U32 Index, U64 SizeInBytes);
        ~F_GPUDataComponentPage() override;
    
    public:
        TF_Optional<F_GPUDataComponentAllocation> Allocate(U32 NumComponents);
        void Deallocate(const F_GPUDataComponentAllocation& Allocation);
    };
}
