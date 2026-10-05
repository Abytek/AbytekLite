module Common;

__exported import Abytek.Renderer.RenderGeometry.Common;

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
        public struct F_Component_GeometryAddress_ECMS
        {
            public F_RenderGeometryAddress Address;
        };
    }
}