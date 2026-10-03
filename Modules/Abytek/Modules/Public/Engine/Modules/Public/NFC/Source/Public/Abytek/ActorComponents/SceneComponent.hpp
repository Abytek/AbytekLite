#pragma once

#include "Abytek/ActorComponents/RenderableComponent.hpp"


namespace Abytek
{
    class ABYTEK_ENGINE_NFC_API F_SceneComponent : public A_ActorComponent
    {
    public:
        ABYTEK_BEGIN_REFLECTOR(A_ActorComponent)
        ABYTEK_END_REFLECTOR(F_SceneComponent);
    
    private:
        TW<F_SceneComponent> _Parent;
        TF_Vector<TW<F_SceneComponent>> _Childs;
        U32 _IndexInParent = ~U32(0);
        F_Transform _LocalTransform = F_Transform::MakeIdentity();
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetParent() const noexcept
        {
            return _Parent;
        }
        ABYTEK_FORCE_INLINE const auto& GetChilds() const noexcept
        {
            return _Childs;
        }
        ABYTEK_FORCE_INLINE auto GetIndexInParent() const noexcept
        {
            return _IndexInParent;
        }
        ABYTEK_FORCE_INLINE const auto& GetLocalTransform() const noexcept
        {
            return _LocalTransform;
        }
        ABYTEK_FORCE_INLINE const auto& GetLocalPosition() const noexcept
        {
            return _LocalTransform.Position;
        }
        ABYTEK_FORCE_INLINE const auto& GetLocalRotation() const noexcept
        {
            return _LocalTransform.Rotation;
        }
        ABYTEK_FORCE_INLINE const auto& GetLocalScale() const noexcept
        {
            return _LocalTransform.Scale;
        }
        
    public:
        F_SceneComponent(const F_SerializableObjectInitParams& InitParams);
        ~F_SceneComponent() override;
        
    protected:
        void OnRegisterComponent() override;
        void OnUnregisterComponent() override;
        
    protected:
        virtual void OnParentChanged();
        virtual void OnTransformChanged();
        
    public:
        void SetParent(const TW<F_SceneComponent>& Value);
        void SetLocalTransform(const F_Transform& Value);
        void SetLocalPosition(const F_Vector3& Value);
        void SetLocalRotation(const F_Rotator& Value);
        void SetLocalScale(const F_Vector3& Value);
        
    public:
        void SetWorldPosition(const F_Vector3& Value);
        void SetWorldRotation(const F_Rotator& Value);
        
    public:
        void AddLocalPositionOffset(const F_Vector3& Value);
        void AddLocalRotationOffset(const F_Vector3& EulerAngles);
        void AddLocalRotationOffset(const F_Rotator& Value);
        
    public:
        void AddWorldPositionOffset(const F_Vector3& Value);
        void AddWorldRotationOffset(const F_Vector3& EulerAngles);
        void AddWorldRotationOffset(const F_Rotator& Value);
        
    public:
        F_Vector3 CalculateChildWorldPosition(const F_Transform& ChildLocalTransform) const noexcept;
        F_Rotator CalculateChildWorldRotation(const F_Transform& ChildLocalTransform) const noexcept;
        F_Matrix4x4 CalculateChildWorldTransformMatrix(const F_Transform& ChildLocalTransform) const noexcept;
        
    public:
        F_Vector3 GetWorldPosition() const noexcept;
        F_Rotator GetBaseWorldRotation() const noexcept;
        F_Rotator GetWorldRotation() const noexcept;
        F_Matrix4x4 GetBaseWorldTransformMatrix() const noexcept;
        F_Matrix4x4 GetWorldTransformMatrix() const noexcept;
        F_Matrix4x4 GetLocalTransformMatrix() const noexcept;
        
    public:
        F_Vector3 GetLocalRight() const;
        F_Vector3 GetLocalLeft() const;
        F_Vector3 GetLocalUp() const;
        F_Vector3 GetLocalDown() const;
        F_Vector3 GetLocalForward() const;
        F_Vector3 GetLocalBackward() const;
        
    public:
        F_Vector3 GetWorldRight() const;
        F_Vector3 GetWorldLeft() const;
        F_Vector3 GetWorldUp() const;
        F_Vector3 GetWorldDown() const;
        F_Vector3 GetWorldForward() const;
        F_Vector3 GetWorldBackward() const;
    };
}