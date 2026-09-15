#pragma once

#include "Abytek/Engine.RHI.prerequisites.hpp"


namespace Abytek
{
    struct I_RHISubmissionItemContainer;
    struct A_RHISubmissionItemLocalCollector;
    class A_RHIProcess;
    
    class ABYTEK_ENGINE_RHI_API A_RHISubmissionItem : public A_RAObject
    {
    public:
        friend struct I_RHISubmissionItemContainer;
        
    private:
#ifdef ABYTEK_ENGINE_RHI_ENABLE_PROFILER
        F_Vector3_F32 _ProfilerEventColor = F_Vector3_F32::One();
#endif
        
    public:
        virtual TW_Valid<A_RHIProcess> GetProcess() const = 0;
#ifdef ABYTEK_ENGINE_RHI_ENABLE_PROFILER
        ABYTEK_FORCE_INLINE const auto& GetProfilerEventColor() const noexcept
        {
            return _ProfilerEventColor;
        }
        void SetProfilerEventColor(const F_Vector3_F32& ProfilerEventColor) noexcept
        {
            _ProfilerEventColor = ProfilerEventColor;
        }
#endif
        
    public:
        ABYTEK_RA_DECLARE_OBJECT(A_RHISubmissionItem);
        virtual void Release() override;

    protected:
        virtual void Build();
        
    protected:
        virtual void OnAddItemsBefore(I_RHISubmissionItemContainer& Container);
        virtual void OnAddItemsAfter(I_RHISubmissionItemContainer& Container);
    };
}
