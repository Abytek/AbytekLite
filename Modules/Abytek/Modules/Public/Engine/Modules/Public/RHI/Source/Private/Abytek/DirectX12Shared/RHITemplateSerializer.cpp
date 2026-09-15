#include "Abytek/DirectX12Shared/RHITemplateSerializer.hpp"
#include "Abytek/RHITemplate.hpp"
#include "Abytek/RHITemplateDatabase.hpp"
#include "Abytek/DirectX12Shared/RHIBindGroupTemplate.hpp"
#include "Abytek/DirectX12Shared/RHIPipelineStateTemplate.hpp"
#include "Abytek/DirectX12Shared/RHIRootSignatureTemplate.hpp"


namespace Abytek
{
    namespace DirectX12SharedRHITemplateSerializer
    {
        E_TemplateType GetTemplateType(const TS_Valid<A_RHITemplate>& Template)
        {
            if (Template.CheckPolymorphism<F_DirectX12SharedRHIBindGroupTemplate>())
            {
                return E_TemplateType::BIND_GROUP;
            }
            if (Template.CheckPolymorphism<F_DirectX12SharedRHIPipelineStateTemplate>())
            {
                return E_TemplateType::PIPELINE_STATE;
            }
            if (Template.CheckPolymorphism<F_DirectX12SharedRHIRootSignatureTemplate>())
            {
                return E_TemplateType::ROOT_SIGNATURE;
            }
            return E_TemplateType::NONE;
        }
    }
    
    F_DirectX12SharedRHITemplateSerializer::F_DirectX12SharedRHITemplateSerializer(const F_RHIFeatureSupports& FeatureSupports) :
        A_RHITemplateSerializer(E_RHIAPI::DIRECTX12, FeatureSupports)
    {
    }
    F_DirectX12SharedRHITemplateSerializer::~F_DirectX12SharedRHITemplateSerializer()
    {
    }

