#include "Abytek/AssetFactory.hpp"


namespace Abytek
{
    ABYTEK_REFLECT(A_AssetFactory)
    {
    }

    A_AssetFactory::A_AssetFactory(const TF_Vector<F_Name>& FileExtensions) :
        _FileExtensions(FileExtensions)
    {
    }
    A_AssetFactory::~A_AssetFactory()
    {
    }

    TS<A_Asset> A_AssetFactory::BuildFromBinary(const F_Text& AbsPath, const F_ArchiveReadOnlyView& ArchiveROView)
    {
        return {};
    }
    TS<A_Asset> A_AssetFactory::BuildFromFile(const F_Text& AbsPath)
    {
        return {};
    }
    E_AssetBuildMode A_AssetFactory::GetBuildMode()
    {
        return E_AssetBuildMode::DEFAULT;
    }
}
