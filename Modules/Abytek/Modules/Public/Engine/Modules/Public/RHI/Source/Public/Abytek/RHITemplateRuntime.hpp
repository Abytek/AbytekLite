#pragma once

#include "Abytek/RHIContextChild.hpp"
#include "Abytek/RHITemplate.hpp"


namespace Abytek
{
    class A_RHITemplate;
    class A_RHITemplateRuntimeDatabase;
    
    class ABYTEK_ENGINE_RHI_API A_RHITemplateRuntimeProxy : public A_RHIContextChildProxy
    {
    private:
        TS<A_RHITemplate> _Template;
        
    public:
        auto GetTemplateRuntime() const noexcept;
        
        ABYTEK_FORCE_INLINE const auto& GetTemplate() const noexcept
        {
            return _Template;
        }
        
    public:
        ABYTEK_RA_DECLARE_OBJECT(A_RHITemplateRuntimeProxy);
        void LateBuildTemplateRuntime();
        virtual void Release() override;
    };
    
    struct ABYTEK_ENGINE_RHI_API F_RHITemplateRuntimeBuildParams : F_RHIContextChildBuildParams
    {
        TS<A_RHITemplate> Template;
    };
    
    class ABYTEK_ENGINE_RHI_API A_RHITemplateRuntime : public A_RHIContextChild
    {
    public:
        friend class A_RHITemplateRuntimeDatabase;
        
    private:
        TS<A_RHITemplate> _Template;
        F_AtomicFlag _IsActivated;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetTemplate() const noexcept
        {
            return _Template;
        }
        ABYTEK_FORCE_INLINE auto& InjectTemplate() noexcept
        {
            return _Template;
        }
        ABYTEK_FORCE_INLINE B8 IsActivated() const noexcept
        {
            return _IsActivated.test(boost::memory_order_acquire);
        }
        ABYTEK_FORCE_INLINE auto GetHashCode() const noexcept
        {
            return _Template->GetHashCode();
        }
        TW_Valid<A_RHITemplateRuntimeDatabase> GetDatabase() const noexcept;
        
    public:
        ABYTEK_RA_DECLARE_OBJECT(A_RHITemplateRuntime);
        virtual void Release() override;

    protected:
        virtual void Build(const F_RHITemplateRuntimeBuildParams& BuildParams);

    protected:
        virtual void FinalizeActivation();
        
    public:
        void FinalizeActivationAndUnlock();

    public:
        ABYTEK_FORCE_INLINE void WaitForActivation() const noexcept
        {
            while (!IsActivated())
            {
                H_TaskUtilities::Switch();
            }
        }
    };
}