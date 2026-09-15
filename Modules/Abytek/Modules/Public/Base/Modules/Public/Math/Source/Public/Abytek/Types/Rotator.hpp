#pragma once

#include "Abytek/Base.Math.prerequisites.pch.hpp"
#include "Abytek/Types/Vecma.hpp"
#include "Abytek/Types/Quaternion.hpp"
#include "Abytek/Functions/IdentityTemplate.hpp"
#include "Abytek/Functions/EulerAnglesToQuaternion.hpp"
#include "Abytek/Functions/QuaternionToEulerAngles.hpp"
#include "Abytek/Functions/AxisAngleToQuaternion.hpp"
#include "Abytek/Functions/AxisAndAngle.hpp"
#include "Abytek/Functions/QuaternionToMatrix.hpp"
#include "Abytek/Functions/MakeRotationMatrix.hpp"
#include "Abytek/Functions/Normalize.hpp"
#include "Abytek/Functions/QuaternionToMatrix.hpp"


namespace Abytek
{
    template<typename __F_Element>
    struct TF_Rotator
    {
        using F_Element = __F_Element;
        using F_Vector3 = TF_Vector3<F_Element>;
        using F_Vector4 = TF_Vector4<F_Element>;
        using F_Quaternion = TF_Quaternion<F_Element>;
        using F_Matrix4x4 = TF_Matrix4x4<F_Element>;

        F_Vector3 EulerAngles;
        
        static TF_Rotator MakeIdentity()
        {
            TF_Rotator Result;
            Result.EulerAngles = F_Vector3::Zero();
            return Result;
        }
        static TF_Rotator FromQuaternion(const F_Quaternion& Quaternion)
        {
            TF_Rotator Result;
            Result.EulerAngles = QuaternionToEulerAngles(Normalize(Quaternion));
            return Result;
        }
        static TF_Rotator FromAxisAndAngle(const F_Vector3& Axis, F_Element Angle)
        {
            return FromQuaternion(AxisAngleToQuaternion(Axis, Angle));
        }
        
        F_Quaternion GetQuaternion() const
        {
            return EulerAnglesToQuaternion(EulerAngles);
        }
        F_Vector4 GetAxisAndAngle() const
        {
            return Abytek::AxisAndAngle(Normalize(GetQuaternion()));
        }
        F_Matrix4x4 GetMatrix4x4() const
        {
            return QuaternionToAffineMatrix(GetQuaternion());
        }
        F_Matrix3x3 GetMatrix3x3() const
        {
            return QuaternionToLinearMatrix(GetQuaternion());
        }
        
        TF_Rotator Inverse() const
        {
            return FromQuaternion(
                Abytek::Inverse(GetQuaternion())  
            );
        }
        
        friend F_Vector3 operator * (const TF_Rotator& A, const F_Vector3& B)
        {
            return A.GetQuaternion() * B;
        }
        friend TF_Rotator operator * (const TF_Rotator& A, const TF_Rotator& B)
        {
            return FromQuaternion(A.GetQuaternion() * B.GetQuaternion());
        }
        friend TF_Rotator operator / (const TF_Rotator& A, const TF_Rotator& B)
        {
            return FromQuaternion(A.GetQuaternion() / B.GetQuaternion());
        }
    };
    
    using F_Rotator = TF_Rotator<F32>;
    using F_Rotator_F32 = TF_Rotator<F32>;
}