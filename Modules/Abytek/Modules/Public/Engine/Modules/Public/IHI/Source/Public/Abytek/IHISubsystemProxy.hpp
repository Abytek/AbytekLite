#pragma once

#include "Abytek/Engine.IHI.prerequisites.hpp"
#include "Abytek/IHIAPI.hpp"


namespace Abytek
{
    class ABYTEK_ENGINE_IHI_API A_IHISubsystemProxy : public A_RAObject
    {
    private:
        
    public:
        
    public:
        ABYTEK_RA_DECLARE_OBJECT_CREATABLE(A_IHISubsystemProxy);
        virtual void Build();
        virtual void Release() override;

    public:
        virtual E_IHIAPI GetAPI() = 0;
    };
}
