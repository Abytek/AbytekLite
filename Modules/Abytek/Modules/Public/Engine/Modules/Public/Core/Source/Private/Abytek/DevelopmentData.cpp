#include "Abytek/DevelopmentData.hpp"
#include "Abytek/TaskUtilities.hpp"


#ifdef ABYTEK_ENABLE_DEVELOPMENT_DATA
namespace Abytek
{
    namespace Internal::DevelopmentDataServerProgramV2
    {
        std::string GenerateTimestamp()
        {
            using namespace std::chrono;

            auto now = system_clock::now();
            auto s = time_point_cast<std::chrono::seconds>(now);
            auto ms = duration_cast<milliseconds>(now - s).count();

            std::time_t tt = system_clock::to_time_t(s);
            std::tm tm{};
    
#if defined(_WIN32)
            localtime_s(&tm, &tt);
#else
            localtime_r(&tt, &tm);
#endif

            std::ostringstream oss;
            oss << std::put_time(&tm, "%Y%m%d_%H%M%S")
                << "_" << std::setw(3) << std::setfill('0') << ms;

            return oss.str();
        }
    }
    
    F_DevelopmentDataServerProgram::F_DevelopmentDataServerProgram()
    {
    }
    F_DevelopmentDataServerProgram::~F_DevelopmentDataServerProgram()
    {
        if (
            auto Status = ExecuteJS(
                ABYTEK_TEXT("Ack(200);")
                ABYTEK_TEXT("DevelopmentDataServer.socketIOServer.close(() => {")
                ABYTEK_TEXT("DevelopmentDataServer.httpServer.close(() => {")
                ABYTEK_TEXT("console.log('Server stopped');")
                ABYTEK_TEXT("process.exit(0);")
                ABYTEK_TEXT("});")
                ABYTEK_TEXT("});")
            ); 
            !Status
        )
        {
            ABYTEK_LOG_FATAL() << Status.ErrorMessage;
        }
    }

    F_FeedbackStatus F_DevelopmentDataServerProgram::Create(const TW_Valid<F_DevelopmentDataInstance>& DevelopmentData, TU<F_DevelopmentDataServerProgram>& OutProgram)
    {
        U32 ServerPort = 0;
        
        // Install program
        {
            F_ProcessDesc ProcessDesc;
            ProcessDesc.CommandLine = ABYTEK_TEXT(ABYTEK_NPM_EXECUTABLE);
            ProcessDesc.Arguments.push_back(ABYTEK_TEXT("install"));
            ProcessDesc.CurrentWorkingDirectory = ABYTEK_TEXT(ABYTEK_DEVELOPMENT_DATA_SERVER_DIR);
 
            TU<A_Process> InstallProcess;
            if (auto Status = A_Process::Create(ProcessDesc, InstallProcess); !Status)
            {
                return Status;
            }
        }
        
        OutProgram = TU<F_DevelopmentDataServerProgram>()();
        
        TU<A_Process> NodeJSProcess;
        
        // Start program
        {
            auto Timestamp = ToText(Internal::DevelopmentDataServerProgramV2::GenerateTimestamp().c_str());
            F_Text Signature = (
                ToText((Sz)OutProgram.GetObjectRawP()) 
                + ABYTEK_TEXT("_") 
                + ToText(H_ThisProcess::GetId()) 
                + ABYTEK_TEXT("_") 
                + Timestamp
            );
            
            F_ProcessDesc ProcessDesc;
            ProcessDesc.CommandLine = ABYTEK_TEXT(ABYTEK_NODEJS_EXECUTABLE);
            ProcessDesc.Arguments.push_back(ABYTEK_TEXT(ABYTEK_DEVELOPMENT_DATA_SERVER_JS_FILE));
            ProcessDesc.Arguments.push_back(DevelopmentData->GetImportJSFilePath());
            ProcessDesc.Arguments.push_back(Signature);

            if (auto Status = A_Process::Create(ProcessDesc, NodeJSProcess); !Status)
            {
                return Status;
            }
            
            F_Text PortFile = (
                F_Text()
                + ABYTEK_TEXT(ABYTEK_DEVELOPMENT_DATA_SERVER_DIR)
                + ABYTEK_TEXT("/Temp/node_")
                + Signature
                + ABYTEK_TEXT(".port")
            );
            F_Text PortDoneFile = (
                F_Text()
                + ABYTEK_TEXT(ABYTEK_DEVELOPMENT_DATA_SERVER_DIR)
                + ABYTEK_TEXT("/Temp/node_")
                + Signature
                + ABYTEK_TEXT(".port_done")
            );
            
            while (!H_FSUtilities::Exists(PortDoneFile, E_FSEntryType::FILE))
            {
                H_TaskUtilities::Switch();
            }
            
            F_String PortStr;
            ABYTEK_FEEDBACK_STATUS_CHECK(
                H_FSUtilities::ReadFileString(PortFile, PortStr)
            );
            
            auto [ptr, ec] = std::from_chars(PortStr.data(), PortStr.data() + PortStr.size(), ServerPort);
            if (ec != std::errc())
            {
                return F_FeedbackStatus::MakeFailed(ABYTEK_TEXT("Failed read feedback port"));
            }
            
            if (auto Status = A_FSAdapter_Standard::GetInstance()->DeleteFile_(PortFile); !Status)
            {
                return F_FeedbackStatus::MakeFailed(ABYTEK_TEXT("Failed to delete port file"));
            }
            if (auto Status = A_FSAdapter_Standard::GetInstance()->DeleteFile_(PortDoneFile); !Status)
            {
                return F_FeedbackStatus::MakeFailed(ABYTEK_TEXT("Failed to delete port done file"));
            }
        }
        
        ABYTEK_LOG_INFO() << ABYTEK_TEXT("Started development data server at localhost:") << ServerPort;
        
        ABYTEK_LOG_INFO() << ABYTEK_TEXT("Connecting to development data server at localhost:") << ServerPort;

        OutProgram->_NodeJSProcess = ABYTEK_MOVE(NodeJSProcess);
        
        OutProgram->_SocketClient.connect(("http://127.0.0.1:" + ToString(ServerPort)).c_str());
        while(!OutProgram->_SocketClient.opened())
        {
            H_TaskUtilities::Switch();
        }
        ABYTEK_LOG_INFO() << ABYTEK_TEXT("Connected to development data server at localhost:") << ServerPort;
        
        return F_FeedbackStatus::MakeSucceeded();
    }

