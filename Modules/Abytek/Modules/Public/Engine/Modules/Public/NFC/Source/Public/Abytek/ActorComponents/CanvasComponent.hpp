#pragma once

#include "Abytek/ActorComponents/RenderableComponent.hpp"
#include "Abytek/Renderable.hpp"
#include "Abytek/Window.hpp"


namespace Abytek
{
    class F_Texture;
    class F_CameraComponent;
    
    enum class E_CanvasTopology : U8
    {
        NONE,
        MONO,
        STEREO,
        DEFAULT = MONO
    };
    enum class E_CanvasPresentationMode : U8
    {
        NONE,
        WINDOW,
        FULLSCREEN,
        DEFAULT = WINDOW
    };
    enum class E_CanvasOutputMode : U8
    {
        NONE,
        DISPLAY,
        REMOTE,
        TEXTURE,
        DEFAULT = DISPLAY
    };
    
    ABYTEK_ENUM_REFLECTOR_LOCALNS(E_CanvasTopology, I_Serializable)
    {
        ABYTEK_REFLECT_ENUM_VALUE(NONE);
        ABYTEK_REFLECT_ENUM_VALUE(MONO);
        ABYTEK_REFLECT_ENUM_VALUE(STEREO);
        ABYTEK_REFLECT_ENUM_VALUE(DEFAULT);
    }
    ABYTEK_ENUM_REFLECTOR_LOCALNS(E_CanvasPresentationMode, I_Serializable)
    {
        ABYTEK_REFLECT_ENUM_VALUE(NONE);
        ABYTEK_REFLECT_ENUM_VALUE(WINDOW);
        ABYTEK_REFLECT_ENUM_VALUE(FULLSCREEN);
        ABYTEK_REFLECT_ENUM_VALUE(DEFAULT);
    }
    ABYTEK_ENUM_REFLECTOR_LOCALNS(E_CanvasOutputMode, I_Serializable)
    {
        ABYTEK_REFLECT_ENUM_VALUE(NONE);
        ABYTEK_REFLECT_ENUM_VALUE(DISPLAY);
        ABYTEK_REFLECT_ENUM_VALUE(REMOTE);
        ABYTEK_REFLECT_ENUM_VALUE(TEXTURE);
        ABYTEK_REFLECT_ENUM_VALUE(DEFAULT);
    }
    
    struct ABYTEK_ENGINE_NFC_API F_CanvasWindowConfig
    {
        ABYTEK_BEGIN_REFLECTOR(I_Serializable)
        ABYTEK_END_REFLECTOR(F_CanvasWindowConfig)
        
        F_Vector2_I32 Offset = F_Vector2_I32::Zero();
        F_Vector2_U32 Size = F_Vector2_U32 { 1920, 1080 };

        B8 IsVisible = true;
        B8 Focus = true;

        E_WindowSizeMode SizeMode = E_WindowSizeMode::DEFAULT;
        
        friend B8 operator == (const F_CanvasWindowConfig& A, const F_CanvasWindowConfig& B)
        {
            return (
                (A.Offset == B.Offset)    
                && (A.Size == B.Size)    
                
                && (A.IsVisible == B.IsVisible)    
                && (A.Focus == B.Focus)    
            );
        }
        friend B8 operator != (const F_CanvasWindowConfig& A, const F_CanvasWindowConfig& B)
        {
            return !(A == B);
        }
    };
    
    class ABYTEK_ENGINE_NFC_API F_CanvasComponent : public A_RenderableComponent
    {
    public:
        ABYTEK_BEGIN_REFLECTOR(A_RenderableComponent)
        ABYTEK_END_REFLECTOR(F_CanvasComponent)
        
    public:
        friend class F_CameraComponent;
        
    public:
        static F_Name GetPreDrawUpdateFunctionName();
        static F_Name GetDrawUpdateFunctionName();
        static F_Name GetPostDrawUpdateFunctionName();
        static void GlobalInit();
        static void GlobalRelease();
        
    private:
        F_Text _Title;
        
        F_Vector2_U32 _Resolution_Mono = { 1920, 1080 };
        F_Vector2_U32 _Resolution_StereoLeft = { 1920, 1080 };
        F_Vector2_U32 _Resolution_StereoRight = { 1920, 1080 };
        
        F_Vector4_F32 _ClearColor = F_Vector4_F32::Zero();
        
        E_RHIFormat _Format = E_RHIFormat::R8G8B8A8_UNORM;
        
        F_CanvasWindowConfig _WindowConfig;
        
        TS<F_Texture> _OutputTexture_Mono;
        TS<F_Texture> _OutputTexture_StereoLeft;
        TS<F_Texture> _OutputTexture_StereoRight;
        
