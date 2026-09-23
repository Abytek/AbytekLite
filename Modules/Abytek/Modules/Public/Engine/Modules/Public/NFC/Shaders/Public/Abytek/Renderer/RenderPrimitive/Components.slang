module Common;

__exported import Abytek.Renderer.RenderMesh.Handle;

namespace Abytek
{
    namespace RenderPrimitive
    {
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
            public F_RenderMeshHandle Handle;
        };
    }
}