    std::shared_ptr<sio::message> F_DevelopmentDataServerProgram::AccessModule(const F_Name& ModuleName, const F_Text& AccessCode)
    {
        B8 Success = false;
        F_String ErrorMsg;
        std::shared_ptr<sio::message> Result;
        
        F_AtomicFlag Done;
        sio::message::list MsgParams;
        MsgParams.push(ToString(*ModuleName).c_str());
        MsgParams.push(ToString(AccessCode).c_str());
        _SocketClient.socket()->emit(
            "access_module",
            MsgParams,
            [&](sio::message::list const& msg)
            {
                if (msg[0]->get_int() == 200)
                {
                    // Result = ToText(msg[1]->get_string().c_str());
                    Result = msg[1];
                    Success = true;
                }
                else
                {
                    ErrorMsg = msg[1]->get_string().c_str();
                }
                boost::atomic_thread_fence(boost::memory_order_release);
                Done.test_and_set(boost::memory_order_relaxed);
            }
        );
        while (!Done.test(boost::memory_order_acquire))
        {
            H_TaskUtilities::Switch();
        }
        return Result;
    }
    std::shared_ptr<sio::message> F_DevelopmentDataServerProgram::AccessModuleProperty(const F_Name& ModuleName, const F_Name& ModulePropertyName)
    {
        return AccessModule(ModuleName, ABYTEK_TEXT("return Module[\"") + *ModulePropertyName + ABYTEK_TEXT("\"]"));
    }

    F_FeedbackStatus F_DevelopmentDataServerProgram::ExecuteJS(const F_Text& JSCode, F_DevelopmentDataExecuteJSCallback&& Callback)
    {
        F_FeedbackStatus Status = F_FeedbackStatus::MakeFailed(ABYTEK_TEXT("unknown error"));
        F_AtomicFlag Done;
        sio::message::list MsgParams;
        MsgParams.push(ToString(JSCode).c_str());
        _SocketClient.socket()->emit(
            "execute_js",
            MsgParams,
            [&, CachedCallback = ABYTEK_FORWARD(Callback)](sio::message::list const& msg)
            {
                if (CachedCallback)
                {
                    Status = CachedCallback(msg);
                }
                else
                {
                    Status = F_FeedbackStatus::MakeSucceeded();
                }
                boost::atomic_thread_fence(boost::memory_order_release);
                Done.test_and_set(boost::memory_order_relaxed);
            }
        );
        while (!Done.test(boost::memory_order_acquire))
        {
            H_TaskUtilities::Switch();
        }
        return ABYTEK_MOVE(Status);
    }

