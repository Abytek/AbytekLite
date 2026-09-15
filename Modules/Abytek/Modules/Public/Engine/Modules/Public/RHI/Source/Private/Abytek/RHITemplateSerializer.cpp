#include "Abytek/RHITemplateSerializer.hpp"
#include "Abytek/RHITemplate.hpp"
#include "Abytek/RHITemplateDatabase.hpp"
#include "Abytek/DirectX12Shared/RHITemplateSerializer.hpp"


namespace Abytek
{
    A_RHITemplateSerializer::A_RHITemplateSerializer(E_RHIAPI API, const F_RHIFeatureSupports& FeatureSupports) :
        _API(API),
        _FeatureSupports(FeatureSupports)
    {
    }
    A_RHITemplateSerializer::~A_RHITemplateSerializer()
    { 
    }

    TU<A_RHITemplateSerializer> A_RHITemplateSerializer::Create(E_RHIAPI API, const F_RHIFeatureSupports& FeatureSupports)
    {
        switch (API)
        {
        case E_RHIAPI::DIRECTX12:
            return TU<F_DirectX12SharedRHITemplateSerializer>()(FeatureSupports);
        default:
            ABYTEK_ENGINE_RHI_ASSERT(false) << "Invalid RHI API, cannot create template Serializer";
        }
        return {};
    }

    F_FeedbackStatus A_RHITemplateSerializer::TryReadTemplatePack(
        F_ArchiveReadOnlyView& View,
        const TW_Valid<A_RHITemplateDatabase>& TemplateDatabase,
        TF_Vector<TS<A_RHITemplate>>& OutTemplates
    )
    {
        Sz NumTemplates;
        ABYTEK_FEEDBACK_STATUS_CHECK(View >> NumTemplates);
        for (Sz TemplateIndex = 0; TemplateIndex < NumTemplates; ++TemplateIndex)
        {
            TS<A_RHITemplate> Template;
            if (auto Status = TryReadTemplate(View, TemplateDatabase, Template); !Status)
            {
                return Status;
            }
            OutTemplates.push_back(Template);
        }
        return F_FeedbackStatus::MakeSucceeded();
    }
    F_FeedbackStatus A_RHITemplateSerializer::TryWriteTemplatePack(
        F_ArchiveReadWriteView& View, 
        const TW_Valid<A_RHITemplateDatabase>& TemplateDatabase, 
        const TF_Vector<TS<A_RHITemplate>>& Templates
    )
    {
        TF_Set<TS<A_RHITemplate>> FullyGatheredTemplateSet;
        TF_Vector<TS<A_RHITemplate>> FullyGatheredTemplates;
        {
            TF_Vector<TS<A_RHITemplate>> TemplateToGather;
            for (const auto& Template : Templates)
            {
                TemplateToGather.push_back(Template);
            }
            while (TemplateToGather.size() > 0)
            {
                TF_Vector<TS<A_RHITemplate>> CachedTemplateToGather = ABYTEK_MOVE(TemplateToGather);
                for (const auto& Template : CachedTemplateToGather)
                {
                    if (FullyGatheredTemplateSet.contains(Template))
                    {
                        continue;
                    }
                    FullyGatheredTemplateSet.insert(Template);
                    for (auto DependenyHashCode : Template->GetDependencyHashCodes())
                    {
                        TemplateToGather.push_back(TemplateDatabase->GetTemplate(DependenyHashCode));
                    }
                }
            }
            for (const auto& Template : FullyGatheredTemplateSet)
            {
                FullyGatheredTemplates.push_back(Template);
            }
        }
        
        auto NumFullyGatheredTemplates = FullyGatheredTemplates.size();
        
        TF_Map<F_RHITemplateHashCode, U32> TemplateHashCodeToIndex;
        for (U32 TemplateIndex = 0; TemplateIndex < NumFullyGatheredTemplates; ++TemplateIndex)
        {
            const auto& Template = FullyGatheredTemplates[TemplateIndex];
            TemplateHashCodeToIndex[Template->GetHashCode()] = TemplateIndex;
        }
        
        TF_Vector<U32> TemplateDependencyLevels;
        TemplateDependencyLevels.resize(NumFullyGatheredTemplates);
        for (auto& TemplateDependencyLevel : TemplateDependencyLevels)
        {
            TemplateDependencyLevel = 0;
        }
        {
            TF_Vector<U32> TemplateIndicesToUpdateDependencyLevels;
            for (U32 TemplateIndex = 0; TemplateIndex < NumFullyGatheredTemplates; ++TemplateIndex)
            {
                TemplateIndicesToUpdateDependencyLevels.push_back(TemplateIndex);
            }
            while (TemplateIndicesToUpdateDependencyLevels.size() > 0)
            {
                TF_Vector<U32> CachedTemplateIndicesToUpdateDependencyLevels = ABYTEK_MOVE(TemplateIndicesToUpdateDependencyLevels);
                for (U32 TemplateIndex : CachedTemplateIndicesToUpdateDependencyLevels)
                {
                    const auto& Template = FullyGatheredTemplates[TemplateIndex];
                    auto& DependencyLevel = TemplateDependencyLevels[TemplateIndex];
                    for (auto DependenyHashCode : Template->GetDependencyHashCodes())
                    {
                        auto DependencyIndex = TemplateHashCodeToIndex.find(DependenyHashCode)->second;
                        auto& DependencyLevelOfDependency = TemplateDependencyLevels[DependencyIndex];
                        DependencyLevelOfDependency = Max(DependencyLevelOfDependency, DependencyLevel + 1);
                        TemplateIndicesToUpdateDependencyLevels.push_back(DependencyIndex);
                    }
                }
            }
        }
        
        TF_Vector<U32> TemplateRemap;
        TemplateRemap.reserve(NumFullyGatheredTemplates);
        for (U32 TemplateIndex = 0; TemplateIndex < NumFullyGatheredTemplates; ++TemplateIndex)
        {
            TemplateRemap.push_back(TemplateIndex);
        }
        boost::sort(
            TemplateRemap,
            [&](U32 A, U32 B)
            {
                auto TemplateDependencyLevelA = TemplateDependencyLevels[A];
                auto TemplateDependencyLevelB = TemplateDependencyLevels[B];
                return TemplateDependencyLevelB < TemplateDependencyLevelA;
            }
        );
        
        TF_Vector<TS<A_RHITemplate>> TemplatesToWrite;
        for (auto TemplateIndex : TemplateRemap)
        {
            TemplatesToWrite.push_back(FullyGatheredTemplates[TemplateIndex]);
        }
        
        ABYTEK_FEEDBACK_STATUS_CHECK(View << NumFullyGatheredTemplates);
        for (auto Template : TemplatesToWrite)
        {
            if (auto Status = TryWriteTemplate(View, TemplateDatabase, Template); !Status)
            {
                return Status;
            }
        }
        return F_FeedbackStatus::MakeSucceeded();
    }
    F_FeedbackStatus A_RHITemplateSerializer::TryWriteTemplatePack(
        F_ArchiveReadWriteView& View,
        const TW_Valid<A_RHITemplateDatabase>& TemplateDatabase
    )
    {
        return TryWriteTemplatePack(
            View,
            TemplateDatabase,
            TemplateDatabase->GatherTemplates()
        );
    }
}
