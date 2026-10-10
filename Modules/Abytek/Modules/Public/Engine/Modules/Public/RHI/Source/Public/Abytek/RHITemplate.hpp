#pragma once

#include "Abytek/Engine.RHI.prerequisites.hpp"
#include "Abytek/RHITemplateHashCode.hpp"


namespace Abytek
{
    class A_RHIContext;
    class A_RHITemplateDatabase;
    class A_RHITemplateRuntime;
    struct A_RHITemplateExportedData;
    
    struct F_RHITemplateCommonSettings
    {
        F_RHITemplateHashCode HashCode = 0;
    };
    
    struct A_RHITemplateCompileParams : F_RHITemplateCommonSettings
    {        
        TS<A_RHITemplateDatabase> Database;
        TF_Optional<F_RHITemplateHashCode> CustomBaseDependencyHashCode;
    };
    
    class ABYTEK_ENGINE_RHI_API A_RHITemplate : public A_Object
    {
    public:
        friend class A_RHITemplateDatabase;
        friend class A_RHITemplateRuntimeDatabase;
        
    private:
        TS<A_RHITemplateDatabase> _Database;
        TF_Vector<F_RHITemplateHashCode> _DependencyHashCodes;
        F_RHITemplateHashCode _HashCode;
        B8 _IsRelaxed = false;

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
        ABYTEK_FORCE_INLINE auto IsRelaxed() const noexcept
        {
            return _IsRelaxed;
        }
        
    protected:
        A_RHITemplate(
            const TS<A_RHITemplateDatabase>& Database,
            F_RHITemplateHashCode HashCode
        );

    public:
        ~A_RHITemplate() override;
        
    public:
        void Relax();

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
        
    protected:
        virtual TS<A_RHITemplateExportedData> CreateExportedData() const = 0;
        virtual void PostCreateExportedData(const TS<A_RHITemplateExportedData>& ExportedData) const;
        
    public:
        TS<A_RHITemplateExportedData> ExportData() const
        {
            auto Result = CreateExportedData();
            PostCreateExportedData(Result);
            return Result;
        }
        
    public:
        virtual B8 IsRootTemplate() const { return false; }
        
    public:
        static void GatherSortedListWithDependencies(
            const TW_Valid<A_RHITemplateDatabase>& TemplateDatabase, 
            const TF_Vector<TS<A_RHITemplate>>& Templates,
            TF_Vector<TS<A_RHITemplate>>& OutList,
            B8 SkipUnlistedRoot = false
        );
    };
    
    struct ABYTEK_ENGINE_RHI_API A_RHITemplateExportedData : A_Object
    {
        F_RHITemplateHashCode HashCode = 0;
        
    protected:
        virtual TS<A_RHITemplate> CreateTemplate(const TS<A_RHITemplateDatabase>& Database) const = 0;
        virtual void PostCreateTemplate(const TS<A_RHITemplate>& Template) const {}
        
    public:
        TS<A_RHITemplate> Import(const TS<A_RHITemplateDatabase>& Database) const
        {
            auto Template = CreateTemplate(Database);
            PostCreateTemplate(Template);
            return Template;
        }
    };
}
