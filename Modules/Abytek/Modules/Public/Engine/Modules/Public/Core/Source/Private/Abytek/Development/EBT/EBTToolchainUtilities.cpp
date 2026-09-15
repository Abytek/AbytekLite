#include "Abytek/Development/EBT/EBTToolchainUtilities.hpp"
#include "Abytek/Development/CMake/CmakeToolchainUtilities.hpp"
#include "Abytek/DevelopmentData.hpp"


#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
namespace Abytek
{
    F_EBTCMakeInstance::F_EBTCMakeInstance(const F_EBTCMakeInstanceParams& Params) :
        F_EBTCMakeInstanceParams(Params)
    {
    }
    F_EBTProjectInstance::F_EBTProjectInstance(const F_EBTProjectInstanceParams& Params) :
        F_EBTProjectInstanceParams(Params)
    {
        for (auto ProcessorArch : Params.ProcessorArchs)
        {
            F_EBTCMakeInstanceParams CMakeInstanceParams;
            CMakeInstanceParams.Environment = Params.Environment;
            CMakeInstanceParams.Platform = Params.Platform;
            CMakeInstanceParams.CMakeGenerator = Params.CMakeGenerator;
            CMakeInstanceParams.CMakeConfig = Params.CMakeConfig;
            CMakeInstanceParams.ProcessorArch = ProcessorArch;
            F_EBTCMakeInstance CMakeInstance(CMakeInstanceParams);
            CMakeInstances.push_back(CMakeInstance);
        }
    }

    TF_Vector<Sz> F_EBTProjectInstance::FindCMakeInstanceIndices(const F_EBTCMakeInstanceQuery& Query) const
    {
        TF_Vector<Sz> Result;
        Sz NumCMakeInstances = CMakeInstances.size();
        for (Sz Idx = 0; Idx < NumCMakeInstances; ++Idx)
        {
            const auto& CMakeInstance = CMakeInstances[Idx];
            if (CMakeInstance.IsMatchQuery(Query))
            {
                Result.push_back(Idx);
            }
        }
        return ABYTEK_MOVE(Result);
    }
    TF_Vector<F_EBTCMakeInstance> F_EBTProjectInstance::FindCMakeInstances(const F_EBTCMakeInstanceQuery& Query) const
    {
        auto Indices = FindCMakeInstanceIndices(Query);
        TF_Vector<F_EBTCMakeInstance> Result;
        for (Sz Idx : Indices)
        {
            Result.push_back(CMakeInstances[Idx]);
        }
        return ABYTEK_MOVE(Result);
    }
    TF_Optional<F_EBTCMakeInstance> F_EBTProjectInstance::FindCMakeInstance(const F_EBTCMakeInstanceQuery& Query) const
    {
        Sz NumCMakeInstances = CMakeInstances.size();
        for (Sz Idx = 0; Idx < NumCMakeInstances; ++Idx)
        {
            const auto& CMakeInstance = CMakeInstances[Idx];
            if (CMakeInstance.IsMatchQuery(Query))
            {
                return CMakeInstance;
            }
        }
        return {};
    }

