#pragma once

#include "Abytek/RHITemplateSerializer.hpp"


namespace Abytek
{
    namespace DirectX12SharedRHITemplateSerializer
    {
        enum class E_TemplateType : U8
        {
            NONE,
            BIND_GROUP,
            PIPELINE_STATE,
            ROOT_SIGNATURE
        };
        ABYTEK_ENGINE_RHI_API E_TemplateType GetTemplateType(const TS_Valid<A_RHITemplate>& Template);
    }
    
    class ABYTEK_ENGINE_RHI_API F_DirectX12SharedRHITemplateSerializer : public A_RHITemplateSerializer
    {
    private:

    public:
 
    public:
        F_DirectX12SharedRHITemplateSerializer(const F_RHIFeatureSupports& FeatureSupports);
        virtual ~F_DirectX12SharedRHITemplateSerializer();
        
    public:
        virtual F_FeedbackStatus TryReadTemplate(F_ArchiveReadOnlyView& View, const TW_Valid<A_RHITemplateDatabase>& TemplateDatabase, TS<A_RHITemplate>& OutTemplate) override;
        virtual F_FeedbackStatus TryWriteTemplate(F_ArchiveReadWriteView& View, const TW_Valid<A_RHITemplateDatabase>& TemplateDatabase, const TS_Valid<A_RHITemplate>& Template) override;
    };
}