#pragma once

#include "Abytek/Base.Math.prerequisites.pch.hpp"
#include "Abytek/Types/Vecma.hpp"
#include "Abytek/Types/Quaternion.hpp"
#include "Abytek/Types/Rotator.hpp"
#include "Abytek/Functions/IdentityTemplate.hpp"
#include "Abytek/Functions/MakeTranslationMatrix.hpp"
#include "Abytek/Functions/MakeRotationMatrix.hpp"
#include "Abytek/Functions/MakeScaleMatrix.hpp"


namespace Abytek
{
    template<typename __F_Element>
    struct TF_Transform
    {
        using F_Element = __F_Element;
        using F_Vector3 = TF_Vector3<F_Element>;
        using F_Quaternion = TF_Quaternion<F_Element>;
        using F_Matrix4x4 = TF_Matrix4x4<F_Element>;
        using F_Rotator = TF_Rotator<F_Element>;

        F_Vector3 Position;
        F_Rotator Rotation;
        F_Vector3 Scale;
        
        static TF_Transform MakeIdentity()
        {
            TF_Transform Result;
            Result.Position = F_Vector3::Zero();
            Result.Rotation = F_Rotator::MakeIdentity();
            Result.Scale = F_Vector3::One();
            return Result;
        }
        
        F_Matrix4x4 GetMatrix4x4() const
        {
            return MakeTranslationMatrix(Position) * Rotation.GetMatrix4x4() * MakeScaleMatrix4x4(Scale);
        }
    };
    
    using F_Transform = TF_Transform<F32>;
    using F_Transform_F32 = TF_Transform<F32>;
}