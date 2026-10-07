#pragma once

#include "Abytek/Engine.NFC.prerequisites.hpp"
#include "Abytek/Assets/SimpleMeshData.hpp"
#include "meshoptimizer.h"


namespace Abytek
{
    using F_ECMSLocalVertexIndex = U8;
    using F_ECMSGlobalVertexIndex = U32;
    
    using F_ECMSMeshTriangle = U32;
    struct H_ECMSMeshTriangle
    {
        static void Pack(F_ECMSMeshTriangle& OutMeshTriangle, F_ECMSLocalVertexIndex Index0, F_ECMSMeshTriangle Index1, F_ECMSMeshTriangle Index2)
        {
            OutMeshTriangle = (
                (F_ECMSMeshTriangle(Index0) << F_ECMSMeshTriangle(0))
                | (F_ECMSMeshTriangle(Index1) << F_ECMSMeshTriangle(8))
                | (F_ECMSMeshTriangle(Index2) << F_ECMSMeshTriangle(16))
            );
        }
        static void Unpack(F_ECMSMeshTriangle MeshTriangle, F_ECMSLocalVertexIndex& OutIndex0, F_ECMSMeshTriangle& OutIndex1, F_ECMSMeshTriangle& OutIndex2)
        {
            OutIndex0 = F_ECMSLocalVertexIndex(
                (MeshTriangle >> F_ECMSMeshTriangle(0)) 
                & F_ECMSMeshTriangle(0xFF)
            );
            OutIndex1 = F_ECMSLocalVertexIndex(
                (MeshTriangle >> F_ECMSMeshTriangle(8)) 
                & F_ECMSMeshTriangle(0xFF)
            );
            OutIndex2 = F_ECMSLocalVertexIndex(
                (MeshTriangle >> F_ECMSMeshTriangle(16)) 
                & F_ECMSMeshTriangle(0xFF)
            );
        }
    };
    
    static constexpr U32 ECMS_MAX_VERTICES_PER_MESHLET = 64;
    static constexpr U32 ECMS_MAX_PRIMITIVES_PER_MESHLET = 126;
    static constexpr U32 ECMS_MAX_INDICES_PER_MESHLET = ECMS_MAX_PRIMITIVES_PER_MESHLET * 3;
    
    struct ABYTEK_ALIGN(16) F_ECMSMeshlet
    {
        U32 Offset_Triangles = 0;
        U32 Count_Triangles = 0;
        U32 Offset_VertexIndices = 0;
        U32 Count_VertexIndices = 0;
        
