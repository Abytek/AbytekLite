#include "Abytek/Geometries/Render/RenderGeometryManager.hpp"
#include "Abytek/Geometries/Render/RenderGeometryPage.hpp"
#include "Abytek/RenderBase/RenderScene.hpp"


namespace Abytek
{
    namespace RenderGeometry
    {
        ABYTEK_DEFINE_GLOBAL_RENDER_BINDING(F_GlobalSRVBinding)
        {
            ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::RenderGeometry::F_GlobalSRVBinding"));
        }
        ABYTEK_DEFINE_GLOBAL_RENDER_BINDING(F_GlobalUAVBinding)
        {
            ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::RenderGeometry::F_GlobalUAVBinding"));
        }
    }
    
    F_RenderGeometryManager::F_RenderGeometryManager(const TW_Valid<F_WorldRenderResource>& WorldRenderResource, const F_RenderGeometryManagerBuildParams& BuildParams) :
        A_RenderObject(WorldRenderResource),
        _Scene(BuildParams.Scene)
    {
        _CapacityLimitInBytes = U64(1024) * U64(1024) * U64(1024);
        _MinimalPageSizeInBytes = U64(32) * U64(1024) * U64(1024);
        _NextPageSizeInBytes = _MinimalPageSizeInBytes;
        
        AddNewPage(0);
        _RecreateBindGroupIfNeeded();
    }
    F_RenderGeometryManager::~F_RenderGeometryManager()
    {
    }

    void F_RenderGeometryManager::BeginUpdate()
    {
    }
    void F_RenderGeometryManager::EndUpdate()
    {
    }
    void F_RenderGeometryManager::BeginPostUpdate()
    {
        _RecreateBindGroupIfNeeded();
        FlushDeallocationQueue();
    }
    void F_RenderGeometryManager::EndPostUpdate()
    {
    }

    void F_RenderGeometryManager::FlushDeallocationQueue()
    {
        F_RenderGeometryAllocation Allocation;
        while (DeallocationQueue.TryPop(Allocation))
        {
            Allocation.Page->Deallocate(Allocation);
        }
    }

    TF_Optional<F_RenderGeometryAllocation> F_RenderGeometryManager::Allocate(U64 SizeInBytes, U64 AlignmentInBytes)
    {
        TF_Optional<F_RenderGeometryAllocation> Result;
        _CriticalSection(
            [this, &Result, SizeInBytes, AlignmentInBytes]
            {
                for (const auto& Page : _Pages)
                {
                    if (auto Allocation = Page->Allocate(SizeInBytes, AlignmentInBytes))
                    {
                        Result = ABYTEK_MOVE(Allocation);
                    }
                }
                if (auto Page = AddNewPage(SizeInBytes + AlignmentInBytes))
                {
                    if (auto Allocation = Page->Allocate(SizeInBytes, AlignmentInBytes))
                    {
                        Result = ABYTEK_MOVE(Allocation);
                    }
                }
            }
        );
        return Result;
    }
    TS<F_RenderGeometryPage> F_RenderGeometryManager::AddNewPage(U64 SizeInBytes)
    {
        U64 ActualSizeInBytes = SizeInBytes + sizeof(F_Vector4_F32); // with alignment
        
        U64 NewPageSizeInBytes = _NextPageSizeInBytes;
        while (NewPageSizeInBytes <= ActualSizeInBytes)
        {
            NewPageSizeInBytes *= 2;
        }
        
        U64 NewCapacityInBytes = NewPageSizeInBytes + _CapacityInBytes;
        if (NewCapacityInBytes > _CapacityLimitInBytes)
        {
            return {};
        }
        
        _NextPageSizeInBytes = NewPageSizeInBytes * 2;
        _CapacityInBytes = NewCapacityInBytes;
        
        _ShouldRecreateBindGroup = true;
        
        auto Page = TS<F_RenderGeometryPage>()(
            ABYTEK_WTHIS(),
            static_cast<U32>(_Pages.size()),
            NewPageSizeInBytes
        );
        _Pages.push_back(Page);
        return Page;
    }

