#include "Abytek/FSFile.hpp"


namespace Abytek
{
    A_FSFile::A_FSFile(const F_FSFileOpenParams& OpenParams) :
        _Path(OpenParams.Path),
        _AccessFlags(OpenParams.AccessFlags),
        _ShareFlags(OpenParams.ShareFlags),
        _AutoCreate(OpenParams.AutoCreate)
    {
    }
    A_FSFile::~A_FSFile()
    {
    }

    F_FeedbackStatus A_FSFile::SetSize(Sz NewSize) const
    {
        return F_FeedbackStatus::MakeFailed(ABYTEK_TEXT("Not implemented"));
    }

    F_FeedbackStatus A_FSFile::GetSize(Sz& OutSize) const
    {
        return F_FeedbackStatus::MakeFailed(ABYTEK_TEXT("Not implemented"));
    }
    F_FeedbackStatus A_FSFile::Seek(PDiff NewOffset)
    {
        return F_FeedbackStatus::MakeFailed(ABYTEK_TEXT("Not implemented"));
    }
    F_FeedbackStatus A_FSFile::ReadBinary(const TF_Span<U8>& OutData, Sz& OutSize)
    {
        return F_FeedbackStatus::MakeFailed(ABYTEK_TEXT("Not implemented"));
    }
    F_FeedbackStatus A_FSFile::WriteBinary(const TF_Span<const U8>& Data, Sz& OutSize)
    {
        return F_FeedbackStatus::MakeFailed(ABYTEK_TEXT("Not implemented"));
    }

    F_FeedbackStatus A_FSFile::Rewind()
    {     
        ABYTEK_FEEDBACK_STATUS_CHECK(Seek(0));
        return F_FeedbackStatus::MakeSucceeded();
    }
    F_FeedbackStatus A_FSFile::Ate()
    {     
        Sz Size = 0;
        ABYTEK_FEEDBACK_STATUS_CHECK(GetSize(Size));
        ABYTEK_FEEDBACK_STATUS_CHECK(Seek(Size));
        return F_FeedbackStatus::MakeSucceeded();
    }
    F_FeedbackStatus A_FSFile::Truncate(Sz NewSize)
    {
        Sz TargetOffset = (_Offset > NewSize) ? NewSize : _Offset;
        ABYTEK_FEEDBACK_STATUS_CHECK(SetSize(NewSize));
        ABYTEK_FEEDBACK_STATUS_CHECK(Seek(TargetOffset));
        return F_FeedbackStatus::MakeSucceeded();
    }
    F_FeedbackStatus A_FSFile::ReadBinary(const TF_Span<U8>& OutData)
    {
        Sz Size = 0;
        ABYTEK_FEEDBACK_STATUS_CHECK(ReadBinary(OutData, Size));
        if (OutData.size() != Size)
        {
            return F_FeedbackStatus::MakeFailed(
                ABYTEK_TEXT("Failed to read binary, target read size = ")
                + ToText(OutData.size())
                + ABYTEK_TEXT(", actual read size = ")
                + ToText(Size)
            );
        }
        return F_FeedbackStatus::MakeSucceeded();
    }
    F_FeedbackStatus A_FSFile::ReadBinaryToEnd(TF_Vector<U8>& OutData, Sz Offset)
    {
        Sz ReadOffset = Offset;
        if (ReadOffset == ~Sz(0))
        {
            ReadOffset = _Offset;
        }
        
        ABYTEK_FEEDBACK_STATUS_CHECK(Seek(ReadOffset));
        
        Sz Size = 0;
        ABYTEK_FEEDBACK_STATUS_CHECK(GetSize(Size));
        Sz ReadSize = Size - ReadOffset;
        OutData.resize(ReadSize);
        ABYTEK_FEEDBACK_STATUS_CHECK(ReadBinary(OutData, ReadSize));
        if (OutData.size() != ReadSize)
        {
            return F_FeedbackStatus::MakeFailed(
                ABYTEK_TEXT("Failed to read binary to end, target read size = ")
                + ToText(OutData.size())
                + ABYTEK_TEXT(", actual read size = ")
                + ToText(ReadSize)
            );
        }
        return F_FeedbackStatus::MakeSucceeded();
    }
    F_FeedbackStatus A_FSFile::ReadBinaryExclusive(TF_Vector<U8>& OutData)
    {
        ABYTEK_FEEDBACK_STATUS_CHECK(Rewind());
        return ReadBinaryToEnd(OutData);
    }
    F_FeedbackStatus A_FSFile::WriteBinary(const TF_Span<const U8>& Data)
    {
        Sz Size = 0;
        ABYTEK_FEEDBACK_STATUS_CHECK(WriteBinary(Data, Size));
        if (Data.size() != Size)
        {
            return F_FeedbackStatus::MakeFailed(
                ABYTEK_TEXT("Failed to read binary, target write size = ")
                + ToText(Data.size())
                + ABYTEK_TEXT(", actual write size = ")
                + ToText(Size)
            );
        }
        return F_FeedbackStatus::MakeSucceeded();
    }
    F_FeedbackStatus A_FSFile::WriteBinaryExclusive(const TF_Span<const U8>& Data)
    {
        ABYTEK_FEEDBACK_STATUS_CHECK(Rewind());
        ABYTEK_FEEDBACK_STATUS_CHECK(Truncate(Data.size()));
        ABYTEK_FEEDBACK_STATUS_CHECK(WriteBinary(Data));
        return F_FeedbackStatus::MakeSucceeded();
    }
    
