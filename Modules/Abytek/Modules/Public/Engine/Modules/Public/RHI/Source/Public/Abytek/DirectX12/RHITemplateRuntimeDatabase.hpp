#pragma once

#include "Abytek/RHITemplateRuntimeDatabase.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    class ABYTEK_ENGINE_RHI_API F_DirectX12RHITemplateRuntimeDatabase : public A_RHITemplateRuntimeDatabase
    {
    private:

    public:

    public:
        ABYTEK_RA_DECLARE_OBJECT(F_DirectX12RHITemplateRuntimeDatabase);
        virtual void Build(const F_RHITemplateRuntimeDatabaseBuildParams& BuildParams) override;
        virtual void Release() override;
    };
}
#endif