    B8 F_RenderGeometryManager::AddMeshData_Simple(
        const F_SimpleMeshDataROView& MeshDataView,
        F_RenderGeometryAllocation& OutGeometryAllocation,
        F_RenderGeometryAllocationStructure_Simple& OutGeometryAllocationStructure
    )
    {
        U32 SizeInBytes = 0;
        
        F_RenderGeometryAllocationStructure_Simple GeometryAllocationStructure;
                
        GeometryAllocationStructure.NumIndices = MeshDataView.GetIndexCount();
        GeometryAllocationStructure.NumVertices = MeshDataView.GetVertexCount();
                
        GeometryAllocationStructure.Indices_LocalOffsetInBytes = SizeInBytes;
        SizeInBytes += AlignAddress_PO2(MeshDataView.GetIndexCount() * sizeof(U32), sizeof(F_Vector4_F32));
                
        GeometryAllocationStructure.Positions_LocalOffsetInBytes = SizeInBytes;
        SizeInBytes += AlignAddress_PO2(MeshDataView.GetVertexCount() * sizeof(F_Vector3_F32), sizeof(F_Vector4_F32));
                
        GeometryAllocationStructure.Normals_LocalOffsetInBytes = SizeInBytes;
        SizeInBytes += AlignAddress_PO2(MeshDataView.GetVertexCount() * sizeof(F_Vector3_F32), sizeof(F_Vector4_F32));
                
        GeometryAllocationStructure.TangentsAndSigns_LocalOffsetInBytes = SizeInBytes;
        SizeInBytes += AlignAddress_PO2(MeshDataView.GetVertexCount() * sizeof(F_Vector4_F32), sizeof(F_Vector4_F32));
                
        GeometryAllocationStructure.UVs_LocalOffsetInBytes = SizeInBytes;
        SizeInBytes += AlignAddress_PO2(MeshDataView.GetVertexCount() * sizeof(F_Vector2_F32), sizeof(F_Vector4_F32));
        
        if (SizeInBytes == 0)
        {
            return false;
        }
        
        if (auto GeometryAllocation = Allocate(SizeInBytes))
        {
            H_RHIPassUtilities::UploadBuffer(
                TF_Span<const U8>(
                    (const U8*)MeshDataView.Indices.data(), 
                    (const U8*)(MeshDataView.Indices.data() + MeshDataView.Indices.size()) 
                ),
                GeometryAllocation->Page->GetRHIBuffer(),
                GeometryAllocation->BeginOffsetInBytes + GeometryAllocationStructure.Indices_LocalOffsetInBytes
            );
            H_RHIPassUtilities::UploadBuffer(
                TF_Span<const U8>(
                    (const U8*)MeshDataView.Positions.data(), 
                    (const U8*)(MeshDataView.Positions.data() + MeshDataView.Positions.size()) 
                ),
                GeometryAllocation->Page->GetRHIBuffer(),
                GeometryAllocation->BeginOffsetInBytes + GeometryAllocationStructure.Positions_LocalOffsetInBytes
            );
            H_RHIPassUtilities::UploadBuffer(
                TF_Span<const U8>(
                    (const U8*)MeshDataView.Normals.data(), 
                    (const U8*)(MeshDataView.Normals.data() + MeshDataView.Normals.size()) 
                ),
                GeometryAllocation->Page->GetRHIBuffer(),
                GeometryAllocation->BeginOffsetInBytes + GeometryAllocationStructure.Normals_LocalOffsetInBytes
            );
            H_RHIPassUtilities::UploadBuffer(
                TF_Span<const U8>(
                    (const U8*)MeshDataView.TangentsAndSigns.data(), 
                    (const U8*)(MeshDataView.TangentsAndSigns.data() + MeshDataView.TangentsAndSigns.size()) 
                ),
                GeometryAllocation->Page->GetRHIBuffer(),
                GeometryAllocation->BeginOffsetInBytes + GeometryAllocationStructure.TangentsAndSigns_LocalOffsetInBytes
            );
            H_RHIPassUtilities::UploadBuffer(
                TF_Span<const U8>(
                    (const U8*)MeshDataView.UVs.data(), 
                    (const U8*)(MeshDataView.UVs.data() + MeshDataView.UVs.size()) 
                ),
                GeometryAllocation->Page->GetRHIBuffer(),
                GeometryAllocation->BeginOffsetInBytes + GeometryAllocationStructure.UVs_LocalOffsetInBytes
            );
            
            OutGeometryAllocation = *GeometryAllocation;
            OutGeometryAllocationStructure = GeometryAllocationStructure;
            return true;
        }
        return false;
    }

    void F_RenderGeometryManager::_RecreateBindGroupIfNeeded()
    {
        if (!_ShouldRecreateBindGroup)
        {
            return;
        }
        _ShouldRecreateBindGroup = false;
        
        _GlobalSRVBindGroup = RenderGeometry::F_GlobalSRVBinding::Instantiate(
            GetRenderRegistryRuntime()    
        ).CreateBindGroup();
        if (_GlobalSRVBindGroup->HasSlot(ABYTEK_NAME("RenderGeometryPages")))
        {
            F_RHIResourceViewSet RenderGeometryPages;
            for (const auto& Page : _Pages)
            {
                RenderGeometryPages.push_back(Page->GetSRV());
            }
            _GlobalSRVBindGroup->BindResourceViewSet(
                ABYTEK_NAME("RenderGeometryPages"),
                RenderGeometryPages
            );
        }
        _GlobalSRVBindGroup->Commit();
        
        _GlobalUAVBindGroup = RenderGeometry::F_GlobalUAVBinding::Instantiate(
            GetRenderRegistryRuntime()    
        ).CreateBindGroup();
        if (_GlobalUAVBindGroup->HasSlot(ABYTEK_NAME("RenderGeometryPages")))
        {
            F_RHIResourceViewSet RenderGeometryPages;
            for (const auto& Page : _Pages)
            {
                RenderGeometryPages.push_back(Page->GetUAV());
            }
            _GlobalUAVBindGroup->BindResourceViewSet(
                ABYTEK_NAME("RenderGeometryPages"),
                RenderGeometryPages
            );
        }
        _GlobalUAVBindGroup->Commit();
    }
}
