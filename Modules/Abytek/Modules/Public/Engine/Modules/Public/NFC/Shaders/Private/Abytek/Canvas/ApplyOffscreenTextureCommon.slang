module ApplyOffscreenTextureCommon;

namespace Abytek
{
    public struct F_CanvasApplyOffscreenTextureVSInput
    {
        public uint VertexId : SV_VertexID;
    };

    public struct F_CanvasApplyOffscreenTexturePSInput
    {
        public float4 ClipPosition : SV_Position;
        public float2 Texcoord : TEXCOORD;
    };
}