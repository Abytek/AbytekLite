#pragma once

#include "Abytek/Engine.Assimp.prerequisites.pch.hpp"


namespace Abytek
{
    struct F_AssimpSimpleMeshData
    {
        TF_Vector<U32> Indices;
        TF_Vector<F_Vector3_F32> Positions;
        TF_Vector<F_Vector3_F32> Normals;
        TF_Vector<F_Vector4_F32> TangentsAndSigns;
        TF_Vector<F_Vector2_F32> UVs;
        U32 GetIndexCount() const
        {
            return Indices.size();
        }
        U32 GetVertexCount() const
        {
            return Positions.size();
        }
    };
    struct ABYTEK_ENGINE_ASSIMP_API H_Assimp
    {
        static F_FeedbackStatus Decode(const TF_Span<const U8>& Bytes, TF_Vector<F_AssimpSimpleMeshData>& OutDataList);
    };
}