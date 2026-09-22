#pragma once

#include "Abytek/Engine.NFC.prerequisites.hpp"
#include "Abytek/Renderer/RenderObject.hpp"
#include "Abytek/Renderer/RenderGeometry/RenderGeometryCommon.hpp"


namespace Abytek
{
    class F_RenderGeometryStorage;

    class ABYTEK_ENGINE_NFC_API F_RenderGeometryPage : public A_RenderObject
    {
    private:
        TW<F_RenderGeometryStorage> _Manager;
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
        ABYTEK_RENDER_OBJECT_CREATABLE(F_RenderGeometryPage, A_RenderObject);
        
    public:
        void Init(
            const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer, 
            const TW<F_RenderGeometryStorage>& Manager, 
            U32 Index, 
            U64 SizeInBytes
        );
        void Release(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer) override;
    
    public:
        TF_Optional<F_RenderGeometryAllocation> Allocate(U64 SizeInBytes, U64 AlignmentInBytes);
        void Deallocate(const F_RenderGeometryAllocation& Allocation);
    };
}
