#pragma once

#include "Abytek/Engine.Core.prerequisites.hpp"
#include "Abytek/RHIAPI.hpp"


namespace Abytek
{
    class ABYTEK_ENGINE_RHI_API A_RHISubsystemProxy : public A_RAObject
    {
    public:
        ABYTEK_DECLARE_OBJECT_SINGLETON_CRT(A_RHISubsystemProxy);

    public:
        ABYTEK_RA_DECLARE_OBJECT_CREATABLE(A_RHISubsystemProxy);
        virtual void Build();
        virtual void Release() override;

    public:
        virtual E_RHIAPI GetAPI() = 0;
        
    public:
        virtual void OnBeginFrameParam(E_FrameParamType FrameParamType);
        virtual void OnEndFrameParam(E_FrameParamType FrameParamType);
    };
}
