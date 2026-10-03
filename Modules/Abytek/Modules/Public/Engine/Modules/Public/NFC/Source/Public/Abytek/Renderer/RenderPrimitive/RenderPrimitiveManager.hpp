#pragma once

#include "Abytek/Renderer/RenderObject.hpp"
#include "Abytek/GlobalRenderPipeline.hpp"
#include "Abytek/Renderer/RenderPrimitive/RenderPrimitiveSet.hpp"
#include "Abytek/Renderer/RenderPrimitive/RenderPrimitiveProcessor.hpp"


namespace Abytek
{
    class A_RenderScene;
    class A_RenderPrimitiveSet;
    
    struct F_RenderPrimitiveManagerBuildParams
    {
        TW<A_RenderScene> Scene;
    };
    class ABYTEK_ENGINE_NFC_API F_RenderPrimitiveManager final : public A_RenderObject
    {
    public:
        friend class A_RenderPrimitiveSet;
        friend class A_RenderPrimitiveProcessor;
        
    private:
        TW<A_RenderScene> _Scene;
        
        TF_Vector<TS<A_RenderPrimitiveProcessor>> _Processors;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetScene() const noexcept
        {
            return _Scene;
        }
        
        ABYTEK_FORCE_INLINE const auto& GetProcessors() const noexcept
        {
            return _Processors;
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
        void FinalizeFrame(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer);
        
    public:
        template<typename __F_Processor, typename... __F_Args>
        auto CreateAndAddProcessor(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer, const F_Name& Name, __F_Args&&... Args)
        {
            auto Processor = __F_Processor::Create(GetWorldRenderResource());
#ifdef ABYTEK_DEBUG_INFO
            Processor->SetDebugName(Name);
#endif
            return AddProcessor<__F_Processor>(
                SubmissionItemContainer,
                Processor,
                ABYTEK_FORWARD(Args)...
            );
        }
        template<typename __F_Processor, typename... __F_Args>
        auto AddProcessor(
            const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer, 
            const TS<__F_Processor>& Processor, 
            __F_Args&&... Args
        )
        {
            ABYTEK_ENGINE_NFC_ASSERT(_Processors.size() < INVALID_RENDER_PRIMITIVE_PROCESSOR_ID) << "Exceeded render primitive processor limit";
            Processor->Init(
                SubmissionItemContainer, 
                ABYTEK_WTHIS(), 
                static_cast<F_RenderPrimitiveProcessorId>(_Processors.size()),
                ABYTEK_FORWARD(Args)...
            );
            _Processors.push_back(Processor);
            return Processor.Weak();
        }
    };
}
