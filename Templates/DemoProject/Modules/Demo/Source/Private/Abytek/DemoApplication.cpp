#include "Abytek/DemoApplication.hpp"
#include "Abytek/ApplicationProgramScope.hpp"
#include "Abytek/DemoProgramModule.hpp"


namespace Abytek
{
    ABYTEK_DEFINE_OBJECT_SINGLETON_CRT(F_DemoFrameProcessor);
    ABYTEK_REFLECT(F_DemoFrameProcessor)
    {
    }
    F_DemoFrameProcessor::F_DemoFrameProcessor()
    {
    }
    F_DemoFrameProcessor::~F_DemoFrameProcessor()
    {
    }
 
    void F_DemoFrameProcessor::Init_Main(const TS_Valid<F_Frame>& CurrentFrame)
    {
        A_FrameProcessor::Init_Main(CurrentFrame);
        
        F_MathDebugger2DBuildParams DebuggerBuildParams;
        DebuggerBuildParams.ModuleName = ABYTEK_MODULE_NAME;
        DebuggerBuildParams.FrameProcessor = ABYTEK_WTHIS();
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
    void F_DemoFrameProcessor::Tick_Main(const TS_Valid<F_Frame>& CurrentFrame)
    {
        A_FrameProcessor::Tick_Main(CurrentFrame);
        
        auto MathDebuggerBeginFrameStatus = _MathDebugger->BeginFrame(CurrentFrame);
        if (MathDebuggerBeginFrameStatus == E_MathDebugger2DBeginFrameStatus::SUCCEEDED)
        {
            ABYTEK_DEFER(this)
            {
                _MathDebugger->EndFrame();
            };
            ImGui::Begin("Demo");
            ImGui::Text("FPS: %f", 1.0f / F_GlobalTimeSubsystem::GetInstance()->GetLastDeltaSeconds());
            ImGui::End();
        }
        if (MathDebuggerBeginFrameStatus == E_MathDebugger2DBeginFrameStatus::SHUTDOWN)
        {
            SignalShutdown();
            return;
        }
    }
    void F_DemoFrameProcessor::Release_Main(const TS_Valid<F_Frame>& CurrentFrame)
    {
        _MathDebugger->Shutdown(CurrentFrame);
        _MathDebugger = {};
        
        F_ApplicationManager::GetInstance()->EnqueueDestroyApplication(
            F_DemoApplication::GetInstance()
        );
        
        A_FrameProcessor::Release_Main(CurrentFrame);
    }

    ABYTEK_DEFINE_OBJECT_SINGLETON_CRT(F_DemoApplication);
    ABYTEK_REFLECT(F_DemoApplication)
    {
    }
    F_DemoApplication::F_DemoApplication(const F_ApplicationConfig& Config) :
        F_Application(Config)
    {
        ABYTEK_BIND_OBJECT_SINGLETON_CRT(); 
        if (!IsCookMode())
        {
            _DemoFrameProcessor = TU<F_DemoFrameProcessor>()();
        }
    }
    F_DemoApplication::~F_DemoApplication()
    {
    }

    B8 F_DemoApplication::OnAddProgramModules()
    {
        if(!F_Application::OnAddProgramModules())
        {
            return false;
        }
        auto ProgramScope = GetProgramScope();
        ProgramScope->AddModule<F_DemoProgramModule>();
        return true;
    }
        
    B8 F_DemoApplication::PostInit()
    {
        return F_Application::PostInit();
    }
    B8 F_DemoApplication::PreRelease()
    {
        _DemoFrameProcessor = {};
        return F_Application::PreRelease();
    }
}
