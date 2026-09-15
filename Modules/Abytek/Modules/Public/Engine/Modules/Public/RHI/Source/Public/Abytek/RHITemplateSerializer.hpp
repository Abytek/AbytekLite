#pragma once

#include "Abytek/Engine.RHI.prerequisites.hpp"
#include "Abytek/RHIFeature.hpp"
#include "Abytek/RHITemplate.hpp"
#include "Abytek/RHIAPI.hpp"


namespace Abytek
{
    class A_RHITemplateDatabase;
    
    class ABYTEK_ENGINE_RHI_API A_RHITemplateSerializer : public A_Object
    {
    private:
        E_RHIAPI _API = E_RHIAPI::NONE;
        F_RHIFeatureSupports _FeatureSupports;
        
    public:
        ABYTEK_FORCE_INLINE const auto GetAPI() const noexcept
        {
            return _API;
        }
        ABYTEK_FORCE_INLINE const auto& GetFeatureSupports() const noexcept
        {
            return _FeatureSupports;
        }

    public: 
        A_RHITemplateSerializer(E_RHIAPI API, const F_RHIFeatureSupports& FeatureSupports);
        virtual ~A_RHITemplateSerializer();

    public:
        static TU<A_RHITemplateSerializer> Create(E_RHIAPI API, const F_RHIFeatureSupports& FeatureSupports);
        
    public:
        virtual F_FeedbackStatus TryReadTemplate(F_ArchiveReadOnlyView& View, const TW_Valid<A_RHITemplateDatabase>& TemplateDatabase, TS<A_RHITemplate>& OutTemplate) = 0;
        virtual F_FeedbackStatus TryWriteTemplate(F_ArchiveReadWriteView& View, const TW_Valid<A_RHITemplateDatabase>& TemplateDatabase, const TS_Valid<A_RHITemplate>& Template) = 0;
        
    public:
        F_FeedbackStatus TryReadTemplatePack(
            F_ArchiveReadOnlyView& View, 
            const TW_Valid<A_RHITemplateDatabase>& TemplateDatabase,
            TF_Vector<TS<A_RHITemplate>>& OutTemplates
        );
        F_FeedbackStatus TryWriteTemplatePack(
            F_ArchiveReadWriteView& View, 
            const TW_Valid<A_RHITemplateDatabase>& TemplateDatabase, 
            const TF_Vector<TS<A_RHITemplate>>& Templates
        );
        F_FeedbackStatus TryWriteTemplatePack(
            F_ArchiveReadWriteView& View, 
            const TW_Valid<A_RHITemplateDatabase>& TemplateDatabase
        );
    };
}
