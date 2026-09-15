/**
 * CPPGenerator.js
 * 
 * Provides a collection of utilities for generating C++ code during build time.
 * This module acts as a central hub for accessing various C++ code generation utilities
 * such as class, enum, and struct generators.
 */
module.exports = (Module) => {
    /**
     * Collection of C++ code generation utilities
     * 
     * @property {Object} Accessibility - Utilities for handling C++ access modifiers
     * @property {Object} InlineMode - Utilities for handling C++ inline functions
     * @property {Object} StaticEnum - Utilities for generating static C++ enums
     * @property {Object} StaticClass - Utilities for generating static C++ classes
     * @property {Object} StaticStruct - Utilities for generating static C++ structs
     * @property {Object} DynamicClass - Utilities for generating dynamic C++ classes
     * @property {Object} DynamicEnum - Utilities for generating dynamic C++ enums
     * @returns {Object} Collection of code generation utilities
     */
    return {
        Accessibility: Module.ImportScript("Abytek/Utilities/CPPAccessibility.js"),
        InlineMode: Module.ImportScript("Abytek/Utilities/CPPInlineMode.js"),
        StaticEnum: Module.ImportScript("Abytek/Utilities/CPPStaticEnum.js"),
        StaticClass: Module.ImportScript("Abytek/Utilities/CPPStaticClass.js"),
        StaticStruct: Module.ImportScript("Abytek/Utilities/CPPStaticStruct.js"),
        DynamicClass: Module.ImportScript("Abytek/Utilities/CPPDynamicClass.js"),
        DynamicEnum: Module.ImportScript("Abytek/Utilities/CPPDynamicEnum.js"),
    };
}