    TF_Vector<F_Name> F_DevelopmentDataServerProgram::GetModuleNames()
    {
        B8 Success = false;
        F_String ErrorMsg;
        TF_Vector<F_Name> Result;
        
        F_AtomicFlag Done;
        _SocketClient.socket()->emit(
            "module_names",
            sio::message::list {},
            [&](sio::message::list const& msg)
            {
                if (msg[0]->get_int() == 200)
                {
                    for (I32 Idx = 1; Idx < msg.size(); Idx++)
                    {
                        Result.push_back(
                            F_Name(ToText(msg[Idx]->get_string().c_str()))
                        );
                    }
                    Success = true;
                }
                else
                {
                    ErrorMsg = msg[1]->get_string().c_str();
                }
                boost::atomic_thread_fence(boost::memory_order_release);
                Done.test_and_set(boost::memory_order_relaxed);
            }
        );
        while (!Done.test(boost::memory_order_acquire))
        {
            H_TaskUtilities::Switch();
        }
        ABYTEK_ENGINE_CORE_ASSERT(Success) << "Failed to connect : " << ErrorMsg;
        return Result;
    }
    
    ABYTEK_REFLECT(F_DevelopmentDataInstance)
    {
    }
    F_DevelopmentDataInstance::F_DevelopmentDataInstance(const F_DevelopmentDataInstanceBuildParams& BuildParams) :
        _Owner(BuildParams.Owner),
        _CMakeInstance(BuildParams.CMakeInstance)
    {
    }
    F_DevelopmentDataInstance::~F_DevelopmentDataInstance()
    {
    }

    F_FeedbackStatus F_DevelopmentDataInstance::Create(const F_DevelopmentDataInstanceBuildParams& BuildParams, TS<F_DevelopmentDataInstance>& OutDevelopmentData)
    {
        auto Result = TS<F_DevelopmentDataInstance>()(BuildParams);
        
        const auto& ProjectPath = BuildParams.Owner->GetProjectPath();
        const auto& BuildDirectoryPath = BuildParams.Owner->GetBuildDirectoryPath();
        
        if (
            auto Status = H_EBTToolchainUtilities::ResolveCMakeBuildDirectoryPath(
                ProjectPath,
                Result->_CMakeInstance,
                Result->_CMakeBuildDirectoryPath
            ); 
            !Status
        )
        {
            return Status;
        }
        Result->_ImportJSFilePath = H_EBTToolchainUtilities::ResolveImportJSFilePath(Result->_CMakeBuildDirectoryPath);

        if (auto Status = Result->StartDevelopmentDataServer(); !Status)
        {
            return Status;
        }
        
        OutDevelopmentData = Result;
        return F_FeedbackStatus::MakeSucceeded();
    }

    F_FeedbackStatus F_DevelopmentDataInstance::StartDevelopmentDataServer()
    {
        if (auto Status = F_DevelopmentDataServerProgram::Create(ABYTEK_WTHIS(), _ServerProgram); !Status)
        {
            return Status;
        }

        _ModuleNames = _ServerProgram->GetModuleNames();
        for (const auto& ModuleName : _ModuleNames)
        {
            _ModuleDevelopmentDataMap[ModuleName] = _LoadModuleDevelopmentData(ModuleName);
        }
        _GatherAccessibleModuleNames();
        return F_FeedbackStatus::MakeSucceeded();
    }

    std::shared_ptr<sio::message> F_DevelopmentDataInstance::_AccessModule(const F_Name& ModuleName, const F_Text& AccessCode)
    {
        return _ServerProgram->AccessModule(ModuleName, AccessCode);
    }
    TF_Optional<F_String> F_DevelopmentDataInstance::_AccessModule_String(const F_Name& ModuleName, const F_Text& AccessCode)
    {
        if (auto Msg = _AccessModule(ModuleName, AccessCode))
        {
            return ToString(Msg->get_string().c_str());
        }
        return {};
    }
    TF_Optional<F_Text> F_DevelopmentDataInstance::_AccessModule_Text(const F_Name& ModuleName, const F_Text& AccessCode)
    {
        if (auto Msg = _AccessModule(ModuleName, AccessCode))
        {
            return ToText(Msg->get_string().c_str());
        }
        return {};
    }
    TF_Optional<I64> F_DevelopmentDataInstance::_AccessModule_I64(const F_Name& ModuleName, const F_Text& AccessCode)
    {
        if (auto Msg = _AccessModule(ModuleName, AccessCode))
        {
            return Msg->get_int();
        }
        return {};
    }
    TF_Optional<F64> F_DevelopmentDataInstance::_AccessModule_F64(const F_Name& ModuleName, const F_Text& AccessCode)
    {
        if (auto Msg = _AccessModule(ModuleName, AccessCode))
        {
            return Msg->get_double();
        }
        return {};
    }
    TF_Optional<B8> F_DevelopmentDataInstance::_AccessModule_Bool(const F_Name& ModuleName, const F_Text& AccessCode)
    {
        if (auto Msg = _AccessModule(ModuleName, AccessCode))
        {
            return Msg->get_bool();
        }
        return {};
    }
    std::shared_ptr<sio::message> F_DevelopmentDataInstance::_AccessModuleProperty(const F_Name& ModuleName, const F_Name& ModulePropertyName)
    {
        return _ServerProgram->AccessModuleProperty(ModuleName, ModulePropertyName);
    }

