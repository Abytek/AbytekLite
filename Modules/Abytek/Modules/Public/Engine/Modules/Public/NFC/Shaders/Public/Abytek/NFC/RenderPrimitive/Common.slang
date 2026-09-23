module Common;

namespace Abytek::NFC::RenderPrimitive
{
    public typealias F_MeshType = uint8_t;
    public typealias F_MeshId = uint2;
    public typealias F_MeshHandle = uint2;

    public struct F_Component_Transform
    {
        public float4x4 Matrix;
    };
    public struct F_Component_InverseTransposeTransform
    {
        public float4x4 Matrix;
    };
    public struct F_Component_MeshHandle
    {
        public F_MeshHandle Handle;
    };
}