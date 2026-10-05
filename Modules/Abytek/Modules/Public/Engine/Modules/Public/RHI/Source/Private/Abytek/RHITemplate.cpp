#include "Abytek/RHITemplate.hpp"
#include "Abytek/RHITemplateDatabase.hpp"


namespace Abytek
{
    A_RHITemplate::A_RHITemplate(
        const TS<A_RHITemplateDatabase>& Database,
        F_RHITemplateHashCode HashCode
    ) :
        _Database(Database),
        _HashCode(HashCode)
    {
        _Database->_TrackTemplate(ABYTEK_WTHIS());
    }
    A_RHITemplate::~A_RHITemplate()
    {
        _Database->_UntrackTemplate(ABYTEK_WTHIS());
    }

    B8 A_RHITemplate::HasDependencyHashCode(F_RHITemplateHashCode DependencyHashCode)
    {
        return (
            std::find(
                _DependencyHashCodes.begin(),    
                _DependencyHashCodes.end(),
                DependencyHashCode
            )    
            != _DependencyHashCodes.end()
        );
    }
    void A_RHITemplate::AddDependencyHashCode(F_RHITemplateHashCode DependencyHashCode)
    {
        ABYTEK_ENGINE_RHI_ASSERT(!HasDependencyHashCode(DependencyHashCode)) << "This dependency was already added";
        _DependencyHashCodes.push_back(DependencyHashCode);
    }
    void A_RHITemplate::RemoveDependencyHashCode(F_RHITemplateHashCode DependencyHashCode)
    {
        ABYTEK_ENGINE_RHI_ASSERT(HasDependencyHashCode(DependencyHashCode)) << "Not found dependency";
        _DependencyHashCodes.erase(
            std::find(
                _DependencyHashCodes.begin(),    
                _DependencyHashCodes.end(),
                DependencyHashCode
            )       
        );
    }
    
    TS_Valid<A_RHITemplateRuntime> A_RHITemplate::CreateAndBuildRuntime(const TW_Valid<A_RHIContext>& Context)
    {
        return {};
    }

    void A_RHITemplate::PostCreateExportedData(const TS<A_RHITemplateExportedData>& ExportedData) const
    {
        ExportedData->HashCode = _HashCode;
    }

    void A_RHITemplate::GatherSortedListWithDependencies(
        const TW_Valid<A_RHITemplateDatabase>& TemplateDatabase, 
        const TF_Vector<TS<A_RHITemplate>>& Templates,
        TF_Vector<TS<A_RHITemplate>>& OutList
    )
    {
        TF_Set<TS<A_RHITemplate>> InitialTemplateSet;
        for (const auto& Template : Templates)
        {
            InitialTemplateSet.insert(Template);
        }
        
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
                    if (
                        Template->IsRootTemplate()
                        && !InitialTemplateSet.contains(Template)
                    )
                    {
                        continue;
                    }
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
        
        for (auto TemplateIndex : TemplateRemap)
        {
            OutList.push_back(FullyGatheredTemplates[TemplateIndex]);
        }
    }
}