    F_FeedbackStatus F_DevelopmentDataInstance::ExecuteJS(const F_Text& JSCode, F_DevelopmentDataExecuteJSCallback&& Callback)
    {
        return _ServerProgram->ExecuteJS(JSCode, ABYTEK_FORWARD(Callback));
    }

    const F_ModuleDevelopmentData& F_DevelopmentDataInstance::GetModuleDevelopmentData(const F_Name& ModuleName)
    {
        auto It = _ModuleDevelopmentDataMap.find(ModuleName);
        ABYTEK_ENGINE_CORE_ASSERT(It != _ModuleDevelopmentDataMap.end()) << "Not found module " << *ModuleName;
        return It->second;
    }
    B8 F_DevelopmentDataInstance::HasModule(const F_Name& ModuleName)
    {
        return _ModuleDevelopmentDataMap.find(ModuleName) != _ModuleDevelopmentDataMap.end();
    }
    const TF_Vector<F_Name>& F_DevelopmentDataInstance::GetAccessibleModuleNames(const F_Name& ModuleName)
    {
        auto It = _ModuleNameToAccessibleModuleNames.find(ModuleName);
        ABYTEK_ENGINE_CORE_ASSERT(It != _ModuleNameToAccessibleModuleNames.end()) << "Not found module " << *ModuleName;
        return It->second;
    }