    F_FeedbackStatus F_DirectX12SharedRHITemplateSerializer::TryReadTemplate(
        F_ArchiveReadOnlyView& View,
        const TW_Valid<A_RHITemplateDatabase>& TemplateDatabase,
        TS<A_RHITemplate>& OutTemplate
    )
    {
        if (TemplateDatabase->GetAPI() != GetAPI())
        {
            return F_FeedbackStatus::MakeFailed(ABYTEK_TEXT("Invalid template database, mismatch API"));
        }
        if (TemplateDatabase->GetFeatureSupports() != GetFeatureSupports())
        {
            return F_FeedbackStatus::MakeFailed(ABYTEK_TEXT("Invalid template database, mismatch feature supports"));
        }
        
        F_RHITemplateHashCode TemplateHashCode = INVALID_RHI_TEMPLATE_HASH_CODE;
        ABYTEK_FEEDBACK_STATUS_CHECK(View >> TemplateHashCode);
        
        if (TemplateDatabase->HasTemplate(TemplateHashCode))
        {
            OutTemplate = TemplateDatabase->GetTemplate(TemplateHashCode);
            return F_FeedbackStatus::MakeSucceeded();
        }
        
        DirectX12SharedRHITemplateSerializer::E_TemplateType TemplateType = DirectX12SharedRHITemplateSerializer::E_TemplateType::NONE;
        ABYTEK_FEEDBACK_STATUS_CHECK(View >> TemplateType);
        switch (TemplateType)
        {
        case DirectX12SharedRHITemplateSerializer::E_TemplateType::BIND_GROUP:
            {
                F_RHIBindGroupTemplateBuildParams BuildParams;
                {
                    ABYTEK_FEEDBACK_STATUS_CHECK(View >> BuildParams);
                    BuildParams.Database = TemplateDatabase.Weak();
                    BuildParams.CustomHashCode = TemplateHashCode;
                }
                
                F_DirectX12SharedRHIBindGroupTemplateCompiledData CompiledData;
                {
                    ABYTEK_FEEDBACK_STATUS_CHECK(View >> CompiledData);
                }
                
#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
                F_RHIBindGroupTemplateCompileConfig CompileConfig;
                if (View.HasDevelopmentBuild)
                {
                    ABYTEK_FEEDBACK_STATUS_CHECK(View >> CompileConfig);
                }
#endif
                
                OutTemplate = TS<F_DirectX12SharedRHIBindGroupTemplate>()(
                    ABYTEK_MOVE(BuildParams),    
                    ABYTEK_MOVE(CompiledData)    
                );
#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
                if (View.HasDevelopmentBuild)
                {
                    OutTemplate.FastCast<F_DirectX12SharedRHIBindGroupTemplate>()->SetCompileConfig(CompileConfig);
                }
#endif
            }
            break;
        case DirectX12SharedRHITemplateSerializer::E_TemplateType::PIPELINE_STATE:
            {
                F_RHIPipelineStateTemplateBuildParams BuildParams;
                {
                    ABYTEK_FEEDBACK_STATUS_CHECK(View >> BuildParams);
                    BuildParams.Database = TemplateDatabase.Weak();
                    BuildParams.CustomHashCode = TemplateHashCode;
                }
                
                F_DirectX12SharedRHIPipelineStateTemplateCompiledData CompiledData;
                {
                    ABYTEK_FEEDBACK_STATUS_CHECK(View >> CompiledData);
                }
                
#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
                F_RHIPipelineStateTemplateCompileConfig CompileConfig;
                TF_Map<F_Text, F_RHISlangShaderFileVersion> SlangShaderFileVersions;
                if (View.HasDevelopmentBuild)
                {
                    ABYTEK_FEEDBACK_STATUS_CHECK(View >> CompileConfig);
                    ABYTEK_FEEDBACK_STATUS_CHECK(View >> SlangShaderFileVersions);
                }
#endif
                
                OutTemplate = TS<F_DirectX12SharedRHIPipelineStateTemplate>()(
                    ABYTEK_MOVE(BuildParams),    
                    ABYTEK_MOVE(CompiledData)    
                );
#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
                if (View.HasDevelopmentBuild)
                {
                    OutTemplate.FastCast<F_DirectX12SharedRHIPipelineStateTemplate>()->SetCompileConfig(CompileConfig);
                    for (const auto& [_, SlangShaderFileVersion] : SlangShaderFileVersions)
                    {
                        OutTemplate.FastCast<F_DirectX12SharedRHIPipelineStateTemplate>()->AddSlangShaderFileVersion(SlangShaderFileVersion);
                    } 
                }
#endif
            }
            break;
        case DirectX12SharedRHITemplateSerializer::E_TemplateType::ROOT_SIGNATURE:
            {
                F_DirectX12SharedRHIRootSignatureTemplateBuildParams BuildParams;
                {
                    ABYTEK_FEEDBACK_STATUS_CHECK(View >> BuildParams);
                    BuildParams.Database = TemplateDatabase.Weak();
                    BuildParams.CustomHashCode = TemplateHashCode;
                }
                
                F_DirectX12SharedRHIRootSignatureTemplateCompiledData CompiledData;
                {
                    ABYTEK_FEEDBACK_STATUS_CHECK(View >> CompiledData);
                }
                
#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
                F_DirectX12SharedRHIRootSignatureTemplateCompileConfig CompileConfig;
                if (View.HasDevelopmentBuild)
                {
                    ABYTEK_FEEDBACK_STATUS_CHECK(View >> CompileConfig);
                }
#endif
                
                OutTemplate = TS<F_DirectX12SharedRHIRootSignatureTemplate>()(
                    ABYTEK_MOVE(BuildParams),    
                    ABYTEK_MOVE(CompiledData)    
                );
#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
                if (View.HasDevelopmentBuild)
                {
                    OutTemplate.FastCast<F_DirectX12SharedRHIRootSignatureTemplate>()->SetCompileConfig(CompileConfig);
                }
#endif
            }
            break;
        default:
            return F_FeedbackStatus::MakeFailed();
        }
        
        return F_FeedbackStatus::MakeSucceeded();
    }
    F_FeedbackStatus F_DirectX12SharedRHITemplateSerializer::TryWriteTemplate(
        F_ArchiveReadWriteView& View, 
        const TW_Valid<A_RHITemplateDatabase>& TemplateDatabase,
        const TS_Valid<A_RHITemplate>& Template
    )
    {
        ABYTEK_FEEDBACK_STATUS_CHECK(View << Template->GetHashCode());
        
        DirectX12SharedRHITemplateSerializer::E_TemplateType TemplateType = DirectX12SharedRHITemplateSerializer::GetTemplateType(
            Template    
        );
        if (TemplateType == DirectX12SharedRHITemplateSerializer::E_TemplateType::NONE)
        {
            return F_FeedbackStatus::MakeFailed(ABYTEK_TEXT("Invalid template type"));
        }
        ABYTEK_FEEDBACK_STATUS_CHECK(View << TemplateType);

        switch (TemplateType)
        {
        case DirectX12SharedRHITemplateSerializer::E_TemplateType::BIND_GROUP:
            {
                auto BindGroupTemplate = Template.FastCast<F_DirectX12SharedRHIBindGroupTemplate>();
                
                {
                    const auto& Config = BindGroupTemplate->GetConfig();
                    ABYTEK_FEEDBACK_STATUS_CHECK(View << Config);
                }
                
                {
                    const auto& CompiledData = BindGroupTemplate->GetCompiledData();
                    ABYTEK_FEEDBACK_STATUS_CHECK(View << CompiledData);
                }
                
#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
                if (View.HasDevelopmentBuild)
                {
                    const auto& CompileConfig = BindGroupTemplate->GetCompileConfig();
                    ABYTEK_FEEDBACK_STATUS_CHECK(View << CompileConfig);
                }
#endif
            }
            break;
        case DirectX12SharedRHITemplateSerializer::E_TemplateType::PIPELINE_STATE:
            {
                auto PipelineStateTemplate = Template.FastCast<F_DirectX12SharedRHIPipelineStateTemplate>();
                
                {
                    const auto& Config = PipelineStateTemplate->GetConfig();
                    ABYTEK_FEEDBACK_STATUS_CHECK(View << Config);
                }
                
                {
                    const auto& CompiledData = PipelineStateTemplate->GetCompiledData();
                    ABYTEK_FEEDBACK_STATUS_CHECK(View << CompiledData);
                }
                
#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
                if (View.HasDevelopmentBuild)
                {
                    const auto& CompileConfig = PipelineStateTemplate->GetCompileConfig();
                    const auto& SlangShaderFileVersions = PipelineStateTemplate->GetSlangShaderFileVersions();
                    ABYTEK_FEEDBACK_STATUS_CHECK(View << CompileConfig);
                    ABYTEK_FEEDBACK_STATUS_CHECK(View << SlangShaderFileVersions);
                }
#endif
            }
            break;
        case DirectX12SharedRHITemplateSerializer::E_TemplateType::ROOT_SIGNATURE:
            {
                auto RootSignatureTemplate = Template.FastCast<F_DirectX12SharedRHIRootSignatureTemplate>();
                
                {
                    const auto& Config = RootSignatureTemplate->GetConfig();
                    ABYTEK_FEEDBACK_STATUS_CHECK(View << Config);
                }
                
                {
                    const auto& CompiledData = RootSignatureTemplate->GetCompiledData();
                    ABYTEK_FEEDBACK_STATUS_CHECK(View << CompiledData);
                }
                
#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
                if (View.HasDevelopmentBuild)
                {
                    const auto& CompileConfig = RootSignatureTemplate->GetCompileConfig();
                    ABYTEK_FEEDBACK_STATUS_CHECK(View << CompileConfig);
                }
#endif
            }
            break;
        default:
            ABYTEK_ENGINE_RHI_ASSERT(false) << "Unknown template type";
            break;
        }
        return F_FeedbackStatus::MakeSucceeded();
    }
}