    F_FeedbackStatus H_EBTToolchainUtilities::ResolveCMakeBuildDirectorySignatureHashFileName(
        const F_EBTCMakeInstance& CMakeInstance,
        F_Text& OutFileName
    )
    {
        F_Name EnvironmentName;
        if (auto Status = GetEnvironmentName(CMakeInstance.Environment, EnvironmentName); !Status)
        {
            return Status;
        }
        
        F_Name PlatformName;
        if (auto Status = H_CommonToolchainUtilities::GetPlatformName(CMakeInstance.Platform, PlatformName); !Status)
        {
            return Status;
        }
        
        F_Name CMakeGeneratorName;
        if (auto Status = H_CMakeToolchainUtilities::GetGeneratorName(CMakeInstance.CMakeGenerator, CMakeGeneratorName); !Status)
        {
            return Status;
        }
        
        F_Name CMakeConfigName;
        if (auto Status = H_CMakeToolchainUtilities::GetConfigName(CMakeInstance.CMakeConfig, CMakeConfigName); !Status)
        {
            return Status;
        }
        
        F_Name ProcessorArchName;
        if (auto Status = H_CommonToolchainUtilities::GetProcessorArchName(CMakeInstance.ProcessorArch, ProcessorArchName); !Status)
        {
            return Status;
        }
        
        OutFileName = (
            ABYTEK_TEXT("CMakeBuildDirectorySignatureHash-")
            + *EnvironmentName
            + ABYTEK_TEXT("-")
            + *PlatformName
            + ABYTEK_TEXT("-")
            + *CMakeGeneratorName
            + ABYTEK_TEXT("-")
            + *CMakeConfigName
            + ABYTEK_TEXT("-")
            + *ProcessorArchName
            + ABYTEK_TEXT(".txt")
        );
        return F_FeedbackStatus::MakeSucceeded();
    }
    F_FeedbackStatus H_EBTToolchainUtilities::ResolveCMakeBuildDirectorySignatureHash(
        const F_Text& ProjectPath,
        const F_EBTCMakeInstance& CMakeInstance,
        F_Text& OutCMakeBuildDirectorySignature
    )
    {
        auto BuildDirectoryPath = ResolveBuildDirectoryPath(ProjectPath);
        if (!H_FSUtilities::Exists(BuildDirectoryPath, E_FSEntryType::DIRECTORY))
        {
            return F_FeedbackStatus::MakeFailed(ABYTEK_TEXT("Not found build directory for project ") + ProjectPath);
        }
        
        F_Text CMakeBuildDirectorySignatureFileName;
        if (
            auto Status = ResolveCMakeBuildDirectorySignatureHashFileName(
                CMakeInstance, 
                CMakeBuildDirectorySignatureFileName
            );
            !Status
        )
        {
            return Status;
        }
        
        auto CMakeBuildDirectorySignatureFilePath = (
            BuildDirectoryPath 
            + ABYTEK_TEXT("/")
            + CMakeBuildDirectorySignatureFileName
        );
        if (!H_FSUtilities::Exists(CMakeBuildDirectorySignatureFilePath, E_FSEntryType::FILE))
        {
            return F_FeedbackStatus::MakeFailed(ABYTEK_TEXT("Not found build directory signature file: ") + CMakeBuildDirectorySignatureFilePath);
        }
        
        ABYTEK_FEEDBACK_STATUS_CHECK(
            H_FSUtilities::ReadFileText(CMakeBuildDirectorySignatureFilePath, OutCMakeBuildDirectorySignature)
        );
        return F_FeedbackStatus::MakeSucceeded();
    }

    F_Text H_EBTToolchainUtilities::ResolveBuildDirectoryPath(const F_Text& ProjectPath)
    {
        return ProjectPath + ABYTEK_TEXT("/Build");
    }
    F_FeedbackStatus H_EBTToolchainUtilities::ResolveCMakeBuildDirectoryPath(
        const F_Text& ProjectPath,
        const F_EBTCMakeInstance& CMakeInstance,
        F_Text& OutBuildDirectoryPath
    )
    {
        F_Text CMakeBuildDirectorySignature;
        if (
            auto Status = ResolveCMakeBuildDirectorySignatureHash(
                ProjectPath, 
                CMakeInstance, 
                CMakeBuildDirectorySignature
            ); 
            !Status
        )
        {
            return Status;
        }
        OutBuildDirectoryPath = ResolveBuildDirectoryPath(ProjectPath) + ABYTEK_TEXT("/") + CMakeBuildDirectorySignature;
        return F_FeedbackStatus::MakeSucceeded();
    }
    F_Text H_EBTToolchainUtilities::ResolveImportJSFilePath(const F_Text& CMakeBuildDirectoryPath)
    {
        return CMakeBuildDirectoryPath + ABYTEK_TEXT("/AbytekEBT/Import.js");
    }
    F_Text H_EBTToolchainUtilities::ResolveDevelopmentDataDirectoryPath(const F_Text& CMakeBuildDirectoryPath)
    {
        return CMakeBuildDirectoryPath + ABYTEK_TEXT("/Abytek/DevelopmentData");
    }

