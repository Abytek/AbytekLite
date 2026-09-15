#pragma once

#include "Abytek/Engine.Core.prerequisites.hpp"
#include "Abytek/Development/EBT/EBTToolchainUtilities.hpp"
#include "Abytek/ModuleDevelopmentData.hpp"


#ifdef ABYTEK_ENABLE_DEVELOPMENT_DATA
namespace Abytek
{
    class F_DevelopmentData;
    class F_DevelopmentDataInstance;
    
    using F_DevelopmentDataExecuteJSCallback = TF_Function<F_FeedbackStatus(sio::message::list const& Msg)>;
    
    struct F_DevelopmentDataBuildParams
    {
        F_Text ProjectPath;
        F_EBTProjectInstance ProjectInstance;
        TF_Vector<F_Name> ApplicationFilter;
        
        static F_DevelopmentDataBuildParams MakeCurrent(
            const F_Text& InProjectPath, 
            const TF_Vector<F_Name>& InApplicationFilter = {}
        )
        {
            F_DevelopmentDataBuildParams Result;
            Result.ProjectPath = InProjectPath;
            Result.ProjectInstance = F_EBTProjectInstance::MakeCurrent();
            Result.ApplicationFilter = InApplicationFilter;
            return Result;
        }
    };
    
    struct F_DevelopmentDataInstanceBuildParams
    {
        TW<F_DevelopmentData> Owner;
        F_EBTCMakeInstance CMakeInstance;
        
        static F_DevelopmentDataInstanceBuildParams MakeCurrent(const TW_Valid<F_DevelopmentData>& InOwner)
        {
            F_DevelopmentDataInstanceBuildParams Result;
            Result.Owner = InOwner;
            Result.CMakeInstance = F_EBTCMakeInstance::MakeCurrent();
            return Result;
        }
    };
    
    class ABYTEK_ENGINE_CORE_API F_DevelopmentDataServerProgram final : public A_Object
    {
    private:
        U32 _ExitCode = 0;
        TU<A_Process> _NodeJSProcess;
        sio::client _SocketClient;

    public:
        ABYTEK_FORCE_INLINE U32 GetExitCode() const noexcept
        {
            return _ExitCode;
        }
        ABYTEK_FORCE_INLINE const auto& GetNodeJSProcess() const noexcept
        {
            return _NodeJSProcess;
        }
        ABYTEK_FORCE_INLINE const auto& GetSocketClient() const noexcept
        {
            return _SocketClient;
        }

    public:
        F_DevelopmentDataServerProgram();
        ~F_DevelopmentDataServerProgram();

        static F_FeedbackStatus Create(const TW_Valid<F_DevelopmentDataInstance>& DevelopmentData, TU<F_DevelopmentDataServerProgram>& OutProgram);

    public:
        std::shared_ptr<sio::message> AccessModule(const F_Name& ModuleName, const F_Text& AccessCode);
        std::shared_ptr<sio::message> AccessModuleProperty(const F_Name& ModuleName, const F_Name& ModulePropertyName);
        
    public:
        F_FeedbackStatus ExecuteJS(const F_Text& JSCode, F_DevelopmentDataExecuteJSCallback&& Callback = {});

    public:
        TF_Vector<F_Name> GetModuleNames();
    };
    
    class ABYTEK_ENGINE_CORE_API F_DevelopmentDataInstance final : public A_Object
    {
    public:
        ABYTEK_BEGIN_REFLECTOR()
        ABYTEK_END_REFLECTOR(F_DevelopmentDataInstance)

    private:
        TW<F_DevelopmentData> _Owner;
        F_EBTCMakeInstance _CMakeInstance;
        
        F_Text _CMakeBuildDirectoryPath;
        F_Text _ImportJSFilePath;
        
