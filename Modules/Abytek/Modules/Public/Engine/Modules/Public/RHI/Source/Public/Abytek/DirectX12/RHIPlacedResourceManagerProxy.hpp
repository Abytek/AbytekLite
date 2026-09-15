#pragma once

#include "Abytek/DirectX12/RHIPlacedResourceManager.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    class ABYTEK_ENGINE_RHI_API F_DirectX12RHIPlacedResourcePageProxy : public A_RHIContextChildProxy
    {
    private:
        F_RHIResourceAccess _Access;
        E_RHIResourceArchetype _ResourceArchetype = E_RHIResourceArchetype::NONE;
        Sz _SizeInBytes = 0;
        Sz _AlignmentInBytes = 0;
        
        Microsoft::WRL::ComPtr<ID3D12Heap> _D3D12Heap;
        
    public:
        ABYTEK_FORCE_INLINE auto GetPlacedResourcePage() const noexcept
        {
            return GetContextChild().FastCast<F_DirectX12RHIPlacedResourcePage>();
        }
        
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
        
        ABYTEK_FORCE_INLINE const auto& GetD3D12Heap() const noexcept
        {
            return _D3D12Heap;
        }
        
    public:
        ABYTEK_RA_DECLARE_OBJECT_CREATABLE(F_DirectX12RHIPlacedResourcePageProxy);
        void LateBuildPlacedResourcePage();
        virtual void Release() override;
    };
    
    class ABYTEK_ENGINE_RHI_API F_DirectX12RHIPlacedResourceManagerProxy : public A_RHIContextChildProxy
    {
    private:
        F_RHIResourceAccess _Access;
        E_RHIResourceArchetype _ResourceArchetype = E_RHIResourceArchetype::NONE;
        Sz _MinPageSizeInBytes = 0;
        Sz _MaxPageSizeInBytes = 0;
        Sz _AlignmentInBytes = 0;
        
    public:
        ABYTEK_FORCE_INLINE auto GetPlacedResourceManager() const noexcept
        {
            return GetContextChild().FastCast<F_DirectX12RHIPlacedResourceManager>();
        }
        
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
        
    public:
        ABYTEK_RA_DECLARE_OBJECT_CREATABLE(F_DirectX12RHIPlacedResourceManagerProxy);
        void LateBuildPlacedResourceManager();
        virtual void Release() override;
    };
}
#endif