        E_CanvasOutputMode _OutputMode = E_CanvasOutputMode::DEFAULT;
        E_CanvasPresentationMode _PresentationMode = E_CanvasPresentationMode::DEFAULT;
        E_CanvasTopology _Topology = E_CanvasTopology::DEFAULT;
        
        TW<A_Window> _Window;
        TS<F_Texture> _RTTexture_Mono;
        TS<F_Texture> _RTTexture_StereoLeft;
        TS<F_Texture> _RTTexture_StereoRight;
        
        TW<F_CameraComponent> _CameraComponent;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetTitle() const noexcept
        {
            return _Title;
        }
        
        ABYTEK_FORCE_INLINE const auto& GetResolution_Mono() const noexcept
        {
            return _Resolution_Mono;
        }
        ABYTEK_FORCE_INLINE const auto& GetResolution_StereoLeft() const noexcept
        {
            return _Resolution_StereoLeft;
        }
        ABYTEK_FORCE_INLINE const auto& GetResolution_StereoRight() const noexcept
        {
            return _Resolution_StereoRight;
        }
        
        ABYTEK_FORCE_INLINE const auto& GetClearColor() const noexcept
        {
            return _ClearColor;
        }
        
        ABYTEK_FORCE_INLINE const auto& GetFormat() const noexcept
        {
            return _Format;
        }
        
        ABYTEK_FORCE_INLINE const auto& GetWindowConfig() const noexcept
        {
            return _WindowConfig;
        }
        
        ABYTEK_FORCE_INLINE const auto& GetOutputTexture_Mono() const noexcept
        {
            return _OutputTexture_Mono;
        }
        ABYTEK_FORCE_INLINE const auto& GetOutputTexture_StereoLeft() const noexcept
        {
            return _OutputTexture_StereoLeft;
        }
        ABYTEK_FORCE_INLINE const auto& GetOutputTexture_StereoRight() const noexcept
        {
            return _OutputTexture_StereoRight;
        }
        
        ABYTEK_FORCE_INLINE const auto& GetOutputMode() const noexcept
        {
            return _OutputMode;
        }
        ABYTEK_FORCE_INLINE const auto& GetPresentationMode() const noexcept
        {
            return _PresentationMode;
        }
        ABYTEK_FORCE_INLINE const auto& GetTopology() const noexcept
        {
            return _Topology;
        }
        
        ABYTEK_FORCE_INLINE const auto& GetWindow() const noexcept
        {
            return _Window;
        }
        ABYTEK_FORCE_INLINE const auto& GetRTTexture_Mono() const noexcept
        {
            return _RTTexture_Mono;
        }
        ABYTEK_FORCE_INLINE const auto& GetRTTexture_StereoLeft() const noexcept
        {
            return _RTTexture_StereoLeft;
        }
        ABYTEK_FORCE_INLINE const auto& GetRTTexture_StereoRight() const noexcept
        {
            return _RTTexture_StereoRight;
        }
        
        ABYTEK_FORCE_INLINE const auto& GetCameraComponent() const noexcept
        {
            return _CameraComponent;
        }
        
    public:
        F_CanvasComponent(const F_SerializableObjectInitParams& InitParams);
        ~F_CanvasComponent() override;
        
    protected:
        void OnRegisterComponent() override;
        void OnUnregisterComponent() override;
        
    protected:
        virtual B8 OnRequestDestroyWindow();
        virtual void OnDestroyWindow();
        virtual B8 ShouldDraw() const;
        
    public:
        B8 IsRenderable() const override;
        
    protected:
        void OnCreateRenderState() override;
        void OnDestroyRenderState() override;
        TS<A_RenderProxy> CreateRenderProxy() override;
        
    public:
        void SetTitle(const F_Text& Value);
        
        void SetResolution_Mono(const F_Vector2_U32& Value);
        void SetResolution_StereoLeft(const F_Vector2_U32& Value);
        void SetResolution_StereoRight(const F_Vector2_U32& Value);
        
        void SetClearColor(const F_Vector4_F32& Value);
        
        void SetFormat(E_RHIFormat Value);
        
        void SetWindowConfig(const F_CanvasWindowConfig& Value);
        
        void SetOutputTexture_Mono(const TS<F_Texture>& Value);
        void SetOutputTexture_StereoLeft(const TS<F_Texture>& Value);
        void SetOutputTexture_StereoRight(const TS<F_Texture>& Value);
        
        void SetOutputMode(E_CanvasOutputMode Value);
        void SetPresentationMode(E_CanvasPresentationMode Value);
        void SetTopology(E_CanvasTopology Value);
        
    protected:
        virtual void OnDraw();
        
    public:
        void Draw();
        
    protected:
        virtual void OnBindCameraComponent();
        virtual void OnUnbindCameraComponent();
        
    private:
        void _BindCameraComponent(const TW_Valid<F_CameraComponent>& CameraComponent);
        void _UnbindCameraComponent();
    };
}