    F_FeedbackStatus H_EBTToolchainUtilities::GetDefaultPrepareMode(const F_EBTProjectInstance& ProjectInstance, E_EBTPrepareMode& OutValue)
    {
        switch (ProjectInstance.Platform)
        {
        case E_Platform::WINDOWS:
            OutValue = E_EBTPrepareMode::CMAKE_CONFIGURE; 
            break;
        case E_Platform::ANDROID_:
            OutValue = E_EBTPrepareMode::CMAKE_CONFIGURE; 
            break;
        default:
            return F_FeedbackStatus::MakeFailed(ABYTEK_TEXT("Unknown platform"));
        }
        return F_FeedbackStatus::MakeSucceeded();
    }

    F_FeedbackStatus H_EBTToolchainUtilities::Prepare(
        const F_Text& ProjectPath,
        const F_EBTProjectInstance& ProjectInstance, 
        const TF_Vector<F_Name>& ApplicationFilter,
        E_EBTPrepareMode Mode
    )
    {
        if (Mode == E_EBTPrepareMode::NONE)
        {
            if (auto Status = GetDefaultPrepareMode(ProjectInstance, Mode); !Status)
            {
                return Status;
            }
        }
        
        // Skip if host and no targets to build
        if (
            (ProjectInstance == F_EBTProjectInstance::MakeCurrent())
            && (ApplicationFilter.size() == 0)
        )
        {
            return F_FeedbackStatus::MakeSucceeded();
        }
        
        F_Text ParsedProjectPath = ProjectPath;
        boost::replace_all(ParsedProjectPath, ABYTEK_TEXT("\\"), ABYTEK_TEXT("/"));
        
        F_ProcessDesc ProcessDesc;
        ProcessDesc.CommandLine = ABYTEK_TEXT("abytekpt");
        ProcessDesc.Arguments.push_back(ParsedProjectPath);
        
        // Signature args
        {
            F_Name EnvironmentName;
            if (auto Status = GetEnvironmentName(ProjectInstance.Environment, EnvironmentName); !Status)
            {
                return Status;
            }
            ProcessDesc.Arguments.push_back(ABYTEK_TEXT("--MainScopeDefines.Private.AbytekEBT/Environment"));
            ProcessDesc.Arguments.push_back(*EnvironmentName);
            
            F_Name PlatformName;
            if (auto Status = H_CommonToolchainUtilities::GetPlatformName(ProjectInstance.Platform, PlatformName); !Status)
            {
                return Status;
            }
            ProcessDesc.Arguments.push_back(ABYTEK_TEXT("--MainScopeDefines.Private.AbytekEBT/Platform"));
            ProcessDesc.Arguments.push_back(*PlatformName);
            
            F_Name CMakeGeneratorName;
            if (auto Status = H_CMakeToolchainUtilities::GetGeneratorName(ProjectInstance.CMakeGenerator, CMakeGeneratorName); !Status)
            {
                return Status;
            }
            ProcessDesc.Arguments.push_back(ABYTEK_TEXT("--MainScopeDefines.Private.AbytekEBT/CMakeGenerator"));
            ProcessDesc.Arguments.push_back(*CMakeGeneratorName);
            
            F_Name CMakeConfigName;
            if (auto Status = H_CMakeToolchainUtilities::GetConfigName(ProjectInstance.CMakeConfig, CMakeConfigName); !Status)
            {
                return Status;
            }
            ProcessDesc.Arguments.push_back(ABYTEK_TEXT("--MainScopeDefines.Private.AbytekEBT/CMakeConfig"));
            ProcessDesc.Arguments.push_back(*CMakeConfigName);
            
            for (auto ProcessorArch : ProjectInstance.ProcessorArchs)
            {
                F_Name ProcessorArchName;
                if (auto Status = H_CommonToolchainUtilities::GetProcessorArchName(ProcessorArch, ProcessorArchName); !Status)
                {
                    return Status;
                }
                ProcessDesc.Arguments.push_back(ABYTEK_TEXT("--MainScopeDefines.Private.AbytekEBT/ProcessorArchs[]"));
                ProcessDesc.Arguments.push_back(*ProcessorArchName);
            }
        }
        
        // Prepare args
        {
            ProcessDesc.Arguments.push_back(ABYTEK_TEXT("--MainScopeDefines.Private.AbytekEBT/Prepare/Enabled"));
            ProcessDesc.Arguments.push_back(ABYTEK_TEXT("true"));
            
            F_Name PrepareModeName;
            if (auto Status = GetPrepareModeName(Mode, PrepareModeName); !Status)
            {
                return Status;
            }
            ProcessDesc.Arguments.push_back(ABYTEK_TEXT("--MainScopeDefines.Private.AbytekEBT/Prepare/Mode"));
            ProcessDesc.Arguments.push_back(*PrepareModeName);
            
            for (const auto& TargetToBuild : ApplicationFilter)
            {
                ProcessDesc.Arguments.push_back(ABYTEK_TEXT("--MainScopeDefines.Private.AbytekEBT/Prepare/ApplicationFilter[]"));
                ProcessDesc.Arguments.push_back(*TargetToBuild);
            }
        }
        
        ProcessDesc.CurrentWorkingDirectory = ABYTEK_TEXT(ABYTEK_DEVELOPMENT_DATA_SERVER_DIR);
 
        ABYTEK_LOG_INFO() 
            << ABYTEK_TEXT("Prepare project instance with command line: ") 
            << *ProcessDesc.CommandLine 
            << ABYTEK_TEXT(", current working directory: ")
            << *ProcessDesc.CurrentWorkingDirectory;
        for (const auto& Argument : ProcessDesc.Arguments)
        {
            ABYTEK_LOG_INFO() << ABYTEK_TEXT("\t") << Argument;
        }
        
        TU<A_Process> Process;
        if (auto Status = A_Process::Create(ProcessDesc, Process); !Status)
        {
            return Status;
        }
        Process->Wait();
        
        const auto& ExitCode = Process->GetExitCode();
        if (!ExitCode)
        {
            return F_FeedbackStatus::MakeFailed(ABYTEK_TEXT("Failed to build cooked executable, unknown error"));
        }
        if (*ExitCode != 0)
        {
            return F_FeedbackStatus::MakeFailed(ABYTEK_TEXT("Failed to build cooked executable, exit code: ") + ToText(*ExitCode));
        } 
        return F_FeedbackStatus::MakeSucceeded();
    }
    
