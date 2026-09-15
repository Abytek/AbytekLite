#include "Abytek/RHITemplate.hpp"
#include "Abytek/RHITemplateDatabase.hpp"


namespace Abytek
{
    A_RHITemplate::A_RHITemplate(const F_RHITemplateBuildParamsBase& BuildParams) :
        _Database(BuildParams.Database),
        _HashCode(BuildParams.CustomHashCode ? (*BuildParams.CustomHashCode) : RHITemplateObjectBasedHashCode(this))
    {
        _Database->_TrackTemplate(ABYTEK_WTHIS());
    }
    A_RHITemplate::~A_RHITemplate()
    {
        _Database->_UntrackTemplate(ABYTEK_WTHIS());
    }

    B8 A_RHITemplate::HasDependencyHashCode(F_RHITemplateHashCode DependencyHashCode)
    {
        return (
            std::find(
                _DependencyHashCodes.begin(),    
                _DependencyHashCodes.end(),
                DependencyHashCode
            )    
            != _DependencyHashCodes.end()
        );
    }
    void A_RHITemplate::AddDependencyHashCode(F_RHITemplateHashCode DependencyHashCode)
    {
        ABYTEK_ENGINE_RHI_ASSERT(!HasDependencyHashCode(DependencyHashCode)) << "This dependency was already added";
        _DependencyHashCodes.push_back(DependencyHashCode);
    }
    void A_RHITemplate::RemoveDependencyHashCode(F_RHITemplateHashCode DependencyHashCode)
    {
        ABYTEK_ENGINE_RHI_ASSERT(HasDependencyHashCode(DependencyHashCode)) << "Not found dependency";
        _DependencyHashCodes.erase(
            std::find(
                _DependencyHashCodes.begin(),    
                _DependencyHashCodes.end(),
                DependencyHashCode
            )       
        );
    }
    
    TS_Valid<A_RHITemplateRuntime> A_RHITemplate::CreateAndBuildRuntime(const TW_Valid<A_RHIContext>& Context)
    {
        return {};
    }
}
