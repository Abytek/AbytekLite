#pragma once

#include "Abytek/Streamable.hpp"
#include "Abytek/FSCommon.hpp"


namespace Abytek
{
    class ABYTEK_BASE_PLATFORM_API A_FSFile : public A_Object
    {
    private:
        F_Text _Path;
        E_FSFileAccessFlag _AccessFlags = E_FSFileAccessFlag::DEFAULT;
        E_FSFileShareFlag _ShareFlags = E_FSFileShareFlag::DEFAULT;
        B8 _AutoCreate = true;
        
    protected:
        PDiff _Offset = 0;

    public:
        ABYTEK_FORCE_INLINE const auto& GetPath() const noexcept
        {
            return _Path;
        }
        ABYTEK_FORCE_INLINE auto GetAccessFlags() const noexcept
        {
            return _AccessFlags;
        }
        ABYTEK_FORCE_INLINE auto GetShareFlags() const noexcept
        {
            return _ShareFlags;
        }
        ABYTEK_FORCE_INLINE auto GetAutoCreate() const noexcept
        {
            return _AutoCreate;
        }
        ABYTEK_FORCE_INLINE PDiff GetOffset() const
        {
            return _Offset;
        }
        
    protected:
        A_FSFile(const F_FSFileOpenParams& OpenParams);
        
    public:
        ~A_FSFile() override;

    protected:
        virtual F_FeedbackStatus SetSize(Sz NewSize) const;
        
    public:
        virtual F_FeedbackStatus GetSize(Sz& OutSize) const;
        virtual F_FeedbackStatus Seek(PDiff NewOffset);
        virtual F_FeedbackStatus ReadBinary(const TF_Span<U8>& OutData, Sz& OutSize);
        virtual F_FeedbackStatus WriteBinary(const TF_Span<const U8>& Data, Sz& OutSize);
        
    public:
        F_FeedbackStatus Rewind();
        F_FeedbackStatus Ate();
        F_FeedbackStatus Truncate(Sz NewSize);
        F_FeedbackStatus ReadBinary(const TF_Span<U8>& OutData);
        F_FeedbackStatus ReadBinaryToEnd(TF_Vector<U8>& OutData, Sz Offset = ~Sz(0));
        F_FeedbackStatus ReadBinaryExclusive(TF_Vector<U8>& OutData);
        F_FeedbackStatus WriteBinary(const TF_Span<const U8>& Data);
        F_FeedbackStatus WriteBinaryExclusive(const TF_Span<const U8>& Data);
        template<typename __F>
        F_FeedbackStatus Read(__F& OutData)
        {
            return ReadBinary({ (U8*)&OutData, sizeof(__F) });
        }
        template<typename __F>
        F_FeedbackStatus Write(const __F& OutData)
        {
            return WriteBinary({ (U8*)&OutData, sizeof(__F) });
        }
        template<typename __F>
        F_FeedbackStatus WriteExclusive(const __F& OutData)
        {
            return WriteBinaryExclusive({ (U8*)&OutData, sizeof(__F) });
        }

    public:
        F_FeedbackStatus ReadStringToEnd(F_String& OutValue, Sz Offset = ~Sz(0));
        F_FeedbackStatus ReadStringExclusive(F_String& OutValue);
        F_FeedbackStatus WriteString(const F_String& Data);
        F_FeedbackStatus WriteStringExclusive(const F_String& Data);
        
    public:
        F_FeedbackStatus ReadTextToEnd(F_Text& OutText, Sz Offset = ~Sz(0));
        F_FeedbackStatus ReadTextExclusive(F_Text& OutText);
        F_FeedbackStatus WriteText(const F_Text& Text);
        F_FeedbackStatus WriteTextExclusive(const F_Text& Text);
    };
}