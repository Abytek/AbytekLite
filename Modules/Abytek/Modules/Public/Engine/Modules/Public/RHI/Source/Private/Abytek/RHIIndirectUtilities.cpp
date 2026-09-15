#include "Abytek/RHIIndirectUtilities.hpp"
#include "Abytek/DirectX12/RHIIndirectUtilities.hpp"
#include "Abytek/RHISubsystem.hpp"


namespace Abytek
{
    ABYTEK_RA_BEGIN_OBJECT(A_RHIIndirectUtilities)
        auto RHISubsystem = F_RHISubsystem::GetInstance();
        switch (RHISubsystem->GetActiveAPI())
        {
#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
        case E_RHIAPI::DIRECTX12:
            return TU<F_DirectX12RHIIndirectUtilities>()();
#endif 
    }
    ABYTEK_RA_END_OBJECT()
    
    ABYTEK_DEFINE_OBJECT_SINGLETON_CRT(A_RHIIndirectUtilities);
    
    void A_RHIIndirectUtilities::Build()
    {
        A_RAObject::BuildMinimal();
        
        ABYTEK_BIND_OBJECT_SINGLETON_CRT();
    }   
    void A_RHIIndirectUtilities::Release()
    {
        A_RAObject::Release();
    }

    Sz A_RHIIndirectUtilities::GetArgumentStride(E_RHIIndirectArgumentType Type)
    {
        ABYTEK_ENGINE_RHI_ASSERT(false) << "Not implemented";
        return 0;
    }
    Sz A_RHIIndirectUtilities::GetCountStride()
    {
        ABYTEK_ENGINE_RHI_ASSERT(false) << "Not implemented";
        return sizeof(U32);
    }

    void* A_RHIIndirectUtilities::WriteArgument_DrawNonIndexed(void* DataP, const F_RHIDrawNonIndexedConfig& DrawNonIndexed)
    {
        ABYTEK_ENGINE_RHI_ASSERT(false) << "Not implemented";
        return nullptr;
    }
    void* A_RHIIndirectUtilities::WriteArgument_DrawIndexed(void* DataP, const F_RHIDrawIndexedConfig& DrawIndexed)
    {
        ABYTEK_ENGINE_RHI_ASSERT(false) << "Not implemented";
        return nullptr;
    }
    void* A_RHIIndirectUtilities::WriteArgument_DispatchMesh(void* DataP, const F_Vector3_U32& NumThreadGroups)
    {
        ABYTEK_ENGINE_RHI_ASSERT(false) << "Not implemented";
        return nullptr;
    }
    void* A_RHIIndirectUtilities::WriteArgument_DispatchCompute(void* DataP, const F_Vector3_U32& NumThreadGroups)
    {
        ABYTEK_ENGINE_RHI_ASSERT(false) << "Not implemented";
        return nullptr;
    }
}
