#pragma once

#include "Abytek/Engine.NFC.prerequisites.hpp"


namespace Abytek
{
    template<class __F_Allocator = TF_DefaultAllocator<U8>>
    struct TF_SimpleMeshData;
    
    template<B8 __EnableWrite>
    struct TF_SimpleMeshDataView;
    
    template<class __F_Allocator>
    struct TF_SimpleMeshData
    {
        using F_Allocator = __F_Allocator;
        
        U32 MaterialIndex = 0;
        ContainerTemplates::TF_Vector<U32, TF_RebindAllocator<U32, F_Allocator>> Indices;
        ContainerTemplates::TF_Vector<F_Vector3_F32, TF_RebindAllocator<F_Vector3_F32, F_Allocator>> Positions;
        ContainerTemplates::TF_Vector<F_Vector3_F32, TF_RebindAllocator<F_Vector3_F32, F_Allocator>> Normals;
        ContainerTemplates::TF_Vector<F_Vector4_F32, TF_RebindAllocator<F_Vector4_F32, F_Allocator>> TangentsAndSigns;
        ContainerTemplates::TF_Vector<F_Vector2_F32, TF_RebindAllocator<F_Vector2_F32, F_Allocator>> UVs;
        
        U32 GetIndexCount() const
        {
            return Indices.size();
        }
        U32 GetVertexCount() const
        {
            return Positions.size();
        }
        
        TF_SimpleMeshData() = default;
        TF_SimpleMeshData(const TF_SimpleMeshData& X) = default;
        TF_SimpleMeshData& operator = (const TF_SimpleMeshData& X) = default;
        TF_SimpleMeshData(TF_SimpleMeshData&& X) = default;
        TF_SimpleMeshData& operator = (TF_SimpleMeshData&& X) = default;
        
        template<B8 __EnableWrite2>
        TF_SimpleMeshData(const TF_SimpleMeshDataView<__EnableWrite2>& X);
        template<B8 __EnableWrite2>
        TF_SimpleMeshData& operator = (const TF_SimpleMeshDataView<__EnableWrite2>& X);
        
