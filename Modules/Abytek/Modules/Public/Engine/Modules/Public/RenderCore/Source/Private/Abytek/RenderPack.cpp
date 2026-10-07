#include "Abytek/RenderPack.hpp"
#include "Abytek/RenderCoreHelper.hpp"
#include "Abytek/RenderRegistry.hpp"
#include "Abytek/TaskScheduler_MediumFrequencyWorkers.hpp"
#include "Abytek/Development/Cook/CookProfile.hpp"
#include "Abytek/Development/Cook/CookSettingContainer.hpp"
#include "Abytek/Development/RenderCore/RenderCoreCookSetting.hpp"
#include "Abytek/Frame/FrameHelper.hpp"
#include "Abytek/World/WorldSubsystemContainer.hpp"


namespace Abytek
{
    A_RenderPackTemplateMap::A_RenderPackTemplateMap()
    {
    }
    A_RenderPackTemplateMap::~A_RenderPackTemplateMap()
    {
    }

    B8 A_RenderPackTemplateMap::HasTemplate(F_RHITemplateHashCode HashCode) const
    {
        return _Templates.find(HashCode) != _Templates.end();
    }
    TS<A_RHITemplate> A_RenderPackTemplateMap::FindTemplate(F_RHITemplateHashCode HashCode)
    {
        auto It = _Templates.find(HashCode);
        if (It == _Templates.end())
        {
            return {};
        }
        return It->second;
    }
    TS<A_RHITemplate> A_RenderPackTemplateMap::GetTemplate(F_RHITemplateHashCode HashCode)
    {
        auto It = _Templates.find(HashCode);
        ABYTEK_ENGINE_RENDER_CORE_ASSERT(It != _Templates.end()) << "Not found template with hash code: " << HashCode;
        return It->second;
    }
    void A_RenderPackTemplateMap::AddTemplate(const TS<A_RHITemplate>& Template)
    {
        ABYTEK_ENGINE_RENDER_CORE_ASSERT(!HasTemplate(Template->GetHashCode())) << "Already added template with hash code: " << Template->GetHashCode();
        
        _Templates.insert({ Template->GetHashCode(), Template });
        _SortedTemplates.push_back(Template.Weak());
        
        OnAddTemplate(Template);
        OnModifyTemplates();
        for (const auto& RefTemplateHashCode : Template->GetDependencyHashCodes())
        {
            EnsureTemplate(Template->GetDatabase()->GetTemplate(RefTemplateHashCode));
        }
    }
    void A_RenderPackTemplateMap::RemoveTemplate(F_RHITemplateHashCode HashCode)
    {
        ABYTEK_ENGINE_RENDER_CORE_ASSERT(HasTemplate(HashCode)) << "Not added template with hash code: " << HashCode;
        
        OnModifyTemplates();
        OnRemoveTemplate(HashCode);
        
        auto It = _Templates.find(HashCode);
        _SortedTemplates.erase(
            std::find(
                _SortedTemplates.begin(),
                _SortedTemplates.end(),
                It->second.Weak()
            )  
        );
        _Templates.erase(It);
    }
    void A_RenderPackTemplateMap::EnsureTemplate(const TS<A_RHITemplate>& Template)
    {
        if (HasTemplate(Template->GetHashCode()))
        {
            return;
        }
        AddTemplate(Template);
    }

