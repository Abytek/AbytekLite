#pragma once

#include "Abytek/Engine.MathDebugger2D.prerequisites.hpp"
#include "Abytek/IHIMouseCommon.hpp"
#include "Abytek/WindowCommon.hpp"


namespace Abytek
{
    class A_WorldContext;
    class F_MathRenderer2D;
    
    namespace MathDebugger2D
    {
        struct F_Curve
        {
            TF_Vector<F_Vector2_F32> Points;
            F_Vector3_F32 Color = F_Vector3_F32 { 0.0f, 0.5f, 1.0f };
            F32 Thickness = 0.004f;
        };
        struct F_Scene
        {
            TF_Vector<F_Curve> Curves;
        };
    }
    
    struct F_MathDebugger2DBuildParams
    {
        F_Name ModuleName;
        TW<A_WorldContext> WorldContext;
    };
    
    enum class E_MathDebugger2DBeginFrameStatus : U8
    {
        NONE,
        SUCCEEDED,
        NOT_RENDERABLE,
        SHUTDOWN
    };
    
    class ABYTEK_ENGINE_MATH_DEBUGGER_2D_API F_MathDebugger2D : public A_Object
    {
    private:
        F_Name _ModuleName;
        TW<A_Window> _Window;
        TS<F_MathRenderer2D> _MathRenderer2D;
        
        IHIMouseEvents::F_OnMove::F_ListenerHandle _OnMouseMoveHandle;
        IHIMouseEvents::F_OnWheel::F_ListenerHandle _OnMouseWheelHandle;
        IHIMouseEvents::F_OnButtonDown::F_ListenerHandle _OnMouseButtonDownHandle;
        IHIMouseEvents::F_OnButtonUp::F_ListenerHandle _OnMouseButtonUpHandle;
        
        B8 _ViewControl = false;
        F32 MouseMoveSensitivity = 0.01f;
        F32 MouseWheelSensitivity = 0.001f;
        
        F_Vector2_I32 _DeltaMousePosition = F_Vector2_I32::Zero();
        I64 _DeltaMouseWheel = 0;
        
        MathDebugger2D::F_Scene _Scene;

    public:
        ABYTEK_FORCE_INLINE const auto& GetModuleName() const noexcept
        {
            return _ModuleName;
        }
        ABYTEK_FORCE_INLINE const auto& GetWindow() const noexcept
        {
            return _Window;
        }
        ABYTEK_FORCE_INLINE const auto& GetMathRenderer2D() const noexcept
        {
            return _MathRenderer2D;
        }
        
        ABYTEK_FORCE_INLINE const auto& GetScene() const noexcept
        {
            return _Scene;
        }
        
    public:
        F_MathDebugger2D(const F_MathDebugger2DBuildParams& BuildParams);
        virtual ~F_MathDebugger2D();
        
    public:
        void Shutdown();
        E_MathDebugger2DBeginFrameStatus BeginFrame();
        void EndFrame();
        
    public:
        void ClearScene();
        void AddCurve(const MathDebugger2D::F_Curve& Curve);
        
    private:
        void _OnWindowLostFocus(const F_WindowState& CurrentState, const F_WindowState& NewState);
        void _OnWindowFocus(const F_WindowState& CurrentState, const F_WindowState& NewState);
        void _OnMouseWheel(I64 Value, const F_IHIMouseState& CurrentState, const F_IHIMouseState& NewState);
        void _OnMouseMove(const F_Vector2_I32& DeltaPosition, const F_IHIMouseState& CurrentState, const F_IHIMouseState& NewState);
        void _OnMouseButtonDown(const F_IHIButton& Button, const F_IHIMouseState& CurrentState, const F_IHIMouseState& NewState);
        void _OnMouseButtonUp(const F_IHIButton& Button, const F_IHIMouseState& CurrentState, const F_IHIMouseState& NewState);
    };
}