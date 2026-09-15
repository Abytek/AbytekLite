#pragma once

#include "Abytek/Engine.RHI.prerequisites.hpp"


namespace Abytek
{
    class A_RHIContext;
    class A_RHIContextProxy;
    class A_RHIContextChild;
    
    class ABYTEK_ENGINE_RHI_API A_RHIContextChildProxy : public A_RAObject
    {
    private:
        TW<A_RHIContextProxy> _ContextProxy;
        TW<A_RHIContextChild> _ContextChild;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetContextProxy() const noexcept
        {
            return _ContextProxy;
        }
        ABYTEK_FORCE_INLINE auto& InjectContextProxy() noexcept
        {
            return _ContextProxy;
        }
        ABYTEK_FORCE_INLINE const auto& GetContextChild() const noexcept
        {
            return _ContextChild;
        }
        ABYTEK_FORCE_INLINE auto& InjectContextChild() noexcept
        {
            return _ContextChild;
        }
        
    public:
        ABYTEK_RA_DECLARE_OBJECT(A_RHIContextChildProxy);
        virtual void Build(const TW_Valid<A_RHIContextChild>& ContextChild);
        virtual void Release() override;
    };
    
    struct F_RHIContextChildBuildParams
    {
        TW<A_RHIContext> Context;
    };
    
    class ABYTEK_ENGINE_RHI_API A_RHIContextChild : public A_RAObject
    {
    private:
        TW<A_RHIContext> _Context;
        TS<A_RHIContextChildProxy> _Proxy;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetContext() const noexcept
        {
            return _Context;
        }
        ABYTEK_FORCE_INLINE auto& InjectContext() noexcept
        {
            return _Context;
        }
        ABYTEK_FORCE_INLINE const auto& GetProxy() const noexcept
        {
            return _Proxy;
        }
        ABYTEK_FORCE_INLINE auto& InjectProxy() noexcept
        {
            return _Proxy;
        }

    public:
        ABYTEK_RA_DECLARE_OBJECT(A_RHIContextChild);
        virtual void Release() override;

    protected:
        virtual void Build(const F_RHIContextChildBuildParams& BuildParams);
        
    protected:
        virtual TS<A_RHIContextChildProxy> CreateProxy();
        virtual void DestroyProxy();
        
    public:
#ifdef ABYTEK_DEBUG_INFO
        virtual void SetDebugName(const F_DebugName& DebugName) noexcept override;
#endif
        
    public:
        virtual B8 UseStrictMode() const;
    };
}

#ifdef ABYTEK_ENGINE_RHI_ENABLE_ASSERTIONS
#define ABYTEK_ENGINE_RHI_CONTEXT_CHILD_VALIDATE() ABYTEK_ENGINE_RHI_ASSERT(!UseStrictMode() || GetContext()->GetCurrentProcess()) << "Invalid process, cannot use RHI context child APIs without attacking the context to any process";
#else
#define ABYTEK_ENGINE_RHI_CONTEXT_CHILD_VALIDATE() ;
#endif