    void A_RenderPackTemplateMap::RemoveUnusedTemplates()
    {
        TF_Vector<F_RHITemplateHashCode> TemplateHashCodesToRemove;
        TF_Map<F_RHITemplateHashCode, TS<A_RHITemplate>> Templates = _Templates;
        
        while (true)
        {
            TF_Set<F_RHITemplateHashCode> UsedTemplateHashCodes;
            for (const auto& [TemplateHashCode, Template] : Templates)
            {
                if (Template->IsRootTemplate())
                {
                    if (!UsedTemplateHashCodes.contains(TemplateHashCode))
                    {
                        UsedTemplateHashCodes.insert(TemplateHashCode);
                    }
                }
                for (F_RHITemplateHashCode DependencyTemplateHashCode : Template->GetDependencyHashCodes())
                {
                    if (!UsedTemplateHashCodes.contains(DependencyTemplateHashCode))
                    {
                        UsedTemplateHashCodes.insert(DependencyTemplateHashCode);
                    }
                }
            }
            
            TF_Vector<F_RHITemplateHashCode> NewTemplateHashCodesToRemove;
            for (const auto& [TemplateHashCode, Template] : Templates)
            {
                if (UsedTemplateHashCodes.contains(TemplateHashCode))
                {
                    continue;
                }
                NewTemplateHashCodesToRemove.push_back(TemplateHashCode);
            }
            
            B8 HasAnyTemplateToRemove = !NewTemplateHashCodesToRemove.empty();
            for (const auto& TemplateHashCode : NewTemplateHashCodesToRemove)
            {
                Templates.erase(Templates.find(TemplateHashCode));
            }
            
            TemplateHashCodesToRemove.insert(
                TemplateHashCodesToRemove.end(), 
                NewTemplateHashCodesToRemove.begin(), 
                NewTemplateHashCodesToRemove.end()
            );
            
            if (!HasAnyTemplateToRemove)
            {
                break;
            }
        }
        
        for (const auto& TemplateHashCode : TemplateHashCodesToRemove)
        {
            RemoveTemplate(TemplateHashCode); 
        }
    }

    void A_RenderPackTemplateMap::OnAddTemplate(const TS<A_RHITemplate>& Template)
    {
    }
    void A_RenderPackTemplateMap::OnRemoveTemplate(F_RHITemplateHashCode HashCode)
    {
    }
    void A_RenderPackTemplateMap::OnModifyTemplates()
    {
    }
    
    F_RenderPackTemplateMap::F_RenderPackTemplateMap()
    {
    }
    F_RenderPackTemplateMap::~F_RenderPackTemplateMap()
    {
    }

    F_RenderPackData::F_RenderPackData(
        const TW_Valid<F_RenderPack>& Pack,
        const TS<A_RenderRegistryPortData>& PortData
    ) :
        _Pack(Pack),
        _PortData(PortData)
    {
    }
    F_RenderPackData::~F_RenderPackData()
    {
    }

    void F_RenderPackData::OnAddTemplate(const TS<A_RHITemplate>& Template)
    {
        auto TemplateRuntimeDatabase = _PortData->GetRegistryRuntime()->GetTemplateRuntimeDatabase();
        _TemplateRuntimes[Template->GetHashCode()] = TemplateRuntimeDatabase->GetOrActivateRuntime(Template);
    }
    void F_RenderPackData::OnRemoveTemplate(F_RHITemplateHashCode HashCode)
    {
        _TemplateRuntimes.erase(_TemplateRuntimes.find(HashCode));
    }

    void F_RenderPackData::EnqueueCommand(TF_Function<void()>&& Command)
    {
        _PortData->GetPort()->EnqueueCommand(ABYTEK_MOVE(Command));
    }

