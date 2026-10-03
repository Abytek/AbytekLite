module StaticMeshVS_Simple;

import Abytek.SRPBasicDrawers.StaticMeshCommon;
import Abytek.Renderer.RenderView;
import Abytek.Renderer.RenderGeometry.Common;
import Abytek.Renderer.RenderGeometry.DeclarePages;
import Abytek.Renderer.RenderGeometry.Access;
import Abytek.ClipPositionDepthReverse;
import Abytek.ClipPositionToNDCPosition;

ConstantBuffer<Abytek::F_RenderViewUniformData> ViewUniformData;
ConstantBuffer<Abytek::F_SRPBasicStaticMeshUniformData> StaticMeshUniformData;
ConstantBuffer<Abytek::F_StaticMeshGeometryUniformData_Simple> StaticMeshGeometryUniformData_Simple;

[shader("vertex")]
Abytek::F_SRPBasicStaticMeshVSOutput MainVS(Abytek::F_SRPBasicStaticMeshVSInput_Simple VSInput)
{
    uint VertexIndex = Abytek::H_SimpleMesh::LoadVertexIndex(
        StaticMeshGeometryUniformData_Simple.GeometryAddress,
        StaticMeshGeometryUniformData_Simple.GeometryAllocationStructure,
        VSInput.VertexId
    );
    float3 VertexPosition = Abytek::H_SimpleMesh::LoadVertexPosition(
        StaticMeshGeometryUniformData_Simple.GeometryAddress,
        StaticMeshGeometryUniformData_Simple.GeometryAllocationStructure,
        VertexIndex
    );
    float3 VertexNormal = Abytek::H_SimpleMesh::LoadVertexNormal(
        StaticMeshGeometryUniformData_Simple.GeometryAddress,
        StaticMeshGeometryUniformData_Simple.GeometryAllocationStructure,
        VertexIndex
    );
    float2 VertexUV = Abytek::H_SimpleMesh::LoadVertexUV(
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
    ClipPosition = Abytek::ClipPositionDepthReverse(ClipPosition);

    Abytek::F_SRPBasicStaticMeshVSOutput VSOutput;
    VSOutput.ClipOrPixelPosition = ClipPosition;
    VSOutput.NDCPosition = Abytek::ClipPositionToNDCPosition(ClipPosition);
    VSOutput.WorldPosition = WorldPosition;
    VSOutput.WorldNormal = WorldNormal;
    VSOutput.UV = VertexUV;
    return VSOutput;
}