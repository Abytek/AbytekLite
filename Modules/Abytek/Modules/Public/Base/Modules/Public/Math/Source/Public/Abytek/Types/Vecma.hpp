/**
 * @file Vecma.hpp
 * @brief Vector and matrix type definitions for the Abytek math system.
 * 
 * This file defines various vector and matrix types using the Data template,
 * with both fixed-layout (TF_*) and platform-aligned (TPA_*) variants.
 */
#pragma once

#include "Abytek/Base.Math.prerequisites.pch.hpp"
#include "Abytek/Types/Data.hpp"


namespace Abytek
{
    /**
     * @struct F_VecmaConfig
     * @brief Configuration struct for vector and matrix types.
     */
    struct F_VecmaConfig : TA_DataConfig<TA_DefaultData>
    {
    };
    
    /**
     * @typedef TF_Vector2
     * @brief 2-component vector with fixed layout.
     * @tparam __F_Element The scalar type used for vector components.
     */
    template<typename __F_Element>
    using TF_Vector2 = TF_Data<2, 1, __F_Element, F_VecmaConfig>;
    using F_Vector2 = TF_Vector2<ABYTEK_DEFAULT_FP_TYPE>;
    using F_Vector2_F32 = TF_Vector2<F32>;
    using F_Vector2_I64 = TF_Vector2<I64>;
    using F_Vector2_I32 = TF_Vector2<I32>;
    using F_Vector2_I16 = TF_Vector2<I16>;
    using F_Vector2_I8 = TF_Vector2<I8>;
    using F_Vector2_U64 = TF_Vector2<U64>;
    using F_Vector2_U32 = TF_Vector2<U32>;
    using F_Vector2_U16 = TF_Vector2<U16>;
    using F_Vector2_U8 = TF_Vector2<U8>;
    
    /**
     * @typedef TF_Vector3
     * @brief 3-component vector with fixed layout.
     * @tparam __F_Element The scalar type used for vector components.
     */
    template<typename __F_Element>
    using TF_Vector3 = TF_Data<3, 1, __F_Element, F_VecmaConfig>;
    using F_Vector3 = TF_Vector3<ABYTEK_DEFAULT_FP_TYPE>;
    using F_Vector3_F32 = TF_Vector3<F32>;
    using F_Vector3_I64 = TF_Vector3<I64>;
    using F_Vector3_I32 = TF_Vector3<I32>;
    using F_Vector3_I16 = TF_Vector3<I16>;
    using F_Vector3_I8 = TF_Vector3<I8>;
    using F_Vector3_U64 = TF_Vector3<U64>;
    using F_Vector3_U32 = TF_Vector3<U32>;
    using F_Vector3_U16 = TF_Vector3<U16>;
    using F_Vector3_U8 = TF_Vector3<U8>;
    
    /**
     * @typedef TF_Vector4
     * @brief 4-component vector with fixed layout.
     * @tparam __F_Element The scalar type used for vector components.
     */
    template<typename __F_Element>
    using TF_Vector4 = TF_Data<4, 1, __F_Element, F_VecmaConfig>;
    using F_Vector4 = TF_Vector4<ABYTEK_DEFAULT_FP_TYPE>;
    using F_Vector4_F32 = TF_Vector4<F32>;
    using F_Vector4_I64 = TF_Vector4<I64>;
    using F_Vector4_I32 = TF_Vector4<I32>;
    using F_Vector4_I16 = TF_Vector4<I16>;
    using F_Vector4_I8 = TF_Vector4<I8>;
    using F_Vector4_U64= TF_Vector4<U64>;
    using F_Vector4_U32 = TF_Vector4<U32>;
    using F_Vector4_U16 = TF_Vector4<U16>;
    using F_Vector4_U8 = TF_Vector4<U8>;
    
    /**
     * @typedef TF_Matrix2x2
     * @brief 2x2 matrix with fixed layout.
     * @tparam __F_Element The scalar type used for matrix elements.
     */
    template<typename __F_Element>
    using TF_Matrix2x2 = TF_Data<2, 2, __F_Element, F_VecmaConfig>;
    using F_Matrix2x2 = TF_Matrix2x2<ABYTEK_DEFAULT_FP_TYPE>;
    using F_Matrix2x2_F32 = TF_Matrix2x2<F32>;
    using F_Matrix2x2_I64 = TF_Matrix2x2<I64>;
    using F_Matrix2x2_I32 = TF_Matrix2x2<I32>;
    using F_Matrix2x2_I16 = TF_Matrix2x2<I16>;
    using F_Matrix2x2_I8 = TF_Matrix2x2<I8>;
    using F_Matrix2x2_U64 = TF_Matrix2x2<U64>;
    using F_Matrix2x2_U32 = TF_Matrix2x2<U32>;
    using F_Matrix2x2_U16 = TF_Matrix2x2<U16>;
    using F_Matrix2x2_U8 = TF_Matrix2x2<U8>;
    
