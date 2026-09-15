#pragma once

#include "Abytek/Engine.IHI.prerequisites.hpp"


namespace Abytek
{
    enum class E_IHIPredefinedButton : U8
    {
        NONE,
        
        A,
        B,
        C,
        D,
        E,
        F,
        G,
        H,
        I,
        J,
        K,
        L,
        M,
        N,
        O,
        P,
        Q,
        R,
        S,
        T,
        U,
        V,
        W,
        X,
        Y,
        Z,

        NUM0,
        NUM1,
        NUM2,
        NUM3,
        NUM4,
        NUM5,
        NUM6,
        NUM7,
        NUM8,
        NUM9,
        
        // NUMpad keys
        NUMLOCK,
        NUMPAD0,
        NUMPAD1,
        NUMPAD2,
        NUMPAD3,
        NUMPAD4,
        NUMPAD5,
        NUMPAD6,
        NUMPAD7,
        NUMPAD8,
        NUMPAD9,
        MULTIPLY,
        ADD,
        SEPARATOR,
        SUBTRACT,
        DECIMAL,
        DIVIDE,

        F1,
        F2,
        F3,
        F4,
        F5,
        F6,
        F7,
        F8,
        F9,
        F10,
        F11,
        F12,

        ESCAPE,
        TAB,
        CAPSLOCK,
        SHIFT,
        CONTROL,
        ALT,
        SPACE,
        ENTER,
        BACKSPACE,

        LEFT,
        UP,
        RIGHT,
        DOWN,

        INSERT,
        DELETE_,
        HOME,
        END,
        PAGE_UP,
        PAGE_DOWN,
        
        // Mouse
        MOUSE_LEFT,
        MOUSE_MIDDLE,
        MOUSE_RIGHT,
        MOUSE_X1,
        MOUSE_X2,
        
        NUM
    };
    static_assert(static_cast<U32>(E_IHIPredefinedButton::NUM) <= ~U8(0));
    ABYTEK_ENUM_REFLECTOR_LOCALNS(E_IHIPredefinedButton)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::E_IHIPredefinedButton"));        
        
        ABYTEK_REFLECT_ENUM_VALUE(NONE);
        
        ABYTEK_REFLECT_ENUM_VALUE(A);
        ABYTEK_REFLECT_ENUM_VALUE(B);
        ABYTEK_REFLECT_ENUM_VALUE(C);
        ABYTEK_REFLECT_ENUM_VALUE(D);
        ABYTEK_REFLECT_ENUM_VALUE(E);
        ABYTEK_REFLECT_ENUM_VALUE(F);
        ABYTEK_REFLECT_ENUM_VALUE(G);
        ABYTEK_REFLECT_ENUM_VALUE(H);
        ABYTEK_REFLECT_ENUM_VALUE(I);
        ABYTEK_REFLECT_ENUM_VALUE(J);
        ABYTEK_REFLECT_ENUM_VALUE(K);
        ABYTEK_REFLECT_ENUM_VALUE(L);
        ABYTEK_REFLECT_ENUM_VALUE(M);
        ABYTEK_REFLECT_ENUM_VALUE(N);
        ABYTEK_REFLECT_ENUM_VALUE(O);
        ABYTEK_REFLECT_ENUM_VALUE(P);
        ABYTEK_REFLECT_ENUM_VALUE(Q);
        ABYTEK_REFLECT_ENUM_VALUE(R);
        ABYTEK_REFLECT_ENUM_VALUE(S);
        ABYTEK_REFLECT_ENUM_VALUE(T);
        ABYTEK_REFLECT_ENUM_VALUE(U);
        ABYTEK_REFLECT_ENUM_VALUE(V);
        ABYTEK_REFLECT_ENUM_VALUE(W);
        ABYTEK_REFLECT_ENUM_VALUE(X);
        ABYTEK_REFLECT_ENUM_VALUE(Y);
        ABYTEK_REFLECT_ENUM_VALUE(Z);

