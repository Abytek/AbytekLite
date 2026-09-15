#include "Abytek/Console.hpp"
#include "Abytek/Executable.hpp"


namespace Abytek
{
    A_ConsoleVariable::A_ConsoleVariable(const TW_Valid<F_Console>& Console, const F_Name& Name, const F_Text& Description) :
        _Console(Console),
        _Name(Name),
        _Description(Description)
    {
    }
    A_ConsoleVariable::~A_ConsoleVariable()
    {
    }

    void A_ConsoleVariable::ParseConfig(const TF_Vector<toml::parse_result>& ParsedResults)
    {
        TF_Vector<F_Text> Elements;
        boost::split(Elements, *_Name, boost::is_any_of(ABYTEK_TEXT(".")));
        for (const auto& RootConfigParsedResult : ParsedResults)
        {
            const toml::node* Node = &RootConfigParsedResult;
            for (U32 Idx = 0; Idx < Elements.size(); ++Idx)
            {
                if (!(Node->is_table()))
                {
                    Node = nullptr;
                    break;
                }
                
                auto Table = Node->as_table();
                auto It = Table->find(ToString(Elements[Idx]));
                if (It == Table->end())
                {
                    Node = nullptr;
                    break;
                }
                
                Node = &It->second;
            }
            
            if (!Node)
            {
                continue;
            }
            
            ParseConfig(*Node);
        }
    }
    
    F_Console::F_Console()
    {
    }
    F_Console::~F_Console()
    {
    }

