#pragma once

#include "Abytek/Base.Math.prerequisites.pch.hpp"
#include "Abytek/Types/Vecma.hpp"


namespace Abytek 
{
    template<typename __F_Element>
    TF_Matrix4x4<__F_Element> CalculatePerspectiveProjectionMatrix(
        TF_Vector2<__F_Element> vertical_fov_and_aspect_ratio,
        __F_Element near_plane,
        __F_Element far_plane
    ) noexcept {

        __F_Element half_vertical_fov = vertical_fov_and_aspect_ratio.X * 0.5f;
        __F_Element sin_half_vertical_fov = sinf(half_vertical_fov);
        __F_Element cos_half_vertical_fov = cosf(half_vertical_fov);

        float ys = cos_half_vertical_fov / sin_half_vertical_fov;
        float xs = ys / vertical_fov_and_aspect_ratio.Y;
        float zs = far_plane / (far_plane - near_plane);

        return {
            TF_Vector4<__F_Element>{xs, 0, 0, 0},
            TF_Vector4<__F_Element>{0, ys, 0, 0},
            TF_Vector4<__F_Element>{0, 0, zs, 1},
            TF_Vector4<__F_Element>{0, 0, -zs * near_plane, 0}
        };
    }
    template<typename __F_Element>
    TF_Matrix4x4<__F_Element> CalculateOrthogonalProjectionMatrix(
        TF_Vector2<__F_Element> view_size,
        __F_Element near_plane,
        __F_Element far_plane
    ) noexcept {

        float range = 1.0f / (far_plane - near_plane);

        return {
            TF_Vector4<__F_Element>{2.0f / view_size.X, 0, 0, 0},
            TF_Vector4<__F_Element>{0, 2.0f / view_size.Y, 0, 0},
            TF_Vector4<__F_Element>{0, 0, range, 0.0f},
            TF_Vector4<__F_Element>{0, 0, -range * near_plane, 1.0f}
        };
    }

}