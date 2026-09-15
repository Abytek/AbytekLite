#pragma once

#include "Abytek/Engine.Core.prerequisites.hpp"


namespace Abytek
{
    class F_CookProfile;
    
    struct ABYTEK_ENGINE_CORE_API I_Cookable
    {
        ABYTEK_BEGIN_REFLECTOR()
        ABYTEK_END_REFLECTOR(I_Cookable);
        
    public:
        friend class F_CoreModule;
        
    public:
#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
        static F_Name GetSerializableEnvironmentMetadataElementName_IsCookMode();
        static B8 GetSerializableEnvironmentMetadataElement_IsCookMode(
            const TW_Valid<F_SerializableEnvironment>& SerializableEnvironment    
        );
        static void SetSerializableEnvironmentMetadataElement_IsCookMode(
            const TW_Valid<F_SerializableEnvironment>& SerializableEnvironment,
            B8 Value
        );
        static void UnsetSerializableEnvironmentMetadataElement_IsCookMode(
            const TW_Valid<F_SerializableEnvironment>& SerializableEnvironment
        );
#endif
        static B8 IsCookModeSerializableEnvironment(const TW_Valid<F_SerializableEnvironment>& SerializableEnvironment); 
        
    public:
        virtual ~I_Cookable() = default;
#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
    public:
        virtual B8 ShouldCook();
        
    protected:
        virtual void PrepareForCooking();
        virtual void Cook();
        virtual void CleanUpAfterCooking();
#endif
    
#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
    public:
        static F_Name GetMainCookFunctionName();
        
    private:
        static void _GlobalInit();
        static void _GlobalRelease();
        
    private:
        static void _CookAsset(const F_Name& AssetPackageName);
#endif
    };
}