    F_FeedbackStatus F_DevelopmentDataInstance::ResolveModuleAutoProperty(
        const F_Name& ModuleName,
        const F_Name& PropertyName, 
        F_Text& OutPropertyValue
    )
    {
        if (!HasModule(ModuleName))
        {
            return F_FeedbackStatus::MakeFailed(ABYTEK_TEXT("Not found module with name: ") + *ModuleName);
        }
        
        if (_CMakeInstance.CMakeConfig == E_CMakeConfig::AUTO)
        {
            return F_FeedbackStatus::MakeFailed(ABYTEK_TEXT("E_CMakeConfig::AUTO is not allowed for development data"));
        }
        F_Name CMakeConfigName;
        if (auto Status = H_CMakeToolchainUtilities::GetConfigName(_CMakeInstance.CMakeConfig, CMakeConfigName); !Status)
        {
            return Status;
        }
        
        F_Text PropertyText = *PropertyName;
        boost::replace_all(PropertyText, ABYTEK_TEXT("\\"), ABYTEK_TEXT("/"));
        
        F_Text JSCode = (
            F_Text()
                
            + ABYTEK_TEXT("const ModuleName = ") 
            + ABYTEK_TEXT("\"")
            + *ModuleName
            + ABYTEK_TEXT("\";\n")
                
            + ABYTEK_TEXT("const PropertyName = ") 
            + ABYTEK_TEXT("\"")
            + PropertyText
            + ABYTEK_TEXT("\";\n")
                
            + ABYTEK_TEXT("const CMakeConfigName = ") 
            + ABYTEK_TEXT("\"")
            + *CMakeConfigName
            + ABYTEK_TEXT("\";\n")
                
            + ABYTEK_TEXT("const Value = ") 
            + ABYTEK_TEXT("DevelopmentDataServer.Modules[ModuleName]")
            + ABYTEK_TEXT(".GetResolvedGeneratorExpression(PropertyName, CMakeConfigName);\n")
                
            + ABYTEK_TEXT("if (Value != null)\n")
            + ABYTEK_TEXT("{\n")
            + ABYTEK_TEXT("Ack(200, Value);\n")
            + ABYTEK_TEXT("}\n")
            + ABYTEK_TEXT("else\n")
            + ABYTEK_TEXT("{\n")
            + ABYTEK_TEXT("Ack(500, `Not found property, module: ${ModuleName}, property: ${PropertyName}, direct config: ${CMakeConfigName}`);\n")
            + ABYTEK_TEXT("}")
        );
        
        return ExecuteJS(
            JSCode,
            [&](sio::message::list const& Msg) -> F_FeedbackStatus
            {
                if (Msg[0]->get_int() != 200)
                {
                    return F_FeedbackStatus::MakeFailed(ToText(Msg[1]->get_string().c_str()));
                }
                OutPropertyValue = ToText(Msg[1]->get_string().c_str());
                return F_FeedbackStatus::MakeSucceeded();
            }
        );
    }
    F_FeedbackStatus F_DevelopmentDataInstance::ResolveModuleRuntimeOutputDirectoryPath(
        const F_Name& ModuleName,
        F_Text& OutValue
    )
    {
        return ResolveModuleAutoProperty(ModuleName, ABYTEK_TEXT("RuntimeOutputDirectory"), OutValue);
    }
    F_FeedbackStatus F_DevelopmentDataInstance::ResolveModuleLibraryOutputDirectoryPath(
        const F_Name& ModuleName,
        F_Text& OutValue
    )
    {
        return ResolveModuleAutoProperty(ModuleName, ABYTEK_TEXT("LibraryOutputDirectory"), OutValue);
    }
    F_FeedbackStatus F_DevelopmentDataInstance::ResolveModuleArchiveOutputDirectoryPath(
        const F_Name& ModuleName,
        F_Text& OutValue
    ) 
    {
        return ResolveModuleAutoProperty(ModuleName, ABYTEK_TEXT("ArchiveOutputDirectory"), OutValue);
    }
    F_FeedbackStatus F_DevelopmentDataInstance::ResolveModuleTargetFileDirectoryPath(
        const F_Name& ModuleName,
        F_Text& OutValue
    )
    {
        return ResolveModuleAutoProperty(ModuleName, ABYTEK_TEXT("TargetFileDirectory"), OutValue);
    }
    F_FeedbackStatus F_DevelopmentDataInstance::ResolveModuleTargetFilePath(
        const F_Name& ModuleName,
        F_Text& OutValue  
    )
    {
        return ResolveModuleAutoProperty(ModuleName, ABYTEK_TEXT("TargetFile"), OutValue);
    }

