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
        const TS<A_RHITemplateDatabase>& TemplateDatabase,
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
        const TS<A_RHITemplateDatabase>& TemplateDatabase, 
        const TF_Vector<TS<A_RHITemplate>>& Templates
    )
    {
        TF_Vector<TS<A_RHITemplate>> TemplatesToWrite;
        A_RHITemplate::GatherSortedListWithDependencies(
            TemplateDatabase.Weak(),
            Templates,
            TemplatesToWrite
        );
        
        ABYTEK_FEEDBACK_STATUS_CHECK(View << TemplatesToWrite.size());
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
        const TS<A_RHITemplateDatabase>& TemplateDatabase
    )
    {
        return TryWriteTemplatePack(
            View,
            TemplateDatabase,
            TemplateDatabase->GatherTemplates()
        );
    }
}
