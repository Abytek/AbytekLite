#include "Abytek/ActorComponents/CanvasComponent.hpp"
#include "Abytek/ActorComponents/CameraComponent.hpp"
#include "Abytek/ApplicationUpdateFunction.hpp"
#include "Abytek/ActorComponents/CanvasComponentManager.hpp"
#include "Abytek/ActorComponents/Render/CameraComponentRenderProxy.hpp"
#include "Abytek/ActorComponents/Render/CanvasComponentRenderProxy.hpp"
#include "Abytek/Assets/Texture.hpp"
#include "Abytek/Assets/Render/TextureRenderProxy.hpp"
#include "Abytek/WindowManager.hpp"
#include "Abytek/ActorComponents/RenderableComponentUpdateRange.hpp"
#include "Abytek/CoreUpdateGraph/PreShutdownUpdateRange.hpp"
#include "Abytek/UpdateBase/UpdateUtilities.hpp"
#include "Abytek/Renderer/RenderSceneUpdateRange.hpp"
#include "Abytek/Frame/FrameHelper.hpp"
#include "Abytek/World/WorldContextHelper.hpp"
#include "Abytek/CoreUpdateGraph/HighLevelUpdateRange.hpp"
#include "Abytek/Renderer/RenderScenePostUpdateRange.hpp"


namespace Abytek
{
    ABYTEK_REFLECT(F_CanvasWindowConfig)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::F_CanvasSettingWindow"));

        ABYTEK_REFLECT_PROPERTY_SERIALIZABLE(Offset);
        ABYTEK_REFLECT_PROPERTY_SERIALIZABLE(Size);
        
        ABYTEK_REFLECT_PROPERTY_SERIALIZABLE(IsVisible);
        ABYTEK_REFLECT_PROPERTY_SERIALIZABLE(Focus);
        