    void F_Console::_RefreshCLIData(F_ConsoleData::F_CLI& CLIData)
    {
        cxxopts::Options CLIOptions("Abytek", "abytek");
        for (const auto& [Name, Variable] : _Variables)
        {
            Variable->AddCLIOption(CLIOptions);
        }
        CLIOptions.allow_unrecognised_options();
        {
            F_Executable& Executable = F_Executable::GetInstance();
            const auto& ExecutableInput = Executable.GetInput();

            const auto& CLIArgs = ExecutableInput.Arguments;
            
            TF_Vector<F_String> CLIArgs_U8;
            for (const auto& CLIArg : CLIArgs)
            {
                CLIArgs_U8.push_back(ToString(CLIArg.c_str()));
            }
            
            // Additional CLI args from file
            {
                F_Text CLIFilePath = (
                    *A_PathManager::GetInstance()->GetCommonDirectoryPath(
                        E_CommonDirectory::USER_DATA
                    ) 
                    + ABYTEK_TEXT("/Abytek/CLI.txt")
                );
                if (H_FSUtilities::Exists(CLIFilePath, E_FSEntryType::FILE))
                {
                    F_String CLIFileContent;
                    ABYTEK_FEEDBACK_STATUS_CHECK_HARD(
                        H_FSUtilities::ReadFileString(CLIFilePath, CLIFileContent)
                    );
                    std::istringstream iss(CLIFileContent.c_str());
                    std::string token;
                    while (iss >> token)
                    {
                        CLIArgs_U8.push_back(token.c_str());
                    }
                }
            }
            
            TF_Vector<const char*> CLIArgs_Raw;
            for (const auto& CLIArg_U8 : CLIArgs_U8)
            {
                CLIArgs_Raw.push_back(CLIArg_U8.c_str());
            }
            
            CLIData.ParseResult = CLIOptions.parse(CLIArgs_Raw.size(), CLIArgs_Raw.data());
        }
    }
    void F_Console::_RefreshConfigData(F_ConsoleData::F_Config& ConfigData)
    {
        /*
        // Load development config files
#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
        if (auto MainDevelopmentData = F_EngineRoot::GetInstance()->GetMainDevelopmentData())
        {
            for (const auto& AccessibleModuleName : MainDevelopmentData->GetAccessibleModuleNames(_ModuleName))
            {
                const auto& ModuleDevelopmentData = MainDevelopmentData->GetModuleDevelopmentData(AccessibleModuleName);
                F_Text ConfigDirectoryPath = ModuleDevelopmentData.Directory + ABYTEK_TEXT("/Configs");
                for (const auto& ConfigFileName : _ConfigFileNames)
                {
                    F_Text ConfigFilePath = ConfigDirectoryPath + ABYTEK_TEXT("/") + *ConfigFileName;
                    if (!A_FSAdapter_Standard::GetInstance()->IsExists(ConfigFilePath))
                    {
                        continue;
                    }
                
                    if (auto ConfigFileContentOpt = H_FSUtilities::ReadFileString(ConfigFilePath))
                    {
                        ConfigFiles.push_back({ ConfigFilePath, *ConfigFileContentOpt });
                    }
                    else
                    {
                        ABYTEK_LOG_FATAL() << "Cannot read config file at: " << ConfigFilePath;
                    }
                }
            }
        }
#endif
        
        // Load bundled config files
        F_Text BundledConfigsDirectoryPath = (
            *A_PathManager::GetInstance()->GetCommonDirectoryPath(
                E_CommonDirectory::BUNDLED_DATA
            )
            + ABYTEK_TEXT("/")
            + Console::GetBundledConfigsDirectoryRelativePath()
            + ABYTEK_TEXT("/")
            + *_ModuleName
        );
        if (auto ModuleConfigsDirectoryNamesOpt = A_FSAdapter_Standard::GetInstance()->ReadDirectory(BundledConfigsDirectoryPath))
        {
            for (const auto& ModuleConfigsDirectoryName : *ModuleConfigsDirectoryNamesOpt)
            {
                F_Text ModuleConfigsDirectoryPath = BundledConfigsDirectoryPath + ABYTEK_TEXT("/") + ModuleConfigsDirectoryName;
                if (!A_FSAdapter_Standard::GetInstance()->IsExists(ModuleConfigsDirectoryPath))
                {
                    continue;
                }
                
                for (const auto& ConfigFileName : _ConfigFileNames)
                {
                    F_Text ConfigFilePath = ModuleConfigsDirectoryPath + ABYTEK_TEXT("/") + *ConfigFileName;
                    if (!A_FSAdapter_Standard::GetInstance()->IsExists(ConfigFilePath))
                    {
                        continue;
                    }
                
                    if (
                        auto ConfigFileContentOpt = H_FSUtilities::ReadFileString(
                            ConfigFilePath, 
                            F_FileDescSourceOptions::MakeBundled()
                        )
                    )
                    {
                        ConfigFiles.push_back({ ConfigFilePath, *ConfigFileContentOpt });
                    }
                }
            }
        }*/
        
        for (const auto& [ConfigFilePath, ConfigFileContent] : _ConfigFiles)
        {
            if (ConfigFileContent.size() == 0)
            {
                continue;
            }
            
            try
            {
                auto ConfigParsedResult = toml::parse(ToString(ConfigFileContent).c_str());
                ConfigData.ParsedResults.push_back(ABYTEK_MOVE(ConfigParsedResult));
            }
            catch (const toml::parse_error& Error)
            {
                std::stringstream SS;
                SS << Error;
                ABYTEK_LOG_FATAL() << "Failed to parse config file (" << ConfigFilePath << "): " << SS.str().c_str();
            }
        }
    }
    
    void F_Console::Update(B8 RefreshCLI, B8 RefreshConfig)
    {
        if (RefreshCLI)
        {
            _Data.CLI = {};
            _RefreshCLIData(_Data.CLI);
        }
        if (RefreshConfig)
        {
            _Data.Config = {};
            _RefreshConfigData(_Data.Config);
        }
        for (const auto& [Name, Variable] : _Variables)
        {
            Variable->TryEvaluateCLIOption(_Data.CLI.ParseResult);
        }
        for (const auto& [Name, Variable] : _Variables)
        {
            Variable->ParseConfig(_Data.Config.ParsedResults);
        }
    }
}
