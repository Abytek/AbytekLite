#pragma once

#include "Abytek/AssetFactory.hpp"


namespace Abytek
{
    class ABYTEK_ENGINE_ASSETS_API F_TextAssetFactory final : public A_AssetFactory
    {
    public:
        ABYTEK_BEGIN_REFLECTOR(A_AssetFactory)
        ABYTEK_END_REFLECTOR(F_TextAssetFactory)

    public:
        F_TextAssetFactory();
        virtual ~F_TextAssetFactory();

    public:
        virtual TS<A_Asset> BuildFromBinary(const F_Text& AbsPath, const F_ArchiveReadOnlyView& ArchiveROView);
        F_Text UTF8Decode(const TF_Span<const U8>& buffer);
    };
}
