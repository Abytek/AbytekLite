#pragma once

#include "Abytek/Renderer/StandardPrimitive/GeometryFactoryType.hpp"


namespace Abytek
{
    class ABYTEK_ENGINE_NFC_API A_GeometryFactoryType_StaticMeshECMS : public A_GeometryFactoryType
    {
    public:
        ABYTEK_BEGIN_REFLECTOR(A_GeometryFactoryType)
        ABYTEK_END_REFLECTOR(A_GeometryFactoryType_StaticMeshECMS);
        
    private:
        
    public:
        
    protected:
        A_GeometryFactoryType_StaticMeshECMS(const F_SerializableObjectInitParams& InitParams);
        
    public:
        ~A_GeometryFactoryType_StaticMeshECMS() override;
    };
}