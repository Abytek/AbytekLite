#include "Abytek/Streamable.hpp"


namespace Abytek
{
    B8 I_Streamable::Ate()
    {     
        if(auto SizeOpt = GetSize())
        {
            return Seek(*SizeOpt);
        }
        return false;
    }

    B8 I_Streamable::ReadBinaryExact(const TF_Span<U8>& OutData)
    {
        if (auto SizeOpt = ReadBinary(OutData))
        {
            return OutData.size() == *SizeOpt;
        }
        return false;
    }
    B8 I_Streamable::WriteBinaryExact(const TF_Span<U8>& Data)
    {
        if (auto SizeOpt = WriteBinary(Data))
        {
            return Data.size() == *SizeOpt;
        }
        return false;
    }

    TF_Optional<F_String> I_Streamable::ReadStringExclusive()
    {
        if (!Seek(0))
        {
            return {};
        }
        
        F_String Data;
        if (auto SizeOpt = GetSize())
        {
            Data.resize(*SizeOpt);
            if (
                ReadBinary(
                    {
                        (U8*)Data.data(),
                        Data.size()
                    }
                )
            )
            {
                return boost::move(Data);
            }
        }
        return {};
    }
    B8 I_Streamable::WriteStringExclusive(const F_String& Data)
    {
        if (!Seek(0))
        {
            return {};
        }
        return WriteString(Data);
    }
    B8 I_Streamable::WriteString(const F_String& Data)
    {
        return static_cast<B8>(
            WriteBinary(
                {
                    (U8*)Data.data(),
                    Data.length()
                }
            )
        );
    }
    B8 I_Streamable::WriteChar(const F_Char& Char)
    {
        return static_cast<B8>(
            WriteBinary(
                {
                    (U8*)&Char,
                    sizeof(F_Char)
                }
            )
        );
    }
    TF_Optional<F_Text> I_Streamable::ReadTextExclusive()
    {
        if (!Seek(0))
        {
            return {};
        }
        
        F_String Data;
        if (auto SizeOpt = GetSize())
        {
            Data.resize(*SizeOpt);
            if (
                ReadBinary(
                    {
                        (U8*)Data.data(),
                        Data.size()
                    }
                )
            )
            {
                return ToText(Data);
            }
        }
        return {};
    }
    B8 I_Streamable::WriteTextExclusive(const F_Text& Text)
    {
        if (!Seek(0))
        {
            return {};
        }
        return WriteText(Text);
    }
    B8 I_Streamable::WriteText(const F_Text& Text)
    {
        F_String UTF8Data = ToString(Text);
        
        return static_cast<B8>(
            WriteBinary(
                {
                    (U8*)UTF8Data.data(),
                    UTF8Data.size()
                }
            )
        );
    }
    B8 I_Streamable::WriteTextChar(const F_TextChar& TextChar)
    {
        F_String UTF8Data = ToString(TextChar);
        
        return static_cast<B8>(
            WriteBinary(
                {
                    (U8*)UTF8Data.data(),
                    UTF8Data.size()
                }
            )
        );
    }
}
