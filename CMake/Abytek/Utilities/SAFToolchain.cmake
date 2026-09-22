
include(Abytek/Utilities/SetGlobal)
include(Abytek/Utilities/CookUtils)

function(Abytek_SAF_SetupApplication)     
    cmake_parse_arguments(
        PARGS
        ""
        "NAME;COOK_PIPELINE"
        "INCLUDE_HEADERS;APPLICATION_CLASS;APPLICATION_MODULE_CLASSES"
        ${ARGN}
    )

    if(NOT PARGS_NAME)
        if(ModuleFullName)
            set(PARGS_NAME ${ModuleFullName})
        else()
            message(FATAL_ERROR "requires NAME")
        endif()
    endif()

    if(NOT PARGS_COOK_PIPELINE)
        set(PARGS_COOK_PIPELINE ${ABYTEK_DEFAULT_COOK_PIPELINE})
    endif()

    if(NOT PARGS_COOK_PIPELINE)
        set(PARGS_COOK_PIPELINE ${ABYTEK_DEFAULT_COOK_PIPELINE})
    endif()

    if(NOT PARGS_INCLUDE_HEADERS)
        set(PARGS_INCLUDE_HEADERS "")
    endif()

    if(NOT PARGS_APPLICATION_CLASS)
        set(PARGS_APPLICATION_CLASS "Abytek::F_Application")
    endif()

    if(NOT PARGS_APPLICATION_MODULE_CLASSES)
        set(PARGS_APPLICATION_MODULE_CLASSES "")
    endif()

    set(CookedExecutable.TargetName ${PARGS_NAME}.CookedExecutable)
    set(CookedExecutable.GeneratedSourcePrivateDirectory "${${PARGS_NAME}.BuildDirectory}/CookedExecutable/GeneratedSource/Private")
    if(NOT EXISTS "${CookedExecutable.GeneratedSourcePrivateDirectory}")
        file(MAKE_DIRECTORY "${CookedExecutable.GeneratedSourcePrivateDirectory}")
    endif()
    set(CookedExecutable.RuntimeDirectory "${${PARGS_NAME}.BuildDirectory}/CookedExecutable/Runtime")
    set(CookedExecutable.TargetFileDirectory "${CookedExecutable.RuntimeDirectory}/$<CONFIG>")
    set(CookedExecutable.ResolvedGeneratorExpressionsDirectory "${${PARGS_NAME}.BuildDirectory}/CookedExecutable/ResolvedGeneratorExpressions")
    if(NOT EXISTS "${CookedExecutable.ResolvedGeneratorExpressionsDirectory}")
        file(MAKE_DIRECTORY "${CookedExecutable.ResolvedGeneratorExpressionsDirectory}")
    endif()

    if(ABYTEK_ENABLE_DEVELOPMENT_BUILD)
        set(DevelopmentExecutable.TargetName ${PARGS_NAME}.DevelopmentExecutable)
        set(DevelopmentExecutable.GeneratedSourcePrivateDirectory "${${PARGS_NAME}.BuildDirectory}/DevelopmentExecutable/GeneratedSource/Private")
        if(NOT EXISTS "${DevelopmentExecutable.GeneratedSourcePrivateDirectory}")
            file(MAKE_DIRECTORY "${DevelopmentExecutable.GeneratedSourcePrivateDirectory}")
        endif()
        set(DevelopmentExecutable.RuntimeDirectory "${${PARGS_NAME}.BuildDirectory}/DevelopmentExecutable/Runtime")
        set(DevelopmentExecutable.TargetFileDirectory "${DevelopmentExecutable.RuntimeDirectory}/$<CONFIG>")
        set(DevelopmentExecutable.ResolvedGeneratorExpressionsDirectory "${${PARGS_NAME}.BuildDirectory}/DevelopmentExecutable/ResolvedGeneratorExpressions")
        if(NOT EXISTS "${DevelopmentExecutable.ResolvedGeneratorExpressionsDirectory}")
            file(MAKE_DIRECTORY "${DevelopmentExecutable.ResolvedGeneratorExpressionsDirectory}")
        endif()
    endif()

    set(HeaderIncludes "")
    foreach(Header ${PARGS_INCLUDE_HEADERS})
        string(REPLACE "\\" "/" SafeHeaderPath "${Header}")
        list(APPEND HeaderIncludes "#include \"${SafeHeaderPath}\"")
    endforeach()

    set(ApplicationModuleRegistrations "")
    foreach(ApplicationModuleClass ${PARGS_APPLICATION_MODULE_CLASSES})
        list(APPEND ApplicationModuleRegistrations "A_ApplicationCore::GetInstance()->RegisterModule<${ApplicationModuleClass}>();\n")
    endforeach()

    set(GeneratedExecutableCPPFileContent "
        #include \"Abytek/Engine.hpp\"
        #include \"Abytek/EngineRuntime.hpp\"
        ${HeaderIncludes}


        namespace Abytek
        {
            class F_SAFExecutable : public F_Executable
            {
            public:
                F_SAFExecutable(const F_ExecutableInput& Input);
                ~F_SAFExecutable() override = default;

            protected:
                virtual void OnStartup() override;
            };
            
            F_SAFExecutable::F_SAFExecutable(const F_ExecutableInput& Input) :
                F_Executable(Input)
            {
            }
            
            void F_SAFExecutable::OnStartup()
            {
                F_Executable::OnStartup();

                F_EngineRuntimeBuildParams EngineRuntimeBuildParams;
        #ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
                EngineRuntimeBuildParams.ProjectDirectoryPath = ABYTEK_TEXT(\"${ABYTEK_PROJECT_DIR}\");
        #endif
                EngineRuntimeBuildParams.ApplicationCreator = []() -> TU<A_ApplicationCore> {
                    F_ApplicationBuildParams ApplicationBuildParams;
                    ApplicationBuildParams.Name = ABYTEK_NAME(\"${PARGS_NAME}\");
                    ApplicationBuildParams.ExternalInitFunction = []
                    {
                        ${ApplicationModuleRegistrations}
                    };
                    return TU<${PARGS_APPLICATION_CLASS}>()(ApplicationBuildParams);
                };

                auto EngineRuntime = TU<F_EngineRuntime>()(
                    EngineRuntimeBuildParams
                );
                EngineRuntime->Start();
            }
        }


        ABYTEK_DEFINE_EXECUTABLE(Abytek::F_SAFExecutable);
    ")

    set(CookedExecutable.CPPFilePath "${CookedExecutable.GeneratedSourcePrivateDirectory}/SAFExecutable.cpp")
    if(EXISTS "${CookedExecutable.CPPFilePath}")
        file(REMOVE "${CookedExecutable.CPPFilePath}")
    endif()
    file(WRITE "${CookedExecutable.CPPFilePath}" "${GeneratedExecutableCPPFileContent}")

    if(ABYTEK_ENABLE_DEVELOPMENT_BUILD)
        set(DevelopmentExecutable.CPPFilePath "${DevelopmentExecutable.GeneratedSourcePrivateDirectory}/SAFExecutable.cpp")
        if(EXISTS "${DevelopmentExecutable.CPPFilePath}")
            file(REMOVE "${DevelopmentExecutable.CPPFilePath}")
        endif()
        file(WRITE "${DevelopmentExecutable.CPPFilePath}" "${GeneratedExecutableCPPFileContent}")
    endif()

    if(ANDROID)
        add_library(${CookedExecutable.TargetName} SHARED "${CookedExecutable.CPPFilePath}")
        if(ABYTEK_ENABLE_DEVELOPMENT_BUILD)
            add_library(${DevelopmentExecutable.TargetName} SHARED "${DevelopmentExecutable.CPPFilePath}")
        endif()
    else()
        add_executable(${CookedExecutable.TargetName} "${CookedExecutable.CPPFilePath}")
        if(ABYTEK_ENABLE_DEVELOPMENT_BUILD)
            add_executable(${DevelopmentExecutable.TargetName} "${DevelopmentExecutable.CPPFilePath}")
        endif()
    endif()

    get_target_property(ModuleVSFolderName ${PARGS_NAME} FOLDER)
    set_target_properties(${CookedExecutable.TargetName}
        PROPERTIES
            FOLDER "${ModuleVSFolderName}/Executables"
            PROJECT_LABEL ${PARGS_NAME}.CookedExecutable
    )
    if(ABYTEK_ENABLE_DEVELOPMENT_BUILD)
        set_target_properties(${DevelopmentExecutable.TargetName}
            PROPERTIES
                FOLDER "${ModuleVSFolderName}/Executables"
                PROJECT_LABEL ${PARGS_NAME}.DevelopmentExecutable
        )
    endif()

    set_target_properties(${CookedExecutable.TargetName} PROPERTIES UNITY_BUILD ON)
    target_link_libraries(${CookedExecutable.TargetName}
        PRIVATE 
            ${PARGS_NAME}
    )
    add_custom_command(TARGET ${CookedExecutable.TargetName} PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E echo "$<TARGET_FILE:${CookedExecutable.TargetName}>" > "${CookedExecutable.ResolvedGeneratorExpressionsDirectory}/TargetFile.txt"
    )
    add_custom_command(TARGET ${CookedExecutable.TargetName} PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E echo "$<TARGET_FILE_DIR:${CookedExecutable.TargetName}>" > "${CookedExecutable.ResolvedGeneratorExpressionsDirectory}/TargetFileDirectory.txt"
    )
    set_target_properties(${CookedExecutable.TargetName} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CookedExecutable.TargetFileDirectory}"
    )
    if(ABYTEK_ENABLE_DEVELOPMENT_BUILD)
        set_target_properties(${DevelopmentExecutable.TargetName} PROPERTIES UNITY_BUILD ON)
        target_link_libraries(${DevelopmentExecutable.TargetName}
            PRIVATE 
                ${PARGS_NAME}
        )
        add_custom_command(TARGET ${DevelopmentExecutable.TargetName} PRE_BUILD
            COMMAND ${CMAKE_COMMAND} -E echo "$<TARGET_FILE:${DevelopmentExecutable.TargetName}>" > "${DevelopmentExecutable.ResolvedGeneratorExpressionsDirectory}/TargetFile.txt"
        )
        add_custom_command(TARGET ${DevelopmentExecutable.TargetName} PRE_BUILD
            COMMAND ${CMAKE_COMMAND} -E echo "$<TARGET_FILE_DIR:${DevelopmentExecutable.TargetName}>" > "${DevelopmentExecutable.ResolvedGeneratorExpressionsDirectory}/TargetFileDirectory.txt"
        )
        set_target_properties(${DevelopmentExecutable.TargetName} PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY "${DevelopmentExecutable.TargetFileDirectory}"
        )
    endif()

    Abytek_ApplyDependencies(
        TARGET ${CookedExecutable.TargetName} 
        DEPENDENT_TARGETS
            ${PARGS_NAME}
    )
    if(ABYTEK_ENABLE_DEVELOPMENT_BUILD)
        Abytek_ApplyDependencies(
            TARGET ${DevelopmentExecutable.TargetName} 
            DEPENDENT_TARGETS
                ${PARGS_NAME}
        )
    endif()

    # Abytek_AddCookTarget(
    #     APPLICATION_NAME "${PARGS_NAME}"
    #     PLATFORM Windows
    #     PREFIX .Desktop0
    #     PROCESSOR_ARCHS
    #         X86_64
    #     EXTRA_ARGUMENTS 
    #         --CookRHIFeatureModel Desktop0
    # )
    # Abytek_AddCookTarget(
    #     APPLICATION_NAME "${PARGS_NAME}"
    #     PLATFORM Windows
    #     PREFIX .Desktop1
    #     PROCESSOR_ARCHS
    #         X86_64
    #     EXTRA_ARGUMENTS 
    #         --CookRHIFeatureModel Desktop1
    # )
    # Abytek_AddCookTarget(
    #     APPLICATION_NAME "${PARGS_NAME}"
    #     PLATFORM Android
    #     PREFIX .Mobile0
    #     CONFIGS 
    #         Debug 
    #         Release
    #     PROCESSOR_ARCHS
    #         ARM_64
    #     EXTRA_ARGUMENTS 
    #         --CookRHIFeatureModel Mobile0
    # )
    # Abytek_AddCookTarget(
    #     APPLICATION_NAME "${PARGS_NAME}"
    #     PLATFORM Android
    #     PREFIX .XR0
    #     CONFIGS 
    #         Debug 
    #         Release
    #     PROCESSOR_ARCHS
    #         ARM_64
    #     EXTRA_ARGUMENTS 
    #         --CookRHIFeatureModel XR0
    # )
    # Abytek_AddCookTarget(
    #     APPLICATION_NAME "${PARGS_NAME}"
    #     PLATFORM Android
    #     PREFIX .XR1
    #     CONFIGS 
    #         Debug 
    #         Release
    #     PROCESSOR_ARCHS
    #         ARM_64
    #     EXTRA_ARGUMENTS 
    #         --CookRHIFeatureModel XR1
    # )
endfunction()

function(Abytek_SAF_AutoSetupApplications)
    set(ApplicationDescriptorsDirectory "${ABYTEK_PROJECT_BUILD_DIR}/../ApplicationDescriptors-${ABYTEK_EBT_PROJECT_SIGNATURE}")
    set(ApplicationDescriptorsDirectoryJSON "${ApplicationDescriptorsDirectory}/JSON")
    set(ApplicationDescriptorsDirectoryCMake "${ApplicationDescriptorsDirectory}/CMake")
    
    file(GLOB CMakeDescriptorFiles "${ApplicationDescriptorsDirectoryCMake}/*")
    foreach(CMakeDescriptorFile ${CMakeDescriptorFiles})
        message(STATUS "Found application descriptor at: ${CMakeDescriptorFile}")
        include("${CMakeDescriptorFile}")
        Abytek_SAF_SetupApplication(
            NAME ${ApplicationName}
            COOK_PIPELINE ${CookPipeline}
            INCLUDE_HEADERS ${IncludeHeaders}
            APPLICATION_CLASS ${ApplicationClass}
            APPLICATION_MODULE_CLASSES ${ApplicationModuleClasses}
        )
    endforeach()
endfunction()
     