    F_FeedbackStatus H_EBTToolchainUtilities::ParseEnvironment(const F_Name& Name, E_EBTEnvironment& OutEnvironment)
    {
        if (Name == ABYTEK_NAME("Development"))
        {
            OutEnvironment = E_EBTEnvironment::DEVELOPMENT;
        }
        else if (Name == ABYTEK_NAME("DevelopmentMaxOptimized"))
        {
            OutEnvironment = E_EBTEnvironment::DEVELOPMENT_MAX_OPTIMIZED;
        }
        else if (Name == ABYTEK_NAME("Cooked"))
        {
            OutEnvironment = E_EBTEnvironment::COOKED;
        }
        else if (Name == ABYTEK_NAME("CookedMaxOptimized"))
        {
            OutEnvironment = E_EBTEnvironment::COOKED_MAX_OPTIMIZED;
        }
        else
        {
            return F_FeedbackStatus::MakeFailed(ABYTEK_TEXT("Unknown generator: ") + *Name);
        }
        return F_FeedbackStatus::MakeSucceeded();
    }
    F_FeedbackStatus H_EBTToolchainUtilities::GetEnvironmentName(E_EBTEnvironment Environment, F_Name& OutName)
    {
        switch (Environment)
        {
        case E_EBTEnvironment::DEVELOPMENT:
            OutName = ABYTEK_NAME("Development");
            break;
        case E_EBTEnvironment::DEVELOPMENT_MAX_OPTIMIZED:
            OutName = ABYTEK_NAME("DevelopmentMaxOptimized");
            break;
        case E_EBTEnvironment::COOKED:
            OutName = ABYTEK_NAME("Cooked");
            break;
        case E_EBTEnvironment::COOKED_MAX_OPTIMIZED:
            OutName = ABYTEK_NAME("CookedMaxOptimized");
            break;
        default:
            return F_FeedbackStatus::MakeFailed(
                ABYTEK_TEXT("Unknown environment: ") 
                + ToText(static_cast<TF_TypeToUInt<E_EBTEnvironment>>(Environment))
            );
        }
        return F_FeedbackStatus::MakeSucceeded();
    }

