#include "Abytek/RenderRegistry.hpp"
#include "Abytek/RenderRegistryRuntime.hpp"
#include "Abytek/RenderPack.hpp"
#include "Abytek/Frame/FrameHelper.hpp"


namespace Abytek
{
    A_RenderRegistryPortData::A_RenderRegistryPortData(const TW_Valid<A_RenderRegistryPort>& Port) :
        _Port(Port)
    {
        _RHIFeatureSupports = Port->GetRegistry()->GetRHIConfig().FeatureSupports;
    }
    A_RenderRegistryPortData::~A_RenderRegistryPortData()
    {
    }

    void A_RenderRegistryPortData::OnInit()
    {
    }

    void A_RenderRegistryPortData::OnRelease()
    {
    }

    void A_RenderRegistryPortData::Init()
    {
        _RegistryRuntime = CreateRegistryRuntime();
        OnInit();
    }
    void A_RenderRegistryPortData::Release()
    {
        OnRelease();
        _RegistryRuntime = {};
    }

    A_RenderRegistryPort::A_RenderRegistryPort(const TW_Valid<F_RenderRegistry>& Registry) :
        _Registry(Registry)
    {
    }
    A_RenderRegistryPort::~A_RenderRegistryPort()
    {
    }

    void A_RenderRegistryPort::OnInit()
    {
    }
    void A_RenderRegistryPort::OnRelease()
    {
    }

    void A_RenderRegistryPort::Init()
    {
        _Registry->_RegistryPort(ABYTEK_WTHIS());
        _Data = CreateData();
        OnInit();
        EnqueueCommand(
            [CachedData = _Data]
            {
                CachedData->Init();
            }
        );
        
        for (const auto& Pack : _Registry->GetPacks())
        {
            Pack->AddData(_Data);
        }
    }
    void A_RenderRegistryPort::Release()
    {
        for (const auto& Pack : _Registry->GetPacks())
        {
            Pack->RemoveData(_Data);
        }
        
        EnqueueCommand(
            [CachedData = _Data]
            {
                CachedData->Release();
            }
        );
        OnRelease();
        _Data = {};
        _Registry->_UnregistryPort(ABYTEK_WTHIS());
    }

    void A_RenderRegistryPort::EnqueueCommand(TF_Function<void()>&& Command)
    {
        Command();
    }

    F_MainRenderRegistryPortData::F_MainRenderRegistryPortData(const TW_Valid<F_MainRenderRegistryPort>& Port) :
        A_RenderRegistryPortData(Port)
    {
    }
    F_MainRenderRegistryPortData::~F_MainRenderRegistryPortData()
    {
    }

    TS<F_RenderRegistryRuntime> F_MainRenderRegistryPortData::CreateRegistryRuntime()
    {
        F_RenderRegistryRuntimeBuildParams BuildParams;
        BuildParams.Context = H_RHI::GetMainContext();
        BuildParams.RHIFeatureSupports = GetRHIFeatureSupports();
        return TS<F_RenderRegistryRuntime>()(BuildParams);
    }

    TS<A_RHISubmissionItemContainer> F_MainRenderRegistryPortData::GetSubmissionItemContainer()
    {
        return H_RHI::GetMainSubmissionQueue();
    }

    F_MainRenderRegistryPort::F_MainRenderRegistryPort(const TW_Valid<F_RenderRegistry>& Registry) :
        A_RenderRegistryPort(Registry)
    {
    }
    F_MainRenderRegistryPort::~F_MainRenderRegistryPort()
    {
    }

    void F_MainRenderRegistryPort::OnInit()
    {
    }
    void F_MainRenderRegistryPort::OnRelease()
    {
    }

    TS<A_RenderRegistryPortData> F_MainRenderRegistryPort::CreateData()
    {
        return TS<F_MainRenderRegistryPortData>()(ABYTEK_WTHIS());
    }

    void F_MainRenderRegistryPort::EnqueueCommand(TF_Function<void()>&& Command)
    {
        H_Frame::EnqueueCommand<E_FrameParamType::RENDER>(ABYTEK_MOVE(Command));
    }

