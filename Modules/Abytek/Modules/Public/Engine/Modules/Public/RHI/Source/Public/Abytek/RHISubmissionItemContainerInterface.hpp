#pragma once

#include "Abytek/Engine.RHI.prerequisites.hpp"


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
        void AddSubmissionItem(const TS_Valid<A_RHISubmissionItem>& SubmissionItem)
        {
            AddBackSubmissionItem(SubmissionItem);
        }
        void AddFrontSubmissionList(const TS_Valid<A_RHISubmissionList>& SubmissionList);
        void AddBackSubmissionItem(const TS_Valid<A_RHISubmissionItem>& SubmissionItem);
        
    protected:
        virtual void OnAddFrontSubmissionList(const TS_Valid<A_RHISubmissionList>& SubmissionList) = 0;
        virtual void OnAddBackSubmissionItem(const TS_Valid<A_RHISubmissionItem>& SubmissionItem) = 0;
    };
}