    /**
     * @typedef TF_Matrix3x3
     * @brief 3x3 matrix with fixed layout.
     * @tparam __F_Element The scalar type used for matrix elements.
     */
    template<typename __F_Element>
    using TF_Matrix3x3 = TF_Data<3, 3, __F_Element, F_VecmaConfig>;
    using F_Matrix3x3 = TF_Matrix3x3<ABYTEK_DEFAULT_FP_TYPE>;
    using F_Matrix3x3_F32 = TF_Matrix3x3<F32>;
    using F_Matrix3x3_I64 = TF_Matrix3x3<I64>;
    using F_Matrix3x3_I32 = TF_Matrix3x3<I32>;
    using F_Matrix3x3_I16 = TF_Matrix3x3<I16>;
    using F_Matrix3x3_I8 = TF_Matrix3x3<I8>;
    using F_Matrix3x3_U64 = TF_Matrix3x3<U64>;
    using F_Matrix3x3_U32 = TF_Matrix3x3<U32>;
    using F_Matrix3x3_U16 = TF_Matrix3x3<U16>;
    using F_Matrix3x3_U8 = TF_Matrix3x3<U8>;
    
    /**
     * @typedef TF_Matrix4x4
     * @brief 4x4 matrix with fixed layout.
     * @tparam __F_Element The scalar type used for matrix elements.
     */
    template<typename __F_Element>
    using TF_Matrix4x4 = TF_Data<4, 4, __F_Element, F_VecmaConfig>;
    using F_Matrix4x4 = TF_Matrix4x4<ABYTEK_DEFAULT_FP_TYPE>;
    using F_Matrix4x4_F32 = TF_Matrix4x4<F32>;
    using F_Matrix4x4_I64 = TF_Matrix4x4<I64>;
    using F_Matrix4x4_I32 = TF_Matrix4x4<I32>;
    using F_Matrix4x4_I16 = TF_Matrix4x4<I16>;
    using F_Matrix4x4_I8 = TF_Matrix4x4<I8>;
    using F_Matrix4x4_U64 = TF_Matrix4x4<U64>;
    using F_Matrix4x4_U32 = TF_Matrix4x4<U32>;
    using F_Matrix4x4_U16 = TF_Matrix4x4<U16>;
    using F_Matrix4x4_U8 = TF_Matrix4x4<U8>;

    /**
     * @typedef TF_Vecma
     * @brief Generic vector/matrix with fixed layout and custom dimensions.
     * @tparam __RowCount Number of rows.
     * @tparam __ColumnCount Number of columns.
     * @tparam __F_Element The scalar type used for elements.
     */
    template<U32 __RowCount, U32 __ColumnCount, typename __F_Element = ABYTEK_DEFAULT_FP_TYPE>
    using TF_Vecma = TF_Data<__RowCount, __ColumnCount, __F_Element, F_VecmaConfig>;

    template<U32 __RowCount, U32 __ColumnCount>
    using TF_Vecma_F32 = TF_Data<__RowCount, __ColumnCount, F32, F_VecmaConfig>;

    template<U32 __RowCount, U32 __ColumnCount>
    using TF_Vecma_I64 = TF_Data<__RowCount, __ColumnCount, I64, F_VecmaConfig>;

    template<U32 __RowCount, U32 __ColumnCount>
    using TF_Vecma_I32 = TF_Data<__RowCount, __ColumnCount, I32, F_VecmaConfig>;

    template<U32 __RowCount, U32 __ColumnCount>
    using TF_Vecma_I16 = TF_Data<__RowCount, __ColumnCount, I16, F_VecmaConfig>;

    template<U32 __RowCount, U32 __ColumnCount>
    using TF_Vecma_I8 = TF_Data<__RowCount, __ColumnCount, I8, F_VecmaConfig>;

    template<U32 __RowCount, U32 __ColumnCount>
    using TF_Vecma_U64 = TF_Data<__RowCount, __ColumnCount, U64, F_VecmaConfig>;

    template<U32 __RowCount, U32 __ColumnCount>
    using TF_Vecma_U32 = TF_Data<__RowCount, __ColumnCount, U32, F_VecmaConfig>;

    template<U32 __RowCount, U32 __ColumnCount>
    using TF_Vecma_U16 = TF_Data<__RowCount, __ColumnCount, U16, F_VecmaConfig>;

