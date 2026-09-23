#include "Abytek/Renderer/RenderGeometry/RenderGeometryStorage.hpp"
#include "Abytek/Renderer/RenderGeometry/RenderGeometryPage.hpp"
#include "Abytek/Renderer/RenderScene.hpp"


namespace Abytek
{
    void F_RenderGeometryStorage::Init(
        const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer,
        const F_RenderGeometryStorageBuildParams& BuildParams
    )
    {
        InitMinimal(SubmissionItemContainer);
        
        _Scene = BuildParams.Scene;
        
        _CapacityLimitInBytes = U64(1024) * U64(1024) * U64(1024);
        _MinimalPageSizeInBytes = U64(32) * U64(1024) * U64(1024);
        _NextPageSizeInBytes = _MinimalPageSizeInBytes;
        
        AddNewPage(SubmissionItemContainer, 0);
        _RecreateBindGroupIfNeeded(SubmissionItemContainer);
    }
    void F_RenderGeometryStorage::Release(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
        _GlobalUAVBindGroup = {};
        _GlobalSRVBindGroup = {};
        
        for (const auto& Page : _Pages)
        {
            Page->Release(SubmissionItemContainer);
        }
        _Pages = {};
        
        _Scene = {};
        
        A_RenderObject::Release(SubmissionItemContainer);
    }

    void F_RenderGeometryStorage::BeginUpdate(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
    }
    void F_RenderGeometryStorage::EndUpdate(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
    }
    void F_RenderGeometryStorage::BeginPostUpdate(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
        _RecreateBindGroupIfNeeded(SubmissionItemContainer);
        _FlushDeallocationQueue(SubmissionItemContainer);
    }
    void F_RenderGeometryStorage::EndPostUpdate(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
    }

