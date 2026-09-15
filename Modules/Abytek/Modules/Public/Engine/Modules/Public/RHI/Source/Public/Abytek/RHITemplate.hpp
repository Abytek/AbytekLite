#pragma once

#include "Abytek/Engine.RHI.prerequisites.hpp"
#include "Abytek/RHITemplateHashCode.hpp"


namespace Abytek
{
    class A_RHIContext;
    class A_RHITemplateDatabase;
    class A_RHITemplateRuntime;

    struct F_RHITemplateBuildParamsBase
    {
        TW<A_RHITemplateDatabase> Database;
        TF_Optional<F_RHITemplateHashCode> CustomHashCode;
        TF_Optional<F_RHITemplateHashCode> CustomBaseDependencyHashCode;
    };
    
    class ABYTEK_ENGINE_RHI_API A_RHITemplate : public A_Object
    {
    public:
        friend class A_RHITemplateDatabase;
        friend class A_RHITemplateRuntimeDatabase;
        
    private:
        TW<A_RHITemplateDatabase> _Database;
        TF_Vector<F_RHITemplateHashCode> _DependencyHashCodes;
        F_RHITemplateHashCode _HashCode;

    public:
        ABYTEK_FORCE_INLINE const auto& GetDatabase() const noexcept
        {
            return _Database;
        }
        ABYTEK_FORCE_INLINE const auto& GetDependencyHashCodes() const noexcept
        {
            return _DependencyHashCodes;
        }
        ABYTEK_FORCE_INLINE auto GetHashCode() const noexcept
        {
            return _HashCode;
        }
        
    protected:
        A_RHITemplate(const F_RHITemplateBuildParamsBase& BuildParams);

    public:
        ~A_RHITemplate() override;

    public:
        B8 HasDependencyHashCode(F_RHITemplateHashCode DependencyHashCode);
        void AddDependencyHashCode(F_RHITemplateHashCode DependencyHashCode);
        void RemoveDependencyHashCode(F_RHITemplateHashCode DependencyHashCode);
        void EnsureDependencyHashCode(F_RHITemplateHashCode DependencyHashCode)
        {
            if (!HasDependencyHashCode(DependencyHashCode))
            {
                AddDependencyHashCode(DependencyHashCode);
            }
        }

    protected:
        virtual TS_Valid<A_RHITemplateRuntime> CreateAndBuildRuntime(const TW_Valid<A_RHIContext>& Context);
    };
}
