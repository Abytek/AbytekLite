#pragma once

#include "Abytek/Engine.Window.prerequisites.hpp"


namespace Abytek
{
    class ABYTEK_ENGINE_WINDOW_API A_WindowSubsystemProxy : public A_RAObject
    {
    public:
        ABYTEK_DECLARE_OBJECT_SINGLETON_CRT(A_WindowSubsystemProxy);
        
    private:
        
    public:

    public:
        ABYTEK_RA_DECLARE_OBJECT_CREATABLE(A_WindowSubsystemProxy);
        virtual void Build();
        virtual void Release() override;
        
    public:
        virtual E_WindowAPI GetAPI() = 0;
    };
}
