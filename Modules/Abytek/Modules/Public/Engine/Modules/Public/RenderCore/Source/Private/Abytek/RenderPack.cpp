#include "Abytek/RenderPack.hpp"
#include "Abytek/RenderCoreHelper.hpp"
#include "Abytek/RenderRegistry.hpp"
#include "Abytek/RenderPackProxy.hpp"
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
        _Templates.erase(_Templates.find(HashCode));
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
            B8 HasAnyTemplateToRemove = false;
            
            TF_Set<F_RHITemplateHashCode> UsedTemplateHashCodes;
            for (const auto& [TemplateHashCode, Template] : Templates)
            {
                if (H_RenderCore::IsRootTemplate(Template.Weak()))
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
        PrepareTemplates(
            GetEnvironment(), 
            ABYTEK_WTHIS().DynamicCast<A_RenderPackTemplateMap>()
        );
#endif
        
        _IsTemplatesLoaded = true;
        SetupRenderable();
    }
    void F_RenderPack::OnUnload()
    {
        CleanUpRenderable();
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
                    TemplateDatabase.Weak(),
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
                    TemplateDatabase.Weak(),
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

    void F_RenderPack::OnCreateRenderState()
    {
        H_Frame::EnqueueCommand<E_FrameParamType::RENDER>(
            [
                RenderProxy = GetRenderProxy().FastCast<F_RenderPackProxy>(),
                Registry = _Registry
            ]
            {
                RenderProxy->_Registry = Registry;
                RenderProxy->_RegistryRuntime = Registry->GetOrActiveRuntime(
                    H_RHI::GetMainContext()
                );
            }
        );
        H_Frame::EnqueueCommand<E_FrameParamType::RENDER>(
            [
                RenderProxy = GetRenderProxy().FastCast<F_RenderPackProxy>(),
                Templates = GetTemplates()
            ]
            {
                for (const auto& [TemplateHashCode, Template] : Templates)
                {
                    RenderProxy->EnsureTemplate(Template);
                }
            }
        );
    }
    void F_RenderPack::OnDestroyRenderState()
    {
    }
    TS<A_RenderProxy> F_RenderPack::CreateRenderProxy()
    {
        return TS<F_RenderPackProxy>()(ABYTEK_WTHIS());
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
        if (IsRenderable())
        {
            H_Frame::EnqueueCommand<E_FrameParamType::RENDER>(
                [
                    RenderProxy = GetRenderProxy().FastCast<F_RenderPackProxy>(),
                    CachedTemplate = Template
                ]
                {
                    RenderProxy->AddTemplate(CachedTemplate);
                }
            );
        }
    }
    void F_RenderPack::OnRemoveTemplate(F_RHITemplateHashCode HashCode)
    {
        auto Template = GetTemplate(HashCode);
        if (!_IsTemplatesLoaded)
        {
            return;
        }
        if (IsRenderable())
        {
            H_Frame::EnqueueCommand<E_FrameParamType::RENDER>(
                [
                    RenderProxy = GetRenderProxy().FastCast<F_RenderPackProxy>(),
                    CachedTemplate = Template
                ]
                {
                    RenderProxy->RemoveTemplate(CachedTemplate->GetHashCode());
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
}
