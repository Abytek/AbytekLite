#pragma once

#include "Abytek/PathManager.hpp"


#ifdef ABYTEK_PLATFORM_WINDOWS
namespace Abytek
{
    class ABYTEK_BASE_PLATFORM_API F_WindowsPathManager : public A_PathManager
    {
    private:
        
    public:
        
    public:
        F_WindowsPathManager();
        virtual ~F_WindowsPathManager() override;
        
    public:
        virtual TF_Optional<F_Text> GetCommonDirectoryPath(E_CommonDirectory CommonDirectory) override;
    };
}
#endif