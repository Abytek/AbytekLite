#pragma once

#include "Abytek/RHITemplateDatabase.hpp"


namespace Abytek
{
    class ABYTEK_ENGINE_RHI_API F_DirectX12SharedRHITemplateDatabase : public A_RHITemplateDatabase
    {
    private:

    public:

    public:
        F_DirectX12SharedRHITemplateDatabase(const F_RHIFeatureSupports& FeatureSupports);
        ~F_DirectX12SharedRHITemplateDatabase() override;
    };
}