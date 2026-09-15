/**
 * CPPDynamicEnum.js
 * 
 * Provides utilities for generating dynamic C++ enums that can have different
 * implementations in different modules or compilations. This enables runtime
 * binding of enum values and is used for cross-module extensibility.
 */
module.exports = (Module) => {
    // Target:
    // - Members (as key->value map)

    let F_CPPStaticEnum = Module.ImportScript("Abytek/Utilities/CPPStaticEnum.js");
    let F_CPPStaticClass = Module.ImportScript("Abytek/Utilities/CPPStaticClass.js");
    let E_CPPAccessibility = Module.ImportScript("Abytek/Utilities/CPPAccessibility.js");
    let E_CPPInlineMode = Module.ImportScript("Abytek/Utilities/CPPInlineMode.js");

    const GeneratedVirtualTableClassName = "TH_GeneratedVirtualTable";
    const GeneratedVirtualTableClassNamespace = "Abytek";

    /**
     * Class for generating C++ dynamic enum types
     */
    class F_CPPDynamicEnum
    {
        /**
         * Creates a new C++ dynamic enum generator
         * 
         * @param {Object} Template - Template object defining the enum
         */
        constructor(Template)
        {
            this.Template = Template;
            this.BuiltClassOrEnum = null;
            this.VirtualTableClass = null;
            this.Targets = new Object();
        }

        /**
         * Validates the enum configuration
         * Throws error if any required property is invalid
         */
        Validate()
        {
            if (this.Template == null)
            {
                throw new Error("Invalid Template");
            }
            this.Template.Validate();
        }

        /**
         * Sets the template object defining the enum
         * 
         * @param {Object} Template - The template object
         * @returns {F_CPPDynamicEnum} This instance for chaining
         */
        SetTemplate(Template)
        {
            this.Template = Template;
            return this;
        }
        
        /**
         * Sets the collection of target implementations
         * 
         * @param {Object} Targets - Map of target implementations
         * @returns {F_CPPDynamicEnum} This instance for chaining
         */
        SetTargets(Targets)
        {
            this.Targets = Targets;
            return this;
        }
        
        /**
         * Adds a target implementation for the enum
         * 
         * @param {string} Key - Identifier for the target
         * @param {Object} Members - Member implementations for this target
         * @param {Array<string>} [DummyMemberNames] - Names of members that are dummies
         * @returns {F_CPPDynamicEnum} This instance for chaining
         */
        AddTarget(Key, Members, DummyMemberNames)
        {
            this.Targets[Key] = {
                Members: Members,
                DummyMemberNames: DummyMemberNames ? DummyMemberNames : [],
            };
            return this;
        }

        /**
         * Builds the dynamic enum
         * 
         * @returns {F_CPPDynamicEnum} This instance for chaining
         */
        Build()
        {
            this.Validate();

            this.Analyze();
            this.AddBuiltMembers();
            return this;
        }
        
        /**
         * Analyzes the configuration to determine build approach
         * 
         * @returns {F_CPPDynamicEnum} This instance for chaining
         */
        Analyze()
        {
            this.IsStaticLinkable = (Object.keys(this.Targets).length === 1);
            for (let Key in this.Targets)
            {
                let Target = this.Targets[Key];
                if (Target.DummyMemberNames.length != 0)
                {
                    // this.IsStaticLinkable = false;
                    break;
                }
            }
            return this;
        }
        
        /**
         * Adds members to the built class or enum
         * 
         * @returns {F_CPPDynamicEnum} This instance for chaining
         */
        AddBuiltMembers()
        {
            if (this.IsStaticLinkable)
            {
                this.BuiltClassOrEnum = F_CPPStaticEnum(this.Template.Name, this.Template.Namespace)
                    .SetUnderlyingType(this.Template.UnderlyingType)
                    .SetAPI(this.Template.API);
            }
            else
            {
                this.BuiltClassOrEnum = F_CPPStaticClass(this.Template.Name, this.Template.Namespace)
                    .SetAPI(this.Template.API)
                    .SetFinal(true);
            }
            if (this.Template.EnableFlagOperators)
            {
                if (this.IsStaticLinkable)
                {
                    this.BuiltClassOrEnum.SetEnableFlagOperators(true);
                }
                else
                {
                    this.BuiltClassOrEnum.SetEnableNonConstexprFlagOperators(true);
                }
            }
            this.VirtualTableClass = F_CPPStaticClass(
                `${GeneratedVirtualTableClassName}<${this.BuiltClassOrEnum.FullName}>`,
                GeneratedVirtualTableClassNamespace
            )
                .SetAPI(this.Template.API)
                .SetFinal(true);
            this.VirtualTableClass.GeneratedPreDeclaration = 'template<>';
            // this.VirtualTableClass.GeneratedPreMemberImplementation = 'template<>';

            // temp variables
            let DynamicMemberNames = [];
            let UninstallBody = "";
            let InstallBody = "";

            // helper functions
            let AddDynamicProperty = function (MemberName)
            {
                let TemplateMember = this.Template.Members[MemberName];

                let MemberAdditionalConfig = { ...TemplateMember.AdditionalConfig };
                MemberAdditionalConfig.Static = true;
                MemberAdditionalConfig.InlineMode = E_CPPInlineMode.NONE;
                MemberAdditionalConfig.DefaultValue = `{}`;

                this.BuiltClassOrEnum.AddProperty(
                    MemberName,
                    this.Template.Name,
                    E_CPPAccessibility.PUBLIC,
                    MemberAdditionalConfig
                );
            }.bind(this);
            let AddDynamicMember = function (MemberName)
            {
                DynamicMemberNames.push(MemberName);

                AddDynamicProperty(MemberName);
            }.bind(this);

            // add value property if static linkable
            if (this.IsStaticLinkable)
            {
                let MainTarget = this.Targets[Object.keys(this.Targets)[0]];
                for (let MemberName in this.Template.Members)
                {
                    if (MemberName in MainTarget.Members)
                    {
                        let Member = this.Template.Members[MemberName];

                        let MemberAdditionalConfig = { ...Member };

                        this.BuiltClassOrEnum.AddMember(
                            MemberName,
                            MainTarget.Members[MemberName],
                            E_CPPAccessibility.PUBLIC,
                            MemberAdditionalConfig
                        );
                    }
                }
                for (let MemberName of MainTarget.DummyMemberNames)
                {
                    this.BuiltClassOrEnum.AddMember(
                        MemberName,
                        '',
                        E_CPPAccessibility.PUBLIC
                    );
                }
            }
            // add value property if dynamic linking
            else 
            {
                this.BuiltClassOrEnum.AddProperty(
                    "Value",
                    this.Template.UnderlyingType,
                    E_CPPAccessibility.PUBLIC
                );
                
                let MethodAdditionalConfig = { ...F_CPPStaticClass.DefaultMemberAdditionalConfig };
                MethodAdditionalConfig.InlineMode = E_CPPInlineMode.FORCE;
                MethodAdditionalConfig.Noexcept = true;

                this.BuiltClassOrEnum.AddMethod(
                    `ABYTEK_INTERNAL_FORWARD_WITH_DUMMY(${this.Template.Name}, ConstructDefault)`,
                    ``,
                    {
                    },
                    `
                        = default;
                    `,
                    E_CPPAccessibility.PUBLIC,
                    MethodAdditionalConfig
                );
                
                this.BuiltClassOrEnum.AddMethod(
                    `ABYTEK_INTERNAL_FORWARD_WITH_DUMMY(${this.Template.Name}, ConstructCopy)`,
                    ``,
                    {
                        X: {
                            DataType: `const ${this.Template.Name}&`
                        }
                    },
                    `
                        = default;
                    `,
                    E_CPPAccessibility.PUBLIC,
                    MethodAdditionalConfig
                );
                this.BuiltClassOrEnum.AddMethod(
                    `operator=`,
                    `${this.Template.Name}&`,
                    {
                        X: {
                            DataType: `const ${this.Template.Name}&`
                        }
                    },
                    `
                        = default;
                    `,
                    E_CPPAccessibility.PUBLIC,
                    MethodAdditionalConfig
                );

                MethodAdditionalConfig.Explicit = true;
                this.BuiltClassOrEnum.AddMethod(
                    `ABYTEK_INTERNAL_FORWARD_WITH_DUMMY(${this.Template.Name}, ConstructValue)`,
                    ``,
                    {
                        InValue: {
                            DataType: `const ${this.Template.UnderlyingType}&`
                        }
                    },
                    `
                        : Value(InValue) {}
                    `,
                    E_CPPAccessibility.PUBLIC,
                    MethodAdditionalConfig
                );
                this.BuiltClassOrEnum.AddMethod(
                    `operator ${this.Template.UnderlyingType}`,
                    ``,
                    {},
                    `
                        {
                            return Value;
                        }
                    `,
                    E_CPPAccessibility.PUBLIC,
                    MethodAdditionalConfig
                );
                MethodAdditionalConfig.Explicit = false;

                MethodAdditionalConfig.Const = true;
                this.BuiltClassOrEnum.AddMethod(
                    `operator==`,
                    `bool`,
                    {
                        X: {
                            DataType: `const ${this.Template.Name}&`
                        }
                    },
                    `
                        {
                            return Value == X.Value;
                        }
                    `,
                    E_CPPAccessibility.PUBLIC,
                    MethodAdditionalConfig
                );
                this.BuiltClassOrEnum.AddMethod(
                    `operator!=`,
                    `bool`,
                    {
                        X: {
                            DataType: `const ${this.Template.Name}&`
                        }
                    },
                    `
                        {
                            return Value != X.Value;
                        }
                    `,
                    E_CPPAccessibility.PUBLIC,
                    MethodAdditionalConfig
                );
                MethodAdditionalConfig.Const = false;
            }

            // add dynamic members
            if (!(this.IsStaticLinkable))
            {
                for (let MemberName in this.Template.Members)
                {
                    AddDynamicMember(MemberName);
                }
            }

            // setup Uninstall method body
            {
                for (let MemberName of DynamicMemberNames)
                {
                    UninstallBody += `${this.BuiltClassOrEnum.FullName}::${MemberName} = {};\n`;
                }

                let MemberAdditionalConfig = { ...F_CPPStaticClass.DefaultMemberAdditionalConfig };
                MemberAdditionalConfig.Static = true;

                this.VirtualTableClass.AddMethod(
                    "Uninstall",
                    "void",
                    {},
                    `{${UninstallBody}}`,
                    E_CPPAccessibility.PUBLIC,
                    MemberAdditionalConfig
                );
            }

            // add Install method
            {
                InstallBody += `
                        if(false) {}  
                    `;
                for (let Key in this.Targets)
                {
                    let Target = this.Targets[Key];
                    InstallBody += `
                            else if (InKey == ABYTEK_NAME("${Key}"))
                            {    
                        `;
                    for (let MemberName in this.Template.Members)
                    {
                        if (MemberName in Target.Members)
                        {
                            if (!(this.IsStaticLinkable))
                            {
                                if (Target.DummyMemberNames.includes(MemberName))
                                {
                                    // InstallBody += `${this.BuiltClassOrEnum.FullName}::${MemberName} = {};\n`;
                                }
                                else
                                {
                                    InstallBody += `${this.BuiltClassOrEnum.FullName}::${MemberName} = { ${Target.Members[MemberName]} };\n`;
                                }
                            }
                        }
                        else
                        {
                            InstallBody += `${MemberName} = {};\n`;
                        }
                    }
                    InstallBody += `
                        }
                    `;
                }

                let MemberAdditionalConfig = { ...F_CPPStaticClass.DefaultMemberAdditionalConfig };
                MemberAdditionalConfig.Static = true;

                this.VirtualTableClass.AddMethod(
                    "Install",
                    "void",
                    {
                        InKey: {
                            DataType: `const Abytek::F_Name&`
                        }
                    },
                    `{${InstallBody}}`,
                    E_CPPAccessibility.PUBLIC,
                    MemberAdditionalConfig
                );
            }
            return this;
        }
        
        /**
         * Generates the C++ declaration code for the dynamic enum
         * 
         * @returns {string} The generated C++ declaration code
         */
        GenerateDeclaration()
        {
            this.Validate();
            if (this.BuiltClassOrEnum == null)
            {
                throw new Error("you must build the dynamic enum before generate code");
            }
            return `
                #include "Abytek/Base.Core.hpp"
                
                namespace ${GeneratedVirtualTableClassNamespace}
                {
                    template<typename>
                    class ${GeneratedVirtualTableClassName};
                }
                ${this.BuiltClassOrEnum.GenerateDeclaration()}
                ${this.VirtualTableClass.GenerateDeclaration()}
            `;
        }
        
        /**
         * Generates the C++ implementation code for the dynamic enum
         * 
         * @returns {string} The generated C++ implementation code
         */
        GenerateImplementation()
        {
            this.Validate();
            if (this.BuiltClassOrEnum == null)
            {
                throw new Error("you must build the dynamic enum before generate code");
            }
            return `
                ${this.BuiltClassOrEnum.GenerateImplementation()}
                ${this.VirtualTableClass.GenerateImplementation()}
            `;
        }
    }

    /**
     * Factory function for creating a CPPDynamicEnum instance
     * 
     * @param {Object} Template - Template object defining the enum
     * @returns {F_CPPDynamicEnum} A new dynamic enum generator instance
     */
    function ResultFunctor(...Args)
    {
        return new F_CPPDynamicEnum(...Args);
    }
    ResultFunctor.GeneratedVirtualTableClassName = GeneratedVirtualTableClassName;
    ResultFunctor.GeneratedVirtualTableClassNamespace = GeneratedVirtualTableClassNamespace;
    return ResultFunctor;
}