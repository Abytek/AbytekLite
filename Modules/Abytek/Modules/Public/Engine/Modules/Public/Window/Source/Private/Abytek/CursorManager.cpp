#include "Abytek/CursorManager.hpp"
#include "Abytek/Cursor.hpp"
#include "Abytek/WindowSubsystem.hpp"
#include "Abytek/Windows/CursorManager.hpp"
#include "Abytek/Android/CursorManager.hpp"
#include "Abytek/ApplicationUpdateFunction.hpp"
#include "Abytek/CoreUpdateGraph/HighLevelUpdateRange.hpp"


namespace Abytek
{
    ABYTEK_RA_BEGIN_OBJECT(A_CursorManager)       
        auto WindowSubsystem = F_WindowSubsystem::GetInstance();
        switch (WindowSubsystem->GetActiveAPI())
        {
#ifdef ABYTEK_PLATFORM_WINDOWS
            case E_WindowAPI::WINDOWS:
                return TU<F_WindowsCursorManager>()();
#endif
#ifdef ABYTEK_PLATFORM_ANDROID
            case E_WindowAPI::ANDROID_:
                return TU<F_AndroidCursorManager>()();
#endif  
        }
    ABYTEK_RA_END_OBJECT()
    ABYTEK_DEFINE_OBJECT_SINGLETON_CRT(A_CursorManager);

    F_Name A_CursorManager::GetUpdateUpdateFunctionName()
    {
        return ABYTEK_NAME("Abytek::A_CursorManager::Update");
    }
    F_Name A_CursorManager::GetPostUpdateUpdateFunctionName()
    {
        return ABYTEK_NAME("Abytek::A_CursorManager::PostUpdate");
    }
    void A_CursorManager::GlobalInit()
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
    void A_CursorManager::GlobalRelease()
    {
        H_UpdateUtilities::UnregisterFunction(
            GetPostUpdateUpdateFunctionName()    
        );
        H_UpdateUtilities::UnregisterFunction(
            GetUpdateUpdateFunctionName()    
        );
    };
    
    void A_CursorManager::Build()
    {
        A_RAObject::BuildMinimal();
        ABYTEK_BIND_OBJECT_SINGLETON_CRT();
    }
    void A_CursorManager::Release()
    {
        ABYTEK_ENGINE_WINDOW_ASSERT(_Cursors.size() == 0) << "All cursors must be destroyed before releasing cursor manager";
        A_RAObject::Release();
    }

    void A_CursorManager::Startup()
    {
        CreateCursor();
    }
    void A_CursorManager::Shutdown()
    {
        DestroyCursor(GetMainCursor());
    }

    TW_Valid<A_Cursor> A_CursorManager::CreateCursor()
    {
        auto Window = RACreateAndBuildShared<A_Cursor>();
        Window->FinalizeNextState();
        Window->InjectCurrentState() = Window->GetNextState();
        _Cursors.insert(Window);
        return Window.Weak();
    }

    void A_CursorManager::DestroyCursor(const TW_Valid<A_Cursor>& Cursor)
    {
        auto& NextState = Cursor->InjectNextState();
        NextState.WillBeDestroyed = true;
    }

    void A_CursorManager::OnUpdate()
    {
        for (const auto& Cursor : _Cursors)
        {
            Cursor->FinalizeNextState();
        }
        
        for (const auto& Cursor : _Cursors)
        {
            Cursor->UpdateCurrentState();
        }
    }
    void A_CursorManager::OnPostUpdate()
    {
        {
            auto Cursors = _Cursors;
            for (const auto& Cursor : Cursors)
            {
                const auto& NextState = Cursor->GetNextState();
                if (NextState.WillBeDestroyed)
                {
                    _Cursors.erase(_Cursors.find(Cursor));
                }
            }
        }
    }

    TW<A_Cursor> A_CursorManager::GetMainCursor() const
    {
        auto It = _Cursors.begin();
        ABYTEK_ENGINE_WINDOW_ASSERT(It != _Cursors.end());
        return It->Weak();
    }
}
