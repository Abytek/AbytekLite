#pragma once

#include "Abytek/RHISubmissionItem.hpp"
#include "Abytek/RHIPassClass.hpp"
#include "Abytek/RHIPassPosition.hpp"
#include "Abytek/RHIGPUWorkClass.hpp"


namespace Abytek
{
    class A_RHIPass;
    class A_RHIContext;
    class A_RHIContextProxy;

    struct F_RHIPassBuildParams
    {
        TW<A_RHIContext> Context;
        E_RHIGPUWorkClass GPUWorkClass = E_RHIGPUWorkClass::DEFAULT;
    };
    
    class ABYTEK_ENGINE_RHI_API A_RHIPassProxy : public A_RAObject
    {
    private:
        TW<A_RHIContextProxy> _ContextProxy;
        TW<A_RHIProcess> _Process;
        E_RHIGPUWorkClass _GPUWorkClass = E_RHIGPUWorkClass::NONE;
        E_RHIPassClass _PassClass = E_RHIPassClass::NONE;
        E_RHIPassPosition _PassPosition = E_RHIPassPosition::NONE;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetContextProxy() const noexcept
        {
            return _ContextProxy;
        }
        ABYTEK_FORCE_INLINE const auto& GetProcess() const noexcept
        {
            return _Process;
        }
        ABYTEK_FORCE_INLINE auto GetGPUWorkClass() const noexcept
        {
            return _GPUWorkClass;
        }
        ABYTEK_FORCE_INLINE auto GetPassClass() const noexcept
        {
            return _PassClass;
        }
        ABYTEK_FORCE_INLINE auto GetPassPosition() const noexcept
        {
            return _PassPosition;
        }
        
    public:
        ABYTEK_RA_DECLARE_OBJECT(A_RHIPassProxy);
        virtual void Release() override;
        
    protected:
        void BuildPassProxy(const TW_Valid<A_RHIPass>& Pass);
    };
    
    class ABYTEK_ENGINE_RHI_API A_RHIPass : public A_RHISubmissionItem
    {
    private:
        TW<A_RHIContext> _Context;
        E_RHIGPUWorkClass _GPUWorkClass = E_RHIGPUWorkClass::NONE;

    public:
        ABYTEK_FORCE_INLINE const auto& GetContext() const noexcept
        {
            return _Context;
        }
        ABYTEK_FORCE_INLINE auto GetGPUWorkClass() const noexcept
        {
            return _GPUWorkClass;
        }
        
    public:
        ABYTEK_RA_DECLARE_OBJECT(A_RHIPass);
        virtual void Build(const F_RHIPassBuildParams& BuildParams);
        void Release() override;
        
    public:
        virtual E_RHIPassClass GetPassClass() const;
        virtual E_RHIPassPosition GetPassPosition() const;
        
    public:
        virtual TS_Valid<A_RHIPassProxy> CreateProxy();
        
    public:
        TW_Valid<A_RHIProcess> GetProcess() const override;
    };
}