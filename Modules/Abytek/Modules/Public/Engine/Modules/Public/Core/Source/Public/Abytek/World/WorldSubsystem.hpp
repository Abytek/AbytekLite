#pragma once

#include "Abytek/Subsystem.hpp"
#include "Abytek/World/WorldContext.hpp"


namespace Abytek
{
    class F_World;

    class ABYTEK_ENGINE_CORE_API A_WorldSubsystem : public F_Subsystem, public I_GetWorld
    {
    public:
        friend class F_World;
        friend class F_WorldSubsystemContainer;
        
    public:
        ABYTEK_BEGIN_REFLECTOR(F_Subsystem)
        ABYTEK_END_REFLECTOR(A_WorldSubsystem);
        
    public:
        static F_Name GetTickUpdateFunctionName();
        static F_Name GetPostTickUpdateFunctionName();
        
    public:
        static void GlobalInit();
        static void GlobalRelease();
        
    private:
        TW<F_World> _World;
        
    public:
        TW_Valid<F_World> GetWorld() const override
        {
            return _World;
        }
    
    protected:
        A_WorldSubsystem(const F_ProgramUnitBuildParams& BuildParams);
        
    public:
        ~A_WorldSubsystem() override;
        
    protected:
        virtual void OnTick();
        virtual void OnPostTick();
        
    protected:
        virtual void OnStartup();
        virtual void OnPostStartup();
        virtual void OnPrepareTick();
        virtual void OnPreShutdown();
        virtual void OnShutdown();
    };
}
