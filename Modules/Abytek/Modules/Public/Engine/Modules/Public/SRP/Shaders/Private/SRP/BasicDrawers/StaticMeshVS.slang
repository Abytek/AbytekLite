module StaticMeshVS;

import SRP.BasicDrawers.StaticMeshCommon;
import NFC.RenderView;
import NFC.RenderGeometry.Common;
import NFC.RenderGeometry.DeclarePages;
import NFC.RenderGeometry.Access;

ConstantBuffer<NFC::RenderView::F_UniformData> ViewUniformData;
ConstantBuffer<SRP::BasicDrawers::StaticMesh::F_UniformData> StaticMeshUniformData;
ConstantBuffer<NFC::RenderGeometry::F_StaticMeshGeometryUniformData_Simple> StaticMeshGeometryUniformData_Simple;

[shader("vertex")]
SRP::BasicDrawers::StaticMesh::F_PSInput MainVS(SRP::BasicDrawers::StaticMesh::F_VSInput VSInput)
{
    uint VertexIndex = NFC::RenderGeometry::SimpleMesh_LoadVertexIndex(
        StaticMeshGeometryUniformData_Simple.GeometryAllocation,
        StaticMeshGeometryUniformData_Simple.GeometryAllocationStructure,
        VSInput.VertexId
    );
    float3 VertexPosition = NFC::RenderGeometry::SimpleMesh_LoadVertexPosition(
        StaticMeshGeometryUniformData_Simple.GeometryAllocation,
        StaticMeshGeometryUniformData_Simple.GeometryAllocationStructure,
        VertexIndex
    );
    float3 VertexNormal = NFC::RenderGeometry::SimpleMesh_LoadVertexNormal(
        StaticMeshGeometryUniformData_Simple.GeometryAllocation,
        StaticMeshGeometryUniformData_Simple.GeometryAllocationStructure,
        VertexIndex
    );
    float2 VertexUV = NFC::RenderGeometry::SimpleMesh_LoadVertexUV(
        StaticMeshGeometryUniformData_Simple.GeometryAllocation,
        StaticMeshGeometryUniformData_Simple.GeometryAllocationStructure,
        VertexIndex
    );

    float3 LocalPosition = VertexPosition;
    float3 LocalNormal = VertexNormal;

    float3 WorldPosition = mul(StaticMeshUniformData.TransformMatrix, float4(LocalPosition, 1.0f)).xyz;
    float3 WorldNormal = mul((float3x3)StaticMeshUniformData.InverseTransposeTransformMatrix, LocalNormal);
    
    float3 ViewSpacePosition = mul(ViewUniformData.ViewMatrix, float4(WorldPosition, 1.0f)).xyz;
    float4 ClipPosition = mul(ViewUniformData.ProjectionMatrix, float4(ViewSpacePosition, 1.0f));
    ClipPosition.z = ClipPosition.w - ClipPosition.z;

    SRP::BasicDrawers::StaticMesh::F_PSInput PSInput;
    PSInput.ClipPosition = ClipPosition;
    PSInput.WorldPosition = WorldPosition;
    PSInput.WorldNormal = WorldNormal;
    PSInput.UV = VertexUV;
    PSInput.TriangleId = VSInput.VertexId / 3;
    return PSInput;
}