#include "Abytek/MathDebugger2D.hpp"
#include "Abytek/MathRenderer2D.hpp"
#include "Abytek/Window.hpp"
#include "Abytek/WindowManager.hpp"
#include "Abytek/IHIMouse.hpp"
#include "Abytek/IHIMouseManager.hpp"
#include "Abytek/CursorManager.hpp"
#include "Abytek/ImGuiContext.hpp"


namespace Abytek
{
    F_MathDebugger2D::F_MathDebugger2D(const F_MathDebugger2DBuildParams& BuildParams) :
        _ModuleName(BuildParams.ModuleName)
    {
        F_WindowBuildParams WindowBuildParams;
        WindowBuildParams.Title = ABYTEK_TEXT("MathDebugger2D"); 
        _Window = A_WindowManager::GetInstance()->CreateWindow_(WindowBuildParams);
        
        _Window->Events.OnLostFocus.AddListener(
            [this](const F_WindowState& CurrentState, const F_WindowState& NewState)
            {
                _OnWindowLostFocus(CurrentState, NewState);
            }
        );
        _Window->Events.OnFocus.AddListener(
            [this](const F_WindowState& CurrentState, const F_WindowState& NewState)
            {
                _OnWindowFocus(CurrentState, NewState);
            }
        );
        
        auto Mouse = A_IHIMouseManager::GetInstance()->GetMainMouse();
        _OnMouseMoveHandle = Mouse->Events.OnMove.AddListener(
            [this](const F_Vector2_I32& DeltaPosition, const F_IHIMouseState& CurrentState, const F_IHIMouseState& NewState)
            {
                _OnMouseMove(DeltaPosition, CurrentState, NewState);
            }
        );
        _OnMouseWheelHandle = Mouse->Events.OnWheel.AddListener(
            [this](I64 Value, const F_IHIMouseState& CurrentState, const F_IHIMouseState& NewState)
            {
                _OnMouseWheel(Value, CurrentState, NewState);
            }
        );
        _OnMouseButtonDownHandle = Mouse->Events.OnButtonDown.AddListener(
            [this](const F_IHIButton& Button, const F_IHIMouseState& CurrentState, const F_IHIMouseState& NewState)
            {
                _OnMouseButtonDown(Button, CurrentState, NewState);
            }
        );
        _OnMouseButtonUpHandle = Mouse->Events.OnButtonUp.AddListener(
            [this](const F_IHIButton& Button, const F_IHIMouseState& CurrentState, const F_IHIMouseState& NewState)
            {
                _OnMouseButtonUp(Button, CurrentState, NewState);
            }
        );
            
        F_MathRenderer2DBuildParams MathRenderer2DBuildParams;
        MathRenderer2DBuildParams.ModuleName = ABYTEK_MODULE_NAME;
        MathRenderer2DBuildParams.Window = _Window;
        MathRenderer2DBuildParams.WorldContext = BuildParams.WorldContext;
        _MathRenderer2D = TS<F_MathRenderer2D>()(MathRenderer2DBuildParams);
    }
    F_MathDebugger2D::~F_MathDebugger2D()
    {
    }