    F_ModuleDevelopmentData F_DevelopmentDataInstance::_LoadModuleDevelopmentData(const F_Name& ModuleName)
    {
        F_ModuleDevelopmentData Result;
        
#define ABYTEK_INTERNAL_PROGRAM_MODULE_DEV_DATA_PROP(CPPPropertyName, JSPropertyName, Type) \
            { \
                if ( \
                    auto Value = _AccessModule_##Type( \
                        ModuleName, \
                        ToText(ABYTEK_TEXT("return Module.")) + ABYTEK_TEXT(#JSPropertyName) + ABYTEK_TEXT(";") \
                    ) \
                ) \
                { \
                    Result.CPPPropertyName = *Value; \
                } \
            }

        ABYTEK_INTERNAL_PROGRAM_MODULE_DEV_DATA_PROP(EnableUnityBuild, EnableUnityBuild, Bool);
        ABYTEK_INTERNAL_PROGRAM_MODULE_DEV_DATA_PROP(MacroName, MacroName, Text);
        ABYTEK_INTERNAL_PROGRAM_MODULE_DEV_DATA_PROP(Type, Type, Text);
        ABYTEK_INTERNAL_PROGRAM_MODULE_DEV_DATA_PROP(Directory, Directory, Text);
        ABYTEK_INTERNAL_PROGRAM_MODULE_DEV_DATA_PROP(BuildDirectory, BuildDirectory, Text);
        ABYTEK_INTERNAL_PROGRAM_MODULE_DEV_DATA_PROP(ModulesInterfaceDirectory, Modules.InterfaceDirectory, Text);
        ABYTEK_INTERNAL_PROGRAM_MODULE_DEV_DATA_PROP(ModulesPrivateDirectory, Modules.PrivateDirectory, Text);
        ABYTEK_INTERNAL_PROGRAM_MODULE_DEV_DATA_PROP(ModulesPublicDirectory, Modules.PublicDirectory, Text);
        ABYTEK_INTERNAL_PROGRAM_MODULE_DEV_DATA_PROP(SourceDirectory, Source.Directory, Text);
        ABYTEK_INTERNAL_PROGRAM_MODULE_DEV_DATA_PROP(SourceInterfaceDirectory, Source.InterfaceDirectory, Text);
        ABYTEK_INTERNAL_PROGRAM_MODULE_DEV_DATA_PROP(SourcePrivateDirectory, Source.PrivateDirectory, Text);
        ABYTEK_INTERNAL_PROGRAM_MODULE_DEV_DATA_PROP(SourcePublicDirectory, Source.PublicDirectory, Text);
        ABYTEK_INTERNAL_PROGRAM_MODULE_DEV_DATA_PROP(AutoCopyDirectory, AutoCopy.Directory, Text);
        ABYTEK_INTERNAL_PROGRAM_MODULE_DEV_DATA_PROP(ThirdPartyDirectory, ThirdParty.Directory, Text);
        ABYTEK_INTERNAL_PROGRAM_MODULE_DEV_DATA_PROP(GeneratedSourceDirectory, GeneratedSource.Directory, Text);
        ABYTEK_INTERNAL_PROGRAM_MODULE_DEV_DATA_PROP(GeneratedSourceTempDirectory, GeneratedSource.TempDirectory, Text);
        ABYTEK_INTERNAL_PROGRAM_MODULE_DEV_DATA_PROP(GeneratedSourceInterfaceDirectory, GeneratedSource.InterfaceDirectory, Text);
        ABYTEK_INTERNAL_PROGRAM_MODULE_DEV_DATA_PROP(GeneratedSourcePrivateDirectory, GeneratedSource.PrivateDirectory, Text);
        ABYTEK_INTERNAL_PROGRAM_MODULE_DEV_DATA_PROP(GeneratedSourcePublicDirectory, GeneratedSource.PublicDirectory, Text);
        ABYTEK_INTERNAL_PROGRAM_MODULE_DEV_DATA_PROP(CMakeDirectory, CMakeDirectory, Text);
        ABYTEK_INTERNAL_PROGRAM_MODULE_DEV_DATA_PROP(JSDirectory, JSDirectory, Text);
        ABYTEK_INTERNAL_PROGRAM_MODULE_DEV_DATA_PROP(GeneratedJSDirectory, GeneratedJSDirectory, Text);
        ABYTEK_INTERNAL_PROGRAM_MODULE_DEV_DATA_PROP(JSModuleFile, JSModuleFile, Text);
        ABYTEK_INTERNAL_PROGRAM_MODULE_DEV_DATA_PROP(JSModulesFile, JSModulesFile, Text);
        ABYTEK_INTERNAL_PROGRAM_MODULE_DEV_DATA_PROP(JSPrivateModulesFile, JSPrivateModulesFile, Text);
        ABYTEK_INTERNAL_PROGRAM_MODULE_DEV_DATA_PROP(JSPublicModulesFile, JSPublicModulesFile, Text);
        ABYTEK_INTERNAL_PROGRAM_MODULE_DEV_DATA_PROP(JSDependenciesFile, JSDependenciesFile, Text);
        ABYTEK_INTERNAL_PROGRAM_MODULE_DEV_DATA_PROP(JSPrivateDependenciesFile, JSPrivateDependenciesFile, Text);
        ABYTEK_INTERNAL_PROGRAM_MODULE_DEV_DATA_PROP(JSPublicDependenciesFile, JSPublicDependenciesFile, Text);
        
#undef ABYTEK_INTERNAL_PROGRAM_MODULE_DEV_DATA_PROP
        
        {
            if (
                auto Msg = _AccessModule(
                    ModuleName,
                    ABYTEK_TEXT("return [...Module.Modules.Public, ...Module.Modules.Private];")
                )
            )
            {
                for (auto& Item : Msg->get_vector())
                {
                    F_Name DependencyName = ToText(Item->get_string().c_str());
                    Result.ModuleNames.insert(DependencyName);
                }
            }
        }
        {
            if (
                auto Msg = _AccessModule(
                    ModuleName,
                    ABYTEK_TEXT("return [...Module.Modules.Private];")
                )
            )
            {
                for (auto& Item : Msg->get_vector())
                {
                    F_Name DependencyName = ToText(Item->get_string().c_str());
                    Result.PrivateModuleNames.insert(DependencyName);
                }
            }
        }
        {
            if (
                auto Msg = _AccessModule(
                    ModuleName,
                    ABYTEK_TEXT("return [...Module.Modules.Public];")
                )
            )
            {
                for (auto& Item : Msg->get_vector())
                {
                    F_Name DependencyName = ToText(Item->get_string().c_str());
                    Result.PublicModuleNames.insert(DependencyName);
                }
            }
        }
        {
            if (
                auto Msg = _AccessModule(
                    ModuleName,
                    ABYTEK_TEXT("return [...Module.Dependencies.Public, ...Module.Dependencies.Private];")
                )
            )
            {
                for (auto& Item : Msg->get_vector())
                {
                    F_Name DependencyName = ToText(Item->get_string().c_str());
                    Result.DependencyNames.insert(DependencyName);
                }
            }
        }
        {
            if (
                auto Msg = _AccessModule(
                    ModuleName,
                    ABYTEK_TEXT("return [...Module.Dependencies.Private];")
                )
            )
            {
                for (auto& Item : Msg->get_vector())
                {
                    F_Name DependencyName = ToText(Item->get_string().c_str());
                    Result.PrivateDependencyNames.insert(DependencyName);
                }
            }
        }
        {
            if (
                auto Msg = _AccessModule(
                    ModuleName,
                    ABYTEK_TEXT("return [...Module.Dependencies.Public];")
                )
            )
            {
                for (auto& Item : Msg->get_vector())
                {
                    F_Name DependencyName = ToText(Item->get_string().c_str());
                    Result.PublicDependencyNames.insert(DependencyName);
                }
            }
        }
        return ABYTEK_MOVE(Result);
    }

