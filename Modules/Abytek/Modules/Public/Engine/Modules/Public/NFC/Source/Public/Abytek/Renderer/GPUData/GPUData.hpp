#pragma once

#include "Abytek/Renderer/RenderObject.hpp"
#include "Abytek/Renderer/GPUData/GPUDataComponentType.hpp"


namespace Abytek
{
    class A_RenderScene;
    class F_GPUDataComponentType;
    class F_GPUDataStorage;
    class F_GPUDataInstanceSet;

    struct F_GPUDataBuildParams
    {
        TW<A_RenderScene> Scene;
        TF_Vector<F_GPUDataComponentTypeConfig> ComponentTypes;
    };
    class ABYTEK_ENGINE_NFC_API F_GPUData final : public A_RenderObject
    {
    public:
        friend class F_GPUDataInstanceSet;
        friend class F_GPUDataComponentType;
        
    private:
        TW<A_RenderScene> _Scene;
        TF_Vector<TS<F_GPUDataComponentType>> _ComponentTypes;
        TF_Vector<U32> _ComponentIndexToSizeInBytes;
        TF_Vector<U32> _ComponentIndexToAlignmentInBytes;
        
        F_YieldCriticalSection _CriticalSection;
        
        TS<F_GPUDataStorage> _Storage;
        TF_Set<TW<F_GPUDataInstanceSet>> _InstanceSets;
        
        TF_Set<TS<F_GPUDataInstanceSet>> _DirtyInstanceSets;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetScene() const noexcept
        {
            return _Scene;
        }
        ABYTEK_FORCE_INLINE const auto& GetComponentTypes() const noexcept
        {
            return _ComponentTypes;
        }
        ABYTEK_FORCE_INLINE const auto& GetComponentIndexToSizeInBytes() const noexcept
        {
            return _ComponentIndexToSizeInBytes;
        }
        ABYTEK_FORCE_INLINE const auto& GetComponentIndexToAlignmentInBytes() const noexcept
        {
            return _ComponentIndexToAlignmentInBytes;
        }
        
        ABYTEK_FORCE_INLINE const auto& GetStorage() const noexcept
        {
            return _Storage;
        }
        ABYTEK_FORCE_INLINE const auto& GetInstanceSets() const noexcept
        {
            return _InstanceSets;
        }
        
    public:
        ABYTEK_RENDER_OBJECT_CREATABLE(F_GPUData, A_RenderObject);
        
    public:
        void Init(
            const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer, 
            const F_GPUDataBuildParams& BuildParams
        );
        void Release(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer) override;
        
    public:
        void BeginUpdate(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer);
        void EndUpdate(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer);
        void BeginPostUpdate(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer);
        void EndPostUpdate(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer);
        
    private:
        void _RegisterInstanceSet(const TW_Valid<F_GPUDataInstanceSet>& InstanceSet);
        void _UnregisterInstanceSet(const TW_Valid<F_GPUDataInstanceSet>& InstanceSet);
        
    private:
        void _AddDirtyInstanceSet(const TS<F_GPUDataInstanceSet>& InstanceSet);
        void _FlushDirtyInstanceSets(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer);
        
    public:
        B8 HasComponentType(const F_Name& Name) const;
        U32 GetComponentTypeIndex(const F_Name& Name) const;
        U32 GetComponentTypeIndex(const TW_Valid<F_GPUDataComponentType>& ComponentType) const;
        TS<F_GPUDataComponentType> FindComponentType(const F_Name& Name) const;
        TS<F_GPUDataComponentType> GetComponentType(const F_Name& Name) const;
        TS<F_GPUDataComponentType> GetComponentType(U32 Index) const;
        
    public:
        template<typename __F>
        B8 HasComponentType() const
        {
            return HasComponentType(__F::GetStaticName());
        }
        template<typename __F>
        U32 GetComponentTypeIndex() const
        {
            return GetComponentTypeIndex(__F::GetStaticName());
        }
        template<typename __F>
        TS<F_GPUDataComponentType> FindComponentType() const
        {
            return FindComponentType(__F::GetStaticName());
        }
        template<typename __F>
        TS<F_GPUDataComponentType> GetComponentType() const
        {
            return GetComponentType(__F::GetStaticName());
        }
    };
}
