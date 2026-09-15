#pragma once

#include "Abytek/Engine.Window.prerequisites.hpp"
#include "Abytek/WindowAPI.hpp"
#include "Abytek/WindowSubsystemProxy.hpp"


namespace Abytek
{
    class A_WindowManager;
    class A_CursorManager;
    
    class ABYTEK_ENGINE_WINDOW_API F_WindowSubsystem final : public A_ApplicationSubsystem
    {
    public:
        ABYTEK_BEGIN_REFLECTOR(A_ApplicationSubsystem)
        ABYTEK_END_REFLECTOR(F_WindowSubsystem)

    public:
        ABYTEK_DECLARE_STATIC_APPLICATION_SUBSYSTEM(F_WindowSubsystem);

    private:
        TF_Vector<E_WindowAPI> _APIs;
        
        TU<A_WindowSubsystemProxy> _Proxy;
        TU<A_WindowManager> _WindowManager;
        TU<A_CursorManager> _CursorManager;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetAPIs() const noexcept
        {
            return _APIs;
        }
        ABYTEK_FORCE_INLINE E_WindowAPI GetActiveAPI() const noexcept
        {
            return _Proxy->GetAPI();
        }
        ABYTEK_FORCE_INLINE B8 IsEnabled() const noexcept
        {
            return GetActiveAPI() != E_WindowAPI::NONE;
        }

    public:
        F_WindowSubsystem(const F_ProgramUnitBuildParams& BuildParams);
        ~F_WindowSubsystem() override;

    protected:
        void OnInit() override;
        void OnStartup() override;
        void OnShutdown() override;
        void OnRelease() override;
    };
}
