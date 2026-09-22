#pragma once

#include "Abytek/World/WorldContext.hpp"


namespace Abytek
{
    class F_World;
    
    class ABYTEK_ENGINE_CORE_API F_WorldBusiness : public A_WorldContext
    {
    public:
        ABYTEK_BEGIN_REFLECTOR(A_WorldContext)
        ABYTEK_END_REFLECTOR(F_WorldBusiness);
    
    private:
        B8 _BeganPlay = false;
        
    public:
        ABYTEK_FORCE_INLINE B8 BeganPlay() const noexcept
        {
            return _BeganPlay;
        }
        
    public:
        F_WorldBusiness(const F_SerializableObjectInitParams& InitParams);
        ~F_WorldBusiness() override;
        
    public:
        virtual void OnBeginPlay();
        virtual void OnEndPlay();
        
    public:
        void BeginPlay();
        void EndPlay();
    };
}