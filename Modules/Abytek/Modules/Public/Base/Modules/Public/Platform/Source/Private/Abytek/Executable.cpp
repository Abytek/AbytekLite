/**
 * @file Executable.cpp
 * @brief Implementation of the executable class and related functionality
 */
#include "Abytek/Executable.hpp"


namespace Abytek
{
    F_Executable* F_Executable::_Instance = 0;
    
    /**
     * Constructor for the F_Executable class
     * Initializes the executable with the provided command-line arguments
     */
    F_Executable::F_Executable(const F_ExecutableInput& Input) :
        _Input(Input)
    {
        _Instance = this;
    }

    F_Executable::~F_Executable()
    {
    }

    void F_Executable::OnStartup()
    {
        Platform = A_Platform::Create();
        Platform->Startup();
    }
    void F_Executable::OnShutdown()
    {
        Platform->Shutdown();
        Platform = {};
    }


    namespace Internal
    {
        /**
         * Parses raw char-based command-line arguments into a platform-independent format
         * Used primarily on Unix-based platforms
         */
        F_ExecutableInput H_ExecutableCaller::ParseInput(const F_RawExecutableInput& RawInput)
        {
            F_ExecutableInput Result;

            for(U32 Idx = 0; Idx < static_cast<U32>(RawInput.ArgumentCount); ++Idx)
            {
                Result.Arguments.push_back(
                    WithRawAllocator::ToText(
                        WithRawAllocator::ToString(RawInput.ArgumentCStrs[Idx])
                    )
                );
            }

            return boost::move(Result);
        }

        /**
         * Parses raw wide-char command-line arguments into a platform-independent format
         * Used primarily on Windows platforms
         */
        F_ExecutableInput H_ExecutableCaller::ParseInput(const F_RawExecutableInput_Text& RawInput)
        {
            F_ExecutableInput Result;

            for(U32 Idx = 0; Idx < static_cast<U32>(RawInput.ArgumentCount); ++Idx)
            {
                Result.Arguments.push_back(RawInput.ArgumentCStrs[Idx]);
            }

            return boost::move(Result);
        }
    }
}