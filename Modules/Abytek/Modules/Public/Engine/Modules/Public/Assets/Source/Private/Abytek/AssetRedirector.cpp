#include "Abytek/AssetRedirector.hpp"


namespace Abytek
{
    ABYTEK_REFLECT(A_AssetRedirector)
    {
    }

    A_AssetRedirector::A_AssetRedirector(const TF_Vector<F_Name>& FileExtensions) :
        _FileExtensions(FileExtensions)
    {
    }
    A_AssetRedirector::~A_AssetRedirector()
    {
    }

    B8 A_AssetRedirector::TryRedirect(F_Text& Path)
    {
        return false;
    }
}
