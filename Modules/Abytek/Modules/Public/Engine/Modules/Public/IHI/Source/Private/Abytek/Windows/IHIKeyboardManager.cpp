#include "Abytek/Windows/IHIKeyboardManager.hpp"
#include "Abytek/Windows/IHIKeyboard.hpp"
#include "Abytek/Windows/IHIButtonCoder.hpp"
#include "Abytek/IHISubsystem.hpp"
#include "Abytek/LLEvents/Windows/LLEvents.hpp"


#ifdef ABYTEK_PLATFORM_WINDOWS
namespace Abytek
{
    void F_WindowsIHIKeyboardManager::Build()
    {
        A_IHIKeyboardManager::Build();

        auto LLEvents = A_LLEvents::GetInstance().FastCast<F_WindowsLLEvents>();

        auto& WinAPIProcessMsgEvent = LLEvents->GetOnWinAPIProcessMsgEvent();
        _OnProcessMsgEvent = WinAPIProcessMsgEvent.AddListener(
            [this](HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, TF_Optional<HRESULT>& ResultOpt)
            {
                _ProcessMessage(hwnd, uMsg, wParam, lParam, ResultOpt);
            }
        );
    }
    void F_WindowsIHIKeyboardManager::Release()
    {
        auto LLEvents = A_LLEvents::GetInstance().FastCast<F_WindowsLLEvents>();

        auto& WinAPIProcessMsgEvent = LLEvents->GetOnWinAPIProcessMsgEvent();
        WinAPIProcessMsgEvent.RemoveListener(_OnProcessMsgEvent);
        
        A_IHIKeyboardManager::Release();
    }

    void F_WindowsIHIKeyboardManager::OnUpdate()
    {
        A_IHIKeyboardManager::OnUpdate();
    }
    void F_WindowsIHIKeyboardManager::OnPostUpdate()
    {
        A_IHIKeyboardManager::OnPostUpdate();
    }

    void F_WindowsIHIKeyboardManager::_ProcessMessage(HWND WinAPIHandle, UINT UMsg, WPARAM WParam, LPARAM LParam, TF_Optional<HRESULT>& ResultOpt)
    {
        auto MainKeyboard = GetMainKeyboard();
        auto& NextState = MainKeyboard->InjectNextState();
        
        switch (UMsg)
        {
        case WM_KEYDOWN:
            if(
                !(LParam & (1 << 30))
            ) {
                F_IHIButton KeyboardButton;
                KeyboardButton.DeviceType = E_IHIButtonDeviceType::KEYBOARD;
                KeyboardButton.Code = F_IHIButtonCode(WParam);
                if (NextState.PressedButtons.find(KeyboardButton) == NextState.PressedButtons.end())
                {
                    NextState.PressedButtons.insert(KeyboardButton);
                }
            }
            break;
        case WM_KEYUP:
            {
                F_IHIButton KeyboardButton;
                KeyboardButton.DeviceType = E_IHIButtonDeviceType::KEYBOARD;
                KeyboardButton.Code = F_IHIButtonCode(WParam);
                if (NextState.PressedButtons.find(KeyboardButton) != NextState.PressedButtons.end())
                {
                    NextState.PressedButtons.erase(NextState.PressedButtons.find(KeyboardButton));
                }
                break;
            }
        case WM_CHAR:
            {
                NextState.TextChars.push_back(F_TextChar(WParam));
                break;
            }
        };
    }
}
#endif