#pragma once

#include "Abytek/Engine.RHI.prerequisites.hpp"


namespace Abytek
{
    class A_RHIDevice;
    class A_RHIProcess;
    class A_RHIContextProxy;
    class A_RHITemplateRuntimeDatabase;
    class F_RHITransientUploadBufferManager;
    class F_RHITransientReadbackBufferManager;
    class F_RHITransientUploadBufferManager_V2;
    class F_RHITransientReadbackBufferManager_V2;
    class F_RHIConstantDataManager;
    
    struct F_RHIContextBuildParams
    {
        TW<A_RHIDevice> Device;
    };
    
    class ABYTEK_ENGINE_RHI_API A_RHIContext : public A_RAObject
    {
    private:
        TW<A_RHIDevice> _Device;
        TW<A_RHIProcess> _CurrentProcess;
        mutable F_YieldSpinLock _CompileLock;
        
        TS<A_RHIContextProxy> _Proxy;
        TS<A_RHITemplateRuntimeDatabase> _TemplateRuntimeDatabase;
        
        B8 _IsFirstCompile = true; 
        
        TS<F_RHITransientUploadBufferManager> _TransientUploadBufferManager;
        TS<F_RHITransientReadbackBufferManager> _TransientReadbackBufferManager;
        TS<F_RHITransientUploadBufferManager_V2> _TransientUploadBufferManager_V2;
        TS<F_RHITransientReadbackBufferManager_V2> _TransientReadbackBufferManager_V2;
        TS<F_RHIConstantDataManager> _ConstantDataManager;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetDevice() const noexcept
        {
            return _Device;
        }
        ABYTEK_FORCE_INLINE auto& InjectDevice() noexcept
        {
            return _Device;
        }
        
        ABYTEK_FORCE_INLINE const auto& GetCurrentProcess() const noexcept
        {
            return _CurrentProcess;
        }
        
        ABYTEK_FORCE_INLINE const auto& GetProxy() const noexcept
        {
            return _Proxy;
        }
        ABYTEK_FORCE_INLINE const auto& GetTemplateRuntimeDatabase() const noexcept
        {
            return _TemplateRuntimeDatabase;
        }
        
        ABYTEK_FORCE_INLINE auto IsFirstCompile() const noexcept
        {
            return _IsFirstCompile;
        }
        
        ABYTEK_FORCE_INLINE const auto& GetTransientUploadBufferManager() const noexcept
        {
            return _TransientUploadBufferManager;
        }
        ABYTEK_FORCE_INLINE const auto& GetTransientReadbackBufferManager() const noexcept
        {
            return _TransientReadbackBufferManager;
        }
        ABYTEK_FORCE_INLINE const auto& GetTransientUploadBufferManager_V2() const noexcept
        {
            return _TransientUploadBufferManager_V2;
        }
        ABYTEK_FORCE_INLINE const auto& GetTransientReadbackBufferManager_V2() const noexcept
        {
            return _TransientReadbackBufferManager_V2;
        }
        ABYTEK_FORCE_INLINE const auto& GetConstantDataManager() const noexcept
        {
            return _ConstantDataManager;
        }

    public:
        ABYTEK_RA_DECLARE_OBJECT_CREATABLE(A_RHIContext);
        virtual void Build(const F_RHIContextBuildParams& BuildParams);
        virtual void Release() override;
        
    private:
        void _RunFirstProcess();
        
    public:
        virtual void FirstCompile();
        virtual void FinalizeRelease();
        
    public:
        virtual void HighLevelInitialize();
        virtual void HighLevelDeinitialize();
        
    public:
        virtual void BeginCompile(const TW_Valid<A_RHIProcess>& Process);
        virtual void EndCompile();
    };
}
