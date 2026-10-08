#pragma once

#include "Abytek/Engine.NFC.prerequisites.hpp"
#include "Abytek/Renderer/RenderObject.hpp"
#include "Abytek/Renderer/GPUData/GPUDataCommon.hpp"


namespace Abytek
{
    class F_GPUData;
    class A_RenderPrimitiveSet;
    class F_RenderPrimitiveManager;
    
    using F_RenderPrimitiveProcessorId = U32;
    static constexpr F_RenderPrimitiveProcessorId INVALID_RENDER_PRIMITIVE_PROCESSOR_ID = F_RenderPrimitiveProcessorId(0x3F);

    class ABYTEK_ENGINE_NFC_API A_RenderPrimitiveProcessor : public A_RenderObject
    {
    public:
        friend class F_RenderPrimitiveManager;
        friend class A_RenderPrimitiveSet;
        
    private:
        TW<F_RenderPrimitiveManager> _Manager;
        F_RenderPrimitiveProcessorId _Id = INVALID_RENDER_PRIMITIVE_PROCESSOR_ID;
        
        F_YieldCriticalSection _CriticalSection;
        
        TF_Vector<TW<A_RenderPrimitiveSet>> _PrimitiveSets;
        
        TS<F_GPUData> _GPUData;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetManager() const noexcept
        {
            return _Manager;
        }
        ABYTEK_FORCE_INLINE auto GetId() const noexcept
        {
            return _Id;
        }
        ABYTEK_FORCE_INLINE const auto& GetPrimitiveSets() const noexcept
        {
            return _PrimitiveSets;
        }
        ABYTEK_FORCE_INLINE U32 GetNumPrimitiveSets() const noexcept
        {
            return static_cast<U32>(_PrimitiveSets.size());
        }
        
        ABYTEK_FORCE_INLINE const auto& GetGPUData() const noexcept
        {
            return _GPUData;
        }
        
    public:
        ABYTEK_RENDER_OBJECT(A_RenderPrimitiveProcessor, A_RenderObject);
        
    protected:
        void InitPrimitiveProcessor(
            const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer, 
            const TW_Valid<F_RenderPrimitiveManager>& Manager,
            F_RenderPrimitiveProcessorId Id
        );
        
    public:
        void Release(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer) override;
        
    private:
        void _RegisterInstanceSet(const TW_Valid<A_RenderPrimitiveSet>& PrimitiveSet);
        void _UnregisterInstanceSet(const TW_Valid<A_RenderPrimitiveSet>& PrimitiveSet);
        
    public:
        virtual void OnBeginUpdate(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer);
        virtual void OnEndUpdate(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer);
        virtual void OnFinalizeFrame(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer);
        
    public:
        void BeginUpdate(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer);
        void EndUpdate(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer);
        void FinalizeFrame(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer);
        
    protected:
        virtual void InitGPUData(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer, const TS<F_GPUData>& GPUData) = 0;
        
    protected:
        virtual void OnActivatePrimitiveSet(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer, const TS<A_RenderPrimitiveSet>& PrimitiveSet);
        virtual void OnDeactivatePrimitiveSet(const TS<A_RHISubmissionItemContainer>& SubmissionItemContainer, const TS<A_RenderPrimitiveSet>& PrimitiveSet);
    };
}
