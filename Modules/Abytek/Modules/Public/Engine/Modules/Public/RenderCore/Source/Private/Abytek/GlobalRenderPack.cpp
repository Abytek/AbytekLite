#include "Abytek/GlobalRenderPack.hpp"
#include "Abytek/GlobalRenderBinding.hpp"
#include "Abytek/GlobalRenderPipeline.hpp"
#include "Abytek/RenderCoreHelper.hpp"
#include "Abytek/ApplicationModuleContainer.hpp"
#include "Abytek/Development/Cook/CookUtilities.hpp"
#include "Abytek/Development/CoreCookGraph/HighLevelCookRange.hpp"


namespace Abytek
{
    ABYTEK_REFLECT(F_GlobalRenderPack)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::GlobalRenderPack"));
    }

    F_Name F_GlobalRenderPack::GetStaticName()
    {
        return ABYTEK_NAME("GlobalRenderPack");
    } 
    F_Name F_GlobalRenderPack::GetStaticPackageName()
    {
        return ABYTEK_TEXT("@") + *A_ApplicationCore::GetInstance()->GetName() + ABYTEK_TEXT("::Intermediate::Assets:/Abytek/Internal/Engine/RenderCore/GlobalRenderPack");
    }

    F_GlobalRenderPack::F_GlobalRenderPack(const F_SerializableObjectInitParams& InitParams) :
        F_RenderPack(InitParams)
    {
    }
    F_GlobalRenderPack::~F_GlobalRenderPack()
    {
    }

#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
    void F_GlobalRenderPack::PrepareTemplates(
        const TW_Valid<F_SerializableEnvironment>& SerializableEnvironment,
        const TW_Valid<A_RenderPackTemplateMap>& RenderPackTemplateMap
    )
    {
        TF_Vector<TF_Function<F_FeedbackStatus(TF_Vector<TS<A_RHITemplate>>& OutTemplates)>> Commands;
        TF_Set<F_RHITemplateHashCode> TemplateHashCodesToCompile;
        
        TF_Vector<TF_ReflectionTypeHandle<A_GlobalRenderBinding>> GlobalRenderBindingTypes;
        {
            auto BaseSubsystemType = TF_ReflectionTypeHandle<A_GlobalRenderBinding>(F_ReflectionContext::GetGlobal());
            ABYTEK_ENGINE_NFC_ASSERT(BaseSubsystemType);
            F_ApplicationModuleContainer::GetInstance()->ForEachUnit(
                [&BaseSubsystemType, &GlobalRenderBindingTypes](const TW_Valid<F_ProgramUnit>& Unit)
                {
                    auto Module = Unit.FastCast<F_Module>();
                    auto ReflectionSession = Module->GetReflectionSession();
                    ReflectionSession->ForEachTypeDerivedFrom(
                        BaseSubsystemType,
                        [&GlobalRenderBindingTypes](const TW_Valid<F_ReflectionType>& Type)
                        {
                            GlobalRenderBindingTypes.push_back(Type);
                            return true;
                        }
                    );
                    return true;
                }
            );
        }
        for (const auto& Type : GlobalRenderBindingTypes)
        {
            const auto& Metadata = Type->GetMetadata();
            auto MetadataElementName = A_GlobalRenderBinding::GetMetadataElementName_BuildCommandsAndCompilationSet();
            ABYTEK_ENGINE_RENDER_CORE_ASSERT(Metadata.HasElement(MetadataElementName))
                << ABYTEK_TEXT("Not found BuildCommandsAndCompilationSet metadata element in global render binding type: ")
                << *Type->GetFullName();
            const auto& MetadataElement = Metadata.Get(MetadataElementName);
            const auto& CastedMetadataElement = AnyCast<A_GlobalRenderBinding::F_Metadata_BuildCommandsAndCompilationSet>(MetadataElement);
            ABYTEK_FEEDBACK_STATUS_CHECK_HARD(
                CastedMetadataElement(ABYTEK_STHIS(), SerializableEnvironment, RenderPackTemplateMap, Commands, TemplateHashCodesToCompile)
            );
        }
            
        TF_Vector<TF_ReflectionTypeHandle<A_GlobalRenderPipeline>> GlobalRenderPipelineTypes;
        {
            auto BaseSubsystemType = TF_ReflectionTypeHandle<A_GlobalRenderPipeline>(F_ReflectionContext::GetGlobal());
            ABYTEK_ENGINE_NFC_ASSERT(BaseSubsystemType);
            F_ApplicationModuleContainer::GetInstance()->ForEachUnit(
                [&BaseSubsystemType, &GlobalRenderPipelineTypes](const TW_Valid<F_ProgramUnit>& Unit)
                {
                    auto Module = Unit.FastCast<F_Module>();
                    auto ReflectionSession = Module->GetReflectionSession();
                    ReflectionSession->ForEachTypeDerivedFrom(
                        BaseSubsystemType,
                        [&GlobalRenderPipelineTypes](const TW_Valid<F_ReflectionType>& Type)
                        {
                            GlobalRenderPipelineTypes.push_back(Type);
                            return true;
                        }
                    );
                    return true;
                }
            );
        }
        for (const auto& Type : GlobalRenderPipelineTypes)
        {
            const auto& Metadata = Type->GetMetadata();
            auto MetadataElementName = A_GlobalRenderPipeline::GetMetadataElementName_BuildCommandsAndCompilationSet();
            ABYTEK_ENGINE_RENDER_CORE_ASSERT(Metadata.HasElement(MetadataElementName))
                << ABYTEK_TEXT("Not found BuildCommandsAndCompilationSet metadata element in global render pipeline type: ")
                << *Type->GetFullName();
            const auto& MetadataElement = Metadata.Get(MetadataElementName);
            const auto& CastedMetadataElement = AnyCast<A_GlobalRenderPipeline::F_Metadata_BuildCommandsAndCompilationSet>(MetadataElement);
            ABYTEK_FEEDBACK_STATUS_CHECK_HARD(
                CastedMetadataElement(ABYTEK_STHIS(), SerializableEnvironment, RenderPackTemplateMap, Commands, TemplateHashCodesToCompile)
            );
        }
        
        for (const auto& TemplateHashCode : TemplateHashCodesToCompile)
        {
            if (RenderPackTemplateMap->HasTemplate(TemplateHashCode))
            {
                RenderPackTemplateMap->RemoveTemplate(TemplateHashCode);
            }
        }
        RenderPackTemplateMap->RemoveUnusedTemplates();
        
        TF_Vector<TS<A_RHITemplate>> NewTemplates;
        for (const auto& Command : Commands)
        {
            ABYTEK_FEEDBACK_STATUS_CHECK_HARD(Command(NewTemplates));
        }
        for (const auto& Template : NewTemplates)
        {
            RenderPackTemplateMap->AddTemplate(Template);
        }
    }
#endif
}
