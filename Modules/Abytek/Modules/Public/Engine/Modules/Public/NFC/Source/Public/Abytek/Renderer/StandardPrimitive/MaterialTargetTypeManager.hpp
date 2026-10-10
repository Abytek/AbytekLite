#pragma once

#include "Abytek/Engine.NFC.prerequisites.hpp"
#include "Abytek/World/WorldSubsystem.hpp"


namespace Abytek
{
    class A_MaterialTargetType;
    
    class ABYTEK_ENGINE_NFC_API F_MaterialTargetTypeManager : public A_WorldSubsystem
    {
    public:
        ABYTEK_BEGIN_REFLECTOR(A_WorldSubsystem)
        ABYTEK_END_REFLECTOR(F_MaterialTargetTypeManager);
        
    public:
        ABYTEK_DECLARE_STATIC_SUBSYSTEM(F_MaterialTargetTypeManager);
        
    public:
        friend class A_MaterialTargetType;
        
    private:
        TF_Vector<TS<A_MaterialTargetType>> _Types;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetTypes() const noexcept
        {
            return _Types;
        }
    
    public:
        F_MaterialTargetTypeManager(const F_ProgramUnitBuildParams& BuildParams);
        ~F_MaterialTargetTypeManager() override;
        
    private:
        void _RegisterType(const TS<A_MaterialTargetType>& Type);
        void _UnregisterType(const TS<A_MaterialTargetType>& Type);
        
    protected:
        void OnInit() override;
        void OnRelease() override;
    };
}
