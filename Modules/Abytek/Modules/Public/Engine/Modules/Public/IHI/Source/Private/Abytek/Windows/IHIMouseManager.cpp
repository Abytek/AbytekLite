#include "Abytek/Windows/IHIMouseManager.hpp"
#include "Abytek/Windows/IHIMouse.hpp"
#include "Abytek/IHISubsystem.hpp"
#include "Abytek/LLEvents/Windows/LLEvents.hpp"


#ifdef ABYTEK_PLATFORM_WINDOWS
namespace Abytek
{
    void F_WindowsIHIMouseManager::Build()
    {
        A_IHIMouseManager::Build();

        auto LLEvents = A_LLEvents::GetInstance().FastCast<F_WindowsLLEvents>();

        auto& WinAPIProcessMsgEvent = LLEvents->GetOnWinAPIProcessMsgEvent();
        _OnProcessMsgEvent = WinAPIProcessMsgEvent.AddListener(
            [this](HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam, TF_Optional<HRESULT>& ResultOpt)
            {
                _ProcessMessage(hwnd, uMsg, wParam, lParam, ResultOpt);
            }
        );
    }
    void F_WindowsIHIMouseManager::Release()
    {
        auto LLEvents = A_LLEvents::GetInstance().FastCast<F_WindowsLLEvents>();

        auto& WinAPIProcessMsgEvent = LLEvents->GetOnWinAPIProcessMsgEvent();
        WinAPIProcessMsgEvent.RemoveListener(_OnProcessMsgEvent);
        
        A_IHIMouseManager::Release();
    }

    void F_WindowsIHIMouseManager::OnUpdate()
    {
        A_IHIMouseManager::OnUpdate();
    }
    void F_WindowsIHIMouseManager::OnPostUpdate()
    {
        A_IHIMouseManager::OnPostUpdate();
    }

