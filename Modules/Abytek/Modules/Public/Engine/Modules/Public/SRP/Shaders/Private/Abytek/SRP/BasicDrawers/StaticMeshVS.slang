module StaticMeshVS;

import Abytek.SRP.BasicDrawers.StaticMeshCommon;
import Abytek.Renderer.RenderView;
import Abytek.Renderer.RenderGeometry.Common;
import Abytek.Renderer.RenderGeometry.DeclarePages;
import Abytek.Renderer.RenderGeometry.Access;

ConstantBuffer<Abytek::F_RenderViewUniformData> ViewUniformData;
ConstantBuffer<Abytek::F_SRPBasicStaticMeshUniformData> StaticMeshUniformData;
ConstantBuffer<Abytek::F_StaticMeshGeometryUniformData_Simple> StaticMeshGeometryUniformData_Simple;

[shader("vertex")]
Abytek::F_SRPBasicStaticMeshPSInput MainVS(Abytek::F_SRPBasicStaticMeshVSInput VSInput)
{
    uint VertexIndex = Abytek::SimpleMesh_LoadVertexIndex(
        StaticMeshGeometryUniformData_Simple.GeometryAddress,
        StaticMeshGeometryUniformData_Simple.GeometryAllocationStructure,
        VSInput.VertexId
    );
    float3 VertexPosition = Abytek::SimpleMesh_LoadVertexPosition(
        StaticMeshGeometryUniformData_Simple.GeometryAddress,
        StaticMeshGeometryUniformData_Simple.GeometryAllocationStructure,
        VertexIndex
    );
    float3 VertexNormal = Abytek::SimpleMesh_LoadVertexNormal(
        StaticMeshGeometryUniformData_Simple.GeometryAddress,
        StaticMeshGeometryUniformData_Simple.GeometryAllocationStructure,
        VertexIndex
    );
    float2 VertexUV = Abytek::SimpleMesh_LoadVertexUV(
        StaticMeshGeometryUniformData_Simple.GeometryAddress,
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

    Abytek::F_SRPBasicStaticMeshPSInput PSInput;
    PSInput.ClipPosition = ClipPosition;
    PSInput.WorldPosition = WorldPosition;
    PSInput.WorldNormal = WorldNormal;
    PSInput.UV = VertexUV;
    PSInput.TriangleId = VSInput.VertexId / 3;
    return PSInput;
}