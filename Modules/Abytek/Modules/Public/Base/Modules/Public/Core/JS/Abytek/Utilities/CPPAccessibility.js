/**
 * CPPAccessibility.js
 * 
 * Provides an enumeration for C++ access modifiers used during code generation.
 * This utility helps specify the accessibility of methods and properties
 * in generated C++ classes.
 */
module.exports = (Module) => {
    /**
     * Enumeration of C++ access modifiers
     * - PUBLIC: Public access
     * - PRIVATE: Private access
     * - PROTECTED: Protected access
     */
    let Result = Module.MakeAutoEnum(
        "PUBLIC",
        "PRIVATE",
        "PROTECTED"
    );
    /**
     * Mapping from enum values to C++ access modifiers
     */
    let Map = {
        PUBLIC: "public",
        PRIVATE: "private",
        PROTECTED: "protected",  
    };
    /**
     * Converts the enum value to its corresponding C++ access modifier
     * 
     * @param {string} Value - The enum value to convert
     * @returns {string} The C++ access modifier keyword
     */
    Result.ToCPP = function (Value)
    {
        return Map[Value];
    }
    return Result;
}