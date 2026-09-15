#pragma once

#include "Abytek/RHIIndirectUtilities.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    class ABYTEK_ENGINE_RHI_API F_DirectX12RHIIndirectUtilities : public A_RHIIndirectUtilities
    {
    private:        

    public:
        
    public:
        ABYTEK_RA_DECLARE_OBJECT(F_DirectX12RHIIndirectUtilities);
        virtual void Build();
        virtual void Release() override;
        
    public:
        virtual Sz GetArgumentStride(E_RHIIndirectArgumentType Type) override;
        virtual Sz GetCountStride() override;
        
        virtual void* WriteArgument_DrawNonIndexed(void* DataP, const F_RHIDrawNonIndexedConfig& DrawNonIndexed) override;
        virtual void* WriteArgument_DrawIndexed(void* DataP, const F_RHIDrawIndexedConfig& DrawIndexed) override;
        virtual void* WriteArgument_DispatchMesh(void* DataP, const F_Vector3_U32& NumThreadGroups) override;
        virtual void* WriteArgument_DispatchCompute(void* DataP, const F_Vector3_U32& NumThreadGroups) override;
    };
}
#endif