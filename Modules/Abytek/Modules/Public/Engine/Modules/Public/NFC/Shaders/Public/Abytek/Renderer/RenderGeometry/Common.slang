module Common;

namespace Abytek
{
    public typealias F_RenderGeometryAddress = uint2;
    
    public struct H_RenderGeometryAddress
    {
        public static void Pack(out F_RenderGeometryAddress OutGeometryAddress, uint PageIndex, uint OffsetInBytes)
        {
            OutGeometryAddress = uint2(PageIndex, OffsetInBytes);
        }
        public static uint GetPageIndex(F_RenderGeometryAddress GeometryAddress)
        {
            return GeometryAddress.x;
        }
        public static uint GetOffsetInBytes(F_RenderGeometryAddress GeometryAddress)
        {
            return GeometryAddress.y;
        }
        public static void Unpack(F_RenderGeometryAddress GeometryAddress, out uint OutPageIndex, out uint OutOffsetInBytes)
        {
            OutPageIndex = GetPageIndex(GeometryAddress);
            OutOffsetInBytes = GetOffsetInBytes(GeometryAddress);
        }
    };

    public struct F_RenderGeometryAllocationStructure_Simple
    {
        public uint NumIndices = 0;
        public uint Indices_LocalOffsetInBytes = 0;

        public uint NumVertices = 0;
        public uint Positions_LocalOffsetInBytes = 0;
        public uint Normals_LocalOffsetInBytes = 0;
        public uint TangentsAndSigns_LocalOffsetInBytes = 0;
        public uint UVs_LocalOffsetInBytes = 0;

        private uint ___Padding___;
    };
    public struct F_StaticMeshGeometryUniformData_Simple
    {
        public F_RenderGeometryAllocationStructure_Simple GeometryAllocationStructure;
        public F_RenderGeometryAddress GeometryAddress;
        public uint Index = 0;
        private uint ___Padding___;
    };
}