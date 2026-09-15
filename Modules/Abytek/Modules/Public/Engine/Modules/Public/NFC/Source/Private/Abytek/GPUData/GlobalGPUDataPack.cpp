#include "Abytek/GPUData/GlobalGPUDataPack.hpp"
#include "Abytek/GPUData/Render/GlobalGPUDataPackProxy.hpp"
#include "Abytek/RenderBase/WorldRenderResource.hpp"
#include "Abytek/ApplicationModuleContainer.hpp"
#include "Abytek/Frame/FrameHelper.hpp"


namespace Abytek
{
    ABYTEK_REFLECT(F_GlobalGPUDataPack)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::GlobalGPUDataPack"));
    }

    F_GlobalGPUDataPack::F_GlobalGPUDataPack(const F_SerializableObjectInitParams& InitParams) :
        A_WorldContext(InitParams)
    {
    }
    F_GlobalGPUDataPack::~F_GlobalGPUDataPack()
    {
    }
    
    void F_GlobalGPUDataPack::OnLoad()
    {
        _Populate();
        SetupRenderable();
    }
    void F_GlobalGPUDataPack::OnUnload()
    {
        CleanUpRenderable();
    }
    
    void F_GlobalGPUDataPack::OnCreateRenderState()
    {
        H_Frame::EnqueueCommand<E_FrameParamType::RENDER>(
            [
                RenderProxy = GetRenderProxy().StaticCast<F_GlobalGPUDataPackProxy>(),
                ComponentTypeCreateFunctions = _ComponentTypeCreateFunctions
            ]
            {
                RenderProxy->_ComponentTypeCreateFunctions = ComponentTypeCreateFunctions;
            }
        );
    }
    void F_GlobalGPUDataPack::OnDestroyRenderState()
    {
    }
    TS<A_RenderProxy> F_GlobalGPUDataPack::CreateRenderProxy()
    {
        return TS<F_GlobalGPUDataPackProxy>()(ABYTEK_WTHIS());
    }

    void F_GlobalGPUDataPack::_Populate()
    {
        TF_Vector<TF_ReflectionTypeHandle<I_GlobalGPUDataComponent>> GlobalGPUDataComponentTypes;
        {
            auto BaseSubsystemType = TF_ReflectionTypeHandle<I_GlobalGPUDataComponent>(F_ReflectionContext::GetGlobal());
            ABYTEK_ENGINE_NFC_ASSERT(BaseSubsystemType);
            F_ApplicationModuleContainer::GetInstance()->ForEachUnit(
                [&BaseSubsystemType, &GlobalGPUDataComponentTypes](const TW_Valid<F_ProgramUnit>& Unit)
                {
                    auto Module = Unit.FastCast<F_Module>();
                    auto ReflectionSession = Module->GetReflectionSession();
                    ReflectionSession->ForEachTypeDerivedFrom(
                        BaseSubsystemType,
                        [&GlobalGPUDataComponentTypes](const TW_Valid<F_ReflectionType>& Type)
                        {
                            GlobalGPUDataComponentTypes.push_back(Type);
                            return true;
                        }
                    );
                    return true;
                }
            );
        }
        for (const auto& Type : GlobalGPUDataComponentTypes)
        {
            const auto& Metadata = Type->GetMetadata();
            ABYTEK_ENGINE_NFC_ASSERT(Metadata.HasElement(I_GlobalGPUDataComponent::GetMetadataElementName_CreateFunction())) 
                << "Not found metadata \"" 
                << I_GlobalGPUDataComponent::GetMetadataElementName_CreateFunction()
                << "\" in type: "
                << Type->GetFullName();
            const auto& CreateFunction = AnyCast<I_GlobalGPUDataComponent::F_CreateFunction>(
                Metadata.Get(I_GlobalGPUDataComponent::GetMetadataElementName_CreateFunction())    
            );
            _ComponentTypeCreateFunctions.push_back(CreateFunction);
        }
    }
}
