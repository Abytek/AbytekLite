#include "Abytek/TransferLogStream.hpp"



namespace Abytek
{
    F_TransferLogStream::F_TransferLogStream(A_LogStream& Target) :
        A_LogStream(),
        TargetP(&Target)
    {
    }
    F_TransferLogStream::~F_TransferLogStream()
    {
    }

    void F_TransferLogStream::AddNewLine()
    {
        TargetP->AddNewLine();
    }
    void F_TransferLogStream::AddU8(U8 Value)
    {
        TargetP->AddU8(Value);
    }
    void F_TransferLogStream::AddU16(U16 Value)
    {
        TargetP->AddU16(Value);
    }
    void F_TransferLogStream::AddU32(U32 Value)
    {
        TargetP->AddU32(Value);
    }
    void F_TransferLogStream::AddU64(U64 Value)
    {
        TargetP->AddU64(Value);
    }
    void F_TransferLogStream::AddI8(I8 Value)
    {
        TargetP->AddI8(Value);
    }
    void F_TransferLogStream::AddI16(I16 Value)
    {
        TargetP->AddI16(Value);
    }
    void F_TransferLogStream::AddI32(I32 Value)
    {
        TargetP->AddI32(Value);
    }
    void F_TransferLogStream::AddI64(I64 Value)
    {
        TargetP->AddI64(Value);
    }
    void F_TransferLogStream::AddVoidP(const void* Value)
    {
        TargetP->AddVoidP(Value);
    }
    void F_TransferLogStream::AddSz(Sz Value)
    {
        TargetP->AddSz(Value);
    }
    void F_TransferLogStream::AddPDiff(PDiff Value)
    {
        TargetP->AddPDiff(Value);
    }
    void F_TransferLogStream::AddF32(F32 Value)
    {
        TargetP->AddF32(Value);
    }
    void F_TransferLogStream::AddF64(F64 Value)
    {
        TargetP->AddF64(Value);
    }
    void F_TransferLogStream::AddChar(F_Char Value)
    {
        TargetP->AddChar(Value);
    }
#ifdef ABYTEK_ENABLE_WIDE_CHAR_TEXT
    void F_TransferLogStream::AddTextChar(F_TextChar Value)
    {
        TargetP->AddTextChar(Value);
    }
#endif
    void F_TransferLogStream::AddCStr(const F_Char* Value)
    {
        TargetP->AddCStr(Value);
    }
#ifdef ABYTEK_ENABLE_WIDE_CHAR_TEXT
    void F_TransferLogStream::AddTextCStr(const F_TextChar* Value)
    {
        TargetP->AddTextCStr(Value);
    }
#endif
    void F_TransferLogStream::AddLogHeader(const F_LogHeader& Value)
    {
        TargetP->AddLogHeader(Value);
    }
    void F_TransferLogStream::UpdateLogColor(const F_UpdateLogColor& Value)
    {
        TargetP->UpdateLogColor(Value);
    }
    void F_TransferLogStream::ResetLogColor()
    {
        TargetP->ResetLogColor();
    }
}