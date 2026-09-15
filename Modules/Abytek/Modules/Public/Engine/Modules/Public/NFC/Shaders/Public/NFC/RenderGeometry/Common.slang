module Common;

namespace NFC::RenderGeometry
{
    public struct F_GeometryAllocation
    {
        public uint BeginOffsetInBytes = 0;
        public uint EndOffsetInBytes = 0;
        public uint PageIndex = 0;

        private uint ___Padding___;
    };

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
        public F_GeometryAllocation GeometryAllocation;
        public F_GeometryAllocationStructure_Simple GeometryAllocationStructure;
        public uint Index = 0;

        private uint3 ___Padding___;
    };
}