    F_FeedbackStatus H_EBTToolchainUtilities::ParsePrepareMode(const F_Name& Name, E_EBTPrepareMode& OutPrepareMode)
    {
        if (Name == ABYTEK_NAME("CMakeConfigure"))
        {
            OutPrepareMode = E_EBTPrepareMode::CMAKE_CONFIGURE;
        }
        else if (Name == ABYTEK_NAME("CMakeBuild"))
        {
            OutPrepareMode = E_EBTPrepareMode::CMAKE_BUILD;
        }
        else if (Name == ABYTEK_NAME("BuildApplication"))
        {
            OutPrepareMode = E_EBTPrepareMode::BUILD_APPLICATION;
        }
        else
        {
            return F_FeedbackStatus::MakeFailed(ABYTEK_TEXT("Unknown prepare mode: ") + *Name);
        }
        return F_FeedbackStatus::MakeSucceeded();
    }
    F_FeedbackStatus H_EBTToolchainUtilities::GetPrepareModeName(E_EBTPrepareMode PrepareMode, F_Name& OutName)
    {
        switch (PrepareMode)
        {
        case E_EBTPrepareMode::CMAKE_CONFIGURE:
            OutName = ABYTEK_NAME("CMakeConfigure");
            break;
        case E_EBTPrepareMode::CMAKE_BUILD:
            OutName = ABYTEK_NAME("CMakeBuild");
            break;
        case E_EBTPrepareMode::BUILD_APPLICATION:
            OutName = ABYTEK_NAME("BuildApplication");
            break;
        default:
            return F_FeedbackStatus::MakeFailed(
                ABYTEK_TEXT("Unknown prepare mode: ") 
                + ToText(static_cast<TF_TypeToUInt<E_EBTPrepareMode>>(PrepareMode))
            );
        }
        return F_FeedbackStatus::MakeSucceeded();
    }
    
    F_FeedbackStatus H_EBTToolchainUtilities::ResolveCookDataDirectoryPath(const F_Text& ModuleBuildDirectoryPath, F_Text& OutValue)
    {
        OutValue = ModuleBuildDirectoryPath + ABYTEK_TEXT("/SAFCookData");
        return F_FeedbackStatus::MakeSucceeded();
    }

    F_Name H_EBTToolchainUtilities::ResolveCookedExecutableName(const F_Name& ApplicationTargetName)
    {
        return *ApplicationTargetName + ABYTEK_TEXT(".CookedExecutable");
    }
    F_Name H_EBTToolchainUtilities::ResolveDevelopmentExecutableName(const F_Name& ApplicationTargetName)
    {
        return *ApplicationTargetName + ABYTEK_TEXT(".DevelopmentExecutable");
    }

