#pragma once

#include "Abytek/Engine.NFC.prerequisites.hpp"
#include "Abytek/World/WorldContext.hpp"
#include "Abytek/World/WorldContextHelper.hpp"
#include "Abytek/Renderable.hpp"
#include "Abytek/Development/WorldContextDevelopmentData.hpp"
#include "Abytek/Cookable.hpp"
#include "Abytek/Renderer/StandardPrimitive/MaterialTargetTypeManager.hpp"
#include "Abytek/Renderer/StandardPrimitive/MaterialTargetCommon.hpp"
#include "Abytek/Assets/StandardMaterialCommon.hpp"


namespace Abytek
{
    class F_StandardMaterial;
    
    class ABYTEK_ENGINE_NFC_API A_MaterialTargetType : public A_WorldContext, public A_Renderable, public I_Cookable
    {
    public:
        ABYTEK_BEGIN_REFLECTOR(A_WorldContext)
        ABYTEK_END_REFLECTOR(A_MaterialTargetType);
        
    private:
#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
        TS<F_WorldContextDevelopmentData> _WorldContextDevelopmentData;
#endif
        
    public:
#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
        TS<F_WorldContextDevelopmentData> GetWorldContextDevelopmentData() const final
        {
            return _WorldContextDevelopmentData;
        }
#endif
        
    protected:
        A_MaterialTargetType(const F_SerializableObjectInitParams& InitParams);
        
    public:
        ~A_MaterialTargetType() override;
        
    protected:
        void OnLoad() override;
        void OnUnload() override;
        
    public:
#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
        virtual void PrepareDevelopmentItems(const TW_Valid<F_SerializableEnvironment>& SerializableEnvironment);
        virtual F_Name GetMainShaderModuleName() = 0;
#endif
        
#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
    protected:
        void PrepareForCooking() override;
        void Cook() override;
        void CleanUpAfterCooking() override;
#endif
        
    protected:
        void OnCreateRenderState() override;
        void OnDestroyRenderState() override;
        
    public:
        F_MaterialTargetTypeHashCode GetHashCode() const; 
        template<typename __F>
        static constexpr F_MaterialTargetTypeHashCode GenerateHashCode()
        {
            return H_GeneralTypeHashCode::MakeStatic<__F>();
        }
        
    public:
        template<typename __F_Callback>
        static void ForEachMaterialTargetType(const TW_Valid<A_WorldContext>& WorldContext, __F_Callback&& Callback)
        {
            for (
                const auto& MaterialTargetType : 
                H_WorldContext::GetUnit<F_MaterialTargetTypeManager>(
                    WorldContext
                )->GetTypes()
            )
            {
                Callback(MaterialTargetType);
            }
        }
        
    public:
#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
        virtual B8 ShouldCompilePermutation(
            const TS<F_RenderRegistry>& RenderRegistry,
            const TW_Valid<A_RenderPackTemplateMap>& RenderPackTemplateMap,
            const TS<F_StandardMaterial>& Material,
            const F_MaterialPropertyInstanceList& MaterialPermutation,
            const F_StandardMaterialCompileTarget& MaterialCompileTarget
        );
        virtual void SetupCompilePermutation(
            const TS<F_RenderRegistry>& RenderRegistry,
            const TW_Valid<A_RenderPackTemplateMap>& RenderPackTemplateMap,
            const TS<F_StandardMaterial>& Material,
            const F_MaterialPropertyInstanceList& MaterialPermutation,
            const F_StandardMaterialCompileTarget& MaterialCompileTarget,
            F_MaterialCompilePermutation& CompilePermutation
        );
#endif
    };
}
