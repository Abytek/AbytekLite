#include "Abytek/DemoPythonExecutable.hpp"
#include "Abytek/Engine.Plugins.Python.Demo.hpp"


namespace Abytek
{
    F_DemoPythonExecutable::F_DemoPythonExecutable(const F_ExecutableInput& Input) :
        F_Executable(Input)
    {
    }

    ABYTEK_BEGIN_PY_METHOD(DemoMethod)
        ABYTEK_LOG_INFO() << "Hello World Internal Call!";
        return F_SharedPyObject();
    ABYTEK_END_PY_METHOD()
    
    void F_DemoPythonExecutable::OnStartup()
    {
        F_Executable::OnStartup();

        // Register method for internal call
        F_PythonBinder::EnqueueModuleDesc({
            "Demo",
            {
                {
                    "DemoMethod",
                    DemoMethod::Invoke
                }
            }
        });

        F_PythonBinder PythonBinder;

        // Internal call
        F_PyModule Demo = F_PyModule::Import({ ABYTEK_TEXT("Demo") });
        H_PyRun::ExecuteText({
            ABYTEK_TEXT(
                "import Demo \n"
                "Demo.DemoMethod()\n"
            )
        });

        // External call
        {
            H_PyRun::ExecuteText({
                ABYTEK_TEXT(
                    "def HelloWorld():\n"
                    "   print('Hello World')\n"
                )
            });

            F_PyModule MainModule = F_PyModule::GetMain();
            ABYTEK_ASSERT_GENERAL(MainModule);

            F_PyFunction HelloWorldFunction = MainModule.GetAttribute("HelloWorld").Cast<F_PyFunction>();
            HelloWorldFunction();
        }
    }
}


ABYTEK_DEFINE_EXECUTABLE(Abytek::F_DemoPythonExecutable);