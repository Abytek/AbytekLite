#pragma once

#include "Abytek/Engine.RHI.prerequisites.hpp"
#include "Abytek/RHICaptureEventState.hpp"


namespace Abytek
{
    class A_RHISubmissionItemContainer;
    struct I_RHISubmissionItemContainer;
    struct A_RHISubmissionItemLocalCollector;
    class A_RHIProcess;
    
    class ABYTEK_ENGINE_RHI_API A_RHISubmissionItem : public A_RAObject
    {
    public:
        friend struct I_RHISubmissionItemContainer;
        
    private:
#ifdef ABYTEK_ENGINE_RHI_ENABLE_CAPTURE
        F_RHICaptureEventState _CaptureEventState;
        TF_Vector<F_RHICaptureEventState> _StackCaptureEventStates;
#endif
        
    public:
        virtual TW_Valid<A_RHIProcess> GetProcess() const = 0;
#ifdef ABYTEK_ENGINE_RHI_ENABLE_CAPTURE
        ABYTEK_FORCE_INLINE auto& GetCaptureEventState() noexcept
        {
            return _CaptureEventState;
        }
        ABYTEK_FORCE_INLINE const auto& GetCaptureEventState() const noexcept
        {
            return _CaptureEventState;
        }
        void SetCaptureEventState(const F_RHICaptureEventState& Value) noexcept
        {
            _CaptureEventState = Value;
        }
        ABYTEK_FORCE_INLINE const auto& GetStackCaptureEventStates() const noexcept
        {
            return _StackCaptureEventStates;
        }
        void AddStackCaptureEventState(const F_RHICaptureEventState& Value) noexcept
        {
            _StackCaptureEventStates.push_back(Value);
        }
#endif
        
    public:
        ABYTEK_RA_DECLARE_OBJECT(A_RHISubmissionItem);
        virtual void Release() override;

    protected:
        void BuildSubmissionItem();
        
    protected:
        virtual void OnAddItemsBefore(const TW_Valid<I_RHISubmissionItemContainer>& Container);
        virtual void OnAddItemsAfter(const TW_Valid<I_RHISubmissionItemContainer>& Container);
        
#ifdef ABYTEK_DEBUG_INFO
    public:
        void SetDebugName(const F_DebugName& Value) noexcept override;
#endif
    };
}
