#pragma once

#include "Abytek/RHIPass.hpp"
#include "Abytek/RHIResource.hpp"


namespace Abytek
{
    class A_RHISubmissionList;
    
    using F_RHIReadbackBufferCallback = TF_Function<void(const F_RHIBufferDataView& BufferDataView)>;
    
    struct F_RHIReadbackBufferPassBuildParams : F_RHIPassBuildParams
    {
        TS<A_RHIResource> Buffer;
        U64 SizeInBytes = 0;
        U64 OffsetInBytes = 0;
        F_RHIReadbackBufferCallback Callback;
    };
    
    class ABYTEK_ENGINE_RHI_API A_RHIReadbackBufferPass : public A_RHIPass
    {
    private:
        TS<A_RHIResource> _Buffer;
        U64 _SizeInBytes = 0;
        U64 _OffsetInBytes = 0;
        F_RHIReadbackBufferCallback _Callback;

    public:
        ABYTEK_FORCE_INLINE const auto& GetBuffer() const noexcept
        {
            return _Buffer;
        }
        ABYTEK_FORCE_INLINE auto GetSizeInBytes() const noexcept
        {
            return _SizeInBytes;
        }
        ABYTEK_FORCE_INLINE auto GetOffsetInBytes() const noexcept
        {
            return _OffsetInBytes;
        }
        ABYTEK_FORCE_INLINE const auto& GetCallback() const noexcept
        {
            return _Callback;
        }
        ABYTEK_FORCE_INLINE auto HasWork() const noexcept
        {
            return _SizeInBytes > 0;
        }

    public:
        ABYTEK_RA_DECLARE_OBJECT_CREATABLE(A_RHIReadbackBufferPass);
        virtual void Build(const F_RHIReadbackBufferPassBuildParams& BuildParams);
        void Release() override;
        
    public:
        E_RHIPassClass GetPassClass() const override
        {
            return E_RHIPassClass::BLIT;
        }
        
    public:
        TS_Valid<A_RHIPassProxy> CreateProxy() override;
        
    protected:
        F_RHIReadbackBufferCallback MoveCallback();
    };
}