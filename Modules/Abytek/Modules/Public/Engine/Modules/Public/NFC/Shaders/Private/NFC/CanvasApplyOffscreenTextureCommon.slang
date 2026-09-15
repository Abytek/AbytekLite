module CanvasApplyOffscreenTextureCommon;

namespace CanvasApplyOffscreenTexture
{
    public struct F_VSInput
    {
        public uint VertexId : SV_VertexID;
    };

    public struct F_PSInput
    {
        public float4 ClipPosition : SV_Position;
        public float2 Texcoord : TEXCOORD;
    };
}