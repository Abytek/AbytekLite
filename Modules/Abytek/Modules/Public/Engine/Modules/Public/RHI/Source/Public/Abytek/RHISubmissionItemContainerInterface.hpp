#pragma once

#include "Abytek/Engine.RHI.prerequisites.hpp"
#include "Abytek/RHISubmissionListOrder.hpp"


namespace Abytek
{
    class A_RHISubmissionList;
    class A_RHISubmissionItem;
    struct I_RHISubmissionItemContainer;
    
    struct ABYTEK_ENGINE_RHI_API I_RHISubmissionItemContainer
    {
    public:
        virtual ~I_RHISubmissionItemContainer() = default;
        
    public:
        void AddSubmissionItem(const TS<A_RHISubmissionItem>& SubmissionItem)
        {
            AddBackSubmissionItem(SubmissionItem);
        }
        void AddFrontSubmissionList(const TS<A_RHISubmissionList>& SubmissionList);
        void AddBackSubmissionItem(const TS<A_RHISubmissionItem>& SubmissionItem);
        
    protected:
        virtual void OnAddFrontSubmissionList(const TS<A_RHISubmissionList>& SubmissionList) = 0;
        virtual void OnAddBackSubmissionItem(const TS<A_RHISubmissionItem>& SubmissionItem) = 0;
        
    public:
        TS<A_RHISubmissionList> AddChild(E_RHISubmissionListOrder Order, const F_DebugName& DebugName = {});
        TS<A_RHISubmissionList> AddChild(const F_DebugName& DebugName = {});
        
    protected:
        virtual TS<A_RHISubmissionList> OnAddChild(E_RHISubmissionListOrder Order, const F_DebugName& DebugName) = 0;
    };
}