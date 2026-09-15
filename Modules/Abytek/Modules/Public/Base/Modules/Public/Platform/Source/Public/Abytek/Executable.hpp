/**
 * @file Executable.hpp
 * @brief Defines the executable class and related structures for application entry points
 */
#pragma once

#include "Abytek/Base.Platform.prerequisites.pch.hpp"
#include "Abytek/Platform.hpp"
#include "Abytek/Windows/ExecutableData.hpp"
#include "Abytek/POSIX/ExecutableData.hpp"
#include "Abytek/Android/ExecutableData.hpp"


namespace Abytek
{
    // \cond INTERNAL
    namespace Internal
    {
        class H_ExecutableCaller;
    }
    // \endcond

    /**
     * @struct F_RawExecutableInput
     * @brief Raw command-line arguments structure for char-based platforms
     */
    struct F_RawExecutableInput
    {
        I32 ArgumentCount = 0;             ///< Number of command-line arguments
        const F_Char** ArgumentCStrs = 0;  ///< Array of C-style strings containing arguments
    };

    /**
     * @struct F_RawExecutableInput_Text
     * @brief Raw command-line arguments structure for wide-char platforms (e.g., Windows)
     */
    struct F_RawExecutableInput_Text
    {
        I32 ArgumentCount = 0;                 ///< Number of command-line arguments
        const F_TextChar** ArgumentCStrs = 0;  ///< Array of wide C-style strings containing arguments
    };

    /**
     * @struct F_ExecutableInput
     * @brief Processed command-line arguments in platform-independent format
     */
    struct F_ExecutableInput
    {
        WithRawAllocator::TF_Vector<WithRawAllocator::F_Text> Arguments;  ///< Vector of text arguments
    };

    /**
     * @class F_Executable
     * @brief Base class for application entry points
     * 
     * The F_Executable class provides the foundation for application entry points.
     * It handles initialization of core systems and processes command-line arguments.
     * Derived classes should override OnStartup() to implement application-specific initialization.
     */
    class ABYTEK_BASE_PLATFORM_API F_Executable
    {
    // \cond INTERNAL
    public:
        friend class Internal::H_ExecutableCaller;
    // \endcond

    private:
        static F_Executable* _Instance;
        
    public:
        static ABYTEK_FORCE_INLINE F_Executable& GetInstance()
        {
            return *_Instance;
        }

    private:
        F_ExecutableInput _Input;          ///< Processed command-line arguments
        F_MemoryManager _MemoryManager;    ///< Memory management system
        TU<A_Platform> Platform;
        F_NameDatabase NameDatabase;       ///< Name database for string interning

    public:
#ifdef ABYTEK_PLATFORM_WINDOWS
        F_WindowsExecutableData WindowsData;
#endif
#ifdef ABYTEK_PLATFORM_POSIX
        F_POSIXExecutableData POSIXData;
#endif
#ifdef ABYTEK_PLATFORM_ANDROID
        F_AndroidExecutableData AndroidData;
#endif
        
    public:
        /**
         * @brief Get the command-line input arguments
         * @return Constant reference to the executable input structure
         */
        ABYTEK_FORCE_INLINE const auto& GetInput() const noexcept { return _Input; }

    public:
        /**
         * @brief Constructor
         * @param Input Processed command-line arguments
         */
        F_Executable(const F_ExecutableInput& Input);
        
        /**
         * @brief Virtual destructor
         */
        virtual ~F_Executable();

    protected:
        virtual void OnStartup();
        virtual void OnShutdown();
    };


    // \cond INTERNAL
    namespace Internal
    {
        /**
         * @class H_ExecutableCaller
         * @brief Internal helper class for executable initialization
         */
        class ABYTEK_BASE_PLATFORM_API H_ExecutableCaller
        {
        public:
            /**
             * @brief Parse raw input arguments into platform-independent format
             * @param RawInput Raw command-line arguments
             * @return Processed executable input structure
             */
            static F_ExecutableInput ParseInput(const F_RawExecutableInput& RawInput);
            
            /**
             * @brief Parse raw text input arguments into platform-independent format
             * @param RawInput Raw wide-char command-line arguments
             * @return Processed executable input structure
             */
            static F_ExecutableInput ParseInput(const F_RawExecutableInput_Text& RawInput);
            
            /**
             * @brief Initialize and start an executable
             * @tparam __F_Executable Type of the executable to start
             * @param Input Processed command-line arguments
             */
            template<class __F_Executable>
            static void Start(__F_Executable& Executable)
            {
                ((F_Executable&)Executable).OnStartup();
                ((F_Executable&)Executable).OnShutdown();
            }
        };
    }
    // \endcond
}

/**
 * @def ABYTEK_DEFINE_EXECUTABLE
 * @brief Macro to define the platform-specific entry point for an application
 * 
 * This macro creates the appropriate main/wmain function for the target platform
 * and handles initialization of the specified executable class.
 * 
 * @param ... The executable class to instantiate
 */
#if defined(ABYTEK_PLATFORM_WINDOWS)
    #define ABYTEK_DEFINE_EXECUTABLE(...) \
        int wmain(int ArgumentCount, const Abytek::F_TextChar* ArgumentCStrs[])\
        {\
            Abytek::F_RawExecutableInput_Text RawExecutableInput = { ArgumentCount, ArgumentCStrs };\
            \
            Abytek::F_ExecutableInput ExecutableInput = Abytek::Internal::H_ExecutableCaller::ParseInput(RawExecutableInput);\
            \
            __VA_ARGS__ Executable(ExecutableInput);\
            Abytek::Internal::H_ExecutableCaller::Start(Executable);\
        }
#elif defined(ABYTEK_PLATFORM_ANDROID)
    #define ABYTEK_DEFINE_EXECUTABLE(...) \
        extern "C" \
        { \
            void android_main(struct android_app *pApp)\
            {\
                Abytek::F_ExecutableInput ExecutableInput;\
                \
                __VA_ARGS__ Executable(ExecutableInput);\
                Executable.AndroidData.AndroidApp = pApp;\
                Abytek::Internal::H_ExecutableCaller::Start(Executable);\
            } \
        }
#elif defined(ABYTEK_PLATFORM_UNIX)
    #define ABYTEK_DEFINE_EXECUTABLE(...) \
        int main(int ArgumentCount, const Abytek::F_Char* ArgumentCStrs[])\
        {\
            Abytek::F_RawExecutableInput RawExecutableInput = { ArgumentCount, ArgumentCStrs };\
            \
            Abytek::F_ExecutableInput ExecutableInput = Abytek::Internal::H_ExecutableCaller::ParseInput(RawExecutableInput);\
            \
            __VA_ARGS__ Executable(ExecutableInput);\
            Abytek::Internal::H_ExecutableCaller::Start(Executable);\
        }
#endif