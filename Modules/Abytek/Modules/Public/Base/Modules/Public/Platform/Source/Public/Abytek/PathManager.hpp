#pragma once

#include "Abytek/Base.Platform.prerequisites.pch.hpp"
#include "Abytek/Path.hpp"


namespace Abytek
{
    class ABYTEK_BASE_PLATFORM_API A_PathManager : public A_Object
    {
    public:
        ABYTEK_DECLARE_OBJECT_SINGLETON_CRT(A_PathManager);

    protected:
        A_PathManager();
        
    public:
        virtual ~A_PathManager() override;
        
    public:
        static TU<A_PathManager> Create();
        
    public:
        virtual TF_Optional<F_Text> GetCommonDirectoryPath(E_CommonDirectory CommonDirectory) = 0;
    };
}