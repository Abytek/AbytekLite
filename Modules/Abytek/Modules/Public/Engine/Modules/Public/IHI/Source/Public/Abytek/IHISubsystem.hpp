#pragma once

#include "Abytek/Engine.IHI.prerequisites.hpp"
#include "Abytek/IHISubsystemProxy.hpp"
#include "Abytek/IHIAPI.hpp"


namespace Abytek
{
    class A_IHISubsystemProxy;
    class A_IHIButtonCoder;
    class A_IHIMouseManager;
    class A_IHIKeyboardManager;
    
    class ABYTEK_ENGINE_IHI_API F_IHISubsystem final : public A_ApplicationSubsystem
    {
    public:
        ABYTEK_BEGIN_REFLECTOR(A_ApplicationSubsystem)
        ABYTEK_END_REFLECTOR(F_IHISubsystem);

    public:
        ABYTEK_DECLARE_STATIC_APPLICATION_SUBSYSTEM(F_IHISubsystem);

    private:
        TF_Vector<E_IHIAPI> _APIs;
        
        TU<A_IHISubsystemProxy> _Proxy;
        TU<A_IHIButtonCoder> _ButtonCoder;
        TU<A_IHIMouseManager> _MouseManager;
        TU<A_IHIKeyboardManager> _KeyboardManager;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetAPIs() const noexcept
        {
            return _APIs;
        }
        ABYTEK_FORCE_INLINE E_IHIAPI GetActiveAPI() const noexcept
        {
            return _Proxy->GetAPI();
        }
        ABYTEK_FORCE_INLINE B8 IsEnabled() const noexcept
        {
            return GetActiveAPI() != E_IHIAPI::NONE;
        }

    public:
        F_IHISubsystem(const F_ProgramUnitBuildParams& BuildParams);
        ~F_IHISubsystem() override;

    protected:
        void OnInit() override;
        void OnStartup() override;
        void OnShutdown() override;
        void OnRelease() override;
    };
}
