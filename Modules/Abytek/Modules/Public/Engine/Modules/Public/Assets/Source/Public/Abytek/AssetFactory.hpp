#pragma once

#include "Abytek/Engine.Assets.prerequisites.hpp"


namespace Abytek
{
    class A_Asset;

    enum class E_AssetBuildMode
    {
        FROM_BINARY,
        FROM_FILE,
        DEFAULT = FROM_BINARY
    };
    
    class ABYTEK_ENGINE_ASSETS_API A_AssetFactory : public A_Object
    {
    public:
        ABYTEK_BEGIN_REFLECTOR()
        ABYTEK_END_REFLECTOR(A_AssetFactory)

    private:
        TF_Vector<F_Name> _FileExtensions;

    public:
        const TF_Vector<F_Name> GetFileExtensions() const noexcept { return _FileExtensions; }

    protected:
        A_AssetFactory(const TF_Vector<F_Name>& FileExtensions);
        
    public:
        virtual ~A_AssetFactory();

    public:
        virtual TS<A_Asset> BuildFromBinary(const F_Text& AbsPath, const F_ArchiveReadOnlyView& ArchiveROView);
        virtual TS<A_Asset> BuildFromFile(const F_Text& AbsPath);
        virtual E_AssetBuildMode GetBuildMode();
    };
}
