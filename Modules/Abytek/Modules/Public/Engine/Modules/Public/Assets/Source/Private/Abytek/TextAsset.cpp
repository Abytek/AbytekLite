#include "Abytek/TextAsset.hpp"


namespace Abytek
{
    ABYTEK_REFLECT(F_TextAsset)
    {
    }

    F_TextAsset::F_TextAsset(const F_Text& AbsolutePath) :
        A_Asset(AbsolutePath)
    {
    }
    F_TextAsset::~F_TextAsset()
    {
    }
}
