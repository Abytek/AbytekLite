#include "Abytek/TextLogStream.hpp"
#include "Abytek/Assert.hpp"



namespace Abytek
{
    A_TextLogStream::A_TextLogStream()
    {
    }
    A_TextLogStream::~A_TextLogStream()
    {
    }

    void A_TextLogStream::AddNewLine() 
    {
        _Lock.Lock();
        _TextStream << std::endl;
        _Lock.Unlock();
    }
    void A_TextLogStream::AddU8(U8 Value) 
    {
        _Lock.Lock();
        _TextStream << Value;
        _Lock.Unlock();
    }
    void A_TextLogStream::AddU16(U16 Value) 
    {
        _Lock.Lock();
        _TextStream << Value;
        _Lock.Unlock();
    }
    void A_TextLogStream::AddU32(U32 Value) 
    {
        _Lock.Lock();
        _TextStream << Value;
        _Lock.Unlock();
    }
    void A_TextLogStream::AddU64(U64 Value) 
    {
        _Lock.Lock();
        _TextStream << Value;
        _Lock.Unlock();
    }
    void A_TextLogStream::AddI8(I8 Value) 
    {
        _Lock.Lock();
        _TextStream << Value;
        _Lock.Unlock();
    }
    void A_TextLogStream::AddI16(I16 Value) 
    {
        _Lock.Lock();
        _TextStream << Value;
        _Lock.Unlock();
    }
    void A_TextLogStream::AddI32(I32 Value) 
    {
        _Lock.Lock();
        _TextStream << Value;
        _Lock.Unlock();
    }
    void A_TextLogStream::AddI64(I64 Value) 
    {
        _Lock.Lock();
        _TextStream << Value;
        _Lock.Unlock();
    }
    void A_TextLogStream::AddVoidP(const void* Value) 
    {
        _Lock.Lock();
        _TextStream << Value;
        _Lock.Unlock();
    }
    void A_TextLogStream::AddSz(Sz Value) 
    {
        _Lock.Lock();
        _TextStream << Value;
        _Lock.Unlock();
    }
    void A_TextLogStream::AddPDiff(PDiff Value) 
    {
        _Lock.Lock();
        _TextStream << Value;
        _Lock.Unlock();
    }
    void A_TextLogStream::AddF32(F32 Value) 
    {
        _Lock.Lock();
        _TextStream << Value;
        _Lock.Unlock();
    }
    void A_TextLogStream::AddF64(F64 Value) 
    {
        _Lock.Lock();
        _TextStream << Value;
        _Lock.Unlock();
    }
    void A_TextLogStream::AddChar(F_Char Value) 
    {
        _Lock.Lock();
        _TextStream << F_TextChar(Value);
        _Lock.Unlock();
    }
#ifdef ABYTEK_ENABLE_WIDE_CHAR_TEXT
    void A_TextLogStream::AddTextChar(F_TextChar Value) 
    {
        _Lock.Lock();
        _TextStream << Value;
        _Lock.Unlock();
    }
#endif
    void A_TextLogStream::AddCStr(const F_Char* Value) 
    {
        if(!Value)
        {
            return;
        }

        while(*Value)
        {
            *this << Value[0];
            ++Value;
        }
    }
#ifdef ABYTEK_ENABLE_WIDE_CHAR_TEXT
    void A_TextLogStream::AddTextCStr(const F_TextChar* Value) 
    {
        if(!Value)
        {
            return;
        }

        _Lock.Lock();
        _TextStream << Value;
        _Lock.Unlock();
    }
#endif
    void A_TextLogStream::AddLogHeader(const F_LogHeader& Value) 
    {
        *this
            << F_ResetLogColor {}
            << Value.Prefix
            << Value.Body
            << Value.Suffix
            << F_UpdateLogColor::LowLight()
            << "["
            << F_UpdateLogColor::Field()
            << "File"
            << F_UpdateLogColor::LowLight()
            << ": "
            << F_UpdateLogColor::String()
            << "\""
            << Value.FilePathCStr
            << "\""
            << F_UpdateLogColor::LowLight()
            << ", "
            << F_UpdateLogColor::Field()
            << "Line"
            << F_UpdateLogColor::LowLight()
            << ": "
            << F_UpdateLogColor::Int()
            << F_UpdateLogColor::UInt()
            << Value.Line
            << F_UpdateLogColor::LowLight()
            << "]:"
            << F_NewLine {}
            << F_ResetLogColor {};
    }
    void A_TextLogStream::UpdateLogColor(const F_UpdateLogColor& Value)
    {
        _Lock.Lock();
        if (!EnableColor)
        {
            _Lock.Unlock();
            return;
        }
        
        switch (Value.Value)
        {
        case E_LogColorValue::BLACK:
            if(Value.Brightness == E_LogColorBrightness::DARK)
            {
                if(Value.Target == E_LogColorTarget::FOREGROUND)
                {
                    _TextStream << ABYTEK_TEXT_LOG_STREAM_FOREGROUND_BLACK;
                }
                else
                {
                    _TextStream << ABYTEK_TEXT_LOG_STREAM_BACKGROUND_BLACK;
                }
            }
            else
            {
                if(Value.Target == E_LogColorTarget::FOREGROUND)
                {
                    _TextStream << ABYTEK_TEXT_LOG_STREAM_FOREGROUND_BRIGHT_BLACK;
                }
                else
                {
                    _TextStream << ABYTEK_TEXT_LOG_STREAM_BACKGROUND_BRIGHT_BLACK;
                }
            }
            break;
        case E_LogColorValue::RED:
            if(Value.Brightness == E_LogColorBrightness::DARK)
            {
                if(Value.Target == E_LogColorTarget::FOREGROUND)
                {
                    _TextStream << ABYTEK_TEXT_LOG_STREAM_FOREGROUND_RED;
                }
                else
                {
                    _TextStream << ABYTEK_TEXT_LOG_STREAM_BACKGROUND_RED;
                }
            }
            else
            {
                if(Value.Target == E_LogColorTarget::FOREGROUND)
                {
                    _TextStream << ABYTEK_TEXT_LOG_STREAM_FOREGROUND_BRIGHT_RED;
                }
                else
                {
                    _TextStream << ABYTEK_TEXT_LOG_STREAM_BACKGROUND_BRIGHT_RED;
                }
            }
            break;
        case E_LogColorValue::GREEN:
            if(Value.Brightness == E_LogColorBrightness::DARK)
            {
                if(Value.Target == E_LogColorTarget::FOREGROUND)
                {
                    _TextStream << ABYTEK_TEXT_LOG_STREAM_FOREGROUND_GREEN;
                }
                else
                {
                    _TextStream << ABYTEK_TEXT_LOG_STREAM_BACKGROUND_GREEN;
                }
            }
            else
            {
                if(Value.Target == E_LogColorTarget::FOREGROUND)
                {
                    _TextStream << ABYTEK_TEXT_LOG_STREAM_FOREGROUND_BRIGHT_GREEN;
                }
                else
                {
                    _TextStream << ABYTEK_TEXT_LOG_STREAM_BACKGROUND_BRIGHT_GREEN;
                }
            }
            break;
        case E_LogColorValue::YELLOW:
            if(Value.Brightness == E_LogColorBrightness::DARK)
            {
                if(Value.Target == E_LogColorTarget::FOREGROUND)
                {
                    _TextStream << ABYTEK_TEXT_LOG_STREAM_FOREGROUND_YELLOW;
                }
                else
                {
                    _TextStream << ABYTEK_TEXT_LOG_STREAM_BACKGROUND_YELLOW;
                }
            }
            else
            {
                if(Value.Target == E_LogColorTarget::FOREGROUND)
                {
                    _TextStream << ABYTEK_TEXT_LOG_STREAM_FOREGROUND_BRIGHT_YELLOW;
                }
                else
                {
                    _TextStream << ABYTEK_TEXT_LOG_STREAM_BACKGROUND_BRIGHT_YELLOW;
                }
            }
            break;
        case E_LogColorValue::BLUE:
            if(Value.Brightness == E_LogColorBrightness::DARK)
            {
                if(Value.Target == E_LogColorTarget::FOREGROUND)
                {
                    _TextStream << ABYTEK_TEXT_LOG_STREAM_FOREGROUND_BLUE;
                }
                else
                {
                    _TextStream << ABYTEK_TEXT_LOG_STREAM_BACKGROUND_BLUE;
                }
            }
            else
            {
                if(Value.Target == E_LogColorTarget::FOREGROUND)
                {
                    _TextStream << ABYTEK_TEXT_LOG_STREAM_FOREGROUND_BRIGHT_BLUE;
                }
                else
                {
                    _TextStream << ABYTEK_TEXT_LOG_STREAM_BACKGROUND_BRIGHT_BLUE;
                }
            }
            break;
        case E_LogColorValue::MAGNETA:
            if(Value.Brightness == E_LogColorBrightness::DARK)
            {
                if(Value.Target == E_LogColorTarget::FOREGROUND)
                {
                    _TextStream << ABYTEK_TEXT_LOG_STREAM_FOREGROUND_MAGNETA;
                }
                else
                {
                    _TextStream << ABYTEK_TEXT_LOG_STREAM_BACKGROUND_MAGNETA;
                }
            }
            else
            {
                if(Value.Target == E_LogColorTarget::FOREGROUND)
                {
                    _TextStream << ABYTEK_TEXT_LOG_STREAM_FOREGROUND_BRIGHT_MAGNETA;
                }
                else
                {
                    _TextStream << ABYTEK_TEXT_LOG_STREAM_BACKGROUND_BRIGHT_MAGNETA;
                }
            }
            break;
        case E_LogColorValue::CYAN:
            if(Value.Brightness == E_LogColorBrightness::DARK)
            {
                if(Value.Target == E_LogColorTarget::FOREGROUND)
                {
                    _TextStream << ABYTEK_TEXT_LOG_STREAM_FOREGROUND_CYAN;
                }
                else
                {
                    _TextStream << ABYTEK_TEXT_LOG_STREAM_BACKGROUND_CYAN;
                }
            }
            else
            {
                if(Value.Target == E_LogColorTarget::FOREGROUND)
                {
                    _TextStream << ABYTEK_TEXT_LOG_STREAM_FOREGROUND_BRIGHT_CYAN;
                }
                else
                {
                    _TextStream << ABYTEK_TEXT_LOG_STREAM_BACKGROUND_BRIGHT_CYAN;
                }
            }
            break;
        case E_LogColorValue::WHITE:
            if(Value.Brightness == E_LogColorBrightness::DARK)
            {
                if(Value.Target == E_LogColorTarget::FOREGROUND)
                {
                    _TextStream << ABYTEK_TEXT_LOG_STREAM_FOREGROUND_WHITE;
                }
                else
                {
                    _TextStream << ABYTEK_TEXT_LOG_STREAM_BACKGROUND_WHITE;
                }
            }
            else
            {
                if(Value.Target == E_LogColorTarget::FOREGROUND)
                {
                    _TextStream << ABYTEK_TEXT_LOG_STREAM_FOREGROUND_BRIGHT_WHITE;
                }
                else
                {
                    _TextStream << ABYTEK_TEXT_LOG_STREAM_BACKGROUND_BRIGHT_WHITE;
                }
            }
            break;
        }
        _Lock.Unlock();
    }
    void A_TextLogStream::ResetLogColor()
    {
        _Lock.Lock();
        if (EnableColor)
        {
            _TextStream << ABYTEK_TEXT_LOG_STREAM_RESET_CONSOLE_COLOR;
        }
        _Lock.Unlock();
    }

    void A_TextLogStream::SetLogType(E_LogType LogType)
    {
        _Lock.Lock();
        _LogType = LogType;
        _Lock.Unlock();
    }

    void A_TextLogStream::Flush()
    {
        _Lock.Lock();
        auto Text = _TextStream.str();
        _Lock.Unlock();
        
        Push(Text.data());
        
        _Lock.Lock();
        _LogType = E_LogType::NONE;
        _TextStream = {};
        _Lock.Unlock();
    }

    void A_TextLogStream::Push(const F_TextChar* TextChars)
    {
    }
}
