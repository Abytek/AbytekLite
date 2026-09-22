#pragma once

#include "Abytek/RHIPass.hpp"
#include "Abytek/RHIResourceAccess.hpp"
#include "Abytek/RHIBindGroup.hpp"


namespace Abytek
{
    class A_RHIResourceView;

    struct F_RHIDummyPassBuildParams : F_RHIPassBuildParams
    {
        TF_SmallVector<TS<A_RHIResourceView>, 16> ResourceViews;
        TF_SmallVector<F_RHIVertexBufferBinding, 8> VertexBufferBindings;
        TF_Optional<F_RHIIndexBufferBinding> IndexBufferBinding;
    };
    
    class ABYTEK_ENGINE_RHI_API A_RHIDummyPass : public A_RHIPass
    {
    private:
        TF_SmallVector<TS<A_RHIResourceView>, 16> _ResourceViews;
        TF_SmallVector<F_RHIVertexBufferBinding, 8> _VertexBufferBindings;
        TF_Optional<F_RHIIndexBufferBinding> _IndexBufferBinding;

    public:
        ABYTEK_FORCE_INLINE const auto& GetResourceViews() const noexcept
        {
            return _ResourceViews;
        }
        ABYTEK_FORCE_INLINE auto& InjectResourceViews() noexcept
        {
            return _ResourceViews;
        }
        ABYTEK_FORCE_INLINE const auto& GetVertexBufferBindings() const noexcept
        {
            return _VertexBufferBindings;
        }
        ABYTEK_FORCE_INLINE auto& InjectVertexBufferBindings() noexcept
        {
            return _VertexBufferBindings;
        }
        ABYTEK_FORCE_INLINE const auto& GetIndexBufferBinding() const noexcept
        {
            return _IndexBufferBinding;
        }
        ABYTEK_FORCE_INLINE auto& InjectIndexBufferBinding() noexcept
        {
            return _IndexBufferBinding;
        }

    public:
        ABYTEK_RA_DECLARE_OBJECT_CREATABLE(A_RHIDummyPass);
        virtual void Build(const F_RHIDummyPassBuildParams& BuildParams);
        virtual void Release() override;
        
    public:
        virtual E_RHIPassClass GetPassClass() const override
        {
            return E_RHIPassClass::GRAPHICS;
        }
    };
}