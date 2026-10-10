#pragma once

#include "Abytek/Engine.NFC.prerequisites.hpp"
#include "Abytek/World/WorldSubsystem.hpp"


namespace Abytek
{
    class A_GeometryFactoryType;
    
    class ABYTEK_ENGINE_NFC_API F_GeometryFactoryTypeManager : public A_WorldSubsystem
    {
    public:
        ABYTEK_BEGIN_REFLECTOR(A_WorldSubsystem)
        ABYTEK_END_REFLECTOR(F_GeometryFactoryTypeManager);
        
    public:
        ABYTEK_DECLARE_STATIC_SUBSYSTEM(F_GeometryFactoryTypeManager);
        
    public:
        friend class A_GeometryFactoryType;
        
    private:
        TF_Vector<TS<A_GeometryFactoryType>> _Types;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetTypes() const noexcept
        {
            return _Types;
        }
    
    public:
        F_GeometryFactoryTypeManager(const F_ProgramUnitBuildParams& BuildParams);
        ~F_GeometryFactoryTypeManager() override;
        
    private:
        void _RegisterType(const TS<A_GeometryFactoryType>& Type);
        void _UnregisterType(const TS<A_GeometryFactoryType>& Type);
        
    protected:
        void OnInit() override;
        void OnRelease() override;
    };
}
