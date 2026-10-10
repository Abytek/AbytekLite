#include "Abytek/NFCModule.hpp"

#include "Abytek/ActorComponents/RenderableComponentManager.hpp"
#include "Abytek/ActorComponents/RenderableComponent.hpp"
#include "Abytek/ActorComponents/RenderableComponentUpdateRange.hpp"

#include "Abytek/ActorComponents/SceneComponent.hpp"

#include "Abytek/ActorComponents/CanvasComponent.hpp"
#include "Abytek/ActorComponents/CanvasComponentManager.hpp"
#include "Abytek/ActorComponents/Render/CanvasComponentRenderProxy.hpp"

#include "Abytek/ActorComponents/CameraComponent.hpp"
#include "Abytek/ActorComponents/CameraComponentManager.hpp"
#include "Abytek/ActorComponents/PrimitiveComponent.hpp"
#include "Abytek/ActorComponents/InputComponent.hpp"
#include "Abytek/ActorComponents/InputComponentManager.hpp"
#include "Abytek/ActorComponents/StaticMeshComponent.hpp"

#include "Abytek/Renderer/RendererManager.hpp"
#include "Abytek/Renderer/RenderPath.hpp"
#include "Abytek/Renderer/RenderView.hpp"
#include "Abytek/Renderer/RenderSceneUpdateRange.hpp"
#include "Abytek/Renderer/RenderScenePostUpdateRange.hpp"

#include "Abytek/Renderer/GPUData/GPUDataStorage.hpp"

#include "Abytek/Renderer/RenderGeometry/RenderGeometryStorage.hpp"

#include "Abytek/Renderer/RenderPrimitive/Archetypes/Data_Simple.hpp"
#include "Abytek/Renderer/RenderPrimitive/RenderPrimitiveManager.hpp"

#include "Abytek/Renderer/StandardPrimitive/GeometryFactoryType.hpp"
#include "Abytek/Renderer/StandardPrimitive/GeometryFactoryTypeManager.hpp"
#include "Abytek/Renderer/StandardPrimitive/GeometryFactoryType_StaticMeshECMS.hpp"
#include "Abytek/Renderer/StandardPrimitive/RenderPrimitiveData.hpp"

#include "Abytek/Assets/Texture.hpp"
#include "Abytek/Assets/StaticMesh.hpp"
#include "Abytek/Assets/MaterialInterface.hpp"
#include "Abytek/Assets/Material.hpp"
#include "Abytek/Assets/MaterialCommon.hpp"
#include "Abytek/Assets/MaterialInstance.hpp"
#include "Abytek/Assets/MaterialProperty.hpp"
#include "Abytek/Assets/StandardMaterialCommon.hpp"
#include "Abytek/Assets/StandardMaterial.hpp"
#include "Abytek/Renderer/StandardPrimitive/MaterialTargetType.hpp"


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
        RegisterStaticType<F_PrimitiveSceneComponent>();
        RegisterStaticType<A_PrimitiveComponent>();
        RegisterStaticType<F_InputComponent>();
        RegisterStaticType<F_InputComponentManager>();
        
        RegisterStaticType<F_StaticMeshComponent>();
        
        RegisterStaticType<F_WorldRenderResourceOwner>();
        RegisterStaticType<F_RendererManager>();
        RegisterStaticType<A_RenderPath>();
        RegisterStaticType<F_RenderViewUniformDataBinding>();
        RegisterStaticType<RenderGeometry::F_GlobalSRVBinding>();
        RegisterStaticType<RenderGeometry::F_GlobalUAVBinding>();

        RegisterStaticType<RenderPrimitive::F_Data_Simple>();
        
        RegisterStaticType<A_GeometryFactoryType>();
        RegisterStaticType<F_GeometryFactoryTypeManager>();
        RegisterStaticType<A_GeometryFactoryType_StaticMeshECMS>();
        RegisterStaticType<A_MaterialTargetType>();
        RegisterStaticType<F_MaterialTargetTypeManager>();
        RegisterStaticType<RenderPrimitive::F_Data_Standard>();
        
        RegisterStaticType<F_Texture>();
        RegisterStaticType<F_TextureSetting>();
        RegisterStaticType<F_StaticMesh>();
        RegisterStaticType<F_StaticMeshSetting>();
        RegisterStaticType<A_MaterialPropertyBase>();
        RegisterStaticType<TF_MaterialPropertyScalar<B8>>();
        RegisterStaticType<TF_MaterialPropertyScalar<U32>>();
        RegisterStaticType<TF_MaterialPropertyScalar<I32>>();
        RegisterStaticType<TF_MaterialPropertyScalar<F32>>();
        RegisterStaticType<F_MaterialPropertyTexture>();
        RegisterStaticType<F_MaterialPropertyList>();
        RegisterStaticType<A_MaterialPropertyInstanceBase>();
        RegisterStaticType<TF_MaterialPropertyInstanceScalar<B8>>();
        RegisterStaticType<TF_MaterialPropertyInstanceScalar<U32>>();
        RegisterStaticType<TF_MaterialPropertyInstanceScalar<I32>>();
        RegisterStaticType<TF_MaterialPropertyInstanceScalar<F32>>();
        RegisterStaticType<F_MaterialPropertyInstanceTexture>();
        RegisterStaticType<F_MaterialPropertyInstanceList>();
        RegisterStaticType<A_MaterialInterface>();
        RegisterStaticType<F_MaterialRenderPack>();
        RegisterStaticType<A_Material>();
        RegisterStaticType<F_MaterialInstance>();
        RegisterStaticType<E_MaterialShaderSourceType>();
        RegisterStaticType<F_MaterialShaderSource>();
        RegisterStaticType<F_MaterialShaderSource_Slang>();
        RegisterStaticType<E_StandardMaterialDomain>();
        RegisterStaticType<E_StandardMaterialShadingModel>();
        RegisterStaticType<F_StandardMaterialShaderParameters>();
        RegisterStaticType<F_StandardMaterial>();
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
