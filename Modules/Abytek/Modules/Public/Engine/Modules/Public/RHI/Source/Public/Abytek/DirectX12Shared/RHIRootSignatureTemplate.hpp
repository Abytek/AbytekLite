#pragma once

#include "Abytek/RHITemplate.hpp"
#include "Abytek/DirectX12Shared/APIWrapper.hpp"


namespace Abytek
{
    struct F_DirectX12SharedRHIRootSignatureTemplateCompiledData
    {
        TF_Vector<U8> Binary;
        
        friend F_FeedbackStatus operator << (F_ArchiveReadWriteView& View, const F_DirectX12SharedRHIRootSignatureTemplateCompiledData& Value) noexcept
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.Binary);
            return F_FeedbackStatus::MakeSucceeded();
        }
        friend F_FeedbackStatus operator >> (F_ArchiveReadOnlyView& View, F_DirectX12SharedRHIRootSignatureTemplateCompiledData& Value) noexcept
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.Binary);
            return F_FeedbackStatus::MakeSucceeded();
        }
    };
    
    struct F_DirectX12SharedRHIRootSignatureTemplateConfig
    {
        TF_SmallVector<DirectX12SharedAPIWrapper::F_RootParameter, 4> RootParameters;
        B8 AllowVertexInputLayout = false;
        B8 AllowBindless = false;
        
        friend F_FeedbackStatus operator << (F_ArchiveReadWriteView& View, const F_DirectX12SharedRHIRootSignatureTemplateConfig& Value) noexcept
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.RootParameters);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.AllowVertexInputLayout);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.AllowBindless);
            return F_FeedbackStatus::MakeSucceeded();
        }
        friend F_FeedbackStatus operator >> (F_ArchiveReadOnlyView& View, F_DirectX12SharedRHIRootSignatureTemplateConfig& Value) noexcept
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.RootParameters);
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.AllowVertexInputLayout);
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.AllowBindless);
            return F_FeedbackStatus::MakeSucceeded();
        }
    };

    struct F_DirectX12SharedRHIRootSignatureTemplateCompileConfig
    {
        friend F_FeedbackStatus operator << (F_ArchiveReadWriteView& View, const F_DirectX12SharedRHIRootSignatureTemplateCompileConfig& Value) noexcept
        {
            return F_FeedbackStatus::MakeSucceeded();
        }
        friend F_FeedbackStatus operator >> (F_ArchiveReadOnlyView& View, F_DirectX12SharedRHIRootSignatureTemplateCompileConfig& Value) noexcept
        {
            return F_FeedbackStatus::MakeSucceeded();
        }
        
        friend B8 operator == (const F_DirectX12SharedRHIRootSignatureTemplateCompileConfig& A, const F_DirectX12SharedRHIRootSignatureTemplateCompileConfig& B)
        {
            return true;
        }
        friend B8 operator != (const F_DirectX12SharedRHIRootSignatureTemplateCompileConfig& A, const F_DirectX12SharedRHIRootSignatureTemplateCompileConfig& B)
        {
            return !(A == B);
        }
    };
    
    struct F_DirectX12SharedRHIRootSignatureTemplateCompileParams : A_RHITemplateCompileParams, F_DirectX12SharedRHIRootSignatureTemplateConfig, F_DirectX12SharedRHIRootSignatureTemplateCompileConfig
    {
    };
    
    class ABYTEK_ENGINE_RHI_API F_DirectX12SharedRHIRootSignatureTemplate : public A_RHITemplate
    {
    private:
        F_DirectX12SharedRHIRootSignatureTemplateConfig _Config;
        F_DirectX12SharedRHIRootSignatureTemplateCompiledData _CompiledData;
#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
        F_DirectX12SharedRHIRootSignatureTemplateCompileConfig _CompileConfig;
#endif
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetConfig() const noexcept
        {
            return _Config;
        }
        ABYTEK_FORCE_INLINE const auto& GetCompiledData() const noexcept
        {
            return _CompiledData;
        }
#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
        ABYTEK_FORCE_INLINE const auto& GetCompileConfig() const noexcept
        {
            return _CompileConfig;
        }
#endif
        
        ABYTEK_FORCE_INLINE const auto& GetRootParameters() const noexcept
        {
            return _Config.RootParameters;
        }
        ABYTEK_FORCE_INLINE const auto& GetAllowVertexInputLayout() const noexcept
        {
            return _Config.AllowVertexInputLayout;
        }
        ABYTEK_FORCE_INLINE const auto& GetAllowBindless() const noexcept
        {
            return _Config.AllowBindless;
        }
        
    public:
        F_DirectX12SharedRHIRootSignatureTemplate(
            const TW_Valid<A_RHITemplateDatabase>& Database,
            F_RHITemplateHashCode HashCode,
            const F_DirectX12SharedRHIRootSignatureTemplateConfig& Config
#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
            , const F_DirectX12SharedRHIRootSignatureTemplateCompileConfig& CompileConfig,
#endif
            const F_DirectX12SharedRHIRootSignatureTemplateCompiledData& CompiledData
        );
        ~F_DirectX12SharedRHIRootSignatureTemplate() override;

    protected:
        TS_Valid<A_RHITemplateRuntime> CreateAndBuildRuntime(const TW_Valid<A_RHIContext>& Context) override;
        
    protected:
        TS<A_RHITemplateExportedData> CreateExportedData() const override;
        void PostCreateExportedData(const TS<A_RHITemplateExportedData>& ExportedData) const override;
    };
    
    struct F_DirectX12SharedRHIRootSignatureTemplateExportedData : A_RHITemplateExportedData
    {
        F_DirectX12SharedRHIRootSignatureTemplateConfig Config;
#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
        F_DirectX12SharedRHIRootSignatureTemplateCompileConfig CompileConfig;
#endif
        F_DirectX12SharedRHIRootSignatureTemplateCompiledData CompiledData;
        
    protected:
        TS<A_RHITemplate> CreateTemplate(const TW_Valid<A_RHITemplateDatabase>& Database) const override
        {
            return TS<F_DirectX12SharedRHIRootSignatureTemplate>()(
                Database,
                HashCode,
                Config,
#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
                CompileConfig,
#endif
                CompiledData
            );
        }
        void PostCreateTemplate(const TS<A_RHITemplate>& Template) const override
        {
            A_RHITemplateExportedData::PostCreateTemplate(Template);
        }
    };
}

namespace boost
{
    template<>
    struct hash<Abytek::F_DirectX12SharedRHIRootSignatureTemplateConfig>
    {
        size_t operator ()(const Abytek::F_DirectX12SharedRHIRootSignatureTemplateConfig& X) const
        {
            size_t Result = 0;
            hash_range(Result, X.RootParameters.begin(), X.RootParameters.end());
            hash_combine(Result, X.AllowVertexInputLayout);
            hash_combine(Result, X.AllowBindless);
            return Result;
        }
    };
}