    ABYTEK_REFLECT(F_RenderRegistry)
    {
    }

    F_Name F_RenderRegistry::GetSerializableEnvironmentMetadataElementName_Registry()
    {
        return ABYTEK_NAME("RenderRegistry");
    }

    TS<F_RenderRegistry> F_RenderRegistry::GetSerializableEnvironmentMetadataElement_Registry(
        const TW_Valid<F_SerializableEnvironment>& SerializableEnvironment
    )
    {
        const auto& Metadata = SerializableEnvironment->Metadata;
        if (Metadata.find(GetSerializableEnvironmentMetadataElementName_Registry()) == Metadata.end())
        {
            return {};
        }
        return AnyCast<TS<F_RenderRegistry>>(
            Metadata.find(GetSerializableEnvironmentMetadataElementName_Registry())
            ->second
        );
    }
    void F_RenderRegistry::SetSerializableEnvironmentMetadataElement_Registry(
        const TW_Valid<F_SerializableEnvironment>& SerializableEnvironment, 
        const TS<F_RenderRegistry>& Value
    )
    {
        auto& Metadata = SerializableEnvironment->Metadata;
        Metadata[GetSerializableEnvironmentMetadataElementName_Registry()] = Value;
    }
    void F_RenderRegistry::UnsetSerializableEnvironmentMetadataElement_Registry(
        const TW_Valid<F_SerializableEnvironment>& SerializableEnvironment
    )
    {
        auto& Metadata = SerializableEnvironment->Metadata;
        Metadata.erase(Metadata.find(GetSerializableEnvironmentMetadataElementName_Registry()));
    }

    F_RenderRegistry::F_RenderRegistry(const F_RenderRegistryBuildParams& BuildParams) :
        _World(BuildParams.World),
        _SerializableEnvironment(BuildParams.SerializableEnvironment),
        _RHIConfig(BuildParams.RHIConfig),
        _DebugGeneratedShaders(BuildParams.DebugGeneratedShaders),
        _Dependencies(BuildParams.Dependencies)
    {
#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
        _Compiler = A_RHICompiler::Create(_RHIConfig.API);
#endif
        _TemplateDatabase = A_RHITemplateDatabase::Create(_RHIConfig.API, _RHIConfig.FeatureSupports);
        _TemplateSerializer = A_RHITemplateSerializer::Create(_RHIConfig.API, _RHIConfig.FeatureSupports);
        
        TW<F_World> World = ABYTEK_WTHIS().DynamicCast<I_GetWorld>()->GetWorld();
        if (World->HasFlags(E_WorldFlag::CREATE_RENDER_SCENE))
        {
            _MainPort = TS<F_MainRenderRegistryPort>()(ABYTEK_WTHIS());
            _MainPort->Init();
        }
    }
    F_RenderRegistry::~F_RenderRegistry()
    {
        if (_MainPort)
        {
            _MainPort->Release();
            _MainPort = {};
        }
    }
    
    TS<A_RHITemplate> F_RenderRegistry::QueryTemplate(F_RHITemplateHashCode HashCode)
    {
        if (auto Template = _TemplateDatabase->GetTemplate(HashCode))
        {
            return Template;
        }
        for (const auto& Dependency : _Dependencies)
        {
            if (auto Template = Dependency->QueryTemplate(HashCode))
            {
                return Template;
            }
        }
        return {};
    }

    void F_RenderRegistry::_RegistryPort(const TW_Valid<A_RenderRegistryPort>& Port)
    {
        _Ports.push_back(Port);
    }
    void F_RenderRegistry::_UnregistryPort(const TW_Valid<A_RenderRegistryPort>& Port)
    {
        _Ports.erase(
            std::find(
                _Ports.begin(),
                _Ports.end(),
                Port
            )
        );
    }

    void F_RenderRegistry::_RegistryPack(const TW_Valid<F_RenderPack>& Pack)
    {
        _Packs.insert(Pack);
    }
    void F_RenderRegistry::_UnregistryPack(const TW_Valid<F_RenderPack>& Pack)
    {
        _Packs.erase(_Packs.find(Pack));
    }
}
