/**
 * CPPStaticStruct.js
 * 
 * Provides utilities for generating static C++ struct declarations and implementations.
 * This module supports generating structs with customizable properties, methods,
 * inheritance, and other features for use in C++ code generation.
 */
module.exports = (Module) => {
    let E_CPPAccessibility = Module.ImportScript("Abytek/Utilities/CPPAccessibility.js");
    let E_CPPInlineMode = Module.ImportScript("Abytek/Utilities/CPPInlineMode.js");
    let CPPMethodArgsToString = Module.ImportScript("Abytek/Utilities/CPPMethodArgsToString.js");

    /**
     * Default additional configuration for struct members
     * 
     * @property {boolean} Static - Whether the member is static
     * @property {boolean} Constexpr - Whether the member is constexpr
     * @property {boolean} Explicit - Whether the constructor is explicit
     * @property {string} InlineMode - One of NONE, AUTO, FORCE
     * @property {boolean} Mutable - Whether the member is mutable
     * @property {boolean} Const - Whether the member is const
     * @property {boolean} Noexcept - Whether the method is noexcept
     * @property {string|null} DefaultValue - Default value, if any
     * @property {boolean} Virtual - Whether the method is virtual
     * @property {boolean} Override - Whether the method overrides a base method
     * @property {boolean} Final - Whether the method is final
     * @property {boolean} Export - Whether to export the member through API macros
     * @property {Object} Metadata - Additional metadata
     */
    const DefaultStructMemberAdditionalConfig = {
        Static: false,
        Constexpr: false,
        Explicit: false,
        InlineMode: E_CPPInlineMode.NONE,
        Mutable: false,
        Const: false,
        Noexcept: false,
        DefaultValue: null,
        Virtual: false,
        Override: false,
        Final: false,
        Export: true,
        Metadata: new Object(),
    };

    /**
     * Class for generating C++ static struct types
     */
    class F_CPPStaticStruct
    {
        /**
         * Creates a new C++ static struct generator
         * 
         * @param {string} [Name=""] - The name of the struct
         * @param {string} [Namespace=""] - The namespace containing the struct
         */
        constructor(Name = "", Namespace = "")
        {
            this.Namespace = Namespace;
            this.Name = Name;
            this.Inheritance = null;
            this.Members = new Object();
            this.API = '';
            this.Final = '';
            this.Metadata = new Object();

            this.GeneratedPreDeclaration = '';
            this.GeneratedPostDeclaration = '';
            this.GeneratedPreMemberDeclaration = '';
            this.GeneratedPostMemberDeclaration = '';

            this.GeneratedPreImplementation = '';
            this.GeneratedPostImplementation = '';
            this.GeneratedPreMemberImplementation = '';
            this.GeneratedPostMemberImplementation = '';
        }

        /**
         * Gets the fully qualified name (with namespace) of the struct
         * 
         * @returns {string} Namespace::Name format or just Name if no namespace
         */
        get FullName()
        {
            return `${
                (
                    (this.Namespace == null)
                    || (this.Namespace === "")
                )
                    ? ""
                    : `${this.Namespace}::`
            }${this.Name}`;
        }

        /**
         * Validates the struct configuration
         * Throws error if any required property is invalid
         */
        Validate()
        {
            if (this.Namespace == null)
            {
                throw new Error("Null C++ struct namespace is not allowed");
            }
            if (this.Name === '' || this.Name == null)
            {
                throw new Error("Empty C++ struct name is not allowed");
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
         * Sets the name of the struct
         * 
         * @param {string} Name - The struct name
         * @returns {F_CPPStaticStruct} This instance for chaining
         */
        SetName(Name)
        {
            this.Name = Name;
            return this;
        }
        
        /**
         * Sets the namespace of the struct
         * 
         * @param {string} Namespace - The struct namespace
         * @returns {F_CPPStaticStruct} This instance for chaining
         */
        SetNamespace(Namespace)
        {
            this.Namespace = Namespace;
            return this;
        }
        
        /**
         * Specifies inheritance from another type
         * 
         * @param {string} Type - The base type to inherit from
         * @param {string} [Accessibility=E_CPPAccessibility.PUBLIC] - Access level for inheritance
         * @returns {F_CPPStaticStruct} This instance for chaining
         */
        Inherit(Type, Accessibility = E_CPPAccessibility.PUBLIC)
        {
            this.Inheritance = {
                Type: Type,
                Accessibility: Accessibility
            };
            return this;
        }
        
        /**
         * Sets the struct members collection
         * 
         * @param {Object} Members - Dictionary of member definitions
         * @returns {F_CPPStaticStruct} This instance for chaining
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
         * Adds a member to the struct
         * 
         * @param {string} Name - The name of the member
         * @param {Object} Member - Member definition
         * @returns {F_CPPStaticStruct} This instance for chaining
         */
        AddMember(Name, Member)
        {
            this.Members[Name] = Member;
            return this;
        }
        
        /**
         * Adds a property to the struct
         * 
         * @param {string} Name - The name of the property
         * @param {string} DataType - The C++ type of the property
         * @param {string} [Accessibility=E_CPPAccessibility.PUBLIC] - Access level
         * @param {Object} [AdditionalConfig=DefaultStructMemberAdditionalConfig] - Additional configuration
         * @returns {F_CPPStaticStruct} This instance for chaining
         */
        AddProperty(Name, DataType, Accessibility = E_CPPAccessibility.PUBLIC, AdditionalConfig = DefaultStructMemberAdditionalConfig)
        {
            this.Members[Name] = {
                MemberType: "Property",
                DataType: DataType,
                Accessibility: Accessibility,
                AdditionalConfig: { ...AdditionalConfig }
            };
            return this;
        }
        
        /**
         * Adds a method to the struct
         * 
         * @param {string} Name - The name of the method
         * @param {string} ReturnType - The C++ return type
         * @param {Object} Args - Arguments definition
         * @param {string} [Body=""] - Method body implementation
         * @param {string} [Accessibility=E_CPPAccessibility.PUBLIC] - Access level
         * @param {Object} [AdditionalConfig=DefaultStructMemberAdditionalConfig] - Additional configuration
         * @returns {F_CPPStaticStruct} This instance for chaining
         */
        AddMethod(Name, ReturnType, Args, Body = "", Accessibility = E_CPPAccessibility.PUBLIC, AdditionalConfig = DefaultStructMemberAdditionalConfig)
        {
            this.Members[Name] = {
                MemberType: "Method",
                ReturnType: ReturnType,
                Args: Args,
                Body: Body,
                Accessibility: Accessibility,
                AdditionalConfig: { ...AdditionalConfig }
            };
            return this;
        }
        
        /**
         * Sets the API export/import macro for the struct
         * 
         * @param {string} API - The API macro name
         * @returns {F_CPPStaticStruct} This instance for chaining
         */
        SetAPI(API)
        {
            this.API = API;
            return this;
        }
        
        /**
         * Sets whether the struct is marked as final
         * 
         * @param {boolean} Final - True to mark as final
         * @returns {F_CPPStaticStruct} This instance for chaining
         */
        SetFinal(Final)
        {
            this.Final = Final;
            return this;
        }
        
        /**
         * Sets metadata for the struct
         * 
         * @param {Object} Metadata - Metadata object
         * @returns {F_CPPStaticStruct} This instance for chaining
         */
        SetMetadata(Metadata)
        {
            this.Metadata = Metadata;
            return this;
        }

        /**
         * Generates declaration code for a specific member
         * 
         * @param {string} MemberName - The name of the member to generate
         * @returns {string} Generated C++ code for the member declaration
         */
        GenerateMemberDeclaration(MemberName)
        {
            let GeneratedMembers = `
                ${this.GeneratedPreMemberDeclaration}
            `;

            let Member = this.Members[MemberName];

            GeneratedMembers += `${E_CPPAccessibility.ToCPP(Member.Accessibility)}:\n`;

            if(Member.MemberType === 'Property')
            {
                GeneratedMembers += `
                        ${Member.AdditionalConfig.Export ? this.API : ""} 
                        ${Member.AdditionalConfig.Static ? "static" : ""} 
                        ${Member.AdditionalConfig.Constexpr ? "constexpr" : ""} 
                        ${E_CPPInlineMode.ToCPP(Member.AdditionalConfig.InlineMode)} 
                        ${Member.AdditionalConfig.Mutable ? 'mutable' : ''} 
                        ${Member.DataType} 
                        ${MemberName} 
                        ${
                    (Member.AdditionalConfig.InlineMode === E_CPPInlineMode.NONE)
                        ? ""
                        : (
                            Member.AdditionalConfig.DefaultValue
                                ? Member.AdditionalConfig.DefaultValue
                                : ''
                        )
                };
                    `;
            }
            else if (Member.MemberType === 'Method')
            {
                let ArgList = CPPMethodArgsToString(Member.Args);
                GeneratedMembers += `
                        ${Member.AdditionalConfig.Export ? this.API : ""} 
                        ${Member.AdditionalConfig.Static ? "static" : ""} 
                        ${Member.AdditionalConfig.Static ? "explicit" : ""} 
                        ${Member.AdditionalConfig.Virtual ? "virtual" : ""} 
                        ${Member.AdditionalConfig.Constexpr ? "constexpr" : ""} 
                        ${E_CPPInlineMode.ToCPP(Member.AdditionalConfig.InlineMode)} 
                        ${Member.ReturnType} 
                        ${MemberName} 
                        (${ArgList})
                        ${Member.AdditionalConfig.Const ? 'const' : ''} 
                        ${Member.AdditionalConfig.Noexcept ? 'noexcept' : ''} 
                        ${Member.AdditionalConfig.Override ? "override" : ""} 
                        ${Member.AdditionalConfig.Final ? "final" : ""} 
                        ${
                    (Member.AdditionalConfig.InlineMode === E_CPPInlineMode.NONE)
                        ? ""
                        : (
                            Member.AdditionalConfig.DefaultValue
                                ? Member.AdditionalConfig.DefaultValue
                                : ''
                        )
                }
                        ${Member.AdditionalConfig.InlineMode === E_CPPInlineMode.NONE ? ";" : Member.Body}
                    `;
            }
            else
            {
                throw new Error(`Invalid member type ${Member.MemberType}`);
            }

            GeneratedMembers += `
                ${this.GeneratedPostMemberDeclaration}
            `;
            return GeneratedMembers;
        }
        
        /**
         * Generates implementation code for a specific member
         * 
         * @param {string} MemberName - The name of the member to generate
         * @returns {string} Generated C++ code for the member implementation
         */
        GenerateMemberImplementation(MemberName)
        {
            let GeneratedMembers = `
                ${this.GeneratedPreMemberImplementation}
            `;

            let Member = this.Members[MemberName];

            if (Member.AdditionalConfig.InlineMode === E_CPPInlineMode.NONE)
            {
                if (Member.MemberType === 'Property')
                {
                    GeneratedMembers += `
                            ${Member.AdditionalConfig.Constexpr ? "constexpr" : ""} 
                            ${E_CPPInlineMode.ToCPP(Member.AdditionalConfig.InlineMode)} 
                            ${Member.AdditionalConfig.Mutable ? 'mutable' : ''} 
                            ${Member.DataType} 
                            ${this.Name}::${MemberName} 
                            ${Member.AdditionalConfig.DefaultValue ? ` = ${Member.AdditionalConfig.DefaultValue}` : ''};
                        `;
                }
                else if (Member.MemberType === 'Method') {
                    let ArgList = '';
                    for (let ArgName in Member.Args) {
                        if (ArgList !== '') {
                            ArgList += ', ';
                        }
                        let Arg = Member.Args[ArgName];
                        ArgList += `${Arg.DataType} ${ArgName}`;
                    }
                    GeneratedMembers += `
                            ${Member.AdditionalConfig.Constexpr ? "explicit" : ""} 
                            ${Member.AdditionalConfig.Constexpr ? "constexpr" : ""} 
                            ${E_CPPInlineMode.ToCPP(Member.AdditionalConfig.InlineMode)} 
                            ${Member.ReturnType} 
                            ${this.Name}::${MemberName} 
                            (${ArgList})
                            ${Member.AdditionalConfig.Const ? 'const' : ''} 
                            ${Member.AdditionalConfig.Noexcept ? 'noexcept' : ''} 
                            ${Member.AdditionalConfig.DefaultValue ? ` = ${Member.AdditionalConfig.DefaultValue}` : ''}
                            ${Member.Body}
                        `;
                }
                else
                {
                    throw new Error(`Invalid member type ${Member.MemberType}`);
                }
            }

            GeneratedMembers += `
                ${this.GeneratedPostMemberImplementation}
            `;
            return GeneratedMembers;
        }
        
        /**
         * Generates the C++ struct declaration code
         * 
         * @returns {string} The generated C++ code for the struct declaration
         */
        GenerateDeclaration()
        {
            this.Validate();

            let GeneratedInheritance = '';
            if (this.Inheritance)
            {
                GeneratedInheritance = `: ${E_CPPAccessibility.ToCPP(this.Inheritance.Accessibility)} ${this.Inheritance.Type}`;
            }

            let GeneratedMembers = '';
            for (let MemberName in this.Members)
            {
                GeneratedMembers += this.GenerateMemberDeclaration(MemberName);
            }

            let Result = `
                ${(this.Namespace === '') ? '' : `namespace ${this.Namespace} {`}
                    ${this.GeneratedPreDeclaration}
                    struct ${this.Name} ${GeneratedInheritance}
                    {
                        ${GeneratedMembers}
                    };
                    ${this.GeneratedPostDeclaration}
                ${(this.Namespace === '') ? '' : `}`}
            `;
            return Result;
        }
        
        /**
         * Generates the C++ struct implementation code
         * 
         * @returns {string} The generated C++ code for the struct implementation
         */
        GenerateImplementation()
        {
            this.Validate();

            let GeneratedMembers = '';
            for (let MemberName in this.Members)
            {
                GeneratedMembers += this.GenerateMemberImplementation(MemberName);
            }

            let Result = `
                ${(this.Namespace === '') ? '' : `namespace ${this.Namespace} {`}
                    ${this.GeneratedPreImplementation}
                    ${GeneratedMembers}
                    ${this.GeneratedPostImplementation}
                ${(this.Namespace === '') ? '' : `}`}
            `;
            return Result;
        }
    }

    /**
     * Factory function for creating a CPPStaticStruct instance
     * 
     * @param {string} [Name=""] - The name of the struct
     * @param {string} [Namespace=""] - The namespace containing the struct
     * @returns {F_CPPStaticStruct} A new static struct generator instance
     */
    let ResultFunctor = function (...Args)
    {
        return new F_CPPStaticStruct(...Args);
    };
    ResultFunctor.DefaultAccessibility = E_CPPAccessibility.PUBLIC;
    ResultFunctor.DefaultMemberAdditionalConfig = DefaultStructMemberAdditionalConfig;
    return ResultFunctor;
}