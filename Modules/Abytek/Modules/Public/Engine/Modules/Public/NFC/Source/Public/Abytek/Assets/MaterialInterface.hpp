#pragma once

#include "Abytek/Engine.NFC.prerequisites.hpp"
#include "Abytek/World/WorldContext.hpp"


namespace Abytek
{
    class A_Material;
    class F_MaterialInstance;
    
    class ABYTEK_ENGINE_NFC_API A_MaterialInterface : public A_WorldContext
    {
    public:
        ABYTEK_BEGIN_REFLECTOR(A_WorldContext)
        ABYTEK_END_REFLECTOR(A_MaterialInterface);
        
    private:
        
    public:
        
    protected:
        A_MaterialInterface(const F_SerializableObjectInitParams& InitParam);
        
    public:
        ~A_MaterialInterface() override;
        
    protected:
        void OnLoad() override;
        void OnUnload() override;
        
    public:
        virtual TS<A_Material> GetMaterial() const = 0;
        virtual TS<F_MaterialInstance> GetMaterialInstance() const = 0;
    };
}