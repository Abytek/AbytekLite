#include "Abytek/RootResource.hpp"


namespace Abytek
{
    ABYTEK_REFLECT(F_RootResource)
    {
    }

    F_RootResource::F_RootResource(const TW_Valid<F_ResourceManager>& Manager) :
        _OverridedManager(Manager)
    {
    }
    F_RootResource::~F_RootResource()
    {
    }

    void F_RootResource::Activate(const F_Name& Name, const TW<F_Resource>& Parent)
    {
        F_Resource::Activate(Name, Parent);
    }
    void F_RootResource::Deactivate()
    {
        F_Resource::Deactivate();
    }

    TW<F_ResourceManager> F_RootResource::GetOverridedManager()
    {
        return _OverridedManager;
    }
}
