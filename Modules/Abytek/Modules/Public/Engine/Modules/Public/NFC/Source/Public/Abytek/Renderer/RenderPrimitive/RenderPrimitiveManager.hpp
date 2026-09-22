#pragma once

#include "Abytek/Renderer/RenderObject.hpp"
#include "Abytek/GlobalRenderPipeline.hpp"
#include "Abytek/Renderer/RenderPrimitive/Components/Component_Transform.hpp"
#include "Abytek/Renderer/RenderPrimitive/Components/Component_InverseTransposeTransform.hpp"


namespace Abytek
{
    class A_RenderScene;
    class F_GPUData;
    class F_RenderPrimitiveSet;
    
    namespace RenderPrimitive
    {
        struct F_DemoPipeline : F_GlobalRenderPipeline
        {
            ABYTEK_GLOBAL_RENDER_PIPELINE(F_DemoPipeline, ABYTEK_NAME("Abytek::RenderPrimitive::F_DemoPipeline"));
            
            static F_FeedbackStatus Build(F_Config& Config)
            {
                Config.Type = E_RHIPipelineStateType::COMPUTE;
                Config.ComputeShader = ABYTEK_GLOBAL_SHADER("MainCS", "NFC/RenderPrimitive/DemoCS", E_RHIShaderFrequency::COMPUTE);
                Config.BindGroups.push_back(
                    F_RHIPipelineStateTemplateBindGroup::Make(
                        F_Component_Transform::F_SRVBinding::GetTemplateHashCode()    
                    )
                );
                Config.BindGroups.push_back(
                    F_RHIPipelineStateTemplateBindGroup::Make(
                        F_Component_InverseTransposeTransform::F_UAVBinding::GetTemplateHashCode()    
                    )
                );
                GPUData::SetupCompileParams(Config);
                return F_FeedbackStatus::MakeSucceeded();
            }
        };
    }

    struct F_RenderPrimitiveManagerBuildParams
    {
        TW<A_RenderScene> Scene;
    };
    class ABYTEK_ENGINE_NFC_API F_RenderPrimitiveManager final : public A_RenderObject
    {
    public:
        friend class F_RenderPrimitiveSet;
        
    private:
        TW<A_RenderScene> _Scene;
        TS<F_GPUData> _GPUData;
        
        U32 _ComponentIndex_Transform = ~U32(0);
        U32 _ComponentIndex_InverseTransposeTransform = ~U32(0);
        U32 _ComponentIndex_MeshHandle = ~U32(0);
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetScene() const noexcept
        {
            return _Scene;
        }
        ABYTEK_FORCE_INLINE const auto& GetGPUData() const noexcept
        {
            return _GPUData;
        }
        
        ABYTEK_FORCE_INLINE auto GetComponentIndex_Transform() const noexcept
        {
            return _ComponentIndex_Transform;
        }
        ABYTEK_FORCE_INLINE auto GetComponentIndex_InverseTransposeTransform() const noexcept
        {
            return _ComponentIndex_InverseTransposeTransform;
        }
        ABYTEK_FORCE_INLINE auto GetComponentIndex_MeshHandle() const noexcept
        {
            return _ComponentIndex_MeshHandle;
        }
        
    public:
        ABYTEK_RENDER_OBJECT_CREATABLE(F_RenderPrimitiveManager, A_RenderObject);
        
    public:
        void Init(
            const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer, 
            const F_RenderPrimitiveManagerBuildParams& BuildParams
        );
        void Release(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer) override;
        
    public:
        void BeginUpdate(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer);
        void EndUpdate(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer);
        void BeginPostUpdate(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer);
        void EndPostUpdate(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer);
    };
}
