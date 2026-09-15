/**
 * CPPInlineMode.js
 * 
 * Provides an enumeration for C++ inlining modes used during code generation.
 * This utility helps specify how functions should be inlined in generated C++ code.
 */
module.exports = (Module) => {
    /**
     * Enumeration of C++ inline modes
     * - NONE: No inlining
     * - AUTO: Standard inline keyword
     * - FORCE: Force inline using ABYTEK_FORCE_INLINE macro
     */
    let Result = Module.MakeAutoEnum(
        "NONE",
        "AUTO",
        "FORCE"
    );
    /**
     * Mapping from enum values to C++ inline keywords
     */
    let Map = {
        NONE: "",
        AUTO: "inline",
        FORCE: "ABYTEK_FORCE_INLINE",
    };
    /**
     * Converts the enum value to its corresponding C++ keyword
     * 
     * @param {string} Value - The enum value to convert
     * @returns {string} The C++ inline keyword
     */
    Result.ToCPP = function (Value)
    {
        return Map[Value];
    }
    return Result;
}