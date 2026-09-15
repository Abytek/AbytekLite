#pragma once

#include "Abytek/Base.Platform.prerequisites.pch.hpp"


namespace Abytek
{
    class A_FSAdapter_Standard;
    class A_PathManager;
    
    class ABYTEK_BASE_PLATFORM_API A_Platform : public A_Object
    {
    public:
        ABYTEK_DECLARE_OBJECT_SINGLETON_CRT(A_Platform);

    private:
        TU<A_FSAdapter_Standard> _FSAdapter_Standard;
        TU<A_PathManager> _PathManager;
        
    protected:
        A_Platform();
        
    public:
        virtual ~A_Platform() override;
        
    public:
        static TU<A_Platform> Create();
        
    protected:
        virtual void OnInit();
        virtual void OnPostInit();
        virtual void OnPreRelease();
        virtual void OnRelease();
    
    public:
        void Startup();
        void Shutdown();
        
    public:
        virtual void OnBeginCurrentThread();
        virtual void OnEndCurrentThread();
    };
}