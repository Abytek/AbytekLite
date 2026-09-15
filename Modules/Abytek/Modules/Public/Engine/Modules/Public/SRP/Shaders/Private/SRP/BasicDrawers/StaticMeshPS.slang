module StaticMeshPS;

import SRP.BasicDrawers.StaticMeshCommon;

ConstantBuffer<SRP::BasicDrawers::StaticMesh::F_UniformData> StaticMeshUniformData;

[shader("fragment")]
float4 MainPS(SRP::BasicDrawers::StaticMesh::F_PSInput PSInput) : SV_Target0
{
    float3 Lighting = lerp(
        float3(0.3f, 0.3f, 0.3f),
        float3(0.84f, 0.84f, 0.84f),
        float3(
            max(
                dot(PSInput.WorldNormal, -normalize(float3(-0.3f, -1.0f, 0.2f))),
                0.0f
            )
        )
    );
    float3 OutRadiance = StaticMeshUniformData.Color.xyz * Lighting;
    return float4(OutRadiance, 1.0f);
}