        friend F_FeedbackStatus operator << (F_ArchiveReadWriteView& View, const F_ECMSMeshlet& Value)
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.Offset_Triangles);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.Count_Triangles);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.Offset_VertexIndices);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.Count_VertexIndices);
            return F_FeedbackStatus::MakeSucceeded();   
        }
        friend F_FeedbackStatus operator >> (F_ArchiveReadOnlyView& View, F_ECMSMeshlet& Value)
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.Offset_Triangles);
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.Count_Triangles);
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.Offset_VertexIndices);
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.Count_VertexIndices);
            return F_FeedbackStatus::MakeSucceeded();   
        }
    };
    
    template<class __F_Allocator = TF_DefaultAllocator<U8>>
    struct TF_ECMSMeshData;
    
    template<B8 __EnableWrite>
    struct TF_ECMSMeshDataView;
    
    template<class __F_Allocator>
    struct TF_ECMSMeshData
    {
        using F_Allocator = __F_Allocator;
        
        ContainerTemplates::TF_Vector<F_ECMSMeshlet, TF_RebindAllocator<F_ECMSMeshlet, F_Allocator>> Meshlets;
        ContainerTemplates::TF_Vector<F_ECMSMeshTriangle, TF_RebindAllocator<F_ECMSMeshTriangle, F_Allocator>> Triangles;
        ContainerTemplates::TF_Vector<F_ECMSGlobalVertexIndex, TF_RebindAllocator<F_ECMSGlobalVertexIndex, F_Allocator>> VertexIndices;
        ContainerTemplates::TF_Vector<F_Vector3_F32, TF_RebindAllocator<F_Vector3_F32, F_Allocator>> Positions;
        ContainerTemplates::TF_Vector<F_Vector3_F32, TF_RebindAllocator<F_Vector3_F32, F_Allocator>> Normals;
        ContainerTemplates::TF_Vector<F_Vector4_F32, TF_RebindAllocator<F_Vector4_F32, F_Allocator>> TangentsAndSigns;
        ContainerTemplates::TF_Vector<F_Vector2_F32, TF_RebindAllocator<F_Vector2_F32, F_Allocator>> UVs;
        
        ABYTEK_FORCE_INLINE auto GetMeshletCount() const noexcept
        {
            return Meshlets.size();
        }
        ABYTEK_FORCE_INLINE auto GetTriangleCount() const noexcept
        {
            return Triangles.size();
        }
        ABYTEK_FORCE_INLINE auto GetVertexIndexCount() const noexcept
        {
            return VertexIndices.size();
        }
        ABYTEK_FORCE_INLINE auto GetVertexCount() const noexcept
        {
            return Positions.size();
        }
        
        TF_ECMSMeshData() = default;
        TF_ECMSMeshData(const TF_ECMSMeshData& X) = default;
        TF_ECMSMeshData& operator = (const TF_ECMSMeshData& X) = default;
        TF_ECMSMeshData(TF_ECMSMeshData&& X) = default;
        TF_ECMSMeshData& operator = (TF_ECMSMeshData&& X) = default;
        
        template<B8 __EnableWrite2>
        TF_ECMSMeshData(const TF_ECMSMeshDataView<__EnableWrite2>& X);
        template<B8 __EnableWrite2>
        TF_ECMSMeshData& operator = (const TF_ECMSMeshDataView<__EnableWrite2>& X);
        
        friend F_FeedbackStatus operator << (F_ArchiveReadWriteView& View, const TF_ECMSMeshData& Value)
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.Meshlets);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.Triangles);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.VertexIndices);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.Positions);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.Normals);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.TangentsAndSigns);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.UVs);
            return F_FeedbackStatus::MakeSucceeded();   
        }
        friend F_FeedbackStatus operator >> (F_ArchiveReadOnlyView& View, TF_ECMSMeshData& Value)
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.Meshlets);
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.Triangles);
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.VertexIndices);
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.Positions);
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.Normals);
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.TangentsAndSigns);
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.UVs);
            return F_FeedbackStatus::MakeSucceeded();   
        }
        
        static TF_ECMSMeshData From(const F_SimpleMeshDataROView& SimpleMeshDataView)
        {
            TF_ECMSMeshData Result;
            
            const Sz MaxNumVerticesPerMeshlet  = ECMS_MAX_VERTICES_PER_MESHLET;
            const Sz MaxNumTrianglesPerMeshlet = ECMS_MAX_PRIMITIVES_PER_MESHLET;
            const float ConeWeight    = 0.0f;

            Sz MaxNumMeshlets = meshopt_buildMeshletsBound(
                SimpleMeshDataView.GetIndexCount(),
                MaxNumVerticesPerMeshlet,
                MaxNumTrianglesPerMeshlet
            );

            ContainerTemplates::TF_Vector<
                meshopt_Meshlet,
                TF_RebindAllocator<meshopt_Meshlet, F_Allocator>
            > Meshlets_meshopt(MaxNumMeshlets);

            ContainerTemplates::TF_Vector<
                unsigned int,
                TF_RebindAllocator<unsigned int, F_Allocator>
            > MeshletVertices_meshopt(SimpleMeshDataView.GetIndexCount());

            ContainerTemplates::TF_Vector<
                unsigned char,
                TF_RebindAllocator<unsigned char, F_Allocator>
            > MeshletTriangleIndices_meshopt(SimpleMeshDataView.GetIndexCount());

            Sz NumMeshlets = meshopt_buildMeshlets(
                Meshlets_meshopt.data(),
                MeshletVertices_meshopt.data(),
                MeshletTriangleIndices_meshopt.data(),

                SimpleMeshDataView.Indices.data(),
                SimpleMeshDataView.Indices.size(),

                &SimpleMeshDataView.Positions[0].X,
                SimpleMeshDataView.GetVertexCount(),
                sizeof(F_Vector3_F32),

                MaxNumVerticesPerMeshlet,
                MaxNumTrianglesPerMeshlet,
                ConeWeight
            );
            
            Result.Positions = TF_RawForward<decltype(Result.Positions)>(
                SimpleMeshDataView.Positions.begin(),    
                SimpleMeshDataView.Positions.end()    
            );
            Result.Normals = TF_RawForward<decltype(Result.Normals)>(
                SimpleMeshDataView.Normals.begin(),    
                SimpleMeshDataView.Normals.end()    
            );
            Result.TangentsAndSigns = TF_RawForward<decltype(Result.TangentsAndSigns)>(
                SimpleMeshDataView.TangentsAndSigns.begin(),    
                SimpleMeshDataView.TangentsAndSigns.end()    
            );
            Result.UVs = TF_RawForward<decltype(Result.UVs)>(
                SimpleMeshDataView.UVs.begin(),    
                SimpleMeshDataView.UVs.end()    
            );
            
            for (U32 Idx = 0; Idx < (MeshletTriangleIndices_meshopt.size() / 3); ++Idx)
            {
                U8 Index0 = MeshletTriangleIndices_meshopt[Idx * 3 + 0];
                U8 Index1 = MeshletTriangleIndices_meshopt[Idx * 3 + 1];
                U8 Index2 = MeshletTriangleIndices_meshopt[Idx * 3 + 2];
                F_ECMSMeshTriangle Triangle;
                H_ECMSMeshTriangle::Pack(
                    Triangle,
                    Index0, 
                    Index1, 
                    Index2
                );
                Result.Triangles.push_back(Triangle);
            }
            Result.VertexIndices = ABYTEK_MOVE(MeshletVertices_meshopt);
            
            for (Sz MeshletIndex = 0; MeshletIndex < NumMeshlets; ++MeshletIndex)
            {
                const auto& Meshlet_meshopt = Meshlets_meshopt[MeshletIndex];
                
                F_ECMSMeshlet Meshlet;
                Meshlet.Offset_Triangles = Meshlet_meshopt.triangle_offset / 3;
                Meshlet.Count_Triangles = Meshlet_meshopt.triangle_count;
                Meshlet.Offset_VertexIndices = Meshlet_meshopt.vertex_offset;
                Meshlet.Count_VertexIndices = Meshlet_meshopt.vertex_count;
                Result.Meshlets.push_back(Meshlet);
            }
            return ABYTEK_MOVE(Result);
        }
    };
    
    template<>
    struct TF_ECMSMeshDataView<false>
    {
        static constexpr B8 EnableWrite = false;
        
        TF_Span<
            std::conditional_t<
                EnableWrite,
                F_ECMSMeshlet,
                const F_ECMSMeshlet
            >    
        > Meshlets;
        TF_Span<
            std::conditional_t<
                EnableWrite,
                F_ECMSMeshTriangle,
                const F_ECMSMeshTriangle
            >    
        > Triangles;
        TF_Span<
            std::conditional_t<
                EnableWrite,
                F_ECMSGlobalVertexIndex,
                const F_ECMSGlobalVertexIndex
            >    
        > VertexIndices;
        TF_Span<
            std::conditional_t<
                EnableWrite,
                F_Vector3_F32,
                const F_Vector3_F32
            >    
        > Positions;
        TF_Span<
            std::conditional_t<
                EnableWrite,
                F_Vector3_F32,
                const F_Vector3_F32
            >    
        > Normals;
        TF_Span<
            std::conditional_t<
                EnableWrite,
                F_Vector4_F32,
                const F_Vector4_F32
            >    
        > TangentsAndSigns;
        TF_Span<
            std::conditional_t<
                EnableWrite,
                F_Vector2_F32,
                const F_Vector2_F32
            >    
        > UVs;
        
        ABYTEK_FORCE_INLINE auto GetMeshletCount() const noexcept
        {
            return Meshlets.size();
        }
        ABYTEK_FORCE_INLINE auto GetTriangleCount() const noexcept
        {
            return Triangles.size();
        }
        ABYTEK_FORCE_INLINE auto GetVertexIndexCount() const noexcept
        {
            return VertexIndices.size();
        }
        ABYTEK_FORCE_INLINE auto GetVertexCount() const noexcept
        {
            return Positions.size();
        }
        
        TF_ECMSMeshDataView() = default;
        TF_ECMSMeshDataView(const TF_ECMSMeshDataView& X) = default;
        TF_ECMSMeshDataView& operator = (const TF_ECMSMeshDataView& X) = default;
        TF_ECMSMeshDataView(TF_ECMSMeshDataView&& X) = default;
        TF_ECMSMeshDataView& operator = (TF_ECMSMeshDataView&& X) = default;
        
        template<typename __F_Allocator2>
        TF_ECMSMeshDataView(const TF_ECMSMeshData<__F_Allocator2>& X);
        template<typename __F_Allocator2>
        TF_ECMSMeshDataView& operator = (const TF_ECMSMeshData<__F_Allocator2>& X);
        
        TF_ECMSMeshDataView<false> GetReadOnly() const noexcept;
        TF_ECMSMeshDataView<true> GetReadWrite() const noexcept;
        
        friend F_FeedbackStatus operator << (F_ArchiveReadWriteView& View, const TF_ECMSMeshDataView& Value)
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.Meshlets);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.Triangles);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.VertexIndices);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.Positions);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.Normals);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.TangentsAndSigns);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.UVs);
            return F_FeedbackStatus::MakeSucceeded();   
        }
    };
    
    template<>
    struct TF_ECMSMeshDataView<true>
    {
        static constexpr B8 EnableWrite = true;
        
        TF_Span<
            std::conditional_t<
                EnableWrite,
                F_ECMSMeshlet,
                const F_ECMSMeshlet
            >    
        > Meshlets;
        TF_Span<
            std::conditional_t<
                EnableWrite,
                F_ECMSMeshTriangle,
                const F_ECMSMeshTriangle
            >    
        > Triangles;
        TF_Span<
            std::conditional_t<
                EnableWrite,
                F_ECMSGlobalVertexIndex,
                const F_ECMSGlobalVertexIndex
            >    
        > VertexIndices;
        TF_Span<
            std::conditional_t<
                EnableWrite,
                F_Vector3_F32,
                const F_Vector3_F32
            >    
        > Positions;
        TF_Span<
            std::conditional_t<
                EnableWrite,
                F_Vector3_F32,
                const F_Vector3_F32
            >    
        > Normals;
        TF_Span<
            std::conditional_t<
                EnableWrite,
                F_Vector4_F32,
                const F_Vector4_F32
            >    
        > TangentsAndSigns;
        TF_Span<
            std::conditional_t<
                EnableWrite,
                F_Vector2_F32,
                const F_Vector2_F32
            >    
        > UVs;
        
        ABYTEK_FORCE_INLINE auto GetMeshletCount() const noexcept
        {
            return Meshlets.size();
        }
        ABYTEK_FORCE_INLINE auto GetTriangleCount() const noexcept
        {
            return Triangles.size();
        }
        ABYTEK_FORCE_INLINE auto GetVertexIndexCount() const noexcept
        {
            return VertexIndices.size();
        }
        ABYTEK_FORCE_INLINE auto GetVertexCount() const noexcept
        {
            return Positions.size();
        }
        
        TF_ECMSMeshDataView() = default;
        TF_ECMSMeshDataView(const TF_ECMSMeshDataView& X) = default;
        TF_ECMSMeshDataView& operator = (const TF_ECMSMeshDataView& X) = default;
        TF_ECMSMeshDataView(TF_ECMSMeshDataView&& X) = default;
        TF_ECMSMeshDataView& operator = (TF_ECMSMeshDataView&& X) = default;
        
        template<typename __F_Allocator2>
        TF_ECMSMeshDataView(TF_ECMSMeshData<__F_Allocator2>& X);
        template<typename __F_Allocator2>
        TF_ECMSMeshDataView& operator = (TF_ECMSMeshData<__F_Allocator2>& X);
        
        TF_ECMSMeshDataView<false> GetReadOnly() const noexcept;
        TF_ECMSMeshDataView<true> GetReadWrite() const noexcept;
        
        friend F_FeedbackStatus operator << (F_ArchiveReadWriteView& View, const TF_ECMSMeshDataView& Value)
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.Meshlets);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.Triangles);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.VertexIndices);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.Positions);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.Normals);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.TangentsAndSigns);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.UVs);
            return F_FeedbackStatus::MakeSucceeded();   
        }
    };

    template <typename __F_Allocator>
    template <B8 __EnableWrite2>
    TF_ECMSMeshData<__F_Allocator>::TF_ECMSMeshData(const TF_ECMSMeshDataView<__EnableWrite2>& X) :
        Meshlets(X.Meshlets.begin(), X.Meshlets.end()),
        Triangles(X.Triangles.begin(), X.Triangles.end()),
        VertexIndices(X.VertexIndices.begin(), X.VertexIndices.end()),
        Positions(X.Positions.begin(), X.Positions.end()),
        Normals(X.Normals.begin(), X.Normals.end()),
        TangentsAndSigns(X.TangentsAndSigns.begin(), X.TangentsAndSigns.end()),
        UVs(X.UVs.begin(), X.UVs.end())
    {
    }
    template <typename __F_Allocator>
    template <B8 __EnableWrite2>
    TF_ECMSMeshData<__F_Allocator>& TF_ECMSMeshData<__F_Allocator>::operator = (const TF_ECMSMeshDataView<__EnableWrite2>& X)
    {
        using F_Meshlets = decltype(Meshlets);
        Meshlets = F_Meshlets(X.Meshlets.begin(), X.Meshlets.end());
        
        using F_Triangles = decltype(Triangles);
        Triangles = F_Triangles(X.Triangles.begin(), X.Triangles.end());
        
        using F_VertexIndices = decltype(VertexIndices);
        VertexIndices = F_VertexIndices(X.VertexIndices.begin(), X.VertexIndices.end());
        
        using F_Positions = decltype(Positions);
        Positions = F_Positions(X.Positions.begin(), X.Positions.end());
        
        using F_Normals = decltype(Normals);
        Normals = F_Normals(X.Normals.begin(), X.Normals.end());
        
        using F_TangentsAndSigns = decltype(TangentsAndSigns);
        TangentsAndSigns = F_TangentsAndSigns(X.TangentsAndSigns.begin(), X.TangentsAndSigns.end());
        
        using F_UVs = decltype(UVs);
        UVs = F_UVs(X.UVs.begin(), X.UVs.end());
        return *this;
    }

    template <typename __F_Allocator2>
    TF_ECMSMeshDataView<false>::TF_ECMSMeshDataView(const TF_ECMSMeshData<__F_Allocator2>& X) :
        Meshlets(X.Meshlets),
        Triangles(X.Triangles),
        VertexIndices(X.VertexIndices),
        Positions(X.Positions),
        Normals(X.Normals),
        TangentsAndSigns(X.TangentsAndSigns),
        UVs(X.UVs)
    {
    }
    template <typename __F_Allocator2>
    TF_ECMSMeshDataView<false>& TF_ECMSMeshDataView<false>::operator = (const TF_ECMSMeshData<__F_Allocator2>& X)
    {
        Meshlets = X.Meshlets;
        Triangles = X.Triangles;
        VertexIndices = X.VertexIndices;
        Positions = X.Positions;
        Normals = X.Normals;
        TangentsAndSigns = X.TangentsAndSigns;
        UVs = X.UVs;
        return *this;
    }
    template <typename __F_Allocator2>
    TF_ECMSMeshDataView<true>::TF_ECMSMeshDataView(TF_ECMSMeshData<__F_Allocator2>& X) :
        Meshlets(X.Meshlets),
        Triangles(X.Triangles),
        VertexIndices(X.VertexIndices),
        Positions(X.Positions),
        Normals(X.Normals),
        TangentsAndSigns(X.TangentsAndSigns),
        UVs(X.UVs)
    {
    }
    template <typename __F_Allocator2>
    TF_ECMSMeshDataView<true>& TF_ECMSMeshDataView<true>::operator = (TF_ECMSMeshData<__F_Allocator2>& X)
    {
        Meshlets = X.Meshlets;
        Triangles = X.Triangles;
        VertexIndices = X.VertexIndices;
        Positions = X.Positions;
        Normals = X.Normals;
        TangentsAndSigns = X.TangentsAndSigns;
        UVs = X.UVs;
        return *this;
    }
    
    inline TF_ECMSMeshDataView<false> TF_ECMSMeshDataView<false>::GetReadOnly() const noexcept
    {
        return *(const TF_ECMSMeshDataView<false>*)this;
    }
    inline TF_ECMSMeshDataView<true> TF_ECMSMeshDataView<false>::GetReadWrite() const noexcept
    {
        return *(const TF_ECMSMeshDataView<true>*)this;
    }
    inline TF_ECMSMeshDataView<false> TF_ECMSMeshDataView<true>::GetReadOnly() const noexcept
    {
        return *(const TF_ECMSMeshDataView<false>*)this;
    }
    inline TF_ECMSMeshDataView<true> TF_ECMSMeshDataView<true>::GetReadWrite() const noexcept
    {
        return *(const TF_ECMSMeshDataView<true>*)this;
    }
    
    using F_ECMSMeshData = TF_ECMSMeshData<>;
    using F_ECMSMeshDataROView = TF_ECMSMeshDataView<false>;
    using F_ECMSMeshDataRWView = TF_ECMSMeshDataView<true>;
}