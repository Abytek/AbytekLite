#include "Abytek/WindowManager.hpp"
#include "Abytek/Window.hpp"
#include "Abytek/WindowSubsystem.hpp"
#include "Abytek/Windows/WindowManager.hpp"
#include "Abytek/Android/WindowManager.hpp"
#include "Abytek/UpdateBase/UpdateUtilities.hpp"
#include "Abytek/CoreUpdateGraph/HighLevelUpdateRange.hpp"


namespace Abytek
{
    ABYTEK_RA_BEGIN_OBJECT(A_WindowManager)        
        auto WindowSubsystem = F_WindowSubsystem::GetInstance();
        switch (WindowSubsystem->GetActiveAPI())
        {
#ifdef ABYTEK_PLATFORM_WINDOWS
        case E_WindowAPI::WINDOWS:
            return TU<F_WindowsWindowManager>()();
#endif
#ifdef ABYTEK_PLATFORM_ANDROID
        case E_WindowAPI::ANDROID_:
            return TU<F_AndroidWindowManager>()();
#endif
        }
    ABYTEK_RA_END_OBJECT()
    ABYTEK_DEFINE_OBJECT_SINGLETON_CRT(A_WindowManager)

    F_Name A_WindowManager::GetUpdateUpdateFunctionName()
    {
        return ABYTEK_NAME("Abytek::F_WindowManager::Update");
    }
    F_Name A_WindowManager::GetPostUpdateUpdateFunctionName()
    {
        return ABYTEK_NAME("Abytek::F_WindowManager::PostUpdate");
    };
    void A_WindowManager::GlobalInit()
    {
        {
            auto UpdateFunction = H_UpdateUtilities::RegisterFunction(
                []
                {
                    GetInstance()->OnUpdate();
                },
                GetUpdateUpdateFunctionName()  
            );
            UpdateFunction->AddReverseDependency(F_HighLevelUpdateRange::GetBeginFunctionName());
        }
        {
            auto UpdateFunction = H_UpdateUtilities::RegisterFunction(
                []
                {
                    GetInstance()->OnPostUpdate();
                },
                GetPostUpdateUpdateFunctionName()  
            );
            UpdateFunction->AddDependency(F_HighLevelUpdateRange::GetEndFunctionName());
        }
    }
    void A_WindowManager::GlobalRelease()
    {
        H_UpdateUtilities::UnregisterFunction(
            GetPostUpdateUpdateFunctionName()    
        );
        H_UpdateUtilities::UnregisterFunction(
            GetUpdateUpdateFunctionName()    
        );
    };
    
    void A_WindowManager::Build()
    {
        A_RAObject::BuildMinimal();
        ABYTEK_BIND_OBJECT_SINGLETON_CRT();
    }
    void A_WindowManager::Release()
    {
        ABYTEK_ENGINE_WINDOW_ASSERT(_Windows.size() == 0) << "All windows must be destroyed before releasing window manager";
        A_RAObject::Release();
    }

    void A_WindowManager::Startup()
    {
    }
    void A_WindowManager::Shutdown()
    {
    }

    TW_Valid<A_Window> A_WindowManager::CreateWindow_(const F_WindowBuildParams& Params)
    {
        auto Window = RACreateAndBuildShared<A_Window>(Params);
        Window->FinalizeNextState();
        Window->InjectCurrentState() = Window->GetNextState();
        _Windows.insert(Window);
        return Window.Weak();
    }
    void A_WindowManager::DestroyWindow(const TW_Valid<A_Window>& Window)
    {
        auto& NextState = Window->InjectNextState();
        NextState.IsRequestedToDestroy = true;
        NextState.IsAcceptedToDestroy = true;
    }

    void A_WindowManager::OnUpdate()
    {
        for (const auto& Window : _Windows)
        {
            Window->FinalizeNextState();
        }
        
        for (const auto& Window : _Windows)
        {
            Window->UpdateCurrentState();
        }
    }
    void A_WindowManager::OnPostUpdate()
    {
        {
            auto Windows = _Windows;
            for (const auto& Window : Windows)
            {
                const auto& NextState = Window->GetNextState();
                if (NextState.IsAcceptedToDestroy)
                {
                    _Windows.erase(_Windows.find(Window));
                }
            }
        }
    }
}
