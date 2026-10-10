#pragma once

#include "Abytek/Engine.NFC.prerequisites.hpp"
#include "Abytek/World/WorldContext.hpp"
#include "Abytek/Assets/MaterialCommon.hpp"
#include "Abytek/Assets/MaterialProperty.hpp"


namespace Abytek
{
    class A_Material;
    
    class ABYTEK_ENGINE_NFC_API A_MaterialInterface : public A_WorldContext, public A_Renderable
    {
    public:
        ABYTEK_BEGIN_REFLECTOR(A_WorldContext)
        ABYTEK_END_REFLECTOR(A_MaterialInterface);
        
    private:
        F_MaterialPropertyInstanceList _OverridePropertyInstanceList;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetPropertyInstanceList() const noexcept
        {
            return _OverridePropertyInstanceList;
        }
        
    protected:
        A_MaterialInterface(const F_SerializableObjectInitParams& InitParam);
        
    public:
        ~A_MaterialInterface() override;
        
    protected:
        void OnLoad() override;
        void OnUnload() override;
        
    public:
        virtual TS<A_Material> GetMaterial() const = 0;
        
    protected:
        void OnCreateRenderState() override;
        void OnDestroyRenderState() override;
    };
}