    void F_DevelopmentDataInstance::_GatherAccessibleModuleNames()
    {
        const auto& ModuleNames = GetModuleNames();
        for (const auto& ModuleName : ModuleNames)
        {
            TF_Vector<F_Name> AccessibleModuleNames;
            for (const auto& AccessibleModuleName : _GatherAccessibleModuleNames(ModuleName))
            {
                AccessibleModuleNames.push_back(AccessibleModuleName);
            }
            ABYTEK_LOG_INFO() << "Accessible module names " << ModuleName << ": " << AccessibleModuleNames.size();
            _ModuleNameToAccessibleModuleNames[ModuleName] = ABYTEK_MOVE(AccessibleModuleNames);
        }
    }
    TF_Set<F_Name> F_DevelopmentDataInstance::_GatherAccessibleModuleNames(const F_Name& ModuleName, B8 GatherPrivate, B8 GatherPublic)
    {
        if (!HasModule(ModuleName))
        {
            return {};
        }
        TF_Set<F_Name> Result;
        Result.insert(ModuleName);
        const auto& ModuleDevelopmentData = GetModuleDevelopmentData(ModuleName);
        if (GatherPublic)
        {
            TF_Set<F_Name> ModuleNames;
            if (
                _ModuleNameToAccessibleModuleNames_Public.find(ModuleName)
                == _ModuleNameToAccessibleModuleNames_Public.end()
            )
            {
                for (const auto& DependencyName : ModuleDevelopmentData.PublicDependencyNames)
                {
                    TF_Set<F_Name> DependencyNames = _GatherAccessibleModuleNames(
                        DependencyName, 
                        false,
                        true
                    );
                    for (const auto& Element : DependencyNames)
                    {
                        if (ModuleNames.find(Element) == ModuleNames.end())
                        {
                            ModuleNames.insert(Element);
                        }
                    }
                }
                _ModuleNameToAccessibleModuleNames_Public[ModuleName] = ModuleNames;
            }
            else
            {
                ModuleNames = _ModuleNameToAccessibleModuleNames_Public.find(ModuleName)->second;
            }
            for (const auto& Element : ModuleNames)
            {
                if (Result.find(Element) == Result.end())
                {
                    Result.insert(Element);
                }
            }
        }
        if (GatherPrivate)
        {
            TF_Set<F_Name> ModuleNames;
            if (
                _ModuleNameToAccessibleModuleNames_Public.find(ModuleName)
                == _ModuleNameToAccessibleModuleNames_Public.end()
            )
            {
                for (const auto& DependencyName : ModuleDevelopmentData.PrivateModuleNames)
                {
                    TF_Set<F_Name> DependencyNames = _GatherAccessibleModuleNames(
                        DependencyName, 
                        false,
                        true
                    );
                    for (const auto& Element : DependencyNames)
                    {
                        if (ModuleNames.find(Element) == ModuleNames.end())
                        {
                            ModuleNames.insert(Element);
                        }
                    }
                }
                _ModuleNameToAccessibleModuleNames_Private[ModuleName] = ModuleNames;
            }
            else
            {
                ModuleNames = _ModuleNameToAccessibleModuleNames_Private.find(ModuleName)->second;
            }
            for (const auto& Element : ModuleNames)
            {
                if (Result.find(Element) == Result.end())
                {
                    Result.insert(Element);
                }
            }
        }
        return ABYTEK_MOVE(Result);
    }
    
