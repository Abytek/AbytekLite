#pragma once

#include "Abytek/RHIContextChild.hpp"
#include "Abytek/RHITemplateHashCode.hpp"


namespace Abytek
{
    class A_RHITemplate;
    class A_RHITemplateRuntime;
    
    struct F_RHITemplateRuntimeDatabaseBuildParams : F_RHIContextChildBuildParams
    {
    };
    
    class ABYTEK_ENGINE_RHI_API A_RHITemplateRuntimeDatabase : public A_RHIContextChild
    {
    public:
        friend class A_RHITemplateRuntime;
        
    private:
        F_SpinLock _SpinLock;
        TF_Map<F_RHITemplateHashCode, TW<A_RHITemplateRuntime>> _TemplateRuntimes;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetTemplateRuntimes() const noexcept
        {
            return _TemplateRuntimes;
        }
        ABYTEK_FORCE_INLINE auto& InjectTemplateRuntimes() noexcept
        {
            return _TemplateRuntimes;
        }

    public:
        ABYTEK_RA_DECLARE_OBJECT_CREATABLE(A_RHITemplateRuntimeDatabase);
        virtual void Build(const F_RHITemplateRuntimeDatabaseBuildParams& BuildParams);
        virtual void Release() override;

    private:
        void _TrackTemplateRuntime(const TW_Valid<A_RHITemplateRuntime>& TemplateRuntime);
        void _UntrackTemplateRuntime(const TW_Valid<A_RHITemplateRuntime>& TemplateRuntime);

    public:
        TS_Valid<A_RHITemplateRuntime> GetOrActivateRuntime(
            const TS_Valid<A_RHITemplate>& Template
        );
        TS<A_RHITemplateRuntime> Find(F_RHITemplateHashCode TemplateHashCode);
        
    public:
        virtual B8 UseStrictMode() const override
        {
            return false;
        }
    };
}