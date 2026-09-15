#pragma once

#include "Abytek/Engine.Window.prerequisites.hpp"
#include "Abytek/Window.hpp"


namespace Abytek
{
    class ABYTEK_ENGINE_WINDOW_API A_WindowManager : public A_RAObject
    {
    public:
        ABYTEK_DECLARE_OBJECT_SINGLETON_CRT(A_WindowManager);
        
    public:
        static F_Name GetUpdateUpdateFunctionName();
        static F_Name GetPostUpdateUpdateFunctionName();
        static void GlobalInit();
        static void GlobalRelease();
        
    private:
        TF_Set<TS<A_Window>> _Windows;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetWindows() const noexcept
        {
            return _Windows;
        }
        ABYTEK_FORCE_INLINE auto& InjectWindows() noexcept
        {
            return _Windows;
        }

    public:
        ABYTEK_RA_DECLARE_OBJECT_CREATABLE(A_WindowManager);
        virtual void Build();
        virtual void Release() override;
        
    public:
        virtual void Startup();
        virtual void Shutdown();

    public:
        TW_Valid<A_Window> CreateWindow_(const F_WindowBuildParams& Params);
        void DestroyWindow(const TW_Valid<A_Window>& Window);
        
    protected:
        virtual void OnUpdate();
        virtual void OnPostUpdate();
    };
}
