/**
 * Platform.js
 * 
 * Provides platform identification and detection utilities.
 * This module helps with platform-specific code generation
 * and compilation options throughout the Abytek build system.
 */
module.exports = (Module) => {
    /**
     * Enumeration of supported platforms
     * - WINDOWS: Microsoft Windows
     * - LINUX: Linux-based systems
     * - MACOS: Apple macOS
     * - IOS: Apple iOS
     * - ANDROID: Android
     * - EMSCRIPTEN: Web (WebAssembly)
     */
    let Result = Module.MakeAutoEnum(
        "WINDOWS",
        "LINUX",
        "MACOS",
        "IOS",
        "ANDROID",
        "EMSCRIPTEN",
    );
    /**
     * Determines if a platform is UNIX-like
     * 
     * @param {string} Value - The platform identifier
     * @returns {boolean} True if the platform is UNIX-like, false otherwise
     */
    Result.IsUNIXLike = function (Value)
    {
        if (Value === "WINDOWS")
        {
            return false;
        }
        else if (Value === "LINUX")
        {
            return true;
        }
        else if (Value === "MACOS")
        {
            return true;
        }
        else if (Value === "IOS")
        {
            return true;
        }
        else if (Value === "ANDROID")
        {
            return true;
        }
        else if (Value === "EMSCRIPTEN")
        {
            return true;
        }
    }
    /**
     * Determines if a platform supports POSIX APIs
     * 
     * @param {string} Value - The platform identifier
     * @returns {boolean} True if the platform supports POSIX, false otherwise
     */
    Result.IsPOSIX = function (Value)
    {
        if (Value === "WINDOWS")
        {
            return false;
        }
        else if (Value === "LINUX")
        {
            return true;
        }
        else if (Value === "MACOS")
        {
            return true;
        }
        else if (Value === "IOS")
        {
            return true;
        }
        else if (Value === "ANDROID")
        {
            return true;
        }
        else if (Value === "EMSCRIPTEN")
        {
            return true;
        }
    }
    /**
     * The current platform detected from Module.Context
     */
    Result.CURRENT = Result[Module.Context.PlatformName];
    return Result;
}