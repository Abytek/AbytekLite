#pragma once

#include "Abytek/Engine.Core.prerequisites.hpp"
#include "Abytek/Development/Common/CommonToolchainUtilities.hpp"
#include "Abytek/Development/CMake/CMakeToolchainUtilities.hpp"


#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
namespace Abytek
{
    class F_DevelopmentData;

    enum class E_EBTEnvironment : U8
    {
        NONE,
        
        DEVELOPMENT,
        DEVELOPMENT_MAX_OPTIMIZED,
        COOKED,
        COOKED_MAX_OPTIMIZED,

#ifdef ABYTEK_EBT_ENVIRONMENT_DEVELOPMENT
        CURRENT = DEVELOPMENT,
#elif defined(ABYTEK_EBT_ENVIRONMENT_DEVELOPMENT_MAX_OPTIMIZED)
        CURRENT = DEVELOPMENT_MAX_OPTIMIZED,
#elif defined(ABYTEK_EBT_ENVIRONMENT_COOKED)
        CURRENT = COOKED,
#elif defined(ABYTEK_EBT_ENVIRONMENT_COOKED_MAX_OPTIMIZED)
        CURRENT = COOKED_MAX_OPTIMIZED,
#else
#error "Unknown environment"
#endif
    };
    
    struct F_EBTProjectInstanceParams
    {
        E_EBTEnvironment Environment = E_EBTEnvironment::NONE;
        E_Platform Platform = E_Platform::NONE;
        E_CMakeGenerator CMakeGenerator = E_CMakeGenerator::NONE;
        E_CMakeConfig CMakeConfig = E_CMakeConfig::NONE;
        TF_Vector<E_ProcessorArch> ProcessorArchs;
        
        static F_EBTProjectInstanceParams Make(
            E_EBTEnvironment InEnvironment = E_EBTEnvironment::NONE,
            E_Platform InPlatform = E_Platform::NONE,
            E_CMakeGenerator InCMakeGenerator = E_CMakeGenerator::NONE,
            E_CMakeConfig InCMakeConfig = E_CMakeConfig::NONE,
            const TF_Vector<E_ProcessorArch>& InProcessorArchs = {}
        )
        {
            F_EBTProjectInstanceParams Result;
            Result.Environment = InEnvironment;
            Result.Platform = InPlatform;
            Result.CMakeGenerator = InCMakeGenerator;
            Result.CMakeConfig = InCMakeConfig;
            Result.ProcessorArchs = InProcessorArchs;
            return Result;
        }
        static F_EBTProjectInstanceParams MakeCurrent()
        {
            F_EBTProjectInstanceParams Result;
            Result.Environment = E_EBTEnvironment::CURRENT;
            Result.Platform = E_Platform::CURRENT;
            Result.CMakeGenerator = E_CMakeGenerator::CURRENT;
            Result.CMakeConfig = E_CMakeConfig::CURRENT;
            Result.ProcessorArchs = { E_ProcessorArch::CURRENT };
            return Result;
        }
        friend B8 operator == (const F_EBTProjectInstanceParams& A, const F_EBTProjectInstanceParams& B)
        {
            return (
                (A.Environment == B.Environment)    
                && (A.Platform == B.Platform)   
                && (A.CMakeGenerator == B.CMakeGenerator)    
                && (A.CMakeConfig == B.CMakeConfig)    
                && (A.ProcessorArchs == B.ProcessorArchs)    
            );
        }
        friend B8 operator != (const F_EBTProjectInstanceParams& A, const F_EBTProjectInstanceParams& B)
        {
            return (
                (A.Environment != B.Environment)    
                || (A.Platform != B.Platform)    
                || (A.CMakeGenerator != B.CMakeGenerator)    
                || (A.CMakeConfig != B.CMakeConfig)    
                || (A.ProcessorArchs != B.ProcessorArchs)    
            );
        }
    };
    struct F_EBTCMakeInstanceQuery
    {
        E_ProcessorArch ProcessorArch = E_ProcessorArch::NONE;
        
        static F_EBTCMakeInstanceQuery Make(
            E_ProcessorArch InProcessorArch = E_ProcessorArch::NONE    
        )
        {
            F_EBTCMakeInstanceQuery Result;
            Result.ProcessorArch = InProcessorArch;
            return Result;
        }
        static F_EBTCMakeInstanceQuery MakeCurrent()
        {
            F_EBTCMakeInstanceQuery Result;
            Result.ProcessorArch = E_ProcessorArch::CURRENT;
            return Result;
        }
    };
    struct F_EBTCMakeInstanceParams
    {
        E_EBTEnvironment Environment = E_EBTEnvironment::NONE;
        E_Platform Platform = E_Platform::NONE;
        E_CMakeGenerator CMakeGenerator = E_CMakeGenerator::NONE;
        E_CMakeConfig CMakeConfig = E_CMakeConfig::NONE;
        E_ProcessorArch ProcessorArch;
        
