#pragma once

#include "Abytek/Engine.RHI.prerequisites.hpp"
#include "Abytek/RHIIndirectConfig.hpp"
#include "Abytek/RHIDrawPass.hpp"
#include "Abytek/RHIDispatchComputePass.hpp"


namespace Abytek
{
    class ABYTEK_ENGINE_RHI_API A_RHIIndirectUtilities : public A_RAObject
    {
    public:
        ABYTEK_DECLARE_OBJECT_SINGLETON_CRT(A_RHIIndirectUtilities);
        
    private:        

    public:
        
    public:
        ABYTEK_RA_DECLARE_OBJECT_CREATABLE(A_RHIIndirectUtilities);
        virtual void Build();
        virtual void Release() override;
        
    public:
        virtual Sz GetArgumentStride(E_RHIIndirectArgumentType Type);
        virtual Sz GetCountStride();
        
        virtual void* WriteArgument_DrawNonIndexed(void* DataP, const F_RHIDrawNonIndexedConfig& DrawNonIndexed);
        virtual void* WriteArgument_DrawIndexed(void* DataP, const F_RHIDrawIndexedConfig& DrawIndexed);
        virtual void* WriteArgument_DispatchMesh(void* DataP, const F_Vector3_U32& NumThreadGroups);
        virtual void* WriteArgument_DispatchCompute(void* DataP, const F_Vector3_U32& NumThreadGroups);
    };
}