        TU<F_DevelopmentDataServerProgram> _ServerProgram;
        TF_Vector<F_Name> _ModuleNames;
        TF_Map<F_Name, TF_Vector<F_Name>> _ModuleNameToAccessibleModuleNames;
        TF_Map<F_Name, TF_Set<F_Name>> _ModuleNameToAccessibleModuleNames_Private;
        TF_Map<F_Name, TF_Set<F_Name>> _ModuleNameToAccessibleModuleNames_Public;
        TF_Map<F_Name, F_ModuleDevelopmentData> _ModuleDevelopmentDataMap;

    public:
        ABYTEK_FORCE_INLINE const auto& GetOwner() const noexcept
        {
            return _Owner;
        }
        ABYTEK_FORCE_INLINE const auto& GetCMakeInstance() const noexcept
        {
            return _CMakeInstance;
        }
        
        ABYTEK_FORCE_INLINE const auto& GetCMakeBuildDirectoryPath() const noexcept
        {
            return _CMakeBuildDirectoryPath;
        }
        ABYTEK_FORCE_INLINE const auto& GetImportJSFilePath() const noexcept
        {
            return _ImportJSFilePath;
        }
        
        ABYTEK_FORCE_INLINE const auto& GetServerProgram() const noexcept
        {
            return _ServerProgram;
        }
        ABYTEK_FORCE_INLINE const auto& GetModuleNames() const noexcept
        {
            return _ModuleNames;
        }
        ABYTEK_FORCE_INLINE const auto& GetModuleNameToAccessibleModuleNames() const noexcept
        {
            return _ModuleNameToAccessibleModuleNames;
        }
        ABYTEK_FORCE_INLINE const auto& GetModuleDevelopmentDataMap() const noexcept
        {
            return _ModuleDevelopmentDataMap;
        }

    public:
        F_DevelopmentDataInstance(const F_DevelopmentDataInstanceBuildParams& BuildParams);
        ~F_DevelopmentDataInstance();

        static F_FeedbackStatus Create(const F_DevelopmentDataInstanceBuildParams& BuildParams, TS<F_DevelopmentDataInstance>& OutDevelopmentData);
    
    private:
        F_FeedbackStatus StartDevelopmentDataServer();

    private:
        std::shared_ptr<sio::message> _AccessModule(const F_Name& ModuleName, const F_Text& AccessCode);
        TF_Optional<F_String> _AccessModule_String(const F_Name& ModuleName, const F_Text& AccessCode);
        TF_Optional<F_Text> _AccessModule_Text(const F_Name& ModuleName, const F_Text& AccessCode);
        TF_Optional<I64> _AccessModule_I64(const F_Name& ModuleName, const F_Text& AccessCode);
        TF_Optional<F64> _AccessModule_F64(const F_Name& ModuleName, const F_Text& AccessCode);
        TF_Optional<B8> _AccessModule_Bool(const F_Name& ModuleName, const F_Text& AccessCode);
        std::shared_ptr<sio::message> _AccessModuleProperty(const F_Name& ModuleName, const F_Name& ModulePropertyName);
        
    public:
        F_FeedbackStatus ExecuteJS(const F_Text& JSCode, F_DevelopmentDataExecuteJSCallback&& Callback = {});
        
    public:
        const F_ModuleDevelopmentData& GetModuleDevelopmentData(const F_Name& ModuleName);
        B8 HasModule(const F_Name& ModuleName);
        const TF_Vector<F_Name>& GetAccessibleModuleNames(const F_Name& ModuleName);
        
    public:
        F_FeedbackStatus ResolveModuleAutoProperty(
            const F_Name& ModuleName, 
            const F_Name& PropertyName, 
            F_Text& OutPropertyValue
        );
        F_FeedbackStatus ResolveModuleRuntimeOutputDirectoryPath(
            const F_Name& ModuleName, 
            F_Text& OutValue
        );
        F_FeedbackStatus ResolveModuleLibraryOutputDirectoryPath(
            const F_Name& ModuleName, 
            F_Text& OutValue
        );
        F_FeedbackStatus ResolveModuleArchiveOutputDirectoryPath(
            const F_Name& ModuleName, 
            F_Text& OutValue
        );
        F_FeedbackStatus ResolveModuleTargetFileDirectoryPath(
            const F_Name& ModuleName, 
            F_Text& OutValue
        ); 
        F_FeedbackStatus ResolveModuleTargetFilePath(
            const F_Name& ModuleName, 
            F_Text& OutValue
        ); 