    ABYTEK_REFLECT(F_RenderPack)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::RenderPack"));
    }

    F_RenderPack::F_RenderPack(const F_SerializableObjectInitParams& InitParams) :
        A_WorldContext(InitParams)
    {
        if (!HasSerializableFlags(E_SerializableObjectFlag::CDO))
        {
            ABYTEK_ENGINE_RENDER_CORE_ASSERT(GetWorld()->GetSubsystemContainer()->GetUnit<F_RenderCoreManager>()->GetAllowCreateRenderPacks());
            const auto& Metadata = GetEnvironment()->Metadata;
            _Registry = AnyCast<TS<F_RenderRegistry>>(
                Metadata.find(
                    F_RenderRegistry::GetSerializableEnvironmentMetadataElementName_Registry()
                )->second
            );
        }
    }
    F_RenderPack::~F_RenderPack()
    {
        if (!HasSerializableFlags(E_SerializableObjectFlag::CDO))
        {
            ABYTEK_ENGINE_RENDER_CORE_ASSERT(GetWorld()->GetSubsystemContainer()->GetUnit<F_RenderCoreManager>()->GetAllowCreateRenderPacks());
        }
    }
    
    void F_RenderPack::OnLoad()
    {
#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
        if (!HasSerializableFlags(E_SerializableObjectFlag::CDO))
        {
            PrepareTemplates(
                GetEnvironment(), 
                ABYTEK_WTHIS().DynamicCast<A_RenderPackTemplateMap>()
            );
        }
#endif
        
        _IsTemplatesLoaded = true;
        if (!HasSerializableFlags(E_SerializableObjectFlag::CDO))
        {
            _Registry->_RegistryPack(ABYTEK_WTHIS());
            for (const auto& Port : _Registry->GetPorts())
            {
                AddData(Port->GetData());
            }
        }
    }
    void F_RenderPack::OnUnload()
    {
        if (!HasSerializableFlags(E_SerializableObjectFlag::CDO))
        {
            for (const auto& Port : _Registry->GetPorts())
            {
                RemoveData(Port->GetData());
            }
            _Registry->_UnregistryPack(ABYTEK_WTHIS());
        }
        _IsTemplatesLoaded = false;
    }

    F_FeedbackStatus F_RenderPack::BinarySerialize(F_SerializableObjectBinarySerializeParams& Params)
    {
        ABYTEK_FEEDBACK_STATUS_CHECK(
            A_SerializableObject::BinarySerialize(Params)
        );
        
        TS<F_RenderRegistry> Registry;
        TW<A_RenderPackTemplateMap> TemplateMap;
#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
        if (IsCookModeSerializableEnvironment(Params.Environment))
        {
            auto CookProfile = F_CookProfile::GetMain();
            auto RenderCoreCookSetting = CookProfile->GetSettingContainer()->GetUnit<F_RenderCoreCookSetting>();
            Registry = RenderCoreCookSetting->GetRenderRegistry();
            TemplateMap = _CookedTemplateMap.Weak().DynamicCast<A_RenderPackTemplateMap>();
        }
        else
#endif
        {
            Registry = _Registry;
            TemplateMap = ABYTEK_WTHIS().DynamicCast<A_RenderPackTemplateMap>();
        }
        if (!Registry)
        {
            return F_FeedbackStatus::MakeFailed(
                ABYTEK_TEXT("Invalid render registry to serialize render pack: ") + *GetPath()    
            );
        }
        
        {
            auto TemplateSerializer = Registry->GetTemplateSerializer();
            auto TemplateDatabase = Registry->GetTemplateDatabase();
            
            TF_Vector<TS<A_RHITemplate>> Templates;
            for (const auto& [TemplateHashCode, Template] : TemplateMap->GetTemplates())
            {
                Templates.push_back(Template);
            }
            
            ABYTEK_FEEDBACK_STATUS_CHECK(
                TemplateSerializer->TryWriteTemplatePack(
                    Params.MainView,
                    TemplateDatabase,
                    Templates
                )
            );
        }
        return F_FeedbackStatus::MakeSucceeded();
    }
    F_FeedbackStatus F_RenderPack::BinaryDeserialize(F_SerializableObjectBinaryDeserializeParams& Params)
    {
        ABYTEK_FEEDBACK_STATUS_CHECK(
            A_SerializableObject::BinaryDeserialize(Params)
        );
        
        {
            auto TemplateSerializer = _Registry->GetTemplateSerializer();
            auto TemplateDatabase = _Registry->GetTemplateDatabase();
            
            TF_Vector<TS<A_RHITemplate>> Templates;
            ABYTEK_FEEDBACK_STATUS_CHECK(
                TemplateSerializer->TryReadTemplatePack(
                    Params.MainView,
                    TemplateDatabase,
                    Templates
                )
            );
            
            _IsInitialTemplateAdding = true;
            for (const auto& Template : Templates)
            {
                AddTemplate(Template);
            }
            _IsInitialTemplateAdding = false;
        }
        return F_FeedbackStatus::MakeSucceeded();
    }

