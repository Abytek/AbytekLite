#include "Abytek/ActorComponents/SceneComponent.hpp"


namespace Abytek
{
    ABYTEK_REFLECT(F_SceneComponent)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::F_SceneComponent"));
    }
    
    F_SceneComponent::F_SceneComponent(const F_SerializableObjectInitParams& InitParams) :
        A_ActorComponent(InitParams)
    {
    }
    F_SceneComponent::~F_SceneComponent()
    {
    }

    void F_SceneComponent::OnRegisterComponent()
    {
    }
    void F_SceneComponent::OnUnregisterComponent()
    {
    }

    void F_SceneComponent::OnParentChanged(const TW<F_SceneComponent>& LastParent)
    {
    }
    void F_SceneComponent::OnLocalTransformChanged(const F_Transform& LastLocalTransform)
    {
    }

    void F_SceneComponent::SetParent(const TW<F_SceneComponent>& Value)
    {
        TW<F_SceneComponent> LastParent = _Parent;
        _Parent = Value;
        OnParentChanged(LastParent);
    }
    void F_SceneComponent::SetLocalTransform(const F_Transform& Value)
    {
        F_Transform LastLocalTransform = _LocalTransform;
        _LocalTransform = Value;
        OnLocalTransformChanged(LastLocalTransform);
    }
    void F_SceneComponent::SetLocalPosition(const F_Vector3& Value)
    {
        F_Transform LastLocalTransform = _LocalTransform;
        _LocalTransform.Position = Value;
        OnLocalTransformChanged(LastLocalTransform);
    }
    void F_SceneComponent::SetLocalRotation(const F_Rotator& Value)
    {
        F_Transform LastLocalTransform = _LocalTransform;
        _LocalTransform.Rotation = Value;
        OnLocalTransformChanged(LastLocalTransform);
    }
    void F_SceneComponent::SetLocalScale(const F_Vector3& Value)
    {
        F_Transform LastLocalTransform = _LocalTransform;
        _LocalTransform.Scale = Value;
        OnLocalTransformChanged(LastLocalTransform);
    }

    void F_SceneComponent::SetWorldPosition(const F_Vector3& Value)
    {
        if (!_Parent)
        {
            _LocalTransform.Position = Value;
            return;
        }
        SetLocalPosition(
            (
                Inverse(GetBaseWorldTransformMatrix()) 
                * MakePosition4(Value)
            ).StaticPermute<0, 1, 2>()
        );
    }
    void F_SceneComponent::SetWorldRotation(const F_Rotator& Value)
    {
        if (!_Parent)
        {
            _LocalTransform.Rotation = Value;
            return;
        }
        SetLocalRotation(
            GetBaseWorldRotation().Inverse() 
            * Value
        );
    }

    void F_SceneComponent::AddLocalPositionOffset(const F_Vector3& Value)
    {
        SetLocalPosition(
            GetLocalPosition()
            + Value    
        );
    }
    void F_SceneComponent::AddLocalRotationOffset(const F_Vector3& EulerAngles)
    {
        F_Rotator Rotation = GetLocalRotation();
        Rotation.EulerAngles += EulerAngles;
        SetLocalRotation(Rotation);
    }
    void F_SceneComponent::AddLocalRotationOffset(const F_Rotator& Value)
    {
        SetLocalRotation(GetLocalRotation() * Value);
    }

    void F_SceneComponent::AddWorldPositionOffset(const F_Vector3& Value)
    {
        SetWorldPosition(
            GetWorldPosition()
            + Value    
        );
    }
    void F_SceneComponent::AddWorldRotationOffset(const F_Vector3& EulerAngles)
    {
        F_Rotator Rotation = GetWorldRotation();
        Rotation.EulerAngles += EulerAngles;
        SetWorldRotation(Rotation);
    }
    void F_SceneComponent::AddWorldRotationOffset(const F_Rotator& Value)
    {
        SetWorldRotation(GetWorldRotation() * Value);
    }

    F_Vector3 F_SceneComponent::CalculateChildWorldPosition(const F_Transform& ChildLocalTransform) const noexcept
    {
        return GetWorldPosition() + GetWorldRotation() * (ChildLocalTransform.Scale * ChildLocalTransform.Position);
    }
    F_Rotator F_SceneComponent::CalculateChildWorldRotation(const F_Transform& ChildLocalTransform) const noexcept
    {
        return GetWorldRotation() * ChildLocalTransform.Rotation;
    }
    F_Matrix4x4 F_SceneComponent::CalculateChildWorldTransformMatrix(const F_Transform& ChildLocalTransform) const noexcept
    {
        return GetWorldTransformMatrix() * ChildLocalTransform.GetMatrix4x4();
    }

    F_Vector3 F_SceneComponent::GetWorldPosition() const noexcept
    {
        if (_Parent)
        {
            return _Parent->CalculateChildWorldPosition(_LocalTransform);
        }
        return _LocalTransform.Position;
    }
    F_Rotator F_SceneComponent::GetBaseWorldRotation() const noexcept
    {
        if (_Parent)
        {
            return _Parent->GetWorldRotation();
        }
        return F_Rotator::MakeIdentity();
    }
    F_Rotator F_SceneComponent::GetWorldRotation() const noexcept
    {
        if (_Parent)
        {
            return _Parent->CalculateChildWorldRotation(_LocalTransform);
        }
        return _LocalTransform.Rotation;
    }
    F_Matrix4x4 F_SceneComponent::GetBaseWorldTransformMatrix() const noexcept
    {
        if (_Parent)
        {
            return _Parent->GetWorldTransformMatrix();
        }
        return Identity<F_Matrix4x4_F32>();
    }
    F_Matrix4x4 F_SceneComponent::GetWorldTransformMatrix() const noexcept
    {
        if (_Parent)
        {
            return _Parent->CalculateChildWorldTransformMatrix(_LocalTransform);
        }
        return GetLocalTransformMatrix();
    }
    F_Matrix4x4 F_SceneComponent::GetLocalTransformMatrix() const noexcept
    {
        return _LocalTransform.GetMatrix4x4();
    }

    F_Vector3 F_SceneComponent::GetLocalRight() const
    {
        return GetLocalRotation() * F_Vector3::Right();
    }
    F_Vector3 F_SceneComponent::GetLocalLeft() const
    {
        return GetLocalRotation() * F_Vector3::Left();
    }
    F_Vector3 F_SceneComponent::GetLocalUp() const
    {
        return GetLocalRotation() * F_Vector3::Up();
    }
    F_Vector3 F_SceneComponent::GetLocalDown() const
    {
        return GetLocalRotation() * F_Vector3::Down();
    }
    F_Vector3 F_SceneComponent::GetLocalForward() const
    {
        return GetLocalRotation() * F_Vector3::Forward();
    }
    F_Vector3 F_SceneComponent::GetLocalBackward() const
    {
        return GetLocalRotation() * F_Vector3::Backward();
    }

    F_Vector3 F_SceneComponent::GetWorldRight() const
    {
        return GetWorldRotation() * F_Vector3::Right();
    }
    F_Vector3 F_SceneComponent::GetWorldLeft() const
    {
        return GetWorldRotation() * F_Vector3::Left();
    }
    F_Vector3 F_SceneComponent::GetWorldUp() const
    {
        return GetWorldRotation() * F_Vector3::Up();
    }
    F_Vector3 F_SceneComponent::GetWorldDown() const
    {
        return GetWorldRotation() * F_Vector3::Down();
    }
    F_Vector3 F_SceneComponent::GetWorldForward() const
    {
        return GetWorldRotation() * F_Vector3::Forward();
    }
    F_Vector3 F_SceneComponent::GetWorldBackward() const
    {
        return GetWorldRotation() * F_Vector3::Backward();
    }
}
