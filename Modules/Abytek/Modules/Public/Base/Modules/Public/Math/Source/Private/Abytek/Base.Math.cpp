#include "Abytek/Base.Math.prerequisites.pch.hpp"
#include "Abytek/Types/Vecma.hpp"
#include "Abytek/Types/Quaternion.hpp"


namespace Abytek
{
    void SetupReflectionSession_Base_Math(const TW_Valid<F_ReflectionSession>& Session)
    {
        Session->RegisterType<F_Vector2_F32>();
        Session->RegisterType<F_Vector2_I32>();
        Session->RegisterType<F_Vector2_U32>();
        Session->RegisterType<F_Matrix2x2_F32>();
        Session->RegisterType<F_Matrix2x2_I32>();
        Session->RegisterType<F_Matrix2x2_U32>();
        Session->RegisterType<F_Vector3_F32>();
        Session->RegisterType<F_Vector3_I32>();
        Session->RegisterType<F_Vector3_U32>();
        Session->RegisterType<F_Matrix3x3_F32>();
        Session->RegisterType<F_Matrix3x3_I32>();
        Session->RegisterType<F_Matrix3x3_U32>();
        Session->RegisterType<F_Vector4_F32>();
        Session->RegisterType<F_Vector4_I32>();
        Session->RegisterType<F_Vector4_U32>();
        Session->RegisterType<F_Matrix4x4_F32>();
        Session->RegisterType<F_Matrix4x4_I32>();
        Session->RegisterType<F_Matrix4x4_U32>();
        
        Session->RegisterType<F_Quaternion_F32>();
    }
}
