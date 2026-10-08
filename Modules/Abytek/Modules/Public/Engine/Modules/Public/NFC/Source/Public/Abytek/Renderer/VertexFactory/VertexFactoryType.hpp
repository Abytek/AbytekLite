#pragma once

#include "Abytek/Engine.NFC.prerequisites.hpp"
#include "Abytek/World/WorldContext.hpp"
#include "Abytek/Renderable.hpp"
#include "Abytek/Development/WorldContextDevelopmentData.hpp"
#include "Abytek/Cookable.hpp"


namespace Abytek
{
    using F_VertexFactoryTypeId = U32;
    static constexpr F_VertexFactoryTypeId INVALID_VERTEX_FACTORY_TYPE_ID = ~F_VertexFactoryTypeId(0);
    
    class ABYTEK_ENGINE_NFC_API A_VertexFactoryType : public A_WorldContext, public A_Renderable, public I_Cookable
    {
    public:
        ABYTEK_BEGIN_REFLECTOR(A_WorldContext)
        ABYTEK_END_REFLECTOR(A_VertexFactoryType);
        
    private:
#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
        TS<F_WorldContextDevelopmentData> _WorldContextDevelopmentData;
#endif
        
    public:
#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
        ABYTEK_FORCE_INLINE const auto& GetWorldContextDevelopmentData() const noexcept
        {
            return _WorldContextDevelopmentData;
        }
#endif
        
    protected:
        A_VertexFactoryType(const F_SerializableObjectInitParams& InitParams);
        
    public:
        ~A_VertexFactoryType() override;
        
    protected:
        void OnLoad() override;
        void OnUnload() override;
        
    public:
#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
        virtual void PrepareDevelopmentItems(const TW_Valid<F_SerializableEnvironment>& SerializableEnvironment);
        virtual F_Text GetMainShaderHeaderPath() = 0;
#endif
        
#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
    protected:
        void PrepareForCooking() override;
        void Cook() override;
        void CleanUpAfterCooking() override;
#endif
    };
}