        static F_EBTCMakeInstanceParams Make(
            E_EBTEnvironment InEnvironment = E_EBTEnvironment::NONE,
            E_Platform InPlatform = E_Platform::NONE,
            E_CMakeGenerator InCMakeGenerator = E_CMakeGenerator::NONE,
            E_CMakeConfig InCMakeConfig = E_CMakeConfig::NONE,
            E_ProcessorArch InProcessorArch = E_ProcessorArch::NONE
        )
        {
            F_EBTCMakeInstanceParams Result;
            Result.Environment = InEnvironment;
            Result.Platform = InPlatform;
            Result.CMakeGenerator = InCMakeGenerator;
            Result.CMakeConfig = InCMakeConfig;
            Result.ProcessorArch = InProcessorArch;
            return Result;
        }
        static F_EBTCMakeInstanceParams MakeCurrent()
        {
            F_EBTCMakeInstanceParams Result;
            Result.Environment = E_EBTEnvironment::CURRENT;
            Result.Platform = E_Platform::CURRENT;
            Result.CMakeGenerator = E_CMakeGenerator::CURRENT;
            Result.CMakeConfig = E_CMakeConfig::CURRENT;
            Result.ProcessorArch = E_ProcessorArch::CURRENT;
            return Result;
        }
        friend B8 operator == (const F_EBTCMakeInstanceParams& A, const F_EBTCMakeInstanceParams& B)
        {
            return (
                (A.Environment == B.Environment)    
                && (A.Platform == B.Platform)    
                && (A.CMakeGenerator == B.CMakeGenerator)    
                && (A.CMakeConfig == B.CMakeConfig)    
                && (A.ProcessorArch == B.ProcessorArch)    
            );
        }
        friend B8 operator != (const F_EBTCMakeInstanceParams& A, const F_EBTCMakeInstanceParams& B)
        {
            return (
                (A.Environment != B.Environment)  
                || (A.Platform != B.Platform)    
                || (A.CMakeGenerator != B.CMakeGenerator)    
                || (A.CMakeConfig != B.CMakeConfig)    
                || (A.ProcessorArch != B.ProcessorArch)    
            );
        }
        
        B8 IsMatchQuery(const F_EBTCMakeInstanceQuery& Query) const
        {
            if (Query.ProcessorArch != E_ProcessorArch::NONE)
            {
                if (Query.ProcessorArch != ProcessorArch)
                {
                    return false;
                }
            }
            return true;
        }
    };
    
    struct ABYTEK_ENGINE_CORE_API F_EBTCMakeInstance : F_EBTCMakeInstanceParams
    {
        F_EBTCMakeInstance(const F_EBTCMakeInstanceParams& Params = {});
        
        F_EBTCMakeInstance(const F_EBTCMakeInstance& X) = default;
        F_EBTCMakeInstance& operator = (const F_EBTCMakeInstance& X) = default;
        F_EBTCMakeInstance(F_EBTCMakeInstance&& X) = default;
        F_EBTCMakeInstance& operator = (F_EBTCMakeInstance&& X) = default;
    };
    struct ABYTEK_ENGINE_CORE_API F_EBTProjectInstance : F_EBTProjectInstanceParams
    {
        TF_Vector<F_EBTCMakeInstance> CMakeInstances;
        
        F_EBTProjectInstance(const F_EBTProjectInstanceParams& Params = {});
        
        F_EBTProjectInstance(const F_EBTProjectInstance& X) = default;
        F_EBTProjectInstance& operator = (const F_EBTProjectInstance& X) = default;
        F_EBTProjectInstance(F_EBTProjectInstance&& X) = default;
        F_EBTProjectInstance& operator = (F_EBTProjectInstance&& X) = default;
        
        TF_Vector<Sz> FindCMakeInstanceIndices(const F_EBTCMakeInstanceQuery& Query) const;
        TF_Vector<F_EBTCMakeInstance> FindCMakeInstances(const F_EBTCMakeInstanceQuery& Query) const;
        TF_Optional<F_EBTCMakeInstance> FindCMakeInstance(const F_EBTCMakeInstanceQuery& Query) const;
    };
    
