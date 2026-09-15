#pragma once

#include "Python.prerequisites.pch.hpp"
#include "SharedPyObject.hpp"


namespace Abytek
{
    class ABYTEK_BASE_PLATFORM_API F_PyString : public F_SharedPyObject
    {
        ABYTEK_DERIVED_SHARED_PY_OBJECT_BODY(F_PyString);

    public:
        F_PyString(const WithCrtAllocator::F_String& Str);
#ifdef ABYTEK_ENABLE_WIDE_CHAR_TEXT
        F_PyString(const WithCrtAllocator::F_Text& Text);
#endif

    public:
        F_PyString Clone() const;

        WithCrtAllocator::F_String GetValue() const;
        ABYTEK_FORCE_INLINE WithCrtAllocator::F_String operator * () const
        {
            return GetValue();
        }

        friend F_PyString operator + (const F_PyString& A, const F_PyString& B)
        {
            return A.GetValue() + B.GetValue();
        }
        F_PyString& operator += (const F_PyString& B)
        {
            *this = F_PyString { GetValue() + B.GetValue() };
            return *this;
        }
        F_PyString& operator + ()
        {
            return *this;
        }
    };
}