#include "Abytek/Renderer/GPUData/GPUDataCommon.hpp"
#include "Abytek/Renderer/GPUData/GPUDataPage.hpp"


namespace Abytek
{
    F_GPUDataInstanceAddress F_GPUDataInstanceAddress::From(const F_GPUDataInstanceAllocation& Allocation, U32 OffsetInInstances)
    {
        F_GPUDataInstanceAddress Result;
        if (Allocation)
        {
            Result.PageIndex = Allocation.Page->GetIndex();
            Result.LocalIndex = Allocation.BeginLocalIndex + OffsetInInstances;
        }
        return Result;
    }
}