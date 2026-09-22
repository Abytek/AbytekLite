#include "Abytek/NFCModule.hpp"

#include "Abytek/ActorComponents/RenderableComponentManager.hpp"
#include "Abytek/ActorComponents/RenderableComponent.hpp"
#include "Abytek/ActorComponents/RenderableComponentUpdateRange.hpp"

#include "Abytek/ActorComponents/SceneComponent.hpp"

#include "Abytek/ActorComponents/CanvasComponent.hpp"
#include "Abytek/ActorComponents/CanvasComponentManager.hpp"
#include "Abytek/ActorComponents/Render/CanvasRenderProxy.hpp"

#include "Abytek/ActorComponents/CameraComponent.hpp"
#include "Abytek/ActorComponents/CameraComponentManager.hpp"
#include "Abytek/ActorComponents/PrimitiveComponent.hpp"
#include "Abytek/ActorComponents/InputComponent.hpp"
#include "Abytek/ActorComponents/InputComponentManager.hpp"

#include "Abytek/Renderer/RendererManager.hpp"
#include "Abytek/Renderer/RenderPath.hpp"
#include "Abytek/Renderer/RenderView.hpp"
#include "Abytek/Renderer/RenderSceneUpdateRange.hpp"
#include "Abytek/Renderer/RenderScenePostUpdateRange.hpp"

#include "Abytek/Renderer/GPUData/GPUDataStorage.hpp"

#include "Abytek/Renderer/RenderPrimitive/Components/Component_Transform.hpp"
#include "Abytek/Renderer/RenderPrimitive/Components/Component_InverseTransposeTransform.hpp"
#include "Abytek/Renderer/RenderPrimitive/Components/Component_MeshHandle.hpp"
#include "Abytek/Renderer/RenderPrimitive/RenderPrimitiveManager.hpp"

#include "Abytek/Assets/Texture.hpp"
#include "Abytek/Assets/StaticMesh.hpp"


namespace Abytek
{
    ABYTEK_REFLECT(F_NFCModule)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::F_NFCModule"));
    }
    
    ABYTEK_DEFINE_STATIC_APPLICATION_MODULE(F_NFCModule)
    
    F_NFCModule::F_NFCModule(const F_ProgramUnitBuildParams& BuildParams) :
        A_ApplicationModule(BuildParams)
    {
        ABYTEK_BIND_STATIC_APPLICATION_MODULE();
        
        AddDependency<F_AssetsModule>();
        AddDependency<F_ResourceModule>();
        AddDependency<F_IHIModule>();
        AddDependency<F_WindowModule>();
        AddDependency<F_RHIModule>();
        AddDependency<F_RenderCoreModule>();
        AddDependency<F_MathDebugger2DModule>();
        AddDependency<F_ImGuiModule>();
    }
    F_NFCModule::~F_NFCModule()
    {
    }

    void F_NFCModule::OnReflect()
    {
        RegisterStaticType<F_RenderableComponentManager>();
        RegisterStaticType<A_RenderableComponent>();
        
        RegisterStaticType<F_SceneComponent>();
        
        RegisterStaticType<E_CanvasTopology>();
        RegisterStaticType<E_CanvasPresentationMode>();
        RegisterStaticType<E_CanvasOutputMode>();
        RegisterStaticType<F_CanvasWindowConfig>();
        RegisterStaticType<CanvasRendering::F_ApplyOfflineTextureBinding>();
        RegisterStaticType<CanvasRendering::F_ApplyOfflineTexturePipeline>();
        RegisterStaticType<F_CameraProjectionOptions>();
        RegisterStaticType<F_CanvasComponent>();
        RegisterStaticType<F_CanvasComponentManager>();
        
        RegisterStaticType<E_CameraProjectionMode>();
        RegisterStaticType<F_CameraComponent>();
        RegisterStaticType<F_CameraComponentManager>();
        RegisterStaticType<A_PrimitiveComponent>();
        RegisterStaticType<F_InputComponent>();
        RegisterStaticType<F_InputComponentManager>();
        
        RegisterStaticType<F_RendererManager>();
        RegisterStaticType<A_RenderPath>();
        RegisterStaticType<F_RenderViewUniformDataBinding>();
        RegisterStaticType<RenderGeometry::F_GlobalSRVBinding>();
        RegisterStaticType<RenderGeometry::F_GlobalUAVBinding>();

        RegisterStaticType<RenderPrimitive::F_Component_Transform>();
        RegisterStaticType<RenderPrimitive::F_Component_InverseTransposeTransform>();
        RegisterStaticType<RenderPrimitive::F_Component_MeshHandle>();
        RegisterStaticType<RenderPrimitive::F_DemoPipeline>();
        
        RegisterStaticType<F_Texture>();
        RegisterStaticType<F_TextureSetting>();
        RegisterStaticType<F_StaticMesh>();
        RegisterStaticType<F_StaticMeshSetting>();
    }

    void F_NFCModule::OnInit()
    {
        _RenderSceneUpdateRange = TU<F_RenderSceneUpdateRange>()();
        _RenderScenePostUpdateRange = TU<F_RenderScenePostUpdateRange>()();
        _RenderableComponentUpdateRange = TU<F_RenderableComponentUpdateRange>()();
        A_RenderableComponent::GlobalInit();
        F_CanvasComponent::GlobalInit();
    }
    void F_NFCModule::OnRelease()
    {
        F_CanvasComponent::GlobalRelease();
        A_RenderableComponent::GlobalRelease();
        _RenderableComponentUpdateRange = {};
        _RenderScenePostUpdateRange = {};
        _RenderSceneUpdateRange = {};
    }
}