    void F_MathDebugger2D::Shutdown()
    {
        _MathRenderer2D->Shutdown();
        _MathRenderer2D = {};
        
        auto Mouse = A_IHIMouseManager::GetInstance()->GetMainMouse();
        Mouse->Events.OnButtonUp.RemoveListener(_OnMouseButtonUpHandle);
        Mouse->Events.OnButtonDown.RemoveListener(_OnMouseButtonDownHandle);
        Mouse->Events.OnWheel.RemoveListener(_OnMouseWheelHandle);
        Mouse->Events.OnMove.RemoveListener(_OnMouseMoveHandle);
    }
    E_MathDebugger2DBeginFrameStatus F_MathDebugger2D::BeginFrame()
    {
        if (!_Window)
        {
            return E_MathDebugger2DBeginFrameStatus::SHUTDOWN;
        }
        if (!_MathRenderer2D->BeginFrame())
        {
            return E_MathDebugger2DBeginFrameStatus::NOT_RENDERABLE;
        }
        return E_MathDebugger2DBeginFrameStatus::SUCCEEDED;
    }
    void F_MathDebugger2D::EndFrame()
    {
        for (const auto& Curve : _Scene.Curves)
        {
            U32 NumPoints = static_cast<U32>(Curve.Points.size());
            if (NumPoints == 0)
            {
                continue;
            }
            if (NumPoints == 1)
            {
                MathRenderer2D::F_Line Line;
                Line.BeginPosition = Curve.Points[0];
                Line.EndPosition = Curve.Points[0] + F_Vector2_F32::One() * Curve.Thickness * 0.1f;
                Line.ColorAndThickness = F_Vector4_F32 { Curve.Color, Curve.Thickness };
                _MathRenderer2D->DrawLine(Line);
            }
            else
            {
                for (U32 Idx = 1; Idx < NumPoints; ++Idx)
                {
                    MathRenderer2D::F_Line Line;
                    Line.BeginPosition = Curve.Points[Idx - 1];
                    Line.EndPosition = Curve.Points[Idx];
                    Line.ColorAndThickness = F_Vector4_F32 { Curve.Color, Curve.Thickness };
                    _MathRenderer2D->DrawLine(Line);
                }
            }
        }
        
        if (ImGui::GetIO().WantCaptureMouse)
        {
            _ViewControl = false;
        }
        else
        {
            if (_ViewControl)
            {
                _MathRenderer2D->MoveView(
                    F_Vector2_F32(
                        static_cast<F32>(_DeltaMousePosition.X),
                        -static_cast<F32>(_DeltaMousePosition.Y) 
                    )
                    * MouseMoveSensitivity
                    * _MathRenderer2D->GetFrameData().ViewScale
                );
                _MathRenderer2D->ScaleViewSize(-F32(_DeltaMouseWheel) * MouseWheelSensitivity);
            }

            auto Cursor = A_CursorManager::GetInstance()->GetMainCursor();
            if (Cursor->IsVisible() == _ViewControl)
            {
                Cursor->SetVisibility(!_ViewControl);
                if (_ViewControl)
                {
                    Cursor->Capture(_Window);
                }
                else
                {
                    Cursor->Capture({});
                }
            }
        }
        _DeltaMousePosition = F_Vector2_I32::Zero();
        _DeltaMouseWheel = 0;
        
        _MathRenderer2D->EndFrame();
    }

    void F_MathDebugger2D::ClearScene()
    {
        _Scene = {};
    }
    void F_MathDebugger2D::AddCurve(const MathDebugger2D::F_Curve& Curve)
    {
        _Scene.Curves.push_back(Curve);
    }

    void F_MathDebugger2D::_OnWindowLostFocus(const F_WindowState& CurrentState, const F_WindowState& NewState)
    {
        _ViewControl = false;
    }
    void F_MathDebugger2D::_OnWindowFocus(const F_WindowState& CurrentState, const F_WindowState& NewState)
    {
    }
    void F_MathDebugger2D::_OnMouseWheel(I64 Value, const F_IHIMouseState& CurrentState, const F_IHIMouseState& NewState)
    {
        _DeltaMouseWheel += Value;
    }
    void F_MathDebugger2D::_OnMouseMove(const F_Vector2_I32& DeltaPosition, const F_IHIMouseState& CurrentState, const F_IHIMouseState& NewState)
    {
        _DeltaMousePosition += DeltaPosition;
    }
    void F_MathDebugger2D::_OnMouseButtonDown(const F_IHIButton& Button, const F_IHIMouseState& CurrentState, const F_IHIMouseState& NewState)
    {
        if (Button == F_IHIButton(E_IHIPredefinedButton::MOUSE_LEFT))
        {
            _ViewControl = true;
        }
    }
    void F_MathDebugger2D::_OnMouseButtonUp(const F_IHIButton& Button, const F_IHIMouseState& CurrentState, const F_IHIMouseState& NewState)
    {
        if (Button == F_IHIButton(E_IHIPredefinedButton::MOUSE_LEFT))
        {
            _ViewControl = false;
        }
    }
}
