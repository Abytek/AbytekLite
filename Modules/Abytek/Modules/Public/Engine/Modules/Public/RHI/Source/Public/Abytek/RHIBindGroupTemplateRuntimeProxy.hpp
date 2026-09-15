#pragma once

#include "Abytek/RHITemplateRuntime.hpp"
#include "Abytek/RHIBindGroupTemplateRuntime.hpp"


namespace Abytek
{
    class ABYTEK_ENGINE_RHI_API A_RHIBindGroupTemplateRuntimeProxy :
        public A_RHITemplateRuntimeProxy
    {
    private:
        
    public:
        ABYTEK_FORCE_INLINE auto GetBindGroupTemplateRuntime() const noexcept
        {
            return GetContextChild().FastCast<A_RHIBindGroupTemplateRuntime>();
        }
        
    public:
        ABYTEK_RA_DECLARE_OBJECT_CREATABLE(A_RHIBindGroupTemplateRuntimeProxy);
        void LateBuildBindGroupTemplateRuntime();
        virtual void Release() override;
    };
}
