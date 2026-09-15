#pragma once

#include "Abytek/Resource.hpp"


namespace Abytek
{
    class ABYTEK_ENGINE_RESOURCE_API F_RootResource final : public F_Resource
    {
    public:
        ABYTEK_BEGIN_REFLECTOR(F_Resource)
        ABYTEK_END_REFLECTOR(F_RootResource)

    private:
        TW<F_ResourceManager> _OverridedManager;
        
    public:

    public:
        F_RootResource(const TW_Valid<F_ResourceManager>& Manager);
        ~F_RootResource() override;

    public:
        void Activate(const F_Name& Name, const TW<F_Resource>& Parent) override;
        void Deactivate() override;
        
    public:
        TW<F_ResourceManager> GetOverridedManager() override;
    };
}