    ABYTEK_REFLECT(F_DevelopmentData)
    {
    }
    F_DevelopmentData::F_DevelopmentData(const F_DevelopmentDataBuildParams& BuildParams) :
        _ProjectPath(BuildParams.ProjectPath),
        _ProjectInstance(BuildParams.ProjectInstance),
        _ApplicationFilter(BuildParams.ApplicationFilter)
    {
    }
    F_DevelopmentData::~F_DevelopmentData()
    {
    }

    F_FeedbackStatus F_DevelopmentData::Create(const F_DevelopmentDataBuildParams& BuildParams, TS<F_DevelopmentData>& OutDevelopmentData)
    {
        auto Result = TS<F_DevelopmentData>()(BuildParams);
        
        if (
            auto Status = H_EBTToolchainUtilities::Prepare(
                Result->_ProjectPath,
                Result->_ProjectInstance,
                Result->_ApplicationFilter
            );
            !Status
        )
        {
            return Status;
        }
        
        Result->_BuildDirectoryPath = H_EBTToolchainUtilities::ResolveBuildDirectoryPath(BuildParams.ProjectPath);
        for (const auto& CMakeInstance : BuildParams.ProjectInstance.CMakeInstances)
        {
            TS<F_DevelopmentDataInstance> Instance;
            
            F_DevelopmentDataInstanceBuildParams InstanceBuildParams;
            InstanceBuildParams.Owner = Result.Weak();
            InstanceBuildParams.CMakeInstance = CMakeInstance;
            if (
                auto Status = F_DevelopmentDataInstance::Create(InstanceBuildParams, Instance);
                !Status
            )
            {
                return Status;
            }
            
            Result->_Instances.push_back(Instance);
        }
        
        OutDevelopmentData = Result;
        return F_FeedbackStatus::MakeSucceeded();
    }

    TS<F_DevelopmentDataInstance> F_DevelopmentData::GetMainInstance() const
    {
        return _Instances.front();
    }

    TF_Vector<Sz> F_DevelopmentData::FindInstanceIndices(const F_EBTCMakeInstanceQuery& CMakeInstanceQuery)
    {
        TF_Vector<Sz> Result;
        Sz NumInstances = _Instances.size();
        for (Sz Idx = 0; Idx < NumInstances; ++Idx)
        {
            const auto& Instance = _Instances[Idx];
            if (Instance->GetCMakeInstance().IsMatchQuery(CMakeInstanceQuery))
            {
                Result.push_back(Idx);
            }
        }
        return ABYTEK_MOVE(Result);
    }
    TF_Vector<TS<F_DevelopmentDataInstance>> F_DevelopmentData::FindInstances(const F_EBTCMakeInstanceQuery& CMakeInstanceQuery)
    {
        auto Indices = FindInstanceIndices(CMakeInstanceQuery);
        TF_Vector<TS<F_DevelopmentDataInstance>> Result;
        for (Sz Idx : Indices)
        {
            Result.push_back(_Instances[Idx]);
        }
        return ABYTEK_MOVE(Result);
    }
    TS<F_DevelopmentDataInstance> F_DevelopmentData::FindInstance(const F_EBTCMakeInstanceQuery& CMakeInstanceQuery)
    {
        for (const auto& Instance : _Instances)
        {
            if (Instance->GetCMakeInstance().IsMatchQuery(CMakeInstanceQuery))
            {
                return Instance;
            }
        }
        return {};
    }

    const F_ModuleDevelopmentData& F_DevelopmentData::GetModuleDevelopmentData(const F_Name& ModuleName)
    {
        return GetMainInstance()->GetModuleDevelopmentData(ModuleName);
    }
    B8 F_DevelopmentData::HasModule(const F_Name& ModuleName)
    {
        return GetMainInstance()->HasModule(ModuleName);
    }
    const TF_Vector<F_Name>& F_DevelopmentData::GetAccessibleModuleNames(const F_Name& ModuleName)
    {
        return GetMainInstance()->GetAccessibleModuleNames(ModuleName);
    }
}
#endif