    void F_RenderGeometryStorage::_FlushDeallocationQueue(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
    {
        F_RenderGeometryAllocation Allocation;
        while (_DeallocationQueue.TryPop(Allocation))
        {
            Allocation.Page->Deallocate(Allocation);
        }
    }

    TF_Optional<F_RenderGeometryAllocation> F_RenderGeometryStorage::Allocate(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer, U64 SizeInBytes, U64 AlignmentInBytes)
    {
        TF_Optional<F_RenderGeometryAllocation> Result;
        _CriticalSection(
            [this, &SubmissionItemContainer, &Result, SizeInBytes, AlignmentInBytes]
            {
                for (const auto& Page : _Pages)
                {
                    if (auto Allocation = Page->Allocate(SizeInBytes, AlignmentInBytes))
                    {
                        Result = ABYTEK_MOVE(Allocation);
                    }
                }
                if (auto Page = AddNewPage(SubmissionItemContainer, SizeInBytes + AlignmentInBytes))
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
    TS<F_RenderGeometryPage> F_RenderGeometryStorage::AddNewPage(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer, U64 SizeInBytes)
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
        
#ifdef ABYTEK_DEBUG_INFO
        auto Page = F_RenderGeometryPage::CreateAndInit_WithDebugName(
            *GetDebugName()
            + ABYTEK_TEXT(".Pages[") 
            + ToText(_Pages.size()) 
            + ABYTEK_TEXT("]"),
#else
        auto Page = F_RenderGeometryPage::CreateAndInit(
#endif
            GetWorldRenderResource(),
            SubmissionItemContainer,
            ABYTEK_WTHIS(),
            static_cast<U32>(_Pages.size()),
            NewPageSizeInBytes
        );
        _Pages.push_back(Page);
        return Page;
    }

    B8 F_RenderGeometryStorage::AddMeshData_Simple(
        const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer,
        const F_SimpleMeshDataROView& MeshDataView,
        F_RenderGeometryAllocation& OutGeometryAllocation,
        F_RenderGeometryAllocationStructure_Simple& OutGeometryAllocationStructure
    )
    {
        ABYTEK_RHI_CAPTURE_EVENT_SCOPE(
            SubmissionItemContainer,
            ABYTEK_NAME("Abytek::F_RenderGeometryStorage::AddMeshData_Simple")
        );
        
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
        
        if (auto GeometryAllocation = Allocate(SubmissionItemContainer, SizeInBytes))
        {
            H_RHISubmissionUtilities::UploadBuffer(
                SubmissionItemContainer,
                TF_Span<const U8>(
                    (const U8*)MeshDataView.Indices.data(), 
                    (const U8*)(MeshDataView.Indices.data() + MeshDataView.Indices.size()) 
                ),
                GeometryAllocation->Page->GetRHIBuffer(),
                GeometryAllocation->BeginOffsetInBytes + GeometryAllocationStructure.Indices_LocalOffsetInBytes,
                ABYTEK_NAME("Indices")
            );
            H_RHISubmissionUtilities::UploadBuffer(
                SubmissionItemContainer,
                TF_Span<const U8>(
                    (const U8*)MeshDataView.Positions.data(), 
                    (const U8*)(MeshDataView.Positions.data() + MeshDataView.Positions.size()) 
                ),
                GeometryAllocation->Page->GetRHIBuffer(),
                GeometryAllocation->BeginOffsetInBytes + GeometryAllocationStructure.Positions_LocalOffsetInBytes,
                ABYTEK_NAME("Positions")
            );
            H_RHISubmissionUtilities::UploadBuffer(
                SubmissionItemContainer,
                TF_Span<const U8>(
                    (const U8*)MeshDataView.Normals.data(), 
                    (const U8*)(MeshDataView.Normals.data() + MeshDataView.Normals.size()) 
                ),
                GeometryAllocation->Page->GetRHIBuffer(),
                GeometryAllocation->BeginOffsetInBytes + GeometryAllocationStructure.Normals_LocalOffsetInBytes,
                ABYTEK_NAME("Normals")
            );
            H_RHISubmissionUtilities::UploadBuffer(
                SubmissionItemContainer,
                TF_Span<const U8>(
                    (const U8*)MeshDataView.TangentsAndSigns.data(), 
                    (const U8*)(MeshDataView.TangentsAndSigns.data() + MeshDataView.TangentsAndSigns.size()) 
                ),
                GeometryAllocation->Page->GetRHIBuffer(),
                GeometryAllocation->BeginOffsetInBytes + GeometryAllocationStructure.TangentsAndSigns_LocalOffsetInBytes,
                ABYTEK_NAME("TangentsAndSigns")
            );
            H_RHISubmissionUtilities::UploadBuffer(
                SubmissionItemContainer,
                TF_Span<const U8>(
                    (const U8*)MeshDataView.UVs.data(), 
                    (const U8*)(MeshDataView.UVs.data() + MeshDataView.UVs.size()) 
                ),
                GeometryAllocation->Page->GetRHIBuffer(),
                GeometryAllocation->BeginOffsetInBytes + GeometryAllocationStructure.UVs_LocalOffsetInBytes,
                ABYTEK_NAME("UVs")
            );
            
            OutGeometryAllocation = *GeometryAllocation;
            OutGeometryAllocationStructure = GeometryAllocationStructure;
            return true;
        }
        return false;
    }
    void F_RenderGeometryStorage::RemoveMeshData_Simple(
        const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer,
        const F_RenderGeometryAllocation& GeometryAllocation
    )
    {
        _DeallocationQueue.Push(GeometryAllocation);
    }

    void F_RenderGeometryStorage::_RecreateBindGroupIfNeeded(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer)
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
#ifdef ABYTEK_DEBUG_INFO
        _GlobalSRVBindGroup->SetDebugName(*GetDebugName() + ABYTEK_TEXT(".GlobalSRVBindGroup"));
#endif
        
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
#ifdef ABYTEK_DEBUG_INFO
        _GlobalUAVBindGroup->SetDebugName(*GetDebugName() + ABYTEK_TEXT(".GlobalUAVBindGroup"));
#endif
    }
}
