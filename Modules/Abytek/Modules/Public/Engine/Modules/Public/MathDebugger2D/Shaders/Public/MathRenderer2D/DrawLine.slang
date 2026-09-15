module DrawLine;

__exported import MathRenderer2D.Common;

namespace MathRenderer2D
{
    public struct F_DrawLineUniformData
    {
        public F_SpaceUniformData Space;
        public F_ViewUniformData View;
        public F_Line Line;
    };

    public struct F_VSInput
    {
        public uint VertexId : SV_VertexID;
    };

    public struct F_PSInput
    {
        public float4 ClipPosition : SV_Position;
    };
}