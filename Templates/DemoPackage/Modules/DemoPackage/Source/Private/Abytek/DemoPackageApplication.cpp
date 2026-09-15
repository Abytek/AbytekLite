#include "Abytek/DemoPackageApplication.hpp"
#include "Abytek/ApplicationProgramScope.hpp"
#include "Abytek/DemoPackageProgramModule.hpp"


namespace Abytek
{
    ABYTEK_DEFINE_OBJECT_SINGLETON_CRT(F_DemoPackageFrameProcessor);
    ABYTEK_REFLECT(F_DemoPackageFrameProcessor)
    {
    }
    F_DemoPackageFrameProcessor::F_DemoPackageFrameProcessor()
    {
    }
    F_DemoPackageFrameProcessor::~F_DemoPackageFrameProcessor()
    {
    }
 
    void F_DemoPackageFrameProcessor::Init_Main(const TS_Valid<F_Frame>& CurrentFrame)
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
    void F_DemoPackageFrameProcessor::Tick_Main(const TS_Valid<F_Frame>& CurrentFrame)
    {
        A_FrameProcessor::Tick_Main(CurrentFrame);
        
        auto MathDebuggerBeginFrameStatus = _MathDebugger->BeginFrame(CurrentFrame);
        if (MathDebuggerBeginFrameStatus == E_MathDebugger2DBeginFrameStatus::SUCCEEDED)
        {
            ABYTEK_DEFER(this)
            {
                _MathDebugger->EndFrame();
            };
            ImGui::Begin("DemoPackage");
            ImGui::Text("FPS: %f", 1.0f / F_GlobalTimeSubsystem::GetInstance()->GetLastDeltaSeconds());
            ImGui::End();
        }
        if (MathDebuggerBeginFrameStatus == E_MathDebugger2DBeginFrameStatus::SHUTDOWN)
        {
            SignalShutdown();
            return;
        }
    }
    void F_DemoPackageFrameProcessor::Release_Main(const TS_Valid<F_Frame>& CurrentFrame)
    {
        _MathDebugger->Shutdown(CurrentFrame);
        _MathDebugger = {};
        
        F_ApplicationManager::GetInstance()->EnqueueDestroyApplication(
            F_DemoPackageApplication::GetInstance()
        );
        
        A_FrameProcessor::Release_Main(CurrentFrame);
    }

    ABYTEK_DEFINE_OBJECT_SINGLETON_CRT(F_DemoPackageApplication);
    ABYTEK_REFLECT(F_DemoPackageApplication)
    {
    }
    F_DemoPackageApplication::F_DemoPackageApplication(const F_ApplicationConfig& Config) :
        F_Application(Config)
    {
        ABYTEK_BIND_OBJECT_SINGLETON_CRT(); 
        if (!IsCookMode())
        {
            _DemoPackageFrameProcessor = TU<F_DemoPackageFrameProcessor>()();
        }
    }
    F_DemoPackageApplication::~F_DemoPackageApplication()
    {
    }

    B8 F_DemoPackageApplication::OnAddProgramModules()
    {
        if(!F_Application::OnAddProgramModules())
        {
            return false;
        }
        auto ProgramScope = GetProgramScope();
        ProgramScope->AddModule<F_DemoPackageProgramModule>();
        return true;
    }
        
    B8 F_DemoPackageApplication::PostInit()
    {
        return F_Application::PostInit();
    }
    B8 F_DemoPackageApplication::PreRelease()
    {
        _DemoPackageFrameProcessor = {};
        return F_Application::PreRelease();
    }
}
