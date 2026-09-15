#include "Abytek/Windows/IHIButtonCoder.hpp"
#include "Abytek/IHISubsystem.hpp"


#ifdef ABYTEK_PLATFORM_WINDOWS
namespace Abytek
{
    void F_WindowsIHIButtonCoder::Build()
    {
        A_IHIButtonCoder::Build();
    }
    void F_WindowsIHIButtonCoder::Release()
    {
        A_IHIButtonCoder::Release();
    }

    F_FeedbackStatus F_WindowsIHIButtonCoder::Encode(E_IHIPredefinedButton PredefinedButton, F_IHIButton& OutButton)
    {
        F_IHIButton Button;
        switch (PredefinedButton)
        {
        case E_IHIPredefinedButton::NONE:
            Button.DeviceType = E_IHIButtonDeviceType::NONE;
            Button.Code = 0x0;
            break;
            
        case E_IHIPredefinedButton::A:
            Button.DeviceType = E_IHIButtonDeviceType::KEYBOARD;
            Button.Code = 0x41;
            break;
        case E_IHIPredefinedButton::B:
            Button.DeviceType = E_IHIButtonDeviceType::KEYBOARD;
            Button.Code = 0x42;
            break;
        case E_IHIPredefinedButton::C:
            Button.DeviceType = E_IHIButtonDeviceType::KEYBOARD;
            Button.Code = 0x43;
            break;
        case E_IHIPredefinedButton::D:
            Button.DeviceType = E_IHIButtonDeviceType::KEYBOARD;
            Button.Code = 0x44;
            break;
        case E_IHIPredefinedButton::E:
            Button.DeviceType = E_IHIButtonDeviceType::KEYBOARD;
            Button.Code = 0x45;
            break;
        case E_IHIPredefinedButton::F:
            Button.DeviceType = E_IHIButtonDeviceType::KEYBOARD;
            Button.Code = 0x46;
            break;
        case E_IHIPredefinedButton::G:
            Button.DeviceType = E_IHIButtonDeviceType::KEYBOARD;
            Button.Code = 0x47;
            break;
        case E_IHIPredefinedButton::H:
            Button.DeviceType = E_IHIButtonDeviceType::KEYBOARD;
            Button.Code = 0x48;
            break;
        case E_IHIPredefinedButton::I:
            Button.DeviceType = E_IHIButtonDeviceType::KEYBOARD;
            Button.Code = 0x49;
            break;
        case E_IHIPredefinedButton::J:
            Button.DeviceType = E_IHIButtonDeviceType::KEYBOARD;
            Button.Code = 0x4A;
            break;
        case E_IHIPredefinedButton::K:
            Button.DeviceType = E_IHIButtonDeviceType::KEYBOARD;
            Button.Code = 0x4B;
            break;
        case E_IHIPredefinedButton::L:
            Button.DeviceType = E_IHIButtonDeviceType::KEYBOARD;
            Button.Code = 0x4C;
            break;
        case E_IHIPredefinedButton::M:
            Button.DeviceType = E_IHIButtonDeviceType::KEYBOARD;
            Button.Code = 0x4D;
            break;
        case E_IHIPredefinedButton::N:
            Button.DeviceType = E_IHIButtonDeviceType::KEYBOARD;
            Button.Code = 0x4E;
            break;
        case E_IHIPredefinedButton::O:
            Button.DeviceType = E_IHIButtonDeviceType::KEYBOARD;
            Button.Code = 0x4F;
            break;
        case E_IHIPredefinedButton::P:
            Button.DeviceType = E_IHIButtonDeviceType::KEYBOARD;
            Button.Code = 0x50;
            break;
        case E_IHIPredefinedButton::Q:
            Button.DeviceType = E_IHIButtonDeviceType::KEYBOARD;
            Button.Code = 0x51;
            break;
        case E_IHIPredefinedButton::R:
            Button.DeviceType = E_IHIButtonDeviceType::KEYBOARD;
            Button.Code = 0x52;
            break;
        case E_IHIPredefinedButton::S:
            Button.DeviceType = E_IHIButtonDeviceType::KEYBOARD;
            Button.Code = 0x53;
            break;
        case E_IHIPredefinedButton::T:
            Button.DeviceType = E_IHIButtonDeviceType::KEYBOARD;
            Button.Code = 0x54;
            break;
        case E_IHIPredefinedButton::U:
            Button.DeviceType = E_IHIButtonDeviceType::KEYBOARD;
            Button.Code = 0x55;
            break;
        case E_IHIPredefinedButton::V:
            Button.DeviceType = E_IHIButtonDeviceType::KEYBOARD;
            Button.Code = 0x56;
            break;
        case E_IHIPredefinedButton::W:
            Button.DeviceType = E_IHIButtonDeviceType::KEYBOARD;
            Button.Code = 0x57;
            break;
        case E_IHIPredefinedButton::X:
            Button.DeviceType = E_IHIButtonDeviceType::KEYBOARD;
            Button.Code = 0x58;
            break;
        case E_IHIPredefinedButton::Y:
            Button.DeviceType = E_IHIButtonDeviceType::KEYBOARD;
            Button.Code = 0x59;
            break;
        case E_IHIPredefinedButton::Z:
            Button.DeviceType = E_IHIButtonDeviceType::KEYBOARD;
            Button.Code = 0x5A;
            break;
            
        case E_IHIPredefinedButton::NUM0:
            Button.DeviceType = E_IHIButtonDeviceType::KEYBOARD;
            Button.Code = 0x30;
            break;
        case E_IHIPredefinedButton::NUM1:
            Button.DeviceType = E_IHIButtonDeviceType::KEYBOARD;
            Button.Code = 0x31;
            break;
        case E_IHIPredefinedButton::NUM2:
            Button.DeviceType = E_IHIButtonDeviceType::KEYBOARD;
            Button.Code = 0x32;
            break;
        case E_IHIPredefinedButton::NUM3:
            Button.DeviceType = E_IHIButtonDeviceType::KEYBOARD;
            Button.Code = 0x33;
            break;
        case E_IHIPredefinedButton::NUM4:
            Button.DeviceType = E_IHIButtonDeviceType::KEYBOARD;
            Button.Code = 0x34;
            break;
        case E_IHIPredefinedButton::NUM5:
            Button.DeviceType = E_IHIButtonDeviceType::KEYBOARD;
            Button.Code = 0x35;
            break;
        case E_IHIPredefinedButton::NUM6:
            Button.DeviceType = E_IHIButtonDeviceType::KEYBOARD;
            Button.Code = 0x36;
            break;
        case E_IHIPredefinedButton::NUM7:
            Button.DeviceType = E_IHIButtonDeviceType::KEYBOARD;
            Button.Code = 0x37;
            break;
        case E_IHIPredefinedButton::NUM8:
            Button.DeviceType = E_IHIButtonDeviceType::KEYBOARD;
            Button.Code = 0x38;
            break;
        case E_IHIPredefinedButton::NUM9:
            Button.DeviceType = E_IHIButtonDeviceType::KEYBOARD;
            Button.Code = 0x39;
            break;
            
        case E_IHIPredefinedButton::NUMLOCK:
            Button.DeviceType = E_IHIButtonDeviceType::KEYBOARD;
            Button.Code = VK_NUMLOCK;
            break;
        case E_IHIPredefinedButton::NUMPAD0:
            Button.DeviceType = E_IHIButtonDeviceType::KEYBOARD;
            Button.Code = VK_NUMPAD0;
            break;
        case E_IHIPredefinedButton::NUMPAD1:
            Button.DeviceType = E_IHIButtonDeviceType::KEYBOARD;
            Button.Code = VK_NUMPAD1;
            break;
        case E_IHIPredefinedButton::NUMPAD2:
            Button.DeviceType = E_IHIButtonDeviceType::KEYBOARD;
            Button.Code = VK_NUMPAD2;
            break;
        case E_IHIPredefinedButton::NUMPAD3:
            Button.DeviceType = E_IHIButtonDeviceType::KEYBOARD;
            Button.Code = VK_NUMPAD3;
            break;
        case E_IHIPredefinedButton::NUMPAD4:
            Button.DeviceType = E_IHIButtonDeviceType::KEYBOARD;
            Button.Code = VK_NUMPAD4;
            break;
        case E_IHIPredefinedButton::NUMPAD5:
            Button.DeviceType = E_IHIButtonDeviceType::KEYBOARD;
            Button.Code = VK_NUMPAD5;
            break;
        case E_IHIPredefinedButton::NUMPAD6:
            Button.DeviceType = E_IHIButtonDeviceType::KEYBOARD;
            Button.Code = VK_NUMPAD6;
            break;
        case E_IHIPredefinedButton::NUMPAD7:
            Button.DeviceType = E_IHIButtonDeviceType::KEYBOARD;
            Button.Code = VK_NUMPAD7;
            break;
        case E_IHIPredefinedButton::NUMPAD8:
            Button.DeviceType = E_IHIButtonDeviceType::KEYBOARD;
            Button.Code = VK_NUMPAD8;
            break;
        case E_IHIPredefinedButton::NUMPAD9:
            Button.DeviceType = E_IHIButtonDeviceType::KEYBOARD;
            Button.Code = VK_NUMPAD9;
            break;
        case E_IHIPredefinedButton::MULTIPLY:
            Button.DeviceType = E_IHIButtonDeviceType::KEYBOARD;
            Button.Code = VK_MULTIPLY;
            break;
        case E_IHIPredefinedButton::ADD:
            Button.DeviceType = E_IHIButtonDeviceType::KEYBOARD;
            Button.Code = VK_ADD;
            break;
        case E_IHIPredefinedButton::SEPARATOR:
            Button.DeviceType = E_IHIButtonDeviceType::KEYBOARD;
            Button.Code = VK_SEPARATOR;
            break;
        case E_IHIPredefinedButton::SUBTRACT:
            Button.DeviceType = E_IHIButtonDeviceType::KEYBOARD;
            Button.Code = VK_SUBTRACT;
            break;
        case E_IHIPredefinedButton::DECIMAL:
            Button.DeviceType = E_IHIButtonDeviceType::KEYBOARD;
            Button.Code = VK_DECIMAL;
            break;
        case E_IHIPredefinedButton::DIVIDE:
            Button.DeviceType = E_IHIButtonDeviceType::KEYBOARD;
            Button.Code = VK_DIVIDE;
            break;
            
        case E_IHIPredefinedButton::F1:
            Button.DeviceType = E_IHIButtonDeviceType::KEYBOARD;
            Button.Code = VK_F1;
            break;
        case E_IHIPredefinedButton::F2:
            Button.DeviceType = E_IHIButtonDeviceType::KEYBOARD;
            Button.Code = VK_F2;
            break;
        case E_IHIPredefinedButton::F3:
            Button.DeviceType = E_IHIButtonDeviceType::KEYBOARD;
            Button.Code = VK_F3;
            break;
        case E_IHIPredefinedButton::F4:
            Button.DeviceType = E_IHIButtonDeviceType::KEYBOARD;
            Button.Code = VK_F4;
            break;
        case E_IHIPredefinedButton::F5:
            Button.DeviceType = E_IHIButtonDeviceType::KEYBOARD;
            Button.Code = VK_F5;
            break;
        case E_IHIPredefinedButton::F6:
            Button.DeviceType = E_IHIButtonDeviceType::KEYBOARD;
            Button.Code = VK_F6;
            break;
        case E_IHIPredefinedButton::F7:
            Button.DeviceType = E_IHIButtonDeviceType::KEYBOARD;
            Button.Code = VK_F7;
            break;
        case E_IHIPredefinedButton::F8:
            Button.DeviceType = E_IHIButtonDeviceType::KEYBOARD;
            Button.Code = VK_F8;
            break;
        case E_IHIPredefinedButton::F9:
            Button.DeviceType = E_IHIButtonDeviceType::KEYBOARD;
            Button.Code = VK_F9;
            break;
        case E_IHIPredefinedButton::F10:
            Button.DeviceType = E_IHIButtonDeviceType::KEYBOARD;
            Button.Code = VK_F10;
            break;
        case E_IHIPredefinedButton::F11:
            Button.DeviceType = E_IHIButtonDeviceType::KEYBOARD;
            Button.Code = VK_F11;
            break;
        case E_IHIPredefinedButton::F12:
            Button.DeviceType = E_IHIButtonDeviceType::KEYBOARD;
            Button.Code = VK_F12;
            break;
            
        case E_IHIPredefinedButton::ESCAPE:
            Button.DeviceType = E_IHIButtonDeviceType::KEYBOARD;
            Button.Code = VK_ESCAPE;
            break;
        case E_IHIPredefinedButton::TAB:
            Button.DeviceType = E_IHIButtonDeviceType::KEYBOARD;
            Button.Code = VK_TAB;
            break;
        case E_IHIPredefinedButton::CAPSLOCK:
            Button.DeviceType = E_IHIButtonDeviceType::KEYBOARD;
            Button.Code = VK_CAPITAL;
            break;
        case E_IHIPredefinedButton::SHIFT:
            Button.DeviceType = E_IHIButtonDeviceType::KEYBOARD;
            Button.Code = VK_SHIFT;
            break;
        case E_IHIPredefinedButton::CONTROL:
            Button.DeviceType = E_IHIButtonDeviceType::KEYBOARD;
            Button.Code = VK_CONTROL;
            break;
        case E_IHIPredefinedButton::ALT:
            Button.DeviceType = E_IHIButtonDeviceType::KEYBOARD;
            Button.Code = VK_MENU;
            break;
        case E_IHIPredefinedButton::SPACE:
            Button.DeviceType = E_IHIButtonDeviceType::KEYBOARD;
            Button.Code = VK_SPACE;
            break;
        case E_IHIPredefinedButton::ENTER:
            Button.DeviceType = E_IHIButtonDeviceType::KEYBOARD;
            Button.Code = VK_RETURN;
            break;
        case E_IHIPredefinedButton::BACKSPACE:
            Button.DeviceType = E_IHIButtonDeviceType::KEYBOARD;
            Button.Code = VK_BACK;
            break;
            
        case E_IHIPredefinedButton::LEFT:
            Button.DeviceType = E_IHIButtonDeviceType::KEYBOARD;
            Button.Code = VK_LEFT;
            break;
        case E_IHIPredefinedButton::UP:
            Button.DeviceType = E_IHIButtonDeviceType::KEYBOARD;
            Button.Code = VK_UP;
            break;
        case E_IHIPredefinedButton::RIGHT:
            Button.DeviceType = E_IHIButtonDeviceType::KEYBOARD;
            Button.Code = VK_RIGHT;
            break;
        case E_IHIPredefinedButton::DOWN:
            Button.DeviceType = E_IHIButtonDeviceType::KEYBOARD;
            Button.Code = VK_DOWN;
            break;
            
        case E_IHIPredefinedButton::INSERT:
            Button.DeviceType = E_IHIButtonDeviceType::KEYBOARD;
            Button.Code = VK_INSERT;
            break;
        case E_IHIPredefinedButton::DELETE_:
            Button.DeviceType = E_IHIButtonDeviceType::KEYBOARD;
            Button.Code = VK_DELETE;
            break;
        case E_IHIPredefinedButton::HOME:
            Button.DeviceType = E_IHIButtonDeviceType::KEYBOARD;
            Button.Code = VK_HOME;
            break;
        case E_IHIPredefinedButton::END:
            Button.DeviceType = E_IHIButtonDeviceType::KEYBOARD;
            Button.Code = VK_END;
            break;
        case E_IHIPredefinedButton::PAGE_UP:
            Button.DeviceType = E_IHIButtonDeviceType::KEYBOARD;
            Button.Code = VK_PRIOR;
            break;
        case E_IHIPredefinedButton::PAGE_DOWN:
            Button.DeviceType = E_IHIButtonDeviceType::KEYBOARD;
            Button.Code = VK_NEXT;
            break;
            
        // Mouse
        case E_IHIPredefinedButton::MOUSE_LEFT:
            Button.DeviceType = E_IHIButtonDeviceType::MOUSE;
            Button.Code = VK_LBUTTON;
            break;
        case E_IHIPredefinedButton::MOUSE_MIDDLE:
            Button.DeviceType = E_IHIButtonDeviceType::MOUSE;
            Button.Code = VK_MBUTTON;
            break;
        case E_IHIPredefinedButton::MOUSE_RIGHT:
            Button.DeviceType = E_IHIButtonDeviceType::MOUSE;
            Button.Code = VK_RBUTTON;
            break;
        case E_IHIPredefinedButton::MOUSE_X1:
            Button.DeviceType = E_IHIButtonDeviceType::MOUSE;
            Button.Code = VK_XBUTTON1;
            break;
        case E_IHIPredefinedButton::MOUSE_X2:
            Button.DeviceType = E_IHIButtonDeviceType::MOUSE;
            Button.Code = VK_XBUTTON2;
            break;
            
        // Failed
        default:
            return F_FeedbackStatus::MakeFailed(ABYTEK_TEXT("Invalid predefined button: ") + ToText(static_cast<U32>(PredefinedButton)));
        }
        OutButton = Button;
        return F_FeedbackStatus::MakeSucceeded();
    }
    F_FeedbackStatus F_WindowsIHIButtonCoder::Decode(E_IHIPredefinedButton& OutPredefinedButton, const F_IHIButton& Button)
    {
        switch (Button.Code)
        {
        case 0x0:
            OutPredefinedButton = E_IHIPredefinedButton::NONE;
            break;
            
        case 0x41:
            OutPredefinedButton = E_IHIPredefinedButton::A;
            break;
        case 0x42:
            OutPredefinedButton = E_IHIPredefinedButton::B;
            break;
        case 0x43:
            OutPredefinedButton = E_IHIPredefinedButton::C;
            break;
        case 0x44:
            OutPredefinedButton = E_IHIPredefinedButton::D;
            break;
        case 0x45:
            OutPredefinedButton = E_IHIPredefinedButton::E;
            break;
        case 0x46:
            OutPredefinedButton = E_IHIPredefinedButton::F;
            break;
        case 0x47:
            OutPredefinedButton = E_IHIPredefinedButton::G;
            break;
        case 0x48:
            OutPredefinedButton = E_IHIPredefinedButton::H;
            break;
        case 0x49:
            OutPredefinedButton = E_IHIPredefinedButton::I;
            break;
        case 0x4A:
            OutPredefinedButton = E_IHIPredefinedButton::J;
            break;
        case 0x4B:
            OutPredefinedButton = E_IHIPredefinedButton::K;
            break;
        case 0x4C:
            OutPredefinedButton = E_IHIPredefinedButton::L;
            break;
        case 0x4D:
            OutPredefinedButton = E_IHIPredefinedButton::M;
            break;
        case 0x4E:
            OutPredefinedButton = E_IHIPredefinedButton::N;
            break;
        case 0x4F:
            OutPredefinedButton = E_IHIPredefinedButton::O;
            break;
        case 0x50:
            OutPredefinedButton = E_IHIPredefinedButton::P;
            break;
        case 0x51:
            OutPredefinedButton = E_IHIPredefinedButton::Q;
            break;
        case 0x52:
            OutPredefinedButton = E_IHIPredefinedButton::R;
            break;
        case 0x53:
            OutPredefinedButton = E_IHIPredefinedButton::S;
            break;
        case 0x54:
            OutPredefinedButton = E_IHIPredefinedButton::T;
            break;
        case 0x55:
            OutPredefinedButton = E_IHIPredefinedButton::U;
            break;
        case 0x56:
            OutPredefinedButton = E_IHIPredefinedButton::V;
            break;
        case 0x57:
            OutPredefinedButton = E_IHIPredefinedButton::W;
            break;
        case 0x58:
            OutPredefinedButton = E_IHIPredefinedButton::X;
            break;
        case 0x59:
            OutPredefinedButton = E_IHIPredefinedButton::Y;
            break;
        case 0x5A:
            OutPredefinedButton = E_IHIPredefinedButton::Z;
            break;
            
        case 0x30:
            OutPredefinedButton = E_IHIPredefinedButton::NUM0;
            break;
        case 0x31:
            OutPredefinedButton = E_IHIPredefinedButton::NUM1;
            break;
        case 0x32:
            OutPredefinedButton = E_IHIPredefinedButton::NUM2;
            break;
        case 0x33:
            OutPredefinedButton = E_IHIPredefinedButton::NUM3;
            break;
        case 0x34:
            OutPredefinedButton = E_IHIPredefinedButton::NUM4;
            break;
        case 0x35:
            OutPredefinedButton = E_IHIPredefinedButton::NUM5;
            break;
        case 0x36:
            OutPredefinedButton = E_IHIPredefinedButton::NUM6;
            break;
        case 0x37:
            OutPredefinedButton = E_IHIPredefinedButton::NUM7;
            break;
        case 0x38:
            OutPredefinedButton = E_IHIPredefinedButton::NUM8;
            break;
        case 0x39:
            OutPredefinedButton = E_IHIPredefinedButton::NUM9;
            break;
            
        case VK_NUMLOCK:
            OutPredefinedButton = E_IHIPredefinedButton::NUMLOCK;
            break;
        case VK_NUMPAD0:
            OutPredefinedButton = E_IHIPredefinedButton::NUMPAD0;
            break;
        case VK_NUMPAD1:
            OutPredefinedButton = E_IHIPredefinedButton::NUMPAD1;
            break;
        case VK_NUMPAD2:
            OutPredefinedButton = E_IHIPredefinedButton::NUMPAD2;
            break;
        case VK_NUMPAD3:
            OutPredefinedButton = E_IHIPredefinedButton::NUMPAD3;
            break;
        case VK_NUMPAD4:
            OutPredefinedButton = E_IHIPredefinedButton::NUMPAD4;
            break;
        case VK_NUMPAD5:
            OutPredefinedButton = E_IHIPredefinedButton::NUMPAD5;
            break;
        case VK_NUMPAD6:
            OutPredefinedButton = E_IHIPredefinedButton::NUMPAD6;
            break;
        case VK_NUMPAD7:
            OutPredefinedButton = E_IHIPredefinedButton::NUMPAD7;
            break;
        case VK_NUMPAD8:
            OutPredefinedButton = E_IHIPredefinedButton::NUMPAD8;
            break;
        case VK_NUMPAD9:
            OutPredefinedButton = E_IHIPredefinedButton::NUMPAD9;
            break;
        case VK_MULTIPLY:
            OutPredefinedButton = E_IHIPredefinedButton::MULTIPLY;
            break;
        case VK_ADD:
            OutPredefinedButton = E_IHIPredefinedButton::ADD;
            break;
        case VK_SEPARATOR:
            OutPredefinedButton = E_IHIPredefinedButton::SEPARATOR;
            break;
        case VK_SUBTRACT:
            OutPredefinedButton = E_IHIPredefinedButton::SUBTRACT;
            break;
        case VK_DECIMAL:
            OutPredefinedButton = E_IHIPredefinedButton::DECIMAL;
            break;
        case VK_DIVIDE:
            OutPredefinedButton = E_IHIPredefinedButton::DIVIDE;
            break;
            
        case VK_F1:
            OutPredefinedButton = E_IHIPredefinedButton::F1;
            break;
        case VK_F2:
            OutPredefinedButton = E_IHIPredefinedButton::F2;
            break;
        case VK_F3:
            OutPredefinedButton = E_IHIPredefinedButton::F3;
            break;
        case VK_F4:
            OutPredefinedButton = E_IHIPredefinedButton::F4;
            break;
        case VK_F5:
            OutPredefinedButton = E_IHIPredefinedButton::F5;
            break;
        case VK_F6:
            OutPredefinedButton = E_IHIPredefinedButton::F6;
            break;
        case VK_F7:
            OutPredefinedButton = E_IHIPredefinedButton::F7;
            break;
        case VK_F8:
            OutPredefinedButton = E_IHIPredefinedButton::F8;
            break;
        case VK_F9:
            OutPredefinedButton = E_IHIPredefinedButton::F9;
            break;
        case VK_F10:
            OutPredefinedButton = E_IHIPredefinedButton::F10;
            break;
        case VK_F11:
            OutPredefinedButton = E_IHIPredefinedButton::F11;
            break;
        case VK_F12:
            OutPredefinedButton = E_IHIPredefinedButton::F12;
            break;
            
        case VK_ESCAPE:
            OutPredefinedButton = E_IHIPredefinedButton::ESCAPE;
            break;
        case VK_TAB:
            OutPredefinedButton = E_IHIPredefinedButton::TAB;
            break;
        case VK_CAPITAL:
            OutPredefinedButton = E_IHIPredefinedButton::CAPSLOCK;
            break;
        case VK_SHIFT:
            OutPredefinedButton = E_IHIPredefinedButton::SHIFT;
            break;
        case VK_CONTROL:
            OutPredefinedButton = E_IHIPredefinedButton::CONTROL;
            break;
        case VK_MENU:
            OutPredefinedButton = E_IHIPredefinedButton::ALT;
            break;
        case VK_SPACE:
            OutPredefinedButton = E_IHIPredefinedButton::SPACE;
            break;
        case VK_RETURN:
            OutPredefinedButton = E_IHIPredefinedButton::ENTER;
            break;
        case VK_BACK:
            OutPredefinedButton = E_IHIPredefinedButton::BACKSPACE;
            break;
            
        case VK_LEFT:
            OutPredefinedButton = E_IHIPredefinedButton::LEFT;
            break;
        case VK_UP:
            OutPredefinedButton = E_IHIPredefinedButton::UP;
            break;
        case VK_RIGHT:
            OutPredefinedButton = E_IHIPredefinedButton::RIGHT;
            break;
        case VK_DOWN:
            OutPredefinedButton = E_IHIPredefinedButton::DOWN;
            break;
            
        case VK_INSERT:
            OutPredefinedButton = E_IHIPredefinedButton::INSERT;
            break;
        case VK_DELETE:
            OutPredefinedButton = E_IHIPredefinedButton::DELETE_;
            break;
        case VK_HOME:
            OutPredefinedButton = E_IHIPredefinedButton::HOME;
            break;
        case VK_END:
            OutPredefinedButton = E_IHIPredefinedButton::END;
            break;
        case VK_PRIOR:
            OutPredefinedButton = E_IHIPredefinedButton::PAGE_UP;
            break;
        case VK_NEXT:
            OutPredefinedButton = E_IHIPredefinedButton::PAGE_DOWN;
            break;
            
            // Mouse
        case VK_LBUTTON:
            OutPredefinedButton = E_IHIPredefinedButton::MOUSE_LEFT;
            break;
        case VK_MBUTTON:
            OutPredefinedButton = E_IHIPredefinedButton::MOUSE_MIDDLE;
            break;
        case VK_RBUTTON:
            OutPredefinedButton = E_IHIPredefinedButton::MOUSE_RIGHT;
            break;
        case VK_XBUTTON1:
            OutPredefinedButton = E_IHIPredefinedButton::MOUSE_X1;
            break;
        case VK_XBUTTON2:
            OutPredefinedButton = E_IHIPredefinedButton::MOUSE_X2;
            break;
            
            // Failed
        default:
            return F_FeedbackStatus::MakeFailed(ABYTEK_TEXT("Unknown button code: ") + ToText(Button.Code));
        }
        OutPredefinedButton = E_IHIPredefinedButton::NONE;
        return F_FeedbackStatus::MakeSucceeded();
    }
}
#endif