    template<U32 __RowCount, U32 __ColumnCount>
    using TF_Vecma_U8 = TF_Data<__RowCount, __ColumnCount, U8, F_VecmaConfig>;
    
    /**
     * @typedef TPA_Vector2
     * @brief 2-component vector with platform-aligned layout.
     * @tparam __F_Element The scalar type used for vector components.
     */
    template<typename __F_Element>
    using TPA_Vector2 = TPA_Data<2, 1, __F_Element, F_VecmaConfig>;
    using PA_Vector2 = TPA_Vector2<ABYTEK_DEFAULT_FP_TYPE>;
    using PA_Vector2_F32 = TPA_Vector2<F32>;
    using PA_Vector2_I64 = TPA_Vector2<I64>;
    using PA_Vector2_I32 = TPA_Vector2<I32>;
    using PA_Vector2_I16 = TPA_Vector2<I16>;
    using PA_Vector2_I8 = TPA_Vector2<I8>;
    using PA_Vector2_U64 = TPA_Vector2<U64>;
    using PA_Vector2_U32 = TPA_Vector2<U32>;
    using PA_Vector2_U16 = TPA_Vector2<U16>;
    using PA_Vector2_U8 = TPA_Vector2<U8>;
    
    /**
     * @typedef TPA_Vector3
     * @brief 3-component vector with platform-aligned layout.
     * @tparam __F_Element The scalar type used for vector components.
     */
    template<typename __F_Element>
    using TPA_Vector3 = TPA_Data<3, 1, __F_Element, F_VecmaConfig>;
    using PA_Vector3 = TPA_Vector3<ABYTEK_DEFAULT_FP_TYPE>;
    using PA_Vector3_F32 = TPA_Vector3<F32>;
    using PA_Vector3_I64 = TPA_Vector3<I64>;
    using PA_Vector3_I32 = TPA_Vector3<I32>;
    using PA_Vector3_I16 = TPA_Vector3<I16>;
    using PA_Vector3_I8 = TPA_Vector3<I8>;
    using PA_Vector3_U64 = TPA_Vector3<U64>;
    using PA_Vector3_U32 = TPA_Vector3<U32>;
    using PA_Vector3_U16 = TPA_Vector3<U16>;
    using PA_Vector3_U8 = TPA_Vector3<U8>;
    
    /**
     * @typedef TPA_Vector4
     * @brief 4-component vector with platform-aligned layout.
     * @tparam __F_Element The scalar type used for vector components.
     */
    template<typename __F_Element>
    using TPA_Vector4 = TPA_Data<4, 1, __F_Element, F_VecmaConfig>;
    using PA_Vector4 = TPA_Vector4<ABYTEK_DEFAULT_FP_TYPE>;
    using PA_Vector4_F32 = TPA_Vector4<F32>;
    using PA_Vector4_I64 = TPA_Vector4<I64>;
    using PA_Vector4_I32 = TPA_Vector4<I32>;
    using PA_Vector4_I16 = TPA_Vector4<I16>;
    using PA_Vector4_I8 = TPA_Vector4<I8>;
    using PA_Vector4_U64 = TPA_Vector4<U64>;
    using PA_Vector4_U32 = TPA_Vector4<U32>;
    using PA_Vector4_U16 = TPA_Vector4<U16>;
    using PA_Vector4_U8 = TPA_Vector4<U8>;
    
    /**
     * @typedef TPA_Matrix2x2
     * @brief 2x2 matrix with platform-aligned layout.
     * @tparam __F_Element The scalar type used for matrix elements.
     */
    template<typename __F_Element>
    using TPA_Matrix2x2 = TPA_Data<2, 2, __F_Element, F_VecmaConfig>;
    using PA_Matrix2x2 = TPA_Matrix2x2<ABYTEK_DEFAULT_FP_TYPE>;
    using PA_Matrix2x2_F32 = TPA_Matrix2x2<F32>;
    using PA_Matrix2x2_I64 = TPA_Matrix2x2<I64>;
    using PA_Matrix2x2_I32 = TPA_Matrix2x2<I32>;
    using PA_Matrix2x2_I16 = TPA_Matrix2x2<I16>;
    using PA_Matrix2x2_I8 = TPA_Matrix2x2<I8>;
    using PA_Matrix2x2_U64= TPA_Matrix2x2<U64>;
    using PA_Matrix2x2_U32 = TPA_Matrix2x2<U32>;
    using PA_Matrix2x2_U16 = TPA_Matrix2x2<U16>;
    using PA_Matrix2x2_U8 = TPA_Matrix2x2<U8>;
    
