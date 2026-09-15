#pragma once

#include "Abytek/Engine.RHI.prerequisites.hpp"
#include "Abytek/RHIBindGroupTemplate.hpp"
#include "Abytek/RHIPipelineStateTemplate.hpp"
#include "Abytek/RHIAPI.hpp"
#include "Abytek/RHIFeedbackStatus.hpp"


#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
namespace Abytek
{
    using F_RHICommonCompilationStatus = F_RHIFeedbackStatus;
    
    class ABYTEK_ENGINE_RHI_API A_RHICompiler : public A_Object
    {
    private:
        E_RHIAPI _API = E_RHIAPI::NONE;

    public:
        ABYTEK_FORCE_INLINE E_RHIAPI GetAPI() const noexcept
        {
            return _API;
        }

    public:
        A_RHICompiler(E_RHIAPI API);
        virtual ~A_RHICompiler();

    public:
        virtual F_RHICommonCompilationStatus ValidateBindGroupTemplate(
            const F_RHIBindGroupTemplateCompileParams& CompileParams,
            const TW_Valid<A_RHITemplateDatabase>& TemplateDatabase
        );
        virtual F_RHICommonCompilationStatus ValidatePipelineStateTemplate(
            const F_RHIPipelineStateTemplateCompileParams& CompileParams,
            const TW_Valid<A_RHITemplateDatabase>& TemplateDatabase
        );

    public:
        virtual F_RHICommonCompilationStatus CompileBindGroupTemplate(
            const F_RHIBindGroupTemplateCompileParams& CompileParams,
            const TW_Valid<A_RHITemplateDatabase>& TemplateDatabase,
            TS<A_RHIBindGroupTemplate>& OutCompiledObject
        );
        virtual F_RHICommonCompilationStatus CompilePipelineStateTemplate(
            const F_RHIPipelineStateTemplateCompileParams& CompileParams,
            const TW_Valid<A_RHITemplateDatabase>& TemplateDatabase,
            TS<A_RHIPipelineStateTemplate>& OutCompiledObject
        );

    public:
        static TU<A_RHICompiler> Create(E_RHIAPI API);
    };
}
#endif