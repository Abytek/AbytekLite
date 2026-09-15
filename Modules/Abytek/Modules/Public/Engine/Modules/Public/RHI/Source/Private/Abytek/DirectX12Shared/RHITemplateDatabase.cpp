#include "Abytek/DirectX12Shared/RHITemplateDatabase.hpp"


namespace Abytek
{
    F_DirectX12SharedRHITemplateDatabase::F_DirectX12SharedRHITemplateDatabase(const F_RHIFeatureSupports& FeatureSupports) :
        A_RHITemplateDatabase(E_RHIAPI::DIRECTX12, FeatureSupports)
    {
    }
    F_DirectX12SharedRHITemplateDatabase::~F_DirectX12SharedRHITemplateDatabase()
    {
    }
}
