#include "Abytek/DirectX12/RHITemplateRuntimeDatabase.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    void F_DirectX12RHITemplateRuntimeDatabase::Build(const F_RHITemplateRuntimeDatabaseBuildParams& BuildParams)
    {
        A_RHITemplateRuntimeDatabase::Build(BuildParams);
    }
    void F_DirectX12RHITemplateRuntimeDatabase::Release()
    {
        A_RHITemplateRuntimeDatabase::Release();
    }
}
#endif