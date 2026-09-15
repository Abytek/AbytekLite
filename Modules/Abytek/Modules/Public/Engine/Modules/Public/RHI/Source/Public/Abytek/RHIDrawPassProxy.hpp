#pragma once

#include "Abytek/RHIDrawPass.hpp"
#include "Abytek/RHIHasGeneralPipelineProxy.hpp"


namespace Abytek
{
    class A_RHIResourceProxy;
    
    class ABYTEK_ENGINE_RHI_API A_RHIDrawPassProxy : public A_RHIPassProxy, public A_RHIHasGeneralPipelineProxy
    {
    private:        
        E_RHIDrawType _DrawType = E_RHIDrawType::NONE;
        F_RHIDrawNonIndexedConfig _NonIndexed;
        F_RHIDrawIndexedConfig _Indexed;
        F_RHIDrawDispatchMeshConfig _DispatchMesh;
        
        F_RHIViewportScissorConfig _ViewportScissor;
        
        TF_Optional<F_RHIIndirectConfigProxy> _IndirectProxy;

    public:
        ABYTEK_FORCE_INLINE const auto& GetDrawType() const noexcept
        {
            return _DrawType;
        }
        ABYTEK_FORCE_INLINE auto& InjectDrawType() noexcept
        {
            return _DrawType;
        }
        ABYTEK_FORCE_INLINE const auto& GetNonIndexed() const noexcept
        {
            return _NonIndexed;
        }
        ABYTEK_FORCE_INLINE auto& InjectNonIndexed() noexcept
        {
            return _NonIndexed;
        }
        ABYTEK_FORCE_INLINE const auto& GetIndexed() const noexcept
        {
            return _Indexed;
        }
        ABYTEK_FORCE_INLINE auto& InjectIndexed() noexcept
        {
            return _Indexed;
        }
        ABYTEK_FORCE_INLINE const auto& GetDispatchMesh() const noexcept
        {
            return _DispatchMesh;
        }
        ABYTEK_FORCE_INLINE auto& InjectDispatchMesh() noexcept
        {
            return _DispatchMesh;
        }

        ABYTEK_FORCE_INLINE const auto& GetViewportScissor() const noexcept
        {
            return _ViewportScissor;
        }
        ABYTEK_FORCE_INLINE auto& InjectViewportScissor() noexcept
        {
            return _ViewportScissor;
        }

        ABYTEK_FORCE_INLINE const auto& GetIndirectProxy() const noexcept
        {
            return _IndirectProxy;
        }
        ABYTEK_FORCE_INLINE auto& InjectIndirectProxy() noexcept
        {
            return _IndirectProxy;
        }
        
    public:
        ABYTEK_RA_DECLARE_OBJECT_CREATABLE(A_RHIDrawPassProxy);
        virtual void Build(const TW_Valid<A_RHIDrawPass>& Pass);
        virtual void Release() override;
    };
}