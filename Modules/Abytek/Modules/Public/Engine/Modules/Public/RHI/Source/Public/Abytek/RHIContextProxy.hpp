#pragma once

#include "Abytek/Engine.RHI.prerequisites.hpp"


namespace Abytek
{
    class A_RHIDevice;
    class A_RHIProcess;
    class A_RHIContext;
    
    struct F_RHIContextProxyBuildParams
    {
        TW<A_RHIContext> Context;
    };
    
    class ABYTEK_ENGINE_RHI_API A_RHIContextProxy : public A_RAObject
    {
    private:
        TW<A_RHIContext> _Context;
        TW<A_RHIDevice> _Device;
        TW<A_RHIProcess> _CurrentProcess;
        mutable F_YieldSpinLock _ExecutionLock;
        
        B8 _IsFirstExecution = true; 
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetContext() const noexcept
        {
            return _Context;
        }
        ABYTEK_FORCE_INLINE const auto& GetDevice() const noexcept
        {
            return _Device;
        }
        ABYTEK_FORCE_INLINE const auto& GetCurrentProcess() const noexcept
        {
            return _CurrentProcess;
        }
        
        ABYTEK_FORCE_INLINE auto IsFirstExecution() const noexcept
        {
            return _IsFirstExecution;
        }

    public:
        ABYTEK_RA_DECLARE_OBJECT_CREATABLE(A_RHIContextProxy);
        virtual void Build(const F_RHIContextProxyBuildParams& BuildParams);
        virtual void Release() override;
        
    public:
        virtual void BeginExecution(const TW_Valid<A_RHIProcess>& Process);
        virtual void EndExecution();
        
    public:
        virtual void FirstExecution();
    };
}