    F_FeedbackStatus A_FSFile::ReadStringToEnd(F_String& OutValue, Sz Offset)
    {
        TF_Vector<U8> Data;
        ABYTEK_FEEDBACK_STATUS_CHECK(ReadBinaryToEnd(Data, Offset));
        OutValue.resize(Data.size());
        memcpy(
            OutValue.data(),
            Data.data(),
            Data.size()
        );
        return F_FeedbackStatus::MakeSucceeded();
    }
    F_FeedbackStatus A_FSFile::ReadStringExclusive(F_String& OutValue)
    {
        TF_Vector<U8> Data;
        ABYTEK_FEEDBACK_STATUS_CHECK(ReadBinaryExclusive(Data));
        OutValue.resize(Data.size());
        memcpy(
            OutValue.data(),
            Data.data(),
            Data.size()
        );
        return F_FeedbackStatus::MakeSucceeded();
    }
    F_FeedbackStatus A_FSFile::WriteString(const F_String& Data)
    {
        return WriteBinary(
            TF_Span<const U8>(
                ((U8*)Data.data()), 
                ((U8*)Data.data()) + Data.length() * sizeof(F_Char)
            )
        );
    }
    F_FeedbackStatus A_FSFile::WriteStringExclusive(const F_String& Data)
    {
        return WriteBinaryExclusive(
            TF_Span<const U8>(
                ((U8*)Data.data()), 
                ((U8*)Data.data()) + Data.length() * sizeof(F_Char)
            )
        );
    }
    
    F_FeedbackStatus A_FSFile::ReadTextToEnd(F_Text& OutText, Sz Offset)
    {
        F_String UTF8String;
        ABYTEK_FEEDBACK_STATUS_CHECK(ReadStringToEnd(UTF8String, Offset));
        OutText = ToText(UTF8String);
        return F_FeedbackStatus::MakeSucceeded();
    }
    F_FeedbackStatus A_FSFile::ReadTextExclusive(F_Text& OutText)
    {
        F_String UTF8String;
        ABYTEK_FEEDBACK_STATUS_CHECK(ReadStringExclusive(UTF8String));
        OutText = ToText(UTF8String);
        return F_FeedbackStatus::MakeSucceeded();
    }
    F_FeedbackStatus A_FSFile::WriteText(const F_Text& Text)
    {
        F_String UTF8String = ToString(Text);
        ABYTEK_FEEDBACK_STATUS_CHECK(WriteString(UTF8String));
        return F_FeedbackStatus::MakeSucceeded();
    }
    F_FeedbackStatus A_FSFile::WriteTextExclusive(const F_Text& Text)
    {
        F_String UTF8String = ToString(Text);
        ABYTEK_FEEDBACK_STATUS_CHECK(WriteStringExclusive(UTF8String));
        return F_FeedbackStatus::MakeSucceeded();
    }
}