    /**
     * @typedef TPA_Matrix3x3
     * @brief 3x3 matrix with platform-aligned layout.
     * @tparam __F_Element The scalar type used for matrix elements.
     */
    template<typename __F_Element>
    using TPA_Matrix3x3 = TPA_Data<3, 3, __F_Element, F_VecmaConfig>;
    using PA_Matrix3x3 = TPA_Matrix3x3<ABYTEK_DEFAULT_FP_TYPE>;
    using PA_Matrix3x3_F32 = TPA_Matrix3x3<F32>;
    using PA_Matrix3x3_I64 = TPA_Matrix3x3<I64>;
    using PA_Matrix3x3_I32 = TPA_Matrix3x3<I32>;
    using PA_Matrix3x3_I16 = TPA_Matrix3x3<I16>;
    using PA_Matrix3x3_I8 = TPA_Matrix3x3<I8>;
    using PA_Matrix3x3_U64 = TPA_Matrix3x3<U64>;
    using PA_Matrix3x3_U32 = TPA_Matrix3x3<U32>;
    using PA_Matrix3x3_U16 = TPA_Matrix3x3<U16>;
    using PA_Matrix3x3_U8 = TPA_Matrix3x3<U8>;
    
    /**
     * @typedef TPA_Matrix4x4
     * @brief 4x4 matrix with platform-aligned layout.
     * @tparam __F_Element The scalar type used for matrix elements.
     */
    template<typename __F_Element>
    using TPA_Matrix4x4 = TPA_Data<4, 4, __F_Element, F_VecmaConfig>;
    using PA_Matrix4x4 = TPA_Matrix4x4<ABYTEK_DEFAULT_FP_TYPE>;
    using PA_Matrix4x4_F32 = TPA_Matrix4x4<F32>;
    using PA_Matrix4x4_I64 = TPA_Matrix4x4<I64>;
    using PA_Matrix4x4_I32 = TPA_Matrix4x4<I32>;
    using PA_Matrix4x4_I16 = TPA_Matrix4x4<I16>;
    using PA_Matrix4x4_I8 = TPA_Matrix4x4<I8>;
    using PA_Matrix4x4_U64 = TPA_Matrix4x4<U64>;
    using PA_Matrix4x4_U32 = TPA_Matrix4x4<U32>;
    using PA_Matrix4x4_U16 = TPA_Matrix4x4<U16>;
    using PA_Matrix4x4_U8 = TPA_Matrix4x4<U8>;

    /**
     * @typedef TPA_Vecma
     * @brief Generic vector/matrix with platform-aligned layout and custom dimensions.
     * @tparam __RowCount Number of rows.
     * @tparam __ColumnCount Number of columns.
     * @tparam __F_Element The scalar type used for elements.
     */
    template<U32 __RowCount, U32 __ColumnCount, typename __F_Element = ABYTEK_DEFAULT_FP_TYPE>
    using TPA_Vecma = TPA_Data<__RowCount, __ColumnCount, __F_Element, F_VecmaConfig>;

    template<U32 __RowCount, U32 __ColumnCount>
    using TPA_Vecma_F32 = TPA_Data<__RowCount, __ColumnCount, F32, F_VecmaConfig>;

    template<U32 __RowCount, U32 __ColumnCount>
    using TPA_Vecma_I64 = TPA_Data<__RowCount, __ColumnCount, I64, F_VecmaConfig>;

    template<U32 __RowCount, U32 __ColumnCount>
    using TPA_Vecma_I32 = TPA_Data<__RowCount, __ColumnCount, I32, F_VecmaConfig>;

    template<U32 __RowCount, U32 __ColumnCount>
    using TPA_Vecma_I16 = TPA_Data<__RowCount, __ColumnCount, I16, F_VecmaConfig>;

    template<U32 __RowCount, U32 __ColumnCount>
    using TPA_Vecma_I8 = TPA_Data<__RowCount, __ColumnCount, I8, F_VecmaConfig>;

    template<U32 __RowCount, U32 __ColumnCount>
    using TPA_Vecma_U64 = TPA_Data<__RowCount, __ColumnCount, U64, F_VecmaConfig>;

    template<U32 __RowCount, U32 __ColumnCount>
    using TPA_Vecma_U32 = TPA_Data<__RowCount, __ColumnCount, U32, F_VecmaConfig>;

    template<U32 __RowCount, U32 __ColumnCount>
    using TPA_Vecma_U16 = TPA_Data<__RowCount, __ColumnCount, U16, F_VecmaConfig>;

    template<U32 __RowCount, U32 __ColumnCount>
    using TPA_Vecma_U8 = TPA_Data<__RowCount, __ColumnCount, U8, F_VecmaConfig>;
}