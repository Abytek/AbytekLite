#pragma once

#include "Abytek/RHIContextChild.hpp"
#include "Abytek/DirectX12/RHIResourcePlacement.hpp"
#include "Abytek/DirectX12Shared/APIWrapper.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    namespace Internal::DirectX12RHIPlacedResourceManager
    {
        D3D12_HEAP_TYPE RHIResourceAccessToD3D12HeapType(const F_RHIResourceAccess& Access);
    }
    
    struct F_DirectX12RHIPlacedResourcePageBuildParams : F_RHIContextChildBuildParams
    {
        F_RHIResourceAccess Access;
        E_RHIResourceArchetype ResourceArchetype = E_RHIResourceArchetype::NONE;
        Sz SizeInBytes = 0;
        Sz AlignmentInBytes = 0;
    };
    
    class ABYTEK_ENGINE_RHI_API F_DirectX12RHIPlacedResourcePage : public A_RHIContextChild
    {
    private:
        F_RHIResourceAccess _Access;
        E_RHIResourceArchetype _ResourceArchetype = E_RHIResourceArchetype::NONE;
        Sz _SizeInBytes = 0;
        Sz _AlignmentInBytes = 0;
        
        F_LinearAllocationDistributor _Distributor;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetAccess() const noexcept
        {
            return _Access;
        }
        ABYTEK_FORCE_INLINE const auto& GetResourceArchetype() const noexcept
        {
            return _ResourceArchetype;
        }
        ABYTEK_FORCE_INLINE auto GetSizeInBytes() const noexcept
        {
            return _SizeInBytes;
        }
        ABYTEK_FORCE_INLINE auto GetAlignmentInBytes() const noexcept
        {
            return _AlignmentInBytes;
        }
        
    public:
        ABYTEK_RA_DECLARE_OBJECT_CREATABLE(F_DirectX12RHIPlacedResourcePage);
        virtual void Build(const F_DirectX12RHIPlacedResourcePageBuildParams& BuildParams);
        virtual void Release() override;
        
    public:
        virtual TS<A_RHIContextChildProxy> CreateProxy() override;
        
    public:
        virtual B8 UseStrictMode() const override
        {
            return false;
        }
        
    public:
        TF_Optional<F_DirectX12RHIResourcePlacement> Allocate(Sz ResourceSizeInBytes, Sz ResourceAlignmentInBytes);
        void Deallocate(const F_DirectX12RHIResourcePlacement& Placement);
    };
    
    struct F_DirectX12RHIPlacedResourceManagerBuildParams : F_RHIContextChildBuildParams
    {
        F_RHIResourceAccess Access;
        E_RHIResourceArchetype ResourceArchetype = E_RHIResourceArchetype::NONE;
        Sz MinPageSizeInBytes = Sz(64) * Sz(1024) * Sz(1024);
        Sz MaxPageSizeInBytes = Sz(8) * Sz(1024) * Sz(1024) * Sz(1024);
        Sz AlignmentInBytes = 0x10000;
    };
    
    class ABYTEK_ENGINE_RHI_API F_DirectX12RHIPlacedResourceManager : public A_RHIContextChild
    {
    private:
        F_RHIResourceAccess _Access;
        E_RHIResourceArchetype _ResourceArchetype = E_RHIResourceArchetype::NONE;
        Sz _MinPageSizeInBytes = 0;
        Sz _MaxPageSizeInBytes = 0;
        Sz _AlignmentInBytes = 0;
        TF_Vector<TS<F_DirectX12RHIPlacedResourcePage>> _Pages;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetAccess() const noexcept
        {
            return _Access;
        }
        ABYTEK_FORCE_INLINE const auto& GetResourceArchetype() const noexcept
        {
            return _ResourceArchetype;
        }
        ABYTEK_FORCE_INLINE auto GetMinPageSizeInBytes() const noexcept
        {
            return _MinPageSizeInBytes;
        }
        ABYTEK_FORCE_INLINE auto GetMaxPageSizeInBytes() const noexcept
        {
            return _MaxPageSizeInBytes;
        }
        ABYTEK_FORCE_INLINE auto GetAlignmentInBytes() const noexcept
        {
            return _AlignmentInBytes;
        }
        ABYTEK_FORCE_INLINE const auto& GetPages() const noexcept
        {
            return _Pages;
        }
        
    public:
        ABYTEK_RA_DECLARE_OBJECT_CREATABLE(F_DirectX12RHIPlacedResourceManager);
        virtual void Build(const F_DirectX12RHIPlacedResourceManagerBuildParams& BuildParams);
        virtual void Release() override;
        
    public:
        virtual TS<A_RHIContextChildProxy> CreateProxy() override;
        
    public:
        virtual B8 UseStrictMode() const override
        {
            return false;
        }
        
    public:
        F_DirectX12RHIResourcePlacement Allocate(Sz ResourceSizeInBytes, Sz ResourceAlignmentInBytes);
        void AddNewPage(Sz ResourceSizeInBytes, Sz ResourceAlignmentInBytes);
    };
}
#endif