        friend F_FeedbackStatus operator << (F_ArchiveReadWriteView& View, const TF_SimpleMeshData& Value)
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.MaterialIndex);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.Indices);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.Positions);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.Normals);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.TangentsAndSigns);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.UVs);
            return F_FeedbackStatus::MakeSucceeded();   
        }
        friend F_FeedbackStatus operator >> (F_ArchiveReadOnlyView& View, TF_SimpleMeshData& Value)
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.MaterialIndex);
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.Indices);
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.Positions);
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.Normals);
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.TangentsAndSigns);
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.UVs);
            return F_FeedbackStatus::MakeSucceeded();   
        }
    };
    
    template<>
    struct TF_SimpleMeshDataView<false>
    {
        static constexpr B8 EnableWrite = false;
        
        U32 MaterialIndex = 0;
        TF_Span<
            std::conditional_t<
                EnableWrite,
                U32,
                const U32
            >    
        > Indices;
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
        
        U32 GetIndexCount() const
        {
            return Indices.size();
        }
        U32 GetVertexCount() const
        {
            return Positions.size();
        }
        
        TF_SimpleMeshDataView() = default;
        TF_SimpleMeshDataView(const TF_SimpleMeshDataView& X) = default;
        TF_SimpleMeshDataView& operator = (const TF_SimpleMeshDataView& X) = default;
        TF_SimpleMeshDataView(TF_SimpleMeshDataView&& X) = default;
        TF_SimpleMeshDataView& operator = (TF_SimpleMeshDataView&& X) = default;
        
        template<typename __F_Allocator2>
        TF_SimpleMeshDataView(const TF_SimpleMeshData<__F_Allocator2>& X);
        template<typename __F_Allocator2>
        TF_SimpleMeshDataView& operator = (const TF_SimpleMeshData<__F_Allocator2>& X);
        
        TF_SimpleMeshDataView<false> GetReadOnly() const noexcept;
        TF_SimpleMeshDataView<true> GetReadWrite() const noexcept;
        
        friend F_FeedbackStatus operator << (F_ArchiveReadWriteView& View, const TF_SimpleMeshDataView& Value)
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.MaterialIndex);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.Indices);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.Positions);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.Normals);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.TangentsAndSigns);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.UVs);
            return F_FeedbackStatus::MakeSucceeded();   
        }
    };
    
    template<>
    struct TF_SimpleMeshDataView<true>
    {
        static constexpr B8 EnableWrite = true;
        
        U32 MaterialIndex = 0;
        TF_Span<
            std::conditional_t<
                EnableWrite,
                U32,
                const U32
            >    
        > Indices;
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
        
        U32 GetIndexCount() const
        {
            return Indices.size();
        }
        U32 GetVertexCount() const
        {
            return Positions.size();
        }
        
        TF_SimpleMeshDataView() = default;
        TF_SimpleMeshDataView(const TF_SimpleMeshDataView& X) = default;
        TF_SimpleMeshDataView& operator = (const TF_SimpleMeshDataView& X) = default;
        TF_SimpleMeshDataView(TF_SimpleMeshDataView&& X) = default;
        TF_SimpleMeshDataView& operator = (TF_SimpleMeshDataView&& X) = default;
        
        template<typename __F_Allocator2>
        TF_SimpleMeshDataView(TF_SimpleMeshData<__F_Allocator2>& X);
        template<typename __F_Allocator2>
        TF_SimpleMeshDataView& operator = (TF_SimpleMeshData<__F_Allocator2>& X);
        
        TF_SimpleMeshDataView<false> GetReadOnly() const noexcept;
        TF_SimpleMeshDataView<true> GetReadWrite() const noexcept;
        
        friend F_FeedbackStatus operator << (F_ArchiveReadWriteView& View, const TF_SimpleMeshDataView& Value)
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.MaterialIndex);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.Indices);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.Positions);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.Normals);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.TangentsAndSigns);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.UVs);
            return F_FeedbackStatus::MakeSucceeded();   
        }
    };

    template <typename __F_Allocator>
    template <B8 __EnableWrite2>
    TF_SimpleMeshData<__F_Allocator>::TF_SimpleMeshData(const TF_SimpleMeshDataView<__EnableWrite2>& X) :
        MaterialIndex(X.MaterialIndex),
        Indices(X.Indices.begin(), X.Indices.end()),
        Positions(X.Positions.begin(), X.Positions.end()),
        Normals(X.Normals.begin(), X.Normals.end()),
        TangentsAndSigns(X.TangentsAndSigns.begin(), X.TangentsAndSigns.end()),
        UVs(X.UVs.begin(), X.UVs.end())
    {
    }
    template <typename __F_Allocator>
    template <B8 __EnableWrite2>
    TF_SimpleMeshData<__F_Allocator>& TF_SimpleMeshData<__F_Allocator>::operator = (const TF_SimpleMeshDataView<__EnableWrite2>& X)
    {
        MaterialIndex = X.MaterialIndex;
        
        using F_Indices = decltype(Indices);
        Indices = F_Indices(X.Indices.begin(), X.Indices.end());
        
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
    TF_SimpleMeshDataView<false>::TF_SimpleMeshDataView(const TF_SimpleMeshData<__F_Allocator2>& X) :
        MaterialIndex(X.MaterialIndex),
        Indices(X.Indices),
        Positions(X.Positions),
        Normals(X.Normals),
        TangentsAndSigns(X.TangentsAndSigns),
        UVs(X.UVs)
    {
    }
    template <typename __F_Allocator2>
    TF_SimpleMeshDataView<false>& TF_SimpleMeshDataView<false>::operator = (const TF_SimpleMeshData<__F_Allocator2>& X)
    {
        MaterialIndex = X.MaterialIndex;
        Indices = X.Indices;
        Positions = X.Positions;
        Normals = X.Normals;
        TangentsAndSigns = X.TangentsAndSigns;
        UVs = X.UVs;
        return *this;
    }
    template <typename __F_Allocator2>
    TF_SimpleMeshDataView<true>::TF_SimpleMeshDataView(TF_SimpleMeshData<__F_Allocator2>& X) :
        MaterialIndex(X.MaterialIndex),
        Indices(X.Indices),
        Positions(X.Positions),
        Normals(X.Normals),
        TangentsAndSigns(X.TangentsAndSigns),
        UVs(X.UVs)
    {
    }
    template <typename __F_Allocator2>
    TF_SimpleMeshDataView<true>& TF_SimpleMeshDataView<true>::operator = (TF_SimpleMeshData<__F_Allocator2>& X)
    {
        MaterialIndex = X.MaterialIndex;
        Indices = X.Indices;
        Positions = X.Positions;
        Normals = X.Normals;
        TangentsAndSigns = X.TangentsAndSigns;
        UVs = X.UVs;
        return *this;
    }
    
    inline TF_SimpleMeshDataView<false> TF_SimpleMeshDataView<false>::GetReadOnly() const noexcept
    {
        return *(const TF_SimpleMeshDataView<false>*)this;
    }
    inline TF_SimpleMeshDataView<true> TF_SimpleMeshDataView<false>::GetReadWrite() const noexcept
    {
        return *(const TF_SimpleMeshDataView<true>*)this;
    }
    inline TF_SimpleMeshDataView<false> TF_SimpleMeshDataView<true>::GetReadOnly() const noexcept
    {
        return *(const TF_SimpleMeshDataView<false>*)this;
    }
    inline TF_SimpleMeshDataView<true> TF_SimpleMeshDataView<true>::GetReadWrite() const noexcept
    {
        return *(const TF_SimpleMeshDataView<true>*)this;
    }
    
    using F_SimpleMeshData = TF_SimpleMeshData<>;
    using F_SimpleMeshDataROView = TF_SimpleMeshDataView<false>;
    using F_SimpleMeshDataRWView = TF_SimpleMeshDataView<true>;
}