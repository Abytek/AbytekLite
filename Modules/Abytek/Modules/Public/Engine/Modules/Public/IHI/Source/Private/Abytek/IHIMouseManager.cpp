#include "Abytek/IHIMouseManager.hpp"
#include "Abytek/IHIButtonCoder.hpp"
#include "Abytek/IHIMouse.hpp"
#include "Abytek/IHISubsystem.hpp"
#include "Abytek/Windows/IHIMouseManager.hpp"
#include "Abytek/Android/IHIMouseManager.hpp"


namespace Abytek
{
    ABYTEK_DEFINE_OBJECT_SINGLETON_CRT(A_IHIMouseManager);
    
    ABYTEK_RA_BEGIN_OBJECT(A_IHIMouseManager)
        auto& IHISubsystem = F_IHISubsystem::GetInstance();
        switch (IHISubsystem->GetActiveAPI())
        {
#ifdef ABYTEK_PLATFORM_WINDOWS
        case E_IHIAPI::WINDOWS:
            return TU<F_WindowsIHIMouseManager>()();
#endif
#ifdef ABYTEK_PLATFORM_ANDROID
        case E_IHIAPI::ANDROID_:
            return TU<F_AndroidIHIMouseManager>()();
#endif
        }
    ABYTEK_RA_END_OBJECT()

    F_Name A_IHIMouseManager::GetUpdateUpdateFunctionName()
    {
        return ABYTEK_NAME("Abytek::A_IHIMouseManager::Update");
    }
    F_Name A_IHIMouseManager::GetPostUpdateUpdateFunctionName()
    {
        return ABYTEK_NAME("Abytek::A_IHIMouseManager::PostUpdate");
    };
    void A_IHIMouseManager::GlobalInit()
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
    void A_IHIMouseManager::GlobalRelease()
    {
        H_UpdateUtilities::UnregisterFunction(
            GetPostUpdateUpdateFunctionName()    
        );
        H_UpdateUtilities::UnregisterFunction(
            GetUpdateUpdateFunctionName()    
        );
    };
    
    void A_IHIMouseManager::Build()
    {
        A_RAObject::BuildMinimal();
        ABYTEK_BIND_OBJECT_SINGLETON_CRT();
    }   
    void A_IHIMouseManager::Release()
    {
        A_RAObject::Release();
    }


    void A_IHIMouseManager::Startup()
    {
        CreateMouse();
    }
    void A_IHIMouseManager::Shutdown()
    {
        DestroyMouse(GetMainMouse());
    }

    TW_Valid<A_IHIMouse> A_IHIMouseManager::CreateMouse()
    {
        auto Mouse = RACreateAndBuildShared<A_IHIMouse>();
        Mouse->FinalizeNextState();
        Mouse->InjectCurrentState() = Mouse->GetNextState();
        _Mouses.insert(Mouse);
        return Mouse.Weak();
    }
    void A_IHIMouseManager::DestroyMouse(const TW_Valid<A_IHIMouse>& Mouse)
    {
        auto& NextState = Mouse->InjectNextState();
        NextState.WillBeDestroyed = true;
    }

    void A_IHIMouseManager::OnUpdate()
    {
        for (const auto& Mouse : _Mouses)
        {
            Mouse->FinalizeNextState();
        }
    
        for (const auto& Mouse : _Mouses)
        {
            Mouse->UpdateCurrentState();
        }
    }
    void A_IHIMouseManager::OnPostUpdate()
    {
        {
            auto Mouses = _Mouses;
            for (const auto& Mouse : Mouses)
            {
                const auto& NextState = Mouse->GetNextState();
                if (NextState.WillBeDestroyed)
                {
                    _Mouses.erase(_Mouses.find(Mouse));
                }
            }
        }
    }

    TW<A_IHIMouse> A_IHIMouseManager::GetMainMouse()
    {
        auto It = _Mouses.begin();
        ABYTEK_ENGINE_WINDOW_ASSERT(It != _Mouses.end());
        return It->Weak();
    }
}