    F_Text H_EBTToolchainUtilities::ResolveCookedExecutableResolvedGeneratorExpressionsDirectoryPath(
        const F_Text& ModuleBuildDirectoryPath
    )
    {
        return (
            ModuleBuildDirectoryPath
            + ABYTEK_TEXT("/CookedExecutable/ResolvedGeneratorExpressions")
        );
    }
    F_Text H_EBTToolchainUtilities::ResolveDevelopmentExecutableResolvedGeneratorExpressionsDirectoryPath(
        const F_Text& ModuleBuildDirectoryPath
    )
    {
        return (
            ModuleBuildDirectoryPath
            + ABYTEK_TEXT("/DevelopmentExecutable/ResolvedGeneratorExpressions")
        );
    }
    F_FeedbackStatus H_EBTToolchainUtilities::ResolveTargetFileTextFilePath(
        const F_Text& ResolvedGeneratorExpressionsDirectoryPath, 
        F_Text& OutValue
    )
    {
        OutValue = (
            ResolvedGeneratorExpressionsDirectoryPath
            + ABYTEK_TEXT("/TargetFile.txt")
        );
        return F_FeedbackStatus::MakeSucceeded();
    }
    F_FeedbackStatus H_EBTToolchainUtilities::ResolveTargetFileDirectoryTextFilePath(
        const F_Text& ResolvedGeneratorExpressionsDirectoryPath, 
        F_Text& OutValue
    )
    {
        OutValue = (
            ResolvedGeneratorExpressionsDirectoryPath
            + ABYTEK_TEXT("/TargetFileDirectory.txt")
        );
        return F_FeedbackStatus::MakeSucceeded();
    }
    F_FeedbackStatus H_EBTToolchainUtilities::ResolveTargetFilePath(
        const F_Text& ResolvedGeneratorExpressionsDirectoryPath, 
        F_Text& OutValue
    )
    {
        F_Text Path;
        ABYTEK_FEEDBACK_STATUS_CHECK(ResolveTargetFileTextFilePath(ResolvedGeneratorExpressionsDirectoryPath, Path));
        F_Text Result;
        ABYTEK_FEEDBACK_STATUS_CHECK(H_FSUtilities::ReadFileText(Path, Result))
        boost::replace_all(Result, ABYTEK_TEXT("\n"), ABYTEK_TEXT(""));
        boost::replace_all(Result, ABYTEK_TEXT("\r"), ABYTEK_TEXT(""));
        OutValue = Result;
        return F_FeedbackStatus::MakeSucceeded();
    }
    F_FeedbackStatus H_EBTToolchainUtilities::ResolveTargetFileDirectoryPath(
        const F_Text& ResolvedGeneratorExpressionsDirectoryPath, 
        F_Text& OutValue
    )
    {
        F_Text Path;
        ABYTEK_FEEDBACK_STATUS_CHECK(ResolveTargetFileDirectoryTextFilePath(ResolvedGeneratorExpressionsDirectoryPath, Path));
        F_Text Result;
        ABYTEK_FEEDBACK_STATUS_CHECK(H_FSUtilities::ReadFileText(Path, Result))
        boost::replace_all(Result, ABYTEK_TEXT("\n"), ABYTEK_TEXT(""));
        boost::replace_all(Result, ABYTEK_TEXT("\r"), ABYTEK_TEXT(""));
        OutValue = Result;
        return F_FeedbackStatus::MakeSucceeded();
    }

    F_FeedbackStatus H_EBTToolchainUtilities::GetApplicationBuildResult(
        const F_Name& ApplicationTargetName,
        const TW_Valid<F_DevelopmentData>& DevelopmentData, 
        const F_EBTProjectInstance& ProjectInstance,
        F_EBTApplicationBuildResult& OutApplicationBuildResult
    )
    {
        F_EBTApplicationBuildResult Result;
        
        // Platforms using single cmake instance executable
        if (
            (ProjectInstance.Platform == E_Platform::WINDOWS)
        )
        {
            F_Text CookedExecutableTargetFilePath;
            F_Text CookedExecutableTargetFileDirectoryPath;
            
            if (!DevelopmentData->HasModule(ApplicationTargetName))
            {
                return F_FeedbackStatus::MakeFailed(
                    ABYTEK_TEXT("Not found module: ")
                    + *ApplicationTargetName
                );
            }
            
            const auto& ModuleDevelopmentData = DevelopmentData->GetModuleDevelopmentData(ApplicationTargetName);
            {
                F_Text CookedExecutableResolvedGeneratorExpressionsDirectoryPath = ResolveCookedExecutableResolvedGeneratorExpressionsDirectoryPath(
                    ModuleDevelopmentData.BuildDirectory
                );
                {
                    if (
                        auto Status = ResolveTargetFilePath(
                            CookedExecutableResolvedGeneratorExpressionsDirectoryPath,
                            CookedExecutableTargetFilePath
                        );
                        !Status
                    )
                    {
                        return Status;
                    }
                }
                {
                    if (
                        auto Status = ResolveTargetFileDirectoryPath(
                            CookedExecutableResolvedGeneratorExpressionsDirectoryPath,
                            CookedExecutableTargetFileDirectoryPath
                        );
                        !Status
                    )
                    {
                        return Status;
                    }
                }
            }
            
            Result.DirectoryPath = CookedExecutableTargetFileDirectoryPath;
            Result.ExecutableFilePath = CookedExecutableTargetFilePath;
        }
        // Platforms using single bundled file
        else if (
            (ProjectInstance.Platform == E_Platform::ANDROID_)    
        )
        {
        }
        else
        {
            return F_FeedbackStatus::MakeFailed(ABYTEK_TEXT("Unknown platform: ") + ToText(static_cast<U32>(ProjectInstance.Platform)));
        }
        
        OutApplicationBuildResult = ABYTEK_MOVE(Result);
        return F_FeedbackStatus::MakeSucceeded();
    }
}
#endif