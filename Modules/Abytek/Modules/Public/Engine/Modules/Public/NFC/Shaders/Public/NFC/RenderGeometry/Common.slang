module Common;

namespace NFC::RenderGeometry
{
    public typealias F_GeometryAddress = uint2;
    public void PackGeometryAddress(out F_GeometryAddress OutGeometryAddress, uint PageIndex, uint OffsetInBytes)
    {
        OutGeometryAddress = uint2(PageIndex, OffsetInBytes);
    }
    public uint GetPageIndexFromGeometryAddress(F_GeometryAddress GeometryAddress)
    {
        return GeometryAddress.x;
    }
    public uint GetOffsetInBytesFromGeometryAddress(F_GeometryAddress GeometryAddress)
    {
        return GeometryAddress.y;
    }
    public void UnpackGeometryAddress(F_GeometryAddress GeometryAddress, out uint OutPageIndex, out uint OutOffsetInBytes)
    {
        OutPageIndex = GetPageIndexFromGeometryAddress(GeometryAddress);
        OutOffsetInBytes = GetOffsetInBytesFromGeometryAddress(GeometryAddress);
    }

    public struct F_GeometryAllocationStructure_Simple
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
        public F_GeometryAllocationStructure_Simple GeometryAllocationStructure;
        public F_GeometryAddress GeometryAddress;
        public uint Index = 0;
        private uint ___Padding___;
    };
}