#pragma once

#include "Abytek/Engine.NFC.prerequisites.hpp"


namespace Abytek
{
    class F_RenderGeometryPage;
    
    struct F_RenderGeometryAllocation
    {
        TS<F_RenderGeometryPage> Page;
        U64 BeginOffsetInBytes = 0;
        U64 EndOffsetInBytes = 0;
    
        ABYTEK_FORCE_INLINE Sz GetSize() const noexcept
        {
            return EndOffsetInBytes - BeginOffsetInBytes;
        }
        ABYTEK_FORCE_INLINE B8 IsValid() const noexcept
        {
            return Page && GetSize();
        }
        ABYTEK_FORCE_INLINE explicit operator B8 () const noexcept
        {
            return IsValid();
        }
    };
    
    struct ABYTEK_ALIGN(16) F_RenderGeometryAllocationUniformData
    {
        U32 BeginOffsetInBytes = 0;
        U32 EndOffsetInBytes = 0;
        U32 PageIndex = 0;
    
        ABYTEK_FORCE_INLINE Sz GetSize() const noexcept
        {
            return EndOffsetInBytes - BeginOffsetInBytes;
        }
        ABYTEK_FORCE_INLINE B8 IsValid() const noexcept
        {
            return GetSize();
        }
        ABYTEK_FORCE_INLINE explicit operator B8 () const noexcept
        {
            return IsValid();
        }
    };
    
    struct ABYTEK_ALIGN(16) F_RenderGeometryAllocationStructure_Simple
    {
        U32 NumIndices = 0;
        U32 Indices_LocalOffsetInBytes = 0;
        
        U32 NumVertices = 0;
        U32 Positions_LocalOffsetInBytes = 0;
        
        U32 Normals_LocalOffsetInBytes = 0;
        U32 TangentsAndSigns_LocalOffsetInBytes = 0;
        
        U32 UVs_LocalOffsetInBytes = 0;
    };
    struct ABYTEK_ALIGN(16) ABYTEK_ENGINE_NFC_API F_StaticMeshGeometryUniformData_Simple
    {
        F_RenderGeometryAllocationUniformData GeometryAllocation;
        F_RenderGeometryAllocationStructure_Simple GeometryAllocationStructure;
        U32 Index = 0;
        
        static F_StaticMeshGeometryUniformData_Simple Make(
            U32 Index,
            const F_RenderGeometryAllocation& GeometryAllocation,
            const F_RenderGeometryAllocationStructure_Simple& GeometryAllocationStructure
        );
    };
}