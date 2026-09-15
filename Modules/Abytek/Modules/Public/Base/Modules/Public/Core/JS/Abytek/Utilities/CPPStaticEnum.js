/**
 * CPPStaticEnum.js
 * 
 * Provides utilities for generating static C++ enum declarations and implementations.
 * This module supports both regular enums and enum classes with customizable options.
 */
module.exports = (Module) => {
    /**
     * Default configuration for enum members
     */
    const DefaultEnumAdditionalConfig = {
        Metadata: new Object(),
    };
    
    /**
     * Class for generating C++ static enum types
     */
    class F_CPPStaticEnum
    {
        /**
         * Creates a new C++ static enum generator
         * 
         * @param {string} Name - The name of the enum
         * @param {string} Namespace - The namespace containing the enum
         */
        constructor(Name = "", Namespace = "")
        {
            this.Namespace = Namespace;
            this.Name = Name;
            this.UnderlyingType = "Abytek::U32";
            this.EnableClassMode = true;
            this.Members = new Object();
            this.API = '';
            this.Metadata = new Object();
            this.EnableFlagOperators = false;
            this.EnableNonConstexprFlagOperators = false;
        }

        /**
         * Gets the fully qualified name (with namespace) of the enum
         * 
         * @returns {string} Namespace::Name format
         */
        get FullName()
        {
            return `${this.Namespace}::${this.Name}`;
        }
        
        /**
         * Validates the enum configuration
         * Throws error if any required property is invalid
         */
        Validate()
        {
            if (this.Namespace == null)
            {
                throw new Error("Null C++ class namespace is not allowed");
            }
            if (this.Name === '' || this.Name == null)
            {
                throw new Error("Empty C++ enum name is not allowed");
            }
            if (this.UnderlyingType == null)
            {
                throw new Error("Invalid underlying type");
            }
            if (this.Members == null)
            {
                throw new Error("Invalid members");
            }
            if (this.API == null)
            {
                throw new Error("Invalid API");
            }
            if (this.Metadata == null)
            {
                throw new Error("Invalid metadata");
            }
        }
        
        /**
         * Sets the name of the enum
         * 
         * @param {string} Name - The enum name
         * @returns {F_CPPStaticEnum} This instance for chaining
         */
        SetName(Name)
        {
            this.Name = Name;
            return this;
        }

        /**
         * Sets the namespace of the enum
         * 
         * @param {string} Namespace - The enum namespace
         * @returns {F_CPPStaticEnum} This instance for chaining
         */
        SetNamespace(Namespace)
        {
            this.Namespace = Namespace;
            return this;
        }

        /**
         * Sets the underlying type of the enum
         * 
         * @param {string} UnderlyingType - The C++ type name (e.g. "int", "Abytek::U32")
         * @returns {F_CPPStaticEnum} This instance for chaining
         */
        SetUnderlyingType(UnderlyingType)
        {
            this.UnderlyingType = UnderlyingType;
            return this;
        }

        /**
         * Sets whether to generate as enum class (true) or regular enum (false)
         * 
         * @param {boolean} EnableClassMode - True for enum class, false for regular enum
         * @returns {F_CPPStaticEnum} This instance for chaining
         */
        SetEnableClassMode(EnableClassMode)
        {
            this.EnableClassMode = EnableClassMode;
            return this;
        }

        /**
         * Sets the enum members
         * 
         * @param {Object} Members - Dictionary of enum members
         * @returns {F_CPPStaticEnum} This instance for chaining
         */
        SetMembers(Members)
        {
            if (Members == null)
            {
                this.Members = new Object();
            }
            else
            {
                this.Members = Members;
            }
            return this;
        }

        /**
         * Adds a single enum member
         * 
         * @param {string} Key - The name of the enum member
         * @param {string|number} Value - The value of the enum member
         * @param {Object} AdditionalConfig - Additional configuration for the member
         * @returns {F_CPPStaticEnum} This instance for chaining
         */
        AddMember(Key, Value, AdditionalConfig = DefaultEnumAdditionalConfig)
        {
            this.Members[Key] = {
                Value: Value,
                AdditionalConfig: AdditionalConfig,
            };
            return this;
        }

        /**
         * Sets the API export/import macro for the enum
         * 
         * @param {string} API - The API macro name
         * @returns {F_CPPStaticEnum} This instance for chaining
         */
        SetAPI(API)
        {
            this.API = API;
            return this;
        }

        /**
         * Sets metadata for the enum
         * 
         * @param {Object} Metadata - Metadata object
         * @returns {F_CPPStaticEnum} This instance for chaining
         */
        SetMetadata(Metadata)
        {
            this.Metadata = Metadata;
            return this;
        }

        /**
         * Sets whether to enable flag operators for the enum
         * 
         * @param {boolean} EnableFlagOperators - True to enable flag operators
         * @returns {F_CPPStaticEnum} This instance for chaining
         */
        SetEnableFlagOperators(EnableFlagOperators)
        {
            this.EnableFlagOperators = EnableFlagOperators;
            return this;
        }

        SetEnableNonConstexprFlagOperators(EnableNonConstexprFlagOperators)
        {
            this.EnableNonConstexprFlagOperators = EnableNonConstexprFlagOperators;
            return this;
        }
        
        /**
         * Generates the C++ enum declaration code
         * 
         * @returns {string} The generated C++ code for the enum declaration
         */
        GenerateDeclaration()
        {
            this.Validate();
            
            let GeneratedMembers = '';
            for (let Key in this.Members)
            {
                if (GeneratedMembers !== '')
                {
                    GeneratedMembers += ',\n';
                }
                GeneratedMembers += `${Key} ${ (this.Members[Key].Value != '') ? `= ${this.Members[Key].Value}` : '/* Auto Value */' }`;
            }
            
            let Result = `
                ${(this.Namespace === '') ? '' : `namespace ${this.Namespace} {`}
                    enum ${this.EnableClassMode ? 'class' : ''} ${this.Name} : ${this.UnderlyingType}
                    {
                        ${GeneratedMembers}
                    };
                    ${this.EnableFlagOperators ? `ABYTEK_DEFINE_FLAG_OPERATORS(${this.Name});` : '' }
                    ${this.EnableNonConstexprFlagOperators ? `ABYTEK_DEFINE_FLAG_OPERATORS_NON_CONSTEXPR(${this.Name});` : '' }
                ${(this.Namespace === '') ? '' : `}`}
            `;
            return Result;
        }

        /**
         * Generates the C++ enum implementation code
         * 
         * @returns {string} The generated C++ code for the enum implementation
         */
        GenerateImplementation()
        {
            this.Validate();
            let Result = `
                ${(this.Namespace === '') ? '' : `namespace ${this.Namespace} {`}
                ${(this.Namespace === '') ? '' : `}`}
            `;
            return Result;
        }
    }

    /**
     * Factory function for creating a CPPStaticEnum instance
     * 
     * @param {...*} Args - Arguments to pass to the constructor
     * @returns {F_CPPStaticEnum} A new static enum generator instance
     */
    return (...Args) => {
        return new F_CPPStaticEnum(...Args);  
    };
}