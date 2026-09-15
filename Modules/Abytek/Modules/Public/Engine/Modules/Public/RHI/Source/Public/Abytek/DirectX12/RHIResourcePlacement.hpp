#pragma once

#include "Abytek/Engine.RHI.prerequisites.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    class F_DirectX12RHIPlacedResourceManager;
    class F_DirectX12RHIPlacedResourcePage;
    class F_DirectX12RHIPlacedResourceManagerProxy;
    class F_DirectX12RHIPlacedResourcePageProxy;
    
    struct F_DirectX12RHIResourcePlacement
    {
        TW<F_DirectX12RHIPlacedResourcePage> Page;
        U64 BeginOffsetInBytes = 0;
        U64 EndOffsetInBytes = 0;
        
        ABYTEK_FORCE_INLINE B8 IsValid() const noexcept
        {
            return (
                Page
                && (BeginOffsetInBytes != ~U64(0))
                && (EndOffsetInBytes != ~U64(0))
            );
        }
        ABYTEK_FORCE_INLINE explicit operator B8 () const noexcept
        {
            return IsValid();
        }
        
        friend ABYTEK_FORCE_INLINE B8 operator == (
            const F_DirectX12RHIResourcePlacement& A,    
            const F_DirectX12RHIResourcePlacement& B    
        ) noexcept
        {
            return (
                (A.Page == B.Page)    
                && (A.BeginOffsetInBytes == B.BeginOffsetInBytes)    
                && (A.EndOffsetInBytes == B.EndOffsetInBytes)    
            );
        }
        friend ABYTEK_FORCE_INLINE B8 operator != (
            const F_DirectX12RHIResourcePlacement& A,    
            const F_DirectX12RHIResourcePlacement& B    
        ) noexcept
        {
            return (  
                (A.Page != B.Page)    
                || (A.BeginOffsetInBytes != B.BeginOffsetInBytes)    
                || (A.EndOffsetInBytes != B.EndOffsetInBytes)    
            );
        }
        
        ABYTEK_FORCE_INLINE B8 IsOverlap(const F_DirectX12RHIResourcePlacement& X) const noexcept
        {
            return (
                (Page == X.Page)    
                && (
                    (BeginOffsetInBytes <= X.EndOffsetInBytes)
                    && (EndOffsetInBytes > X.BeginOffsetInBytes)
                )    
            );
        }
    };
    
    struct ABYTEK_ENGINE_RHI_API F_DirectX12RHIResourcePlacementProxy
    {
        TW<F_DirectX12RHIPlacedResourcePageProxy> PageProxy;
        U64 BeginOffsetInBytes = 0;
        U64 EndOffsetInBytes = 0;
        
        static F_DirectX12RHIResourcePlacementProxy From(const F_DirectX12RHIResourcePlacement& X);
        
        ABYTEK_FORCE_INLINE B8 IsValid() const noexcept
        {
            return (
                PageProxy
                && (BeginOffsetInBytes != ~U64(0))
                && (EndOffsetInBytes != ~U64(0))
            );
        }
        ABYTEK_FORCE_INLINE explicit operator B8 () const noexcept
        {
            return IsValid();
        }
        
        friend ABYTEK_FORCE_INLINE B8 operator == (
            const F_DirectX12RHIResourcePlacementProxy& A,    
            const F_DirectX12RHIResourcePlacementProxy& B    
        ) noexcept
        {
            return (
                (A.PageProxy == B.PageProxy)    
                && (A.BeginOffsetInBytes == B.BeginOffsetInBytes)    
                && (A.EndOffsetInBytes == B.EndOffsetInBytes)    
            );
        }
        friend ABYTEK_FORCE_INLINE B8 operator != (
            const F_DirectX12RHIResourcePlacementProxy& A,    
            const F_DirectX12RHIResourcePlacementProxy& B    
        ) noexcept
        {
            return (  
                (A.PageProxy != B.PageProxy)    
                || (A.BeginOffsetInBytes != B.BeginOffsetInBytes)    
                || (A.EndOffsetInBytes != B.EndOffsetInBytes)    
            );
        }
        
        ABYTEK_FORCE_INLINE B8 IsOverlap(const F_DirectX12RHIResourcePlacementProxy& X) const noexcept
        {
            return (
                (PageProxy == X.PageProxy)    
                && (
                    (BeginOffsetInBytes <= X.EndOffsetInBytes)
                    && (EndOffsetInBytes > X.BeginOffsetInBytes)
                )    
            );
        }
    };
}
#endif