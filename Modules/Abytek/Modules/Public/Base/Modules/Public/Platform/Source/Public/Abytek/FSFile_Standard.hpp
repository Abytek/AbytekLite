#pragma once

#include "Abytek/FSFile.hpp"


namespace Abytek
{
    class ABYTEK_BASE_PLATFORM_API A_FSFile_Standard : public A_FSFile
    {
    private:

    public:
        
    protected:
        A_FSFile_Standard(const F_FSFileOpenParams& OpenParams);
        
    public:
        ~A_FSFile_Standard() override;
    };
}