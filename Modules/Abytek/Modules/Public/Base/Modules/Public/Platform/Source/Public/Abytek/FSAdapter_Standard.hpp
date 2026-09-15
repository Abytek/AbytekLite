#pragma once

#include "Abytek/FSAdapter.hpp"


namespace Abytek
{
    class ABYTEK_BASE_PLATFORM_API A_FSAdapter_Standard : public A_FSAdapter
    {
    public:
        ABYTEK_DECLARE_OBJECT_SINGLETON_CRT(A_FSAdapter_Standard);
        
    protected:
        A_FSAdapter_Standard();
        
    public:
        ~A_FSAdapter_Standard() override;
        
    public:
        static TU<A_FSAdapter_Standard> Create();
    };
}