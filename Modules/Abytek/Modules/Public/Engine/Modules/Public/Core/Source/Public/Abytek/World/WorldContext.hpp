#pragma once

#include "Abytek/Engine.Core.prerequisites.hpp"


namespace Abytek
{
    class F_World;

    struct I_GetWorld
    {
        virtual ~I_GetWorld() = default;
        virtual TW_Valid<F_World> GetWorld() const = 0;
    };
    
    class ABYTEK_ENGINE_CORE_API A_WorldContext : public A_SerializableObject, public I_GetWorld
    {
    public:
        ABYTEK_BEGIN_REFLECTOR(A_SerializableObject)
        ABYTEK_END_REFLECTOR(A_WorldContext);
    
    private:
        
    public:
        TW_Valid<F_World> GetWorld() const override;
        
    public:
        A_WorldContext(const F_SerializableObjectInitParams& InitParams);
        ~A_WorldContext() override;
    };
}