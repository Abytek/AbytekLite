#pragma once

#include "Abytek/Engine.NFC.prerequisites.hpp"
#include "Abytek/Renderer/RenderObject.hpp"


namespace Abytek
{
    class F_RenderPrimitiveManager;
    class A_RenderPrimitiveProcessor;
    class F_GPUDataInstanceSet;
    
    struct F_RenderPrimitiveSetConfig
    {
        U32 Num = 1;
    };

    class ABYTEK_ENGINE_NFC_API A_RenderPrimitiveSet : public A_RenderObject
    {
    public:
        friend class F_RenderPrimitiveManager;
        friend class A_RenderPrimitiveProcessor;
        
    private:
        TW<A_RenderPrimitiveProcessor> _Processor;
        U32 _Num = 0;
        TS<F_GPUDataInstanceSet> _GPUDataInstanceSet;
        U32 _Index = 0;
        
    public:
        TW_Valid<F_RenderPrimitiveManager> GetManager() const;
        ABYTEK_FORCE_INLINE const auto& GetProcessor() const noexcept
        {
            return _Processor;
        }
        ABYTEK_FORCE_INLINE auto GetNum() const noexcept
        {
            return _Num;
        }
        ABYTEK_FORCE_INLINE const auto& GetGPUDataInstanceSet() const noexcept
        {
            return _GPUDataInstanceSet;
        }
        ABYTEK_FORCE_INLINE auto GetIndex() const noexcept
        {
            return _Index;
        }
        
    public:
        ABYTEK_RENDER_OBJECT(A_RenderPrimitiveSet, A_RenderObject);
        
    protected:
        void InitPrimitiveSet(
            const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer, 
            const TW_Valid<A_RenderPrimitiveProcessor>& Processor,
            const F_RenderPrimitiveSetConfig& Config
        );
        
    public:
        void Release(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer) override;
    };
}
