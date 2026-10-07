#pragma once

#include "Abytek/Engine.NFC.prerequisites.hpp"
#include "Abytek/Assets/MaterialInterface.hpp"


namespace Abytek
{
    class ABYTEK_ENGINE_NFC_API F_MaterialInstance : public A_MaterialInterface
    {
    public:
        ABYTEK_BEGIN_REFLECTOR(A_MaterialInterface)
        ABYTEK_END_REFLECTOR(F_MaterialInstance);
        
    private:
        TS<A_Material> _Material;
        
    public:
        
    public:
        F_MaterialInstance(const F_SerializableObjectInitParams& InitParam);
        ~F_MaterialInstance() override;
        
    protected:
        void OnLoad() override;
        void OnUnload() override;
        
    public:
        TS<A_Material> GetMaterial() const override;
        TS<F_MaterialInstance> GetMaterialInstance() const override;
    };
}