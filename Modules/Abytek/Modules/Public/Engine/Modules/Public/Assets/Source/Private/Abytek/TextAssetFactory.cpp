#include "Abytek/TextAssetFactory.hpp"
#include "Abytek/TextAsset.hpp"


namespace Abytek
{
    ABYTEK_REFLECT(F_TextAssetFactory)
    {
    }

    F_TextAssetFactory::F_TextAssetFactory() :
        A_AssetFactory({ ABYTEK_NAME("txt"), ABYTEK_NAME("text") })
    {
    }
    F_TextAssetFactory::~F_TextAssetFactory()
    {
    }

    TS<A_Asset> F_TextAssetFactory::BuildFromBinary(const F_Text& AbsPath, const F_ArchiveReadOnlyView& ArchiveROView)
    {
        auto Asset = TS<F_TextAsset>()(AbsPath);
        Asset->Text = UTF8Decode(ArchiveROView.GetSpan());
        return ABYTEK_MOVE(Asset);
    }
    F_Text F_TextAssetFactory::UTF8Decode(const TF_Span<const U8>& buffer)
    {
        F_Text out;
        unsigned int codepoint;

        auto buffer_end = buffer.cend();
        auto* in = buffer.cbegin();

        while (in != buffer_end)
        {
            unsigned char ch = static_cast<unsigned char>(*in);
            if (ch <= 0x7f)
                codepoint = ch;
            else if (ch <= 0xbf)
                codepoint = (codepoint << 6) | (ch & 0x3f);
            else if (ch <= 0xdf)
                codepoint = ch & 0x1f;
            else if (ch <= 0xef)
                codepoint = ch & 0x0f;
            else
                codepoint = ch & 0x07;
            ++in;
            if (((*in & 0xc0) != 0x80) && (codepoint <= 0x10ffff))
            {
                if (sizeof(wchar_t) > 2)
                    out.append(1, static_cast<wchar_t>(codepoint));
                else if (codepoint > 0xffff)
                {
                    codepoint -= 0x10000;
                    out.append(1, static_cast<wchar_t>(0xd800 + (codepoint >> 10)));
                    out.append(1, static_cast<wchar_t>(0xdc00 + (codepoint & 0x03ff)));
                }
                else if (codepoint < 0xd800 || codepoint >= 0xe000)
                    out.append(1, static_cast<wchar_t>(codepoint));
            }
        }
        return std::move(out);
    }
}
