/**
 * CPPDynamicClass.js
 * 
 * Provides utilities for generating dynamic C++ classes that can have different
 * implementations in different modules or compilation units. This enables runtime
 * binding of class methods and properties and is used for cross-module extensibility.
 */
module.exports = (Module) => {
    // Target:
    // - FullName
    // - Members (as name array)
    
    let F_CPPStaticClass = Module.ImportScript("Abytek/Utilities/CPPStaticClass.js");
    let E_CPPAccessibility = Module.ImportScript("Abytek/Utilities/CPPAccessibility.js");
    let E_CPPInlineMode = Module.ImportScript("Abytek/Utilities/CPPInlineMode.js");
    let CPPMethodArgsToString = Module.ImportScript("Abytek/Utilities/CPPMethodArgsToString.js");
    
    const GeneratedVirtualTableClassName = "TH_GeneratedVirtualTable";
    const GeneratedVirtualTableClassNamespace = "Abytek";
    
    /**
     * Class for generating C++ dynamic class types
     */
    class F_CPPDynamicClass
    {
        /**
         * Creates a new C++ dynamic class generator
         * 
         * @param {Object} Template - Template object defining the class structure
         */
        constructor(Template)
        {
            this.Template = Template;
            this.BuiltClass = null;
            this.VirtualTableClass = null;
            this.Targets = new Object();
        }
        
        /**
         * Validates the class configuration
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
         * Sets the template object defining the class
         * 
         * @param {Object} Template - The template object
         * @returns {F_CPPDynamicClass} This instance for chaining
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
         * @returns {F_CPPDynamicClass} This instance for chaining
         */
        SetTargets(Targets)
        {
            this.Targets = Targets;
            return this;
        }
        
        /**
         * Adds a target implementation for the class
         * 
         * @param {string} Key - Identifier for the target
         * @param {string} FullName - Full name of the implementing class
         * @param {Array<string>} Members - List of member names implemented by this target
         * @returns {F_CPPDynamicClass} This instance for chaining
         */
        AddTarget(Key, FullName, Members)
        {
            this.Targets[Key] = {
                FullName: FullName,
                Members: Members,
            };
            return this;
        }
        
        /**
         * Builds the dynamic class
         * 
         * @returns {F_CPPDynamicClass} This instance for chaining
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
         * @returns {F_CPPDynamicClass} This instance for chaining
         */
        Analyze()
        {
            this.IsStaticLinkable = (Object.keys(this.Targets).length === 1);
            this.AnalyzeMemberNameToIsAbstract();
            return this;
        }
        
        /**
         * Analyzes which members are abstract (not implemented by any target)
         * 
         * @returns {F_CPPDynamicClass} This instance for chaining
         */
        AnalyzeMemberNameToIsAbstract()
        {
            this.MemberNameToIsAbstract = new Object();
            for (let MemberName in this.Template.Members)
            {
                this.MemberNameToIsAbstract[MemberName] = true;
            }
            for (let Key in this.Targets)
            {
                let Target = this.Targets[Key];
                for (let MemberName of Target.Members)
                {
                    this.MemberNameToIsAbstract[MemberName] = false;
                }
            }
            return this;
        }
        
        /**
         * Adds members to the built class based on targets and template
         * 
         * @returns {F_CPPDynamicClass} This instance for chaining
         */
        AddBuiltMembers()
        {
            this.BuiltClass = F_CPPStaticClass(this.Template.Name, this.Template.Namespace)
                .SetAPI(this.Template.API)
                .SetFinal(true);
            this.BuiltClass.SetEnableFlagOperators(this.Template.EnableFlagOperators);
            this.VirtualTableClass = F_CPPStaticClass(
                `${GeneratedVirtualTableClassName}<${this.BuiltClass.FullName}>`, 
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
            let AddDynamicMethod = function (MemberName)
            {
                let TemplateMember = this.Template.Members[MemberName];
                
                let MemberAdditionalConfig = { ...TemplateMember.AdditionalConfig };
                MemberAdditionalConfig.Static = true;
                MemberAdditionalConfig.InlineMode = E_CPPInlineMode.NONE;
                MemberAdditionalConfig.DefaultValue = `0`;
                
                this.BuiltClass.AddProperty(
                    MemberName,
                    `Abytek::TF_RawForward<${TemplateMember.ReturnType}(${CPPMethodArgsToString(TemplateMember.Args)})>*`,
                    E_CPPAccessibility.PUBLIC,
                    MemberAdditionalConfig
                );
            }.bind(this);
            let AddDynamicProperty = function (MemberName)
            {
                let TemplateMember = this.Template.Members[MemberName];

                let MemberAdditionalConfig = { ...TemplateMember.AdditionalConfig };
                MemberAdditionalConfig.Static = true;
                MemberAdditionalConfig.InlineMode = E_CPPInlineMode.NONE;
                MemberAdditionalConfig.DefaultValue = `0`;

                this.BuiltClass.AddProperty(
                    MemberName,
                    `Abytek::TF_RawForward<${TemplateMember.DataType}>*`,
                    E_CPPAccessibility.PUBLIC,
                    MemberAdditionalConfig
                );
            }.bind(this);
            let AddDynamicMember = function (MemberName)
            {
                DynamicMemberNames.push(MemberName);
                
                let TemplateMember = this.Template.Members[MemberName];
                if (TemplateMember.MemberType === "Method")
                {
                    AddDynamicMethod(MemberName);
                }
                else if (TemplateMember.MemberType === "Property")
                {
                    AddDynamicProperty(MemberName);
                }
                else
                {
                    throw new Error(`Unknown member type "${TemplateMember.MemberType}"`);
                }
            }.bind(this);
            
            // set parent class for built class
            if (this.IsStaticLinkable)
            {
                this.BuiltClass.Inherit(
                    this.Targets[Object.keys(this.Targets)[0]].FullName,
                    E_CPPAccessibility.PUBLIC
                );
            }

            // add dynamic members for non-abstract members
            if (!(this.IsStaticLinkable))
            {
                for (let MemberName in this.Template.Members)
                {
                    let IsAbstract = this.MemberNameToIsAbstract[MemberName];
                    if (!IsAbstract)
                    {
                        AddDynamicMember(MemberName);
                    }
                }
            }

            // add dynamic members for abstract members
            for (let MemberName in this.Template.Members)
            {
                let IsAbstract = this.MemberNameToIsAbstract[MemberName];
                if (IsAbstract)
                {
                    AddDynamicMember(MemberName);
                }
            }
            
            // setup Uninstall method body
            {
                for (let MemberName of DynamicMemberNames)
                {
                    UninstallBody += `${MemberName} = 0;\n`;
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
                        if (Target.Members.includes(MemberName))
                        {
                            if (!(this.IsStaticLinkable))
                            {
                                InstallBody += `${this.BuiltClass.FullName}::${MemberName} = &${Target.FullName}::${MemberName};\n`;
                            }
                        }
                        else
                        {
                            InstallBody += `${MemberName} = 0;\n`;
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
         * Generates the C++ declaration code for the dynamic class
         * 
         * @returns {string} The generated C++ declaration code
         */
        GenerateDeclaration()
        {
            this.Validate();
            if (this.BuiltClass == null)
            {
                throw new Error("you must build the dynamic class before generate code");
            }
            return `
                #include "Abytek/Base.Core.hpp"
                
                namespace ${GeneratedVirtualTableClassNamespace}
                {
                    template<typename>
                    class ${GeneratedVirtualTableClassName};
                }
                ${this.BuiltClass.GenerateDeclaration()}
                ${this.VirtualTableClass.GenerateDeclaration()}
            `;
        }
        
        /**
         * Generates the C++ implementation code for the dynamic class
         * 
         * @returns {string} The generated C++ implementation code
         */
        GenerateImplementation()
        {
            this.Validate();
            if (this.BuiltClass == null)
            {
                throw new Error("you must build the dynamic class before generate code");
            }
            return `
                ${this.BuiltClass.GenerateImplementation()}
                ${this.VirtualTableClass.GenerateImplementation()}
            `;
        }
    }
    
    /**
     * Factory function for creating a CPPDynamicClass instance
     * 
     * @param {Object} Template - Template object defining the class structure
     * @returns {F_CPPDynamicClass} A new dynamic class generator instance
     */
    function ResultFunctor(...Args)
    {
        return new F_CPPDynamicClass(...Args);
    }
    ResultFunctor.GeneratedVirtualTableClassName = GeneratedVirtualTableClassName;
    ResultFunctor.GeneratedVirtualTableClassNamespace = GeneratedVirtualTableClassNamespace;
    
    // Add the default member config as a static property of the factory
    ResultFunctor.DefaultMemberAdditionalConfig = F_CPPStaticClass.DefaultMemberAdditionalConfig;
    
    return ResultFunctor;
}