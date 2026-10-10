#include "Abytek/GlobalRenderPack.hpp"
#include "Abytek/GlobalRenderBinding.hpp"
#include "Abytek/GlobalRenderPipeline.hpp"
#include "Abytek/RenderCoreHelper.hpp"
#include "Abytek/ApplicationModuleContainer.hpp"
#include "Abytek/TaskScheduler_LowFrequencyWorkers.hpp"
#include "Abytek/TaskScheduler_MediumFrequencyWorkers.hpp"
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

    void F_GlobalRenderPack::OnLoad()
    {
        F_RenderPack::OnLoad();
#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
        PrepareTemplates(
            GetRegistry(), 
            ABYTEK_WTHIS().DynamicCast<A_RenderPackTemplateMap>()
        );
#endif
    }
    void F_GlobalRenderPack::OnUnload()
    {
        F_RenderPack::OnUnload();
    }

#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
    void F_GlobalRenderPack::PrepareTemplates(
        const TS<F_RenderRegistry>& RenderRegistry,
        const TW_Valid<A_RenderPackTemplateMap>& RenderPackTemplateMap
    )
    {
        TF_Set<F_RHITemplateHashCode> TemplateHashCodesToCompile;
        TF_Set<F_RHITemplateHashCode> TemplateHashCodes;
        
        // Gather global render binding types
        TF_Vector<TF_ReflectionTypeHandle<F_GlobalRenderBinding>> GlobalRenderBindingTypes;
        {
            auto BaseSubsystemType = TF_ReflectionTypeHandle<F_GlobalRenderBinding>(F_ReflectionContext::GetGlobal());
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
        
        // Gather global render pipeline types
        TF_Vector<TF_ReflectionTypeHandle<F_GlobalRenderPipeline>> GlobalRenderPipelineTypes;
        {
            auto BaseSubsystemType = TF_ReflectionTypeHandle<F_GlobalRenderPipeline>(F_ReflectionContext::GetGlobal());
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
        
        // Gather necessary items to compile global bindings
        TF_Vector<TF_Function<void(TF_Vector<TS<A_RHITemplate>>& OutTemplates)>> Commands_CompileBinding;
        for (const auto& Type : GlobalRenderBindingTypes)
        {
            const auto& Metadata = Type->GetMetadata();
            auto MetadataElementName = F_GlobalRenderBinding::GetMetadataElementName_BuildCommandsAndCompilationSet();
            ABYTEK_ENGINE_RENDER_CORE_ASSERT(Metadata.HasElement(MetadataElementName))
                << ABYTEK_TEXT("Not found BuildCommandsAndCompilationSet metadata element in global render binding type: ")
                << *Type->GetFullName();
            const auto& MetadataElement = Metadata.Get(MetadataElementName);
            const auto& CastedMetadataElement = AnyCast<F_GlobalRenderBinding::F_Metadata_BuildCommandsAndCompilationSet>(MetadataElement);
            ABYTEK_FEEDBACK_STATUS_CHECK_HARD(
                CastedMetadataElement(
                    ABYTEK_STHIS(), 
                    RenderRegistry, 
                    RenderPackTemplateMap, 
                    Commands_CompileBinding, 
                    TemplateHashCodesToCompile,
                    TemplateHashCodes
                )
            );
        }
        
        // Gather necessary items to compile global pipelines
        TF_Vector<TF_Function<void(TF_Vector<TS<A_RHITemplate>>& OutTemplates)>> Commands_CompilePipeline;
        for (const auto& Type : GlobalRenderPipelineTypes)
        {
            const auto& Metadata = Type->GetMetadata();
            auto MetadataElementName = F_GlobalRenderPipeline::GetMetadataElementName_BuildCommandsAndCompilationSet();
            ABYTEK_ENGINE_RENDER_CORE_ASSERT(Metadata.HasElement(MetadataElementName))
                << ABYTEK_TEXT("Not found BuildCommandsAndCompilationSet metadata element in global render pipeline type: ")
                << *Type->GetFullName();
            const auto& MetadataElement = Metadata.Get(MetadataElementName);
            const auto& CastedMetadataElement = AnyCast<F_GlobalRenderPipeline::F_Metadata_BuildCommandsAndCompilationSet>(MetadataElement);
            ABYTEK_FEEDBACK_STATUS_CHECK_HARD(
                CastedMetadataElement(
                    ABYTEK_STHIS(), 
                    RenderRegistry, 
                    RenderPackTemplateMap, 
                    Commands_CompilePipeline, 
                    TemplateHashCodesToCompile,
                    TemplateHashCodes
                )
            );
        }
        
        // Compile
        ExecuteExclusiveTemplateCompilation(
            RenderRegistry,
            RenderPackTemplateMap,
            TemplateHashCodesToCompile,
            TemplateHashCodes,
            [&](TF_Vector<TS<A_RHITemplate>>& OutNewTemplates)
            {
                ExecuteParallelCompileCommands(Commands_CompileBinding, OutNewTemplates);
                ExecuteParallelCompileCommands(Commands_CompilePipeline, OutNewTemplates);
            }
        );
    }
#endif
}
