#pragma once

#include "Abytek/Asset.hpp"


namespace Abytek
{
    class ABYTEK_ENGINE_ASSETS_API F_TextAsset final : public A_Asset
    {
    public:
        ABYTEK_BEGIN_REFLECTOR(A_Asset)
        ABYTEK_END_REFLECTOR(F_TextAsset)

    public:
        F_Text Text;

    public:
        F_TextAsset(const F_Text& AbsolutePath);
        virtual ~F_TextAsset();
    };
}