    struct F_EBTApplicationBuildResult
    {
        F_Text DirectoryPath;
        F_Text ExecutableFilePath;
    };
    
    enum class E_EBTPrepareMode : U8
    {
        NONE,
        CMAKE_CONFIGURE,
        CMAKE_BUILD,
        BUILD_APPLICATION
    };
    
    struct ABYTEK_ENGINE_CORE_API H_EBTToolchainUtilities
    {
        static F_FeedbackStatus ResolveCMakeBuildDirectorySignatureHashFileName(
            const F_EBTCMakeInstance& CMakeInstance,
            F_Text& OutFileName
        );
        static F_FeedbackStatus ResolveCMakeBuildDirectorySignatureHash(
            const F_Text& ProjectPath, 
            const F_EBTCMakeInstance& CMakeInstance,
            F_Text& OutCMakeBuildDirectorySignature
        );
        static F_Text ResolveBuildDirectoryPath(const F_Text& ProjectPath);
        static F_FeedbackStatus ResolveCMakeBuildDirectoryPath(
            const F_Text& ProjectPath,
            const F_EBTCMakeInstance& CMakeInstance,
            F_Text& OutBuildDirectoryPath
        );
        static F_Text ResolveImportJSFilePath(const F_Text& BuildDirectoryPath);
        static F_Text ResolveDevelopmentDataDirectoryPath(const F_Text& BuildDirectoryPath);

        static F_FeedbackStatus GetDefaultPrepareMode(const F_EBTProjectInstance& ProjectInstance, E_EBTPrepareMode& OutValue);
        static F_FeedbackStatus Prepare(
            const F_Text& ProjectPath,
            const F_EBTProjectInstance& ProjectInstance,
            const TF_Vector<F_Name>& ApplicationFilter = {},
            E_EBTPrepareMode Mode = E_EBTPrepareMode::NONE
        );
        
        static F_FeedbackStatus ParseEnvironment(const F_Name& Name, E_EBTEnvironment& OutEnvironment);
        static F_FeedbackStatus GetEnvironmentName(E_EBTEnvironment Environment, F_Name& OutName);
        
        static F_FeedbackStatus ParsePrepareMode(const F_Name& Name, E_EBTPrepareMode& OutPrepareMode);
        static F_FeedbackStatus GetPrepareModeName(E_EBTPrepareMode PrepareMode, F_Name& OutName);
        
        static F_FeedbackStatus ResolveCookDataDirectoryPath(const F_Text& ModuleBuildDirectoryPath, F_Text& OutValue);
        
        static F_Name ResolveCookedExecutableName(const F_Name& ApplicationTargetName);
        static F_Name ResolveDevelopmentExecutableName(const F_Name& ApplicationTargetName);
        static F_Text ResolveCookedExecutableResolvedGeneratorExpressionsDirectoryPath(const F_Text& ModuleBuildDirectoryPath);
        static F_Text ResolveDevelopmentExecutableResolvedGeneratorExpressionsDirectoryPath(const F_Text& ModuleBuildDirectoryPath);
        static F_FeedbackStatus ResolveTargetFileTextFilePath(
            const F_Text& ResolvedGeneratorExpressionsDirectoryPath,
            F_Text& OutValue
        );
        static F_FeedbackStatus ResolveTargetFileDirectoryTextFilePath(
            const F_Text& ResolvedGeneratorExpressionsDirectoryPath,
            F_Text& OutValue
        );
        static F_FeedbackStatus ResolveTargetFilePath(
            const F_Text& ResolvedGeneratorExpressionsDirectoryPath,
            F_Text& OutValue
        );
        static F_FeedbackStatus ResolveTargetFileDirectoryPath(
            const F_Text& ResolvedGeneratorExpressionsDirectoryPath,
            F_Text& OutValue
        );
        
        static F_FeedbackStatus GetApplicationBuildResult(
            const F_Name& ApplicationTargetName,
            const TW_Valid<F_DevelopmentData>& DevelopmentData,
            const F_EBTProjectInstance& ProjectInstance,
            F_EBTApplicationBuildResult& OutApplicationBuildResult
        );
    };
}

namespace boost
{
    template<>
    struct hash<Abytek::F_EBTCMakeInstance>
    {
        size_t operator()(const Abytek::F_EBTCMakeInstance& X) const
        {
            size_t result = 0;
            hash_combine(result, X.Environment);
            hash_combine(result, X.Platform);
            hash_combine(result, X.CMakeGenerator);
            hash_combine(result, X.CMakeConfig);
            hash_combine(result, X.ProcessorArch);
            return result;
        }
    };
}
#endif