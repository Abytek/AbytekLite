#include "Abytek/PyString.hpp"


namespace Abytek
{
    F_PyString::F_PyString(const WithCrtAllocator::F_String& Str) :
        F_SharedPyObject(
            PyUnicode_DecodeFSDefault(Str.c_str())
        )
    {
    }
#ifdef ABYTEK_ENABLE_WIDE_CHAR_TEXT
    F_PyString::F_PyString(const WithCrtAllocator::F_Text& Text) :
        F_SharedPyObject(
            PyUnicode_FromWideChar(Text.c_str(), Text.length())
        )
    {
    }
#endif

    F_PyString F_PyString::Clone() const
    {
        ABYTEK_ENGINE_PLUGINS_PYTHON_ASSERT(*this) << "invalid py object";
        return F_PyString(
            PyUnicode_FromObject(_Handle)
        );
    }

    WithCrtAllocator::F_String F_PyString::GetValue() const
    {
        ABYTEK_ENGINE_PLUGINS_PYTHON_ASSERT(*this) << "invalid py object";
        return PyUnicode_AsUTF8(_Handle);
    }
}