#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
    void F_RenderPack::PrepareTemplates(
        const TW_Valid<F_SerializableEnvironment>& SerializableEnvironment,
        const TW_Valid<A_RenderPackTemplateMap>& RenderPackTemplateMap
    )
    {
    }
#endif

    void F_RenderPack::OnAddTemplate(const TS<A_RHITemplate>& Template)
    {
        if (!_IsTemplatesLoaded)
        {
            return;
        }
        if (!_DataList.empty())
        {
            auto ExportedData = Template->ExportData();
            for (const auto& Data : _DataList)
            {
                Data->EnqueueCommand(
                    [
                        CachedData = Data,
                        CachedExportedData = ExportedData
                    ]
                    {
                        auto TemplateDatabase = CachedData->GetPortData()->GetRegistryRuntime()->GetTemplateDatabase();
                        CachedData->AddTemplate(
                            CachedExportedData->Import(TemplateDatabase)
                        );
                    }
                );
            }
        }
    }
    void F_RenderPack::OnRemoveTemplate(F_RHITemplateHashCode HashCode)
    {
        auto Template = GetTemplate(HashCode);
        if (!_IsTemplatesLoaded)
        {
            return;
        }
        for (const auto& Data : _DataList)
        {
            Data->EnqueueCommand(
                [
                    CachedData = Data,
                    CachedTemplateHashCode = Template->GetHashCode()
                ]
                {
                    CachedData->RemoveTemplate(CachedTemplateHashCode);
                }
            );
        }
    }
    void F_RenderPack::OnModifyTemplates()
    {
        if (_IsInitialTemplateAdding)
        {
            return;
        }
        MarkPackageDirty();
    }

#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
    void F_RenderPack::PrepareForCooking()
    {
        _CookedTemplateMap = TS<F_RenderPackTemplateMap>()();
    }
    void F_RenderPack::Cook()
    {
        auto CookProfile = F_CookProfile::GetMain();
        auto SerializableEnvironment = CookProfile->GetSerializableEnvironment();
        PrepareTemplates(
            SerializableEnvironment.Weak(),
            _CookedTemplateMap.Weak()
        );
    }
    void F_RenderPack::CleanUpAfterCooking()
    {
        _CookedTemplateMap = {};
    }
#endif

    void F_RenderPack::AddData(const TS<A_RenderRegistryPortData>& PortData)
    {
        auto Data = TS<F_RenderPackData>()(ABYTEK_WTHIS(), PortData);
        _DataList.push_back(Data);
        
        const auto& Templates = GetSortedTemplates();
        TF_Vector<TS<A_RHITemplateExportedData>> ExportedDataList;
        ExportedDataList.reserve(Templates.size());
        for (const auto& Template : Templates)
        {
            ExportedDataList.push_back(Template->ExportData());
        }
        Data->EnqueueCommand(
            [
                CachedData = Data,
                CachedExportedDataList = ABYTEK_MOVE(ExportedDataList)
            ]
            {
                auto TemplateDatabase = CachedData->GetPortData()->GetRegistryRuntime()->GetTemplateDatabase();
                for (const auto& ExportedData : CachedExportedDataList)
                {
                    CachedData->EnsureTemplate(
                        ExportedData->Import(TemplateDatabase)
                    );
                }
            }
        );
    }
    void F_RenderPack::RemoveData(const TS<A_RenderRegistryPortData>& PortData)
    {
        for (auto It = _DataList.begin(); It != _DataList.end(); ++It)
        {
            const auto& Data = *It;
            if (Data->GetPortData() == PortData)
            {
                Data->EnqueueCommand(
                    [
                        CachedData = Data
                    ]
                    {
                    }
                );
                _DataList.erase(It);
                break;
            }
        }
    }