        ABYTEK_REFLECT_PROPERTY_SERIALIZABLE(SizeMode);
    }
    
    ABYTEK_REFLECT(F_CanvasComponent)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::F_CanvasComponent"));
        
        ABYTEK_REFLECT_PROPERTY_SERIALIZABLE(_Title);
        
        ABYTEK_REFLECT_PROPERTY_SERIALIZABLE(_Format);
        
        ABYTEK_REFLECT_PROPERTY_SERIALIZABLE(_Resolution_Mono);
        ABYTEK_REFLECT_PROPERTY_SERIALIZABLE(_Resolution_StereoLeft);
        ABYTEK_REFLECT_PROPERTY_SERIALIZABLE(_Resolution_StereoRight);
        
        ABYTEK_REFLECT_PROPERTY_SERIALIZABLE(_ClearColor);
        
        ABYTEK_REFLECT_PROPERTY_SERIALIZABLE(_WindowConfig);
        
        ABYTEK_REFLECT_PROPERTY_SERIALIZABLE(_OutputTexture_Mono);
        ABYTEK_REFLECT_PROPERTY_SERIALIZABLE(_OutputTexture_StereoLeft);
        ABYTEK_REFLECT_PROPERTY_SERIALIZABLE(_OutputTexture_StereoRight);
        
        ABYTEK_REFLECT_PROPERTY_SERIALIZABLE(_OutputMode);
        ABYTEK_REFLECT_PROPERTY_SERIALIZABLE(_PresentationMode);
        ABYTEK_REFLECT_PROPERTY_SERIALIZABLE(_Topology);
    }

    F_Name F_CanvasComponent::GetPreDrawUpdateFunctionName()
    {
        return ABYTEK_NAME("Abytek::F_CanvasComponent::PreDraw");
    }
    F_Name F_CanvasComponent::GetDrawUpdateFunctionName()
    {
        return ABYTEK_NAME("Abytek::F_CanvasComponent::Draw");
    }
    F_Name F_CanvasComponent::GetPostDrawUpdateFunctionName()
    {
        return ABYTEK_NAME("Abytek::F_CanvasComponent::PostDraw");
    }
    void F_CanvasComponent::GlobalInit()
    {
        {
            auto UpdateFunction = H_ApplicationUpdateFunction::Register(
                []
                {
                },
                GetPreDrawUpdateFunctionName(),
                E_ApplicationState::TICKING
            );
            UpdateFunction->AddDependency(
                F_RenderScenePostUpdateRange::GetBeginFunctionName()  
            );
            UpdateFunction->AddReverseDependency(
                F_RenderScenePostUpdateRange::GetEndFunctionName()  
            );
        }
        {
            auto UpdateFunction = H_ApplicationUpdateFunction::Register(
                []
                {
                    F_CanvasComponentManager::GetInstance()->DrawCanvasComponents();
                },
                GetDrawUpdateFunctionName(),
                E_ApplicationState::TICKING
            );
            UpdateFunction->AddDependency(
                GetPreDrawUpdateFunctionName()  
            );
            UpdateFunction->AddReverseDependency(
                F_RenderScenePostUpdateRange::GetEndFunctionName()  
            );
        }
        {
            auto UpdateFunction = H_ApplicationUpdateFunction::Register(
                []
                {
                },
                GetPostDrawUpdateFunctionName(),
                E_ApplicationState::TICKING
            );
            UpdateFunction->AddDependency(
                GetDrawUpdateFunctionName()  
            );
            UpdateFunction->AddReverseDependency(
                F_RenderScenePostUpdateRange::GetEndFunctionName()  
            );
        }
    }
    void F_CanvasComponent::GlobalRelease()
    {
        H_UpdateUtilities::UnregisterFunction(
            GetPostDrawUpdateFunctionName()
        );
        H_UpdateUtilities::UnregisterFunction(
            GetDrawUpdateFunctionName()
        );
        H_UpdateUtilities::UnregisterFunction(
            GetPreDrawUpdateFunctionName()
        );
    }

    F_CanvasComponent::F_CanvasComponent(const F_SerializableObjectInitParams& InitParams) :
        A_RenderableComponent(InitParams)
    {
    }
    F_CanvasComponent::~F_CanvasComponent()
    {
    }

    void F_CanvasComponent::OnRegisterComponent()
    {
        A_RenderableComponent::OnRegisterComponent();
        F_CanvasComponentManager::GetInstance()->_RegisterCanvasComponent(ABYTEK_WTHIS());
    }
    void F_CanvasComponent::OnUnregisterComponent()
    {
        F_CanvasComponentManager::GetInstance()->_UnregisterCanvasComponent(ABYTEK_WTHIS());
        A_RenderableComponent::OnUnregisterComponent();
    }

    B8 F_CanvasComponent::OnRequestDestroyWindow()
    {
        return true;
    }
    void F_CanvasComponent::OnDestroyWindow()
    {
    }

    B8 F_CanvasComponent::ShouldDraw() const
    {
        ABYTEK_ENGINE_NFC_ASSERT(CreatedRenderState());
        if (_Window)
        {
            auto ScaledClientSize = _Window->GetScaledClientSize();
            if (
                (ScaledClientSize.X == 0)
                || (ScaledClientSize.Y == 0)
            )
            {
                return false;
            }
        }
        return true;
    }

    B8 F_CanvasComponent::IsRenderable() const
    {
        if (!A_Renderable::IsRenderable())
        {
            return false;
        }
        if (!H_WorldContext::HasFlags(ABYTEK_WTHIS(), E_WorldFlag::ENABLE_RENDERING))
        {
            return false;
        }
        
        // Presentation mode checking
        if (_OutputMode == E_CanvasOutputMode::TEXTURE)
        {
            if (_PresentationMode != E_CanvasPresentationMode::NONE)
            {
                return false;
            }
        }
        else if (_OutputMode == E_CanvasOutputMode::REMOTE)
        {
            if (_PresentationMode == E_CanvasPresentationMode::NONE)
            {
                return false;
            }
        }
        else if (_OutputMode == E_CanvasOutputMode::DISPLAY)
        {
            if (_PresentationMode == E_CanvasPresentationMode::NONE)
            {
                return false;
            }
        }
        
        // Presentation mode checking
        if (_OutputMode == E_CanvasOutputMode::TEXTURE)
        {
            if (_PresentationMode != E_CanvasPresentationMode::NONE)
            {
                return false;
            }
        }
        else if (_OutputMode == E_CanvasOutputMode::REMOTE)
        {
            if (_PresentationMode == E_CanvasPresentationMode::NONE)
            {
                return false;
            }
        }
        else if (_OutputMode == E_CanvasOutputMode::DISPLAY)
        {
            if (_PresentationMode == E_CanvasPresentationMode::NONE)
            {
                return false;
            }
        }
        
        // Topology checking
        if (_Topology == E_CanvasTopology::STEREO)
        {
            if (_PresentationMode != E_CanvasPresentationMode::FULLSCREEN)
            {
                return false;
            }
        }
        
        // Output texture checking
        if (_OutputMode == E_CanvasOutputMode::TEXTURE)
        {
            if (_Topology == E_CanvasTopology::MONO) 
            {
                if (_OutputTexture_Mono)
                {
                    if (!_OutputTexture_Mono->IsRenderable())
                    {
                        return false;
                    }
                }
                else
                {
                    return false;
                }
            }
            if (_Topology == E_CanvasTopology::STEREO) 
            {
                if (_OutputTexture_StereoLeft)
                {
                    if (!_OutputTexture_StereoLeft->IsRenderable())
                    {
                        return false;
                    }
                }
                else
                {
                    return false;
                }
                if (_OutputTexture_StereoRight)
                {
                    if (!_OutputTexture_StereoRight->IsRenderable())
                    {
                        return false;
                    }
                }
                else
                {
                    return false;
                }
            }
        }
        return (
            (_OutputMode != E_CanvasOutputMode::NONE)
            && (_Topology != E_CanvasTopology::NONE)
        );
    }
    void F_CanvasComponent::OnCreateRenderState()
    {
        ABYTEK_ENGINE_NFC_ASSERT(
            (_Format == E_RHIFormat::R8G8B8A8_UNORM)    
        ) << "Invalid format: " << static_cast<U32>(_Format);
        
        H_Frame::EnqueueCommand<E_FrameParamType::RENDER>(
            [
                RenderProxy = GetRenderProxy().FastCast<F_CanvasComponentRenderProxy>(), 
                Format = _Format,
                Resolution_Mono = _Resolution_Mono,
                Resolution_StereoLeft = _Resolution_StereoLeft,
                Resolution_StereoRight = _Resolution_StereoRight,
                ClearColor = _ClearColor
            ]
            {
                RenderProxy->_Format = Format;
                RenderProxy->_Resolution_Mono = Resolution_Mono;
                RenderProxy->_Resolution_StereoLeft = Resolution_StereoLeft;
                RenderProxy->_Resolution_StereoRight = Resolution_StereoRight;
                RenderProxy->_ClearColor = ClearColor;
            }
        );
        
        switch (_PresentationMode)
        {
        case E_CanvasPresentationMode::WINDOW:
            {
                F_WindowBuildParams WindowBuildParams;
                WindowBuildParams.Title = _Title;
                WindowBuildParams.Offset = _WindowConfig.Offset;
                WindowBuildParams.Size = _WindowConfig.Size;
                WindowBuildParams.Focus = _WindowConfig.Focus;
                WindowBuildParams.IsVisible = _WindowConfig.IsVisible;
                _Window = A_WindowManager::GetInstance()->CreateWindow_(WindowBuildParams);
                _Window->Events.OnDestroy.AddListener(
                    [this](const F_WindowState& CurrentState, const F_WindowState& NewState)
                    {
                        F_HighLevelUpdateRange::EnqueueCommand(
                            [this]
                            {
                                OnDestroyWindow();
                                DestroyRenderState();
                                _PresentationMode = E_CanvasPresentationMode::NONE;
                            }
                        );
                    }
                );
                _Window->Events.OnRequestDestroy.AddListener(
                    [this](const F_WindowState& CurrentState, const F_WindowState& NewState, B8& OutApprove)
                    {
                        OutApprove = OnRequestDestroyWindow();
                    }
                );
                _Window->Events.OnResize.AddListener(
                    [this](const F_WindowState& CurrentState, const F_WindowState& NewState)
                    {
                        auto NewScaledClientSize = NewState.GetScaledClientSize();
                        if (
                            (NewScaledClientSize.X == 0)    
                            || (NewScaledClientSize.Y == 0)    
                        )
                        {
                            return;
                        }
                        auto RenderProxy = GetRenderProxy().FastCast<F_CanvasComponentRenderProxy>();
                        H_Frame::EnqueueCommand<E_FrameParamType::RENDER>(
                            [
                                RenderProxy, 
                                NewScaledClientSize
                            ]
                            {
                                RenderProxy->ResizeRHIViewport_Window_RenderTask(H_RHI::GetMainSubmissionQueue(), NewScaledClientSize);
                            }
                        );
                    }
                );
            }
            break;
        case E_CanvasPresentationMode::FULLSCREEN:
            ABYTEK_LOG_FATAL() << "Not supported fullscreen mode";
            break;
        default:
            ABYTEK_LOG_FATAL() << "Invalid presentation mode: " << static_cast<U32>(_PresentationMode);
        }

        switch (_OutputMode)
        {
        case E_CanvasOutputMode::DISPLAY:
        case E_CanvasOutputMode::TEXTURE:
            {
                switch (_Topology)
                {
                case E_CanvasTopology::MONO:
                    {
                        _RTTexture_Mono = H_WorldContext::CreateObject<F_Texture>(
                            ABYTEK_WTHIS(), 
                            *GetName() + ABYTEK_TEXT(".RTTextureMono")
                        );
                        
                        F_RHIImageSettingMinimal ImageSettingMinimal;
                        ImageSettingMinimal.Width = _Resolution_Mono.X;
                        ImageSettingMinimal.Height = _Resolution_Mono.Y;
                        ImageSettingMinimal.DimensionCount = 2;
                
                        F_TextureSetting TextureSetting;
                        TextureSetting.EnableSRV = true;
                        TextureSetting.EnableUAV = true;
                        TextureSetting.EnableRTV = true;
                        
                        _RTTexture_Mono->Import(
                            false,
                            _Format,
                            ImageSettingMinimal,
                            TextureSetting
                        );
                    }
                    break;
                case E_CanvasTopology::STEREO:
                    {
                        {
                            _RTTexture_StereoLeft = H_WorldContext::CreateObject<F_Texture>(
                                ABYTEK_WTHIS(), 
                                *GetName() + ABYTEK_TEXT(".RTTextureStereoLeft")
                            );
                        
                            F_RHIImageSettingMinimal ImageSettingMinimal;
                            ImageSettingMinimal.Width = _Resolution_StereoLeft.X;
                            ImageSettingMinimal.Height = _Resolution_StereoLeft.Y;
                            ImageSettingMinimal.DimensionCount = 2;
                
                            F_TextureSetting TextureSetting;
                            TextureSetting.EnableSRV = true;
                            TextureSetting.EnableUAV = true;
                            TextureSetting.EnableRTV = true;
                            
                            _RTTexture_StereoLeft->Import(
                                false,
                                _Format,
                                ImageSettingMinimal,
                                TextureSetting
                            );
                        }
                        
                        {
                            _RTTexture_StereoRight = H_WorldContext::CreateObject<F_Texture>(
                                ABYTEK_WTHIS(), 
                                *GetName() + ABYTEK_TEXT(".RTTextureStereoRight")
                            );
                        
                            F_RHIImageSettingMinimal ImageSettingMinimal;
                            ImageSettingMinimal.Width = _Resolution_StereoRight.X;
                            ImageSettingMinimal.Height = _Resolution_StereoRight.Y;
                            ImageSettingMinimal.DimensionCount = 2;
                
                            F_TextureSetting TextureSetting;
                            TextureSetting.EnableSRV = true;
                            TextureSetting.EnableUAV = true;
                            TextureSetting.EnableRTV = true;
                            
                            _RTTexture_StereoRight->Import(
                                false,
                                _Format,
                                ImageSettingMinimal,
                                TextureSetting
                            );
                        }
                    }
                    break;
                default:
                    ABYTEK_LOG_FATAL() << "Invalid topology: " << static_cast<U32>(_Topology);
                    break;
                }
            }
            break;
        case E_CanvasOutputMode::REMOTE:
            break;
        default:
            ABYTEK_LOG_FATAL() << "Invalid output mode: " << static_cast<U32>(_OutputMode);
            break;
        }
        
        if (_Window)
        {
            H_Frame::EnqueueCommand<E_FrameParamType::RENDER>(
                [
                    RenderProxy = GetRenderProxy().FastCast<F_CanvasComponentRenderProxy>(), 
                    Window = _Window
                ]
                {
                    RenderProxy->_Window = Window;
                }
            );
            H_Frame::EnqueueCommand<E_FrameParamType::RENDER>(
                [
                    RenderProxy = GetRenderProxy().FastCast<F_CanvasComponentRenderProxy>(), 
                    ScaledClientSize = _Window->GetScaledClientSize()
                ]
                {
                    RenderProxy->CreateRHIViewport_Window_RenderTask(H_RHI::GetMainSubmissionQueue(), ScaledClientSize);
                }
            );
        }
        
        if (_OutputTexture_Mono)
        {
            H_Frame::EnqueueCommand<E_FrameParamType::RENDER>(
                [
                    RenderProxy = GetRenderProxy().FastCast<F_CanvasComponentRenderProxy>(), 
                    TextureRenderProxy = _OutputTexture_Mono->GetRenderProxy().FastCast<F_TextureRenderProxy>()
                ]
                {
                    RenderProxy->_OutputTextureRenderProxy_Mono = TextureRenderProxy;
                }
            );
        }
        if (_OutputTexture_StereoLeft)
        {
            H_Frame::EnqueueCommand<E_FrameParamType::RENDER>(
                [
                    RenderProxy = GetRenderProxy().FastCast<F_CanvasComponentRenderProxy>(), 
                    TextureRenderProxy = _OutputTexture_StereoLeft->GetRenderProxy().FastCast<F_TextureRenderProxy>()
                ]
                {
                    RenderProxy->_OutputTextureRenderProxy_Mono = TextureRenderProxy;
                }
            );
        }
        if (_OutputTexture_StereoRight)
        {
            H_Frame::EnqueueCommand<E_FrameParamType::RENDER>(
                [
                    RenderProxy = GetRenderProxy().FastCast<F_CanvasComponentRenderProxy>(), 
                    TextureRenderProxy = _OutputTexture_StereoRight->GetRenderProxy().FastCast<F_TextureRenderProxy>()
                ]
                {
                    RenderProxy->_OutputTextureRenderProxy_Mono = TextureRenderProxy;
                }
            );
        }
        
        H_Frame::EnqueueCommand<E_FrameParamType::RENDER>(
            [
                RenderProxy = GetRenderProxy().FastCast<F_CanvasComponentRenderProxy>(), 
                OutputMode = _OutputMode,
                PresentationMode = _PresentationMode,
                Topology = _Topology
            ]
            {
                RenderProxy->_OutputMode = OutputMode;
                RenderProxy->_PresentationMode = PresentationMode;
                RenderProxy->_Topology = Topology;
            }
        );
        
        if (_RTTexture_Mono)
        {
            H_Frame::EnqueueCommand<E_FrameParamType::RENDER>(
                [
                    RenderProxy = GetRenderProxy().FastCast<F_CanvasComponentRenderProxy>(), 
                    TextureRenderProxy = _RTTexture_Mono->GetRenderProxy().FastCast<F_TextureRenderProxy>()
                ]
                {
                    RenderProxy->_RTTextureRenderProxy_Mono = TextureRenderProxy;
                }
            );
        }
        if (_RTTexture_StereoLeft)
        {
            H_Frame::EnqueueCommand<E_FrameParamType::RENDER>(
                [
                    RenderProxy = GetRenderProxy().FastCast<F_CanvasComponentRenderProxy>(), 
                    TextureRenderProxy = _RTTexture_StereoLeft->GetRenderProxy().FastCast<F_TextureRenderProxy>()
                ]
                {
                    RenderProxy->_RTTextureRenderProxy_StereoLeft = TextureRenderProxy;
                }
            );
        }
        if (_RTTexture_StereoRight)
        {
            H_Frame::EnqueueCommand<E_FrameParamType::RENDER>(
                [
                    RenderProxy = GetRenderProxy().FastCast<F_CanvasComponentRenderProxy>(), 
                    TextureRenderProxy = _RTTexture_StereoRight->GetRenderProxy().FastCast<F_TextureRenderProxy>()
                ]
                {
                    RenderProxy->_RTTextureRenderProxy_StereoRight = TextureRenderProxy;
                }
            );
        }
        
        if (_CameraComponent)
        {
            H_Frame::EnqueueCommand<E_FrameParamType::RENDER>(
                [
                    RenderProxy = GetRenderProxy().FastCast<F_CanvasComponentRenderProxy>(),
                    CameraComponentRenderProxy = _CameraComponent->GetRenderProxy().FastCast<F_CameraComponentRenderProxy>()
                ]
                {
                    RenderProxy->_CameraComponentRenderProxy = CameraComponentRenderProxy;
                }
            );
            
            if (!_CameraComponent->CreatedRenderState())
            {
                _CameraComponent->CreateRenderState();
            }
        }
    }
    void F_CanvasComponent::OnDestroyRenderState()
    {
        if (_CameraComponent)
        {
            _CameraComponent->DestroyRenderState();
        }
        
        _RTTexture_StereoRight = {};
        _RTTexture_StereoLeft = {};
        _RTTexture_Mono = {};
        if (_Window)
        {
            H_Frame::EnqueueCommand<E_FrameParamType::FINAL>(
                [Window = _Window]
                {
                    A_WindowManager::GetInstance()->DestroyWindow(Window);
                }
            );
        }
    }
    TS<A_RenderProxy> F_CanvasComponent::CreateRenderProxy()
    {
        return TS<F_CanvasComponentRenderProxy>()(ABYTEK_WTHIS());
    }

    void F_CanvasComponent::SetTitle(const F_Text& Value)
    {
        _Title = Value;
        if (_Window)
        {
            _Window->SetTitle(Value);
        }
    }

    void F_CanvasComponent::SetResolution_Mono(const F_Vector2_U32& Value)
    {
        _Resolution_Mono = Value;
        MarkRenderStateDirty();
    }
    void F_CanvasComponent::SetResolution_StereoLeft(const F_Vector2_U32& Value)
    {
        _Resolution_StereoLeft = Value;
        MarkRenderStateDirty();
    }
    void F_CanvasComponent::SetResolution_StereoRight(const F_Vector2_U32& Value)
    {
        _Resolution_StereoRight = Value;
        MarkRenderStateDirty();
    }

    void F_CanvasComponent::SetClearColor(const F_Vector4_F32& Value)
    {
        _ClearColor = Value;
        if (auto RenderProxy = GetRenderProxy())
        {
            H_Frame::EnqueueCommand<E_FrameParamType::RENDER>(
                [CachedRenderProxy = RenderProxy.FastCast<F_CanvasComponentRenderProxy>(), ClearColor = _ClearColor]
                {
                    CachedRenderProxy->_ClearColor = ClearColor;
                }
            );
        }
    }

    void F_CanvasComponent::SetFormat(E_RHIFormat Value)
    {
        _Format = Value;
        MarkRenderStateDirty();
    }

    void F_CanvasComponent::SetWindowConfig(const F_CanvasWindowConfig& Value)
    {
        _WindowConfig = Value;
        MarkRenderStateDirty();
    }

    void F_CanvasComponent::SetOutputTexture_Mono(const TS<F_Texture>& Value)
    {
        _OutputTexture_Mono = Value;
        MarkRenderStateDirty();
    }
    void F_CanvasComponent::SetOutputTexture_StereoLeft(const TS<F_Texture>& Value)
    {
        _OutputTexture_StereoLeft = Value;
        MarkRenderStateDirty();
    }
    void F_CanvasComponent::SetOutputTexture_StereoRight(const TS<F_Texture>& Value)
    {
        _OutputTexture_StereoRight = Value;
        MarkRenderStateDirty();
    }

    void F_CanvasComponent::SetOutputMode(E_CanvasOutputMode Value)
    {
        _OutputMode = Value;
        MarkRenderStateDirty();
    }
    void F_CanvasComponent::SetPresentationMode(E_CanvasPresentationMode Value)
    {
        _PresentationMode = Value;
        MarkRenderStateDirty();
    }
    void F_CanvasComponent::SetTopology(E_CanvasTopology Value)
    {
        _Topology = Value;
        MarkRenderStateDirty();
    }

    void F_CanvasComponent::OnDraw()
    {
        if (_CameraComponent)
        {
            _CameraComponent->_Draw();
        }
    }

    void F_CanvasComponent::Draw()
    {
        if (!IsRenderable())
        {
            return;
        }
        
        if (_PresentationMode == E_CanvasPresentationMode::WINDOW)
        {
            ABYTEK_ENGINE_NFC_ASSERT(_Window);
            auto ScaledClientSize = _Window->GetScaledClientSize();
            if (
                (ScaledClientSize.X == 0)    
                || (ScaledClientSize.Y == 0)    
            )
            {
                return;
            }
        }
        
        H_Frame::EnqueueCommand<E_FrameParamType::RENDER>(
            [CachedRenderProxy = GetRenderProxy().FastCast<F_CanvasComponentRenderProxy>()]
            {
                CachedRenderProxy->BeginDraw_RenderTask(H_RHI::GetMainSubmissionQueue());
            }
        );
        OnDraw();
        H_Frame::EnqueueCommand<E_FrameParamType::RENDER>(
            [CachedRenderProxy = GetRenderProxy().FastCast<F_CanvasComponentRenderProxy>()]
            {
                CachedRenderProxy->EndDraw_RenderTask(H_RHI::GetMainSubmissionQueue());
            }
        );
    }

    void F_CanvasComponent::OnBindCameraComponent()
    {
    }
    void F_CanvasComponent::OnUnbindCameraComponent()
    {
    }

    void F_CanvasComponent::_BindCameraComponent(const TW_Valid<F_CameraComponent>& CameraComponent)
    {
        _CameraComponent = CameraComponent;
        OnBindCameraComponent();
    }
    void F_CanvasComponent::_UnbindCameraComponent()
    {
        OnUnbindCameraComponent();
        _CameraComponent = {};
    }
}
