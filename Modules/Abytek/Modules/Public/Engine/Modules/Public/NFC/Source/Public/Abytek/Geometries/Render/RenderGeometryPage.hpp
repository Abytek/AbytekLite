#pragma once

#include "Abytek/Engine.NFC.prerequisites.hpp"
#include "Abytek/Geometries/Render/RenderGeometryCommon.hpp"


namespace Abytek
{
    class F_RenderGeometryManager;

    class ABYTEK_ENGINE_NFC_API F_RenderGeometryPage : public A_Object
    {
    private:
        TW<F_RenderGeometryManager> _Manager;
        U32 _Index = 0;
        U64 _SizeInBytes = 0;
        F_LinearAllocationDistributor _Distributor;
        TS<A_RHIResource> _RHIBuffer;
        TS<A_RHIResourceView> _SRV;
        TS<A_RHIResourceView> _UAV;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetManager() const noexcept
        {
            return _Manager;
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
        F_RenderGeometryPage(const TW<F_RenderGeometryManager>& Manager, U32 Index, U64 SizeInBytes);
        ~F_RenderGeometryPage() override;
    
    public:
        TF_Optional<F_RenderGeometryAllocation> Allocate(U64 SizeInBytes, U64 AlignmentInBytes);
        void Deallocate(const F_RenderGeometryAllocation& Allocation);
    };
}
