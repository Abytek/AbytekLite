#include "Abytek/IHIKeyboardManager.hpp"
#include "Abytek/IHIButtonCoder.hpp"
#include "Abytek/IHIKeyboard.hpp"
#include "Abytek/IHISubsystem.hpp"
#include "Abytek/Windows/IHIKeyboardManager.hpp"
#include "Abytek/Android/IHIKeyboardManager.hpp"
#include "Abytek/UpdateBase/UpdateUtilities.hpp"
#include "Abytek/CoreUpdateGraph/HighLevelUpdateRange.hpp"


namespace Abytek
{
    ABYTEK_DEFINE_OBJECT_SINGLETON_CRT(A_IHIKeyboardManager);
    
    ABYTEK_RA_BEGIN_OBJECT(A_IHIKeyboardManager)
        auto& IHISubsystem = F_IHISubsystem::GetInstance();
        switch (IHISubsystem->GetActiveAPI())
        {
#ifdef ABYTEK_PLATFORM_WINDOWS
        case E_IHIAPI::WINDOWS:
            return TU<F_WindowsIHIKeyboardManager>()();
#endif
#ifdef ABYTEK_PLATFORM_ANDROID
        case E_IHIAPI::ANDROID_:
            return TU<F_AndroidIHIKeyboardManager>()();
#endif
        }
    ABYTEK_RA_END_OBJECT()

    F_Name A_IHIKeyboardManager::GetUpdateUpdateFunctionName()
    {
        return ABYTEK_NAME("Abytek::A_IHIKeyboardManager::Update");
    }
    F_Name A_IHIKeyboardManager::GetPostUpdateUpdateFunctionName()
    {
        return ABYTEK_NAME("Abytek::A_IHIKeyboardManager::PostUpdate");
    }
    void A_IHIKeyboardManager::GlobalInit()
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
    void A_IHIKeyboardManager::GlobalRelease()
    {
        H_UpdateUtilities::UnregisterFunction(
            GetPostUpdateUpdateFunctionName()    
        );
        H_UpdateUtilities::UnregisterFunction(
            GetUpdateUpdateFunctionName()    
        );
    };
    
    void A_IHIKeyboardManager::Build()
    {
        A_RAObject::BuildMinimal();
        ABYTEK_BIND_OBJECT_SINGLETON_CRT();
    }   
    void A_IHIKeyboardManager::Release()
    {
        A_RAObject::Release();
    }

    void A_IHIKeyboardManager::Startup()
    {
        CreateKeyboard();
    }
    void A_IHIKeyboardManager::Shutdown()
    {
        DestroyKeyboard(GetMainKeyboard());
    }

    TW_Valid<A_IHIKeyboard> A_IHIKeyboardManager::CreateKeyboard()
    {
        auto Keyboard = RACreateAndBuildShared<A_IHIKeyboard>();
        Keyboard->FinalizeNextState();
        Keyboard->InjectCurrentState() = Keyboard->GetNextState();
        _Keyboards.insert(Keyboard);
        return Keyboard.Weak();
    }
    void A_IHIKeyboardManager::DestroyKeyboard(const TW_Valid<A_IHIKeyboard>& Keyboard)
    {
        auto& NextState = Keyboard->InjectNextState();
        NextState.WillBeDestroyed = true;
    }

    void A_IHIKeyboardManager::OnUpdate()
    {
        for (const auto& Keyboard : _Keyboards)
        {
            Keyboard->FinalizeNextState();
        }
    
        for (const auto& Keyboard : _Keyboards)
        {
            Keyboard->UpdateCurrentState();
        }
    }
    void A_IHIKeyboardManager::OnPostUpdate()
    {
        {
            auto Keyboards = _Keyboards;
            for (const auto& Keyboard : Keyboards)
            {
                const auto& NextState = Keyboard->GetNextState();
                if (NextState.WillBeDestroyed)
                {
                    _Keyboards.erase(_Keyboards.find(Keyboard));
                }
            }
        }
    }

    TW<A_IHIKeyboard> A_IHIKeyboardManager::GetMainKeyboard()
    {
        auto It = _Keyboards.begin();
        ABYTEK_ENGINE_WINDOW_ASSERT(It != _Keyboards.end());
        return It->Weak();
    }
}