    private:
        F_ModuleDevelopmentData _LoadModuleDevelopmentData(const F_Name& ModuleName);
        
    private:
        void _GatherAccessibleModuleNames();
        TF_Set<F_Name> _GatherAccessibleModuleNames(const F_Name& ModuleName, B8 GatherPrivate = true, B8 GatherPublic = true);
    };
    
    class ABYTEK_ENGINE_CORE_API F_DevelopmentData final : public A_Object
    {
    public:
        ABYTEK_BEGIN_REFLECTOR()
        ABYTEK_END_REFLECTOR(F_DevelopmentData)

    private:
        F_Text _ProjectPath;
        F_EBTProjectInstance _ProjectInstance;
        TF_Vector<F_Name> _ApplicationFilter;
        
        F_Text _BuildDirectoryPath;
        TF_Vector<TS<F_DevelopmentDataInstance>> _Instances;

    public:
        ABYTEK_FORCE_INLINE const auto& GetProjectPath() const noexcept
        {
            return _ProjectPath;
        }
        ABYTEK_FORCE_INLINE const auto& GetProjectInstance() const noexcept
        {
            return _ProjectInstance;
        }
        ABYTEK_FORCE_INLINE const auto& GetApplicationFilter() const noexcept
        {
            return _ApplicationFilter;
        }

        ABYTEK_FORCE_INLINE const auto& GetBuildDirectoryPath() const noexcept
        {
            return _BuildDirectoryPath;
        }
        ABYTEK_FORCE_INLINE const auto& GetInstances() const noexcept
        {
            return _Instances;
        }
        TS<F_DevelopmentDataInstance> GetMainInstance() const;

        ABYTEK_FORCE_INLINE const auto& GetCMakeBuildDirectoryPath() const noexcept
        {
            return GetMainInstance()->GetCMakeBuildDirectoryPath();
        }
        ABYTEK_FORCE_INLINE const auto& GetImportJSFilePath() const noexcept
        {
            return GetMainInstance()->GetImportJSFilePath();
        }
        
        ABYTEK_FORCE_INLINE const auto& GetModuleNames() const noexcept
        {
            return GetMainInstance()->GetModuleNames();
        }
        ABYTEK_FORCE_INLINE const auto& GetModuleNameToAccessibleModuleNames() const noexcept
        {
            return GetMainInstance()->GetModuleNameToAccessibleModuleNames();
        }
        ABYTEK_FORCE_INLINE const auto& GetModuleDevelopmentDataMap() const noexcept
        {
            return GetMainInstance()->GetModuleDevelopmentDataMap();
        }
        
    public:
        F_DevelopmentData(const F_DevelopmentDataBuildParams& BuildParams);
        ~F_DevelopmentData();

        static F_FeedbackStatus Create(const F_DevelopmentDataBuildParams& BuildParams, TS<F_DevelopmentData>& OutDevelopmentData);
        
    public:
        TF_Vector<Sz> FindInstanceIndices(const F_EBTCMakeInstanceQuery& CMakeInstanceQuery);
        TF_Vector<TS<F_DevelopmentDataInstance>> FindInstances(const F_EBTCMakeInstanceQuery& CMakeInstanceQuery);
        TS<F_DevelopmentDataInstance> FindInstance(const F_EBTCMakeInstanceQuery& CMakeInstanceQuery);
        
    public:
        const F_ModuleDevelopmentData& GetModuleDevelopmentData(const F_Name& ModuleName);
        B8 HasModule(const F_Name& ModuleName);
        const TF_Vector<F_Name>& GetAccessibleModuleNames(const F_Name& ModuleName);
    };
}
#endif