#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
    void F_RenderPack::ExecuteExclusiveTemplateCompilation(
        const TW_Valid<F_SerializableEnvironment>& SerializableEnvironment,
        const TW_Valid<A_RenderPackTemplateMap>& RenderPackTemplateMap,
        const TF_Set<F_RHITemplateHashCode>& TemplateHashCodesToCompile,
        const TF_Set<F_RHITemplateHashCode>& RootTemplateHashCodes, 
        TF_Function<void(TF_Vector<TS<A_RHITemplate>>& OutNewTemplates)>&& MainWork
    )
    {
        auto Registry = F_RenderRegistry::GetSerializableEnvironmentMetadataElement_Registry(SerializableEnvironment);
        auto TemplateDatabase = Registry->GetTemplateDatabase();
        
        {
            auto LastTemplates = RenderPackTemplateMap->GetTemplates();
            for (const auto& [TemplateHashCode, Template] : LastTemplates)
            {
                if (!Template->IsRootTemplate())
                {
                    continue;
                }
                if (!RootTemplateHashCodes.contains(TemplateHashCode))
                {
                    RenderPackTemplateMap->RemoveTemplate(TemplateHashCode);
                }
            }
        }
        for (const auto& TemplateHashCode : TemplateHashCodesToCompile)
        {
            if (RenderPackTemplateMap->HasTemplate(TemplateHashCode))
            {
                RenderPackTemplateMap->RemoveTemplate(TemplateHashCode);
            }
        }
        RenderPackTemplateMap->RemoveUnusedTemplates();
        
        TF_Vector<TS<A_RHITemplate>> NewTemplates;
        MainWork(NewTemplates);
        
        TF_Vector<TS<A_RHITemplate>> AllNewTemplates; // with non-root templates, notes there could be some templates that were already added
        A_RHITemplate::GatherSortedListWithDependencies(
            TemplateDatabase.Weak(),
            NewTemplates,
            AllNewTemplates,
            true // skip unlisted roots because we just need to add necessary non-root templates
        );
        
        for (const auto& Template : AllNewTemplates)
        {
            if (RenderPackTemplateMap->HasTemplate(Template->GetHashCode()))
            {
                continue;
            }
            RenderPackTemplateMap->AddTemplate(Template);
        }
    }

    void F_RenderPack::ExecuteParallelCompileCommands(
        const TF_Vector<TF_Function<void(TF_Vector<TS<A_RHITemplate>>& OutTemplates)>>& Commands,
        TF_Vector<TS<A_RHITemplate>>& OutNewTemplates
    )
    {
        U32 NumCommands = static_cast<U32>(Commands.size());
                    
        TF_Vector<TS_Unmanaged<F_TaskPromise>> TaskPromises;
        TF_Vector<TF_Vector<TS<A_RHITemplate>>> GroupedOutputTemplates(NumCommands);
                    
        for (U32 Idx = 0; Idx < NumCommands; ++Idx)
        {
            TaskPromises.push_back(
                H_TaskUtilities::Schedule(
                    F_TaskScheduler_MediumFrequencyWorkers::GetInstance(),
                    [&Commands, &GroupedOutputTemplates, Idx]
                    {
                        const auto& Command = Commands[Idx];
                        Command(GroupedOutputTemplates[Idx]);
                    }
                )
            );
        }
        ABYTEK_AWAIT TaskPromises;
                    
        for (const auto& GroupedOutputTemplateList : GroupedOutputTemplates)
        {
            for (const auto& Template : GroupedOutputTemplateList)
            {
                OutNewTemplates.push_back(Template);
            }
        }
    }
#endif
}
