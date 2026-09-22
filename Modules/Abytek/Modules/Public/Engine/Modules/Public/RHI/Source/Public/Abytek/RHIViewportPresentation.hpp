#pragma once

#include "Abytek/RHISubmissionItem.hpp"


namespace Abytek
{
    class A_RHIViewport;

    struct F_RHIViewportPresentationBuildParams
    {
        TS<A_RHIViewport> Viewport;
    };
    
    class ABYTEK_ENGINE_RHI_API A_RHIViewportPresentation : public A_RHISubmissionItem
    {
    private:
        TS<A_RHIViewport> _Viewport;

    public:
        ABYTEK_FORCE_INLINE const auto& GetViewport() const noexcept
        {
            return _Viewport;
        }
        
    public:
        ABYTEK_RA_DECLARE_OBJECT_CREATABLE(A_RHIViewportPresentation);
        virtual void Build(const F_RHIViewportPresentationBuildParams& BuildParams);
        void Release() override;
        
    public:
        TW_Valid<A_RHIProcess> GetProcess() const override;
    };
}
