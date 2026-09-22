#pragma once

#include "Abytek/Engine.Assets.prerequisites.hpp"


namespace Abytek
{
    class ABYTEK_ENGINE_ASSETS_API A_AssetRedirector : public A_Object
    {
    public:
        ABYTEK_BEGIN_REFLECTOR()
        ABYTEK_END_REFLECTOR(A_AssetRedirector);

    private:
        TF_Vector<F_Name> _FileExtensions;

    public:
        const TF_Vector<F_Name> GetFileExtensions() const noexcept { return _FileExtensions; }

    protected:
        A_AssetRedirector(const TF_Vector<F_Name>& FileExtensions);

    public:
        virtual ~A_AssetRedirector();

    public:
        virtual B8 TryRedirect(F_Text& Path);
    };
}
