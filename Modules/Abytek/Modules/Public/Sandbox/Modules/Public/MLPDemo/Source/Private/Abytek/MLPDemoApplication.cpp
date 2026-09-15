#include "Abytek/MLPDemoApplication.hpp"
#include "Abytek/MLPDemoModule.hpp"
#include "Abytek/Development/Cook/CookSubsystem.hpp"
#include "Abytek/GlobalTime.hpp"
#include "Abytek/Frame/FrameHelper.hpp"


namespace Abytek
{
    ABYTEK_REFLECT(F_MLPDemoApplication)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::F_MLPDemoApplication"));
    }
    
    ABYTEK_DEFINE_OBJECT_SINGLETON_CRT(F_MLPDemoApplication);
    
    F_MLPDemoApplication::F_MLPDemoApplication(const F_ApplicationBuildParams& BuildParams) :
        F_MinimalApplication(BuildParams)
    {
        ABYTEK_BIND_OBJECT_SINGLETON_CRT(); 
    }
    F_MLPDemoApplication::~F_MLPDemoApplication()
    {
    }

    void F_MLPDemoApplication::Init()
    {
        F_MinimalApplication::Init();
        RegisterModule<F_MLPDemoModule>();
    }
    void F_MLPDemoApplication::PostStartup()
    {
        F_MinimalApplication::PostStartup();
        if (!IsCookMode())
        {
            F_MathDebugger2DBuildParams DebuggerBuildParams;
            DebuggerBuildParams.ModuleName = ABYTEK_MODULE_NAME;
            _MathDebugger = TS<F_MathDebugger2D>()(DebuggerBuildParams);
        
            {
                MathDebugger2D::F_Curve Curve;
            
                U32 NumPoints = 32;
                F32 MaxX = 5.0f;
                for (U32 Idx = 0; Idx < NumPoints; ++Idx)
                {
                    F32 X = (MaxX / static_cast<F32>(NumPoints - 1)) * static_cast<F32>(Idx);
                    F32 Y = X * X;
                    Curve.Points.push_back({ X, Y });
                }
            
                _MathDebugger->AddCurve(Curve);
            }
        }
    }
    void F_MLPDemoApplication::Tick()
    {
        F_MinimalApplication::Tick();
        if (!IsCookMode())
        {
            auto MathDebuggerBeginFrameStatus = _MathDebugger->BeginFrame();
            if (MathDebuggerBeginFrameStatus == E_MathDebugger2DBeginFrameStatus::SUCCEEDED)
            {
                ABYTEK_DEFER(this)
                {
                    _MathDebugger->EndFrame();
                };
                ImGui::Begin("MLPDemo");
                ImGui::Text("FPS: %f", 1.0f / F_GlobalTime::GetInstance()->GetLastDeltaSeconds());
                ImGui::End();
            }
            if (MathDebuggerBeginFrameStatus == E_MathDebugger2DBeginFrameStatus::SHUTDOWN)
            {
                SignalShutdown();
                return;
            }
        }
    }
    void F_MLPDemoApplication::PreShutdown()
    {
        if (!IsCookMode())
        {
            _MathDebugger->Shutdown();
            _MathDebugger = {};
        }
        F_MinimalApplication::PreShutdown();
    }
}