        ABYTEK_REFLECT_ENUM_VALUE(NUM0);
        ABYTEK_REFLECT_ENUM_VALUE(NUM1);
        ABYTEK_REFLECT_ENUM_VALUE(NUM2);
        ABYTEK_REFLECT_ENUM_VALUE(NUM3);
        ABYTEK_REFLECT_ENUM_VALUE(NUM4);
        ABYTEK_REFLECT_ENUM_VALUE(NUM5);
        ABYTEK_REFLECT_ENUM_VALUE(NUM6);
        ABYTEK_REFLECT_ENUM_VALUE(NUM7);
        ABYTEK_REFLECT_ENUM_VALUE(NUM8);
        ABYTEK_REFLECT_ENUM_VALUE(NUM9);
        
        // NUMpad keys
        ABYTEK_REFLECT_ENUM_VALUE(NUMLOCK);
        ABYTEK_REFLECT_ENUM_VALUE(NUMPAD0);
        ABYTEK_REFLECT_ENUM_VALUE(NUMPAD1);
        ABYTEK_REFLECT_ENUM_VALUE(NUMPAD2);
        ABYTEK_REFLECT_ENUM_VALUE(NUMPAD3);
        ABYTEK_REFLECT_ENUM_VALUE(NUMPAD4);
        ABYTEK_REFLECT_ENUM_VALUE(NUMPAD5);
        ABYTEK_REFLECT_ENUM_VALUE(NUMPAD6);
        ABYTEK_REFLECT_ENUM_VALUE(NUMPAD7);
        ABYTEK_REFLECT_ENUM_VALUE(NUMPAD8);
        ABYTEK_REFLECT_ENUM_VALUE(NUMPAD9);
        ABYTEK_REFLECT_ENUM_VALUE(MULTIPLY);
        ABYTEK_REFLECT_ENUM_VALUE(ADD);
        ABYTEK_REFLECT_ENUM_VALUE(SEPARATOR);
        ABYTEK_REFLECT_ENUM_VALUE(SUBTRACT);
        ABYTEK_REFLECT_ENUM_VALUE(DECIMAL);
        ABYTEK_REFLECT_ENUM_VALUE(DIVIDE);

        ABYTEK_REFLECT_ENUM_VALUE(F1);
        ABYTEK_REFLECT_ENUM_VALUE(F2);
        ABYTEK_REFLECT_ENUM_VALUE(F3);
        ABYTEK_REFLECT_ENUM_VALUE(F4);
        ABYTEK_REFLECT_ENUM_VALUE(F5);
        ABYTEK_REFLECT_ENUM_VALUE(F6);
        ABYTEK_REFLECT_ENUM_VALUE(F7);
        ABYTEK_REFLECT_ENUM_VALUE(F8);
        ABYTEK_REFLECT_ENUM_VALUE(F9);
        ABYTEK_REFLECT_ENUM_VALUE(F10);
        ABYTEK_REFLECT_ENUM_VALUE(F11);
        ABYTEK_REFLECT_ENUM_VALUE(F12);

        ABYTEK_REFLECT_ENUM_VALUE(ESCAPE);
        ABYTEK_REFLECT_ENUM_VALUE(TAB);
        ABYTEK_REFLECT_ENUM_VALUE(CAPSLOCK);
        ABYTEK_REFLECT_ENUM_VALUE(SHIFT);
        ABYTEK_REFLECT_ENUM_VALUE(CONTROL);
        ABYTEK_REFLECT_ENUM_VALUE(ALT);
        ABYTEK_REFLECT_ENUM_VALUE(SPACE);
        ABYTEK_REFLECT_ENUM_VALUE(ENTER);
        ABYTEK_REFLECT_ENUM_VALUE(BACKSPACE);

        ABYTEK_REFLECT_ENUM_VALUE(LEFT);
        ABYTEK_REFLECT_ENUM_VALUE(UP);
        ABYTEK_REFLECT_ENUM_VALUE(RIGHT);
        ABYTEK_REFLECT_ENUM_VALUE(DOWN);

        ABYTEK_REFLECT_ENUM_VALUE(INSERT);
        ABYTEK_REFLECT_ENUM_VALUE(DELETE_);
        ABYTEK_REFLECT_ENUM_VALUE(HOME);
        ABYTEK_REFLECT_ENUM_VALUE(END);
        ABYTEK_REFLECT_ENUM_VALUE(PAGE_UP);
        ABYTEK_REFLECT_ENUM_VALUE(PAGE_DOWN);
        