    void F_WindowsIHIMouseManager::_ProcessMessage(HWND WinAPIHandle, UINT UMsg, WPARAM WParam, LPARAM LParam, TF_Optional<HRESULT>& ResultOpt)
    {
        auto MainMouse = GetMainMouse();
        auto& NextState = MainMouse->InjectNextState();
        
        switch (UMsg)
        {
        case WM_LBUTTONDOWN:
            {
                F_IHIButton MouseButton;
                ABYTEK_FEEDBACK_STATUS_CHECK_HARD(
                    A_IHIButtonCoder::GetInstance()->Encode(E_IHIPredefinedButton::MOUSE_LEFT, MouseButton)  
                );
                if (NextState.PressedButtons.find(MouseButton) == NextState.PressedButtons.end())
                {
                    NextState.PressedButtons.insert(MouseButton);
                }
                break;
            }
        case WM_MBUTTONDOWN:
            {
                F_IHIButton MouseButton;
                ABYTEK_FEEDBACK_STATUS_CHECK_HARD(
                    A_IHIButtonCoder::GetInstance()->Encode(E_IHIPredefinedButton::MOUSE_MIDDLE, MouseButton)  
                );
                if (NextState.PressedButtons.find(MouseButton) == NextState.PressedButtons.end())
                {
                    NextState.PressedButtons.insert(MouseButton);
                }
                break;
            }
        case WM_RBUTTONDOWN:
            {
                F_IHIButton MouseButton;
                ABYTEK_FEEDBACK_STATUS_CHECK_HARD(
                    A_IHIButtonCoder::GetInstance()->Encode(E_IHIPredefinedButton::MOUSE_RIGHT, MouseButton)  
                );
                if (NextState.PressedButtons.find(MouseButton) == NextState.PressedButtons.end())
                {
                    NextState.PressedButtons.insert(MouseButton);
                }
                break;
            }
        case WM_XBUTTONDOWN:
            {
                if (WParam == XBUTTON1)
                {
                    F_IHIButton MouseButton;
                    ABYTEK_FEEDBACK_STATUS_CHECK_HARD(
                        A_IHIButtonCoder::GetInstance()->Encode(E_IHIPredefinedButton::MOUSE_X1, MouseButton)  
                    );
                    if (NextState.PressedButtons.find(MouseButton) == NextState.PressedButtons.end())
                    {
                        NextState.PressedButtons.insert(MouseButton);
                    }
                }
                else if (WParam == XBUTTON2)
                {
                    F_IHIButton MouseButton;
                    ABYTEK_FEEDBACK_STATUS_CHECK_HARD(
                        A_IHIButtonCoder::GetInstance()->Encode(E_IHIPredefinedButton::MOUSE_X2, MouseButton)  
                    );
                    if (NextState.PressedButtons.find(MouseButton) == NextState.PressedButtons.end())
                    {
                        NextState.PressedButtons.insert(MouseButton);
                    }
                }
                break;
            }

        case WM_LBUTTONUP:
            {
                F_IHIButton MouseButton;
                ABYTEK_FEEDBACK_STATUS_CHECK_HARD(
                    A_IHIButtonCoder::GetInstance()->Encode(E_IHIPredefinedButton::MOUSE_LEFT, MouseButton)  
                );
                if (NextState.PressedButtons.find(MouseButton) != NextState.PressedButtons.end())
                {
                    NextState.PressedButtons.erase(NextState.PressedButtons.find(MouseButton));
                }
                break;
            }
        case WM_MBUTTONUP:
            {
                F_IHIButton MouseButton;
                ABYTEK_FEEDBACK_STATUS_CHECK_HARD(
                    A_IHIButtonCoder::GetInstance()->Encode(E_IHIPredefinedButton::MOUSE_MIDDLE, MouseButton)  
                );
                if (NextState.PressedButtons.find(MouseButton) != NextState.PressedButtons.end())
                {
                    NextState.PressedButtons.erase(NextState.PressedButtons.find(MouseButton));
                }
                break;
            }
        case WM_RBUTTONUP:
            {
                F_IHIButton MouseButton;
                ABYTEK_FEEDBACK_STATUS_CHECK_HARD(
                    A_IHIButtonCoder::GetInstance()->Encode(E_IHIPredefinedButton::MOUSE_RIGHT, MouseButton)  
                );
                if (NextState.PressedButtons.find(MouseButton) != NextState.PressedButtons.end())
                {
                    NextState.PressedButtons.erase(NextState.PressedButtons.find(MouseButton));
                }
                break;
            }
        case WM_XBUTTONUP:
            {
                if (WParam == XBUTTON1)
                {
                    F_IHIButton MouseButton;
                    ABYTEK_FEEDBACK_STATUS_CHECK_HARD(
                        A_IHIButtonCoder::GetInstance()->Encode(E_IHIPredefinedButton::MOUSE_X1, MouseButton)  
                    );
                    if (NextState.PressedButtons.find(MouseButton) != NextState.PressedButtons.end())
                    {
                        NextState.PressedButtons.erase(NextState.PressedButtons.find(MouseButton));
                    }
                }
                else if (WParam == XBUTTON2)
                {
                    F_IHIButton MouseButton;
                    ABYTEK_FEEDBACK_STATUS_CHECK_HARD(
                        A_IHIButtonCoder::GetInstance()->Encode(E_IHIPredefinedButton::MOUSE_X2, MouseButton)  
                    );
                    if (NextState.PressedButtons.find(MouseButton) != NextState.PressedButtons.end())
                    {
                        NextState.PressedButtons.erase(NextState.PressedButtons.find(MouseButton));
                    }
                }
                break;
            }

        case WM_MOUSEWHEEL:
            {
                NextState.DeltaWheel += GET_WHEEL_DELTA_WPARAM(WParam);
                break;
            }

        case WM_INPUT:
            {
                RAWINPUT RawInput;
                U32 RawInputSize = sizeof(RAWINPUT);
                if(
                    GetRawInputData(
                        reinterpret_cast<HRAWINPUT>(LParam),
                        RID_INPUT,
                        &RawInput,
                        &RawInputSize,
                        sizeof(RAWINPUTHEADER)
                    ) == U32(-1)
                )
                {
                    ABYTEK_ENGINE_IHI_ASSERT(false) << "can't get raw input data";
                }
                if (RawInput.header.dwType == RIM_TYPEMOUSE)
                {
                    if(!(RawInput.data.mouse.usFlags & MOUSE_MOVE_ABSOLUTE))
                    {
                        F_Vector2_I32 DeltaPosition;
                        DeltaPosition.X = RawInput.data.mouse.lLastX;
                        DeltaPosition.Y = RawInput.data.mouse.lLastY;
                        NextState.DeltaPosition += DeltaPosition;
                    }
                }
                break;
            }

        };
    }
}
#endif