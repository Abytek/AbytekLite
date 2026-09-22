#pragma once

#include "Abytek/Engine.NFC.prerequisites.hpp"
#include "Abytek/Renderer/RenderObject.hpp"
#include "Abytek/Renderer/RenderPrimitive/RenderPrimitiveCommon.hpp"
#include "Abytek/Renderer/RenderPrimitive/Components/Component_Transform.hpp"
#include "Abytek/Renderer/RenderPrimitive/Components/Component_InverseTransposeTransform.hpp"
#include "Abytek/Renderer/RenderPrimitive/Components/Component_MeshHandle.hpp"


namespace Abytek
{
    class F_RenderPrimitiveManager;
    class F_GPUDataInstanceSet;

    struct F_RenderPrimitiveSetBuildParams
    {
        TW<F_RenderPrimitiveManager> Manager;
        U32 Num = 0;
    };
    class ABYTEK_ENGINE_NFC_API F_RenderPrimitiveSet final : public A_RenderObject
    {
    public:
        friend class F_RenderPrimitiveManager;
        
    private:
        TW<F_RenderPrimitiveManager> _Manager;
        U32 _Num = 0;
        TS<F_GPUDataInstanceSet> _GPUDataInstanceSet;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetManager() const noexcept
        {
            return _Manager;
        }
        ABYTEK_FORCE_INLINE auto GetNum() const noexcept
        {
            return _Num;
        }
        ABYTEK_FORCE_INLINE const auto& GetGPUDataInstanceSet() const noexcept
        {
            return _GPUDataInstanceSet;
        }
        
    public:
        ABYTEK_RENDER_OBJECT_CREATABLE(F_RenderPrimitiveSet, A_RenderObject);
        
    public:
        void Init(
            const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer, 
            const F_RenderPrimitiveSetBuildParams& BuildParams
        );
        void Release(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer) override;
        
    public:
        void UploadComponents_Transform(const RenderPrimitive::F_Component_Transform* ValuePtr);
        void UploadComponents_InverseTransposeTransform(const RenderPrimitive::F_Component_InverseTransposeTransform* ValuePtr);
        void UploadComponents_MeshHandle(const RenderPrimitive::F_Component_MeshHandle* ValuePtr);
    };
}