        // Mouse
        ABYTEK_REFLECT_ENUM_VALUE(MOUSE_LEFT);
        ABYTEK_REFLECT_ENUM_VALUE(MOUSE_MIDDLE);
        ABYTEK_REFLECT_ENUM_VALUE(MOUSE_RIGHT);
        ABYTEK_REFLECT_ENUM_VALUE(MOUSE_X1);
        ABYTEK_REFLECT_ENUM_VALUE(MOUSE_X2);
        
        ABYTEK_REFLECT_ENUM_VALUE(NUM);
    }
    using F_IHIPredefinedButtonIndex = TF_UInt<sizeof(E_IHIPredefinedButton)>;
    using F_IHIButtonCode = U32;
    
    enum class E_IHIButtonDeviceType : U8
    {
        NONE,
        MOUSE,
        KEYBOARD
    };
    ABYTEK_ENUM_REFLECTOR_LOCALNS(E_IHIButtonDeviceType)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::E_IHIButtonDeviceType"));
        
        ABYTEK_REFLECT_ENUM_VALUE(NONE);
        ABYTEK_REFLECT_ENUM_VALUE(MOUSE);
        ABYTEK_REFLECT_ENUM_VALUE(KEYBOARD);
    }
    using F_IHIButtonDeviceTypeIndex = TF_UInt<sizeof(E_IHIButtonDeviceType)>;
    
    struct ABYTEK_ENGINE_IHI_API F_IHIButton
    {
        ABYTEK_BEGIN_REFLECTOR()
        ABYTEK_END_REFLECTOR(F_IHIButton)
        
        E_IHIButtonDeviceType DeviceType = E_IHIButtonDeviceType::NONE;
        F_IHIButtonCode Code = 0;
        
        F_IHIButton() = default;
        F_IHIButton(E_IHIPredefinedButton Predefined);
        F_IHIButton(const F_IHIButton&) = default;
        F_IHIButton& operator = (const F_IHIButton&) = default;
        F_IHIButton(F_IHIButton&&) = default;
        F_IHIButton& operator = (F_IHIButton&&) = default;
        
        ABYTEK_FORCE_INLINE B8 IsValid() const noexcept
        {
            return DeviceType != E_IHIButtonDeviceType::NONE;
        }
        ABYTEK_FORCE_INLINE explicit operator B8 () const noexcept
        {
            return IsValid();
        }
        
        friend B8 operator < (const F_IHIButton& A, const F_IHIButton& B) noexcept
        {
            if (A.DeviceType != B.DeviceType)
            {
                return A.DeviceType < B.DeviceType;
            }
            return A.Code < B.Code;
        }
        friend B8 operator > (const F_IHIButton& A, const F_IHIButton& B) noexcept
        {
            if (A.DeviceType != B.DeviceType)
            {
                return A.DeviceType > B.DeviceType;
            }
            return A.Code > B.Code;
        }
        
        friend B8 operator == (const F_IHIButton& A, const F_IHIButton& B) noexcept
        {
            return (
                (A.DeviceType == B.DeviceType)    
                && (A.Code == B.Code)    
            );
        }
        friend B8 operator != (const F_IHIButton& A, const F_IHIButton& B) noexcept
        {
            return (
                (A.DeviceType != B.DeviceType)
                || (A.Code != B.Code)    
            );
        }
        
        friend F_FeedbackStatus operator << (F_ArchiveReadWriteView& View, const F_IHIButton& Value);
        friend F_FeedbackStatus operator >> (F_ArchiveReadOnlyView& View, F_IHIButton& Value);
    };
    
    template<>
    struct ABYTEK_ENGINE_IHI_API TH_JSONConvert<F_IHIButton>
    {
        static F_FeedbackStatus ValueToBoostJSONValue(const F_IHIButton& Value, boost::json::value& JSONValue, const F_JSONOptions& Options);
        static F_FeedbackStatus BoostJSONValueToValue(F_IHIButton& Value, const boost::json::value& JSONValue, const F_JSONOptions& Options);
    };
}

namespace boost
{
    template<>
    struct hash<Abytek::F_IHIButton>
    {
        size_t operator()(const Abytek::F_IHIButton& X) const
        {
            size_t Result = 0;
            hash_combine(Result, X.DeviceType);
            hash_combine(Result, X.Code);
            return Result;
        }
    };
}