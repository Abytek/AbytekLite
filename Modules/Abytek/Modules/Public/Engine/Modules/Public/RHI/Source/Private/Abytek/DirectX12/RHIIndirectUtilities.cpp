#include "Abytek/DirectX12/RHIIndirectUtilities.hpp"


#ifdef ABYTEK_ENGINE_RHI_ENABLE_DIRECTX12
namespace Abytek
{
    void F_DirectX12RHIIndirectUtilities::Build()
    {
        A_RHIIndirectUtilities::Build();
    }   
    void F_DirectX12RHIIndirectUtilities::Release()
    {
        A_RHIIndirectUtilities::Release();
    }

    Sz F_DirectX12RHIIndirectUtilities::GetArgumentStride(E_RHIIndirectArgumentType Type)
    {
        switch (Type)
        {
        case E_RHIIndirectArgumentType::DRAW_NON_INDEXED:
            return sizeof(D3D12_DRAW_ARGUMENTS);
        case E_RHIIndirectArgumentType::DRAW_INDEXED:
            return sizeof(D3D12_DRAW_INDEXED_ARGUMENTS);
        case E_RHIIndirectArgumentType::DISPATCH_COMPUTE:
            return sizeof(D3D12_DISPATCH_ARGUMENTS);
        case E_RHIIndirectArgumentType::DISPATCH_MESH:
            return sizeof(D3D12_DISPATCH_MESH_ARGUMENTS);
        default:
            ABYTEK_ENGINE_RHI_ASSERT(false) << "Invalid argument type";
            break;
        }
        return 0;
    }
    Sz F_DirectX12RHIIndirectUtilities::GetCountStride()
    {
        return sizeof(U32);
    }

    void* F_DirectX12RHIIndirectUtilities::WriteArgument_DrawNonIndexed(
        void* DataP,
        const F_RHIDrawNonIndexedConfig& DrawNonIndexed
    )
    {
        auto& Data = *((D3D12_DRAW_ARGUMENTS*)DataP);
        Data.VertexCountPerInstance = DrawNonIndexed.NumVerticesPerInstance;
        Data.InstanceCount = DrawNonIndexed.NumInstances;
        Data.StartVertexLocation = DrawNonIndexed.VertexOffset;
        Data.StartInstanceLocation = DrawNonIndexed.InstanceOffset;
        return (&Data) + 1;
    }
    void* F_DirectX12RHIIndirectUtilities::WriteArgument_DrawIndexed(
        void* DataP,
        const F_RHIDrawIndexedConfig& DrawIndexed
    )
    {
        auto& Data = *((D3D12_DRAW_INDEXED_ARGUMENTS*)DataP);
        Data.IndexCountPerInstance = DrawIndexed.NumIndicesPerInstance;
        Data.InstanceCount = DrawIndexed.NumInstances;
        Data.StartIndexLocation = DrawIndexed.IndexOffset;
        Data.BaseVertexLocation = DrawIndexed.VertexOffset;
        Data.StartInstanceLocation = DrawIndexed.InstanceOffset;
        return (&Data) + 1;
    }
    void* F_DirectX12RHIIndirectUtilities::WriteArgument_DispatchMesh(
        void* DataP, 
        const F_Vector3_U32& NumThreadGroups
    )
    {
        auto& Data = *((D3D12_DISPATCH_MESH_ARGUMENTS*)DataP);
        Data.ThreadGroupCountX = NumThreadGroups.X;
        Data.ThreadGroupCountY = NumThreadGroups.Y;
        Data.ThreadGroupCountZ = NumThreadGroups.Z;
        return (&Data) + 1;
    }
    void* F_DirectX12RHIIndirectUtilities::WriteArgument_DispatchCompute(
        void* DataP,
        const F_Vector3_U32& NumThreadGroups
    )
    {
        auto& Data = *((D3D12_DISPATCH_ARGUMENTS*)DataP);
        Data.ThreadGroupCountX = NumThreadGroups.X;
        Data.ThreadGroupCountY = NumThreadGroups.Y;
        Data.ThreadGroupCountZ = NumThreadGroups.Z;
        return (&Data) + 1;
    }
}
#endif