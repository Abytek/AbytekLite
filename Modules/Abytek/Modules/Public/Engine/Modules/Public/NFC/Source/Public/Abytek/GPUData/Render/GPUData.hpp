#pragma once

#include "Abytek/RenderBase/RenderObject.hpp"
#include "Abytek/GPUData/Render/GPUDataComponentType.hpp"
#include "Abytek/GPUData/Render/GPUDataIndexSet.hpp"


namespace Abytek
{
    class A_RenderScene;
    class F_GPUDataInstanceSet;
    class F_GPUDataIndexSet;
    class F_GPUDataComponentType;

    struct F_GPUDataBuildParams
    {
        TW<A_RenderScene> Scene;
    };
    class ABYTEK_ENGINE_NFC_API F_GPUData final : public A_RenderObject
    {
    public:
        friend class F_GPUDataInstanceSet;
        friend class F_GPUDataComponentType;
        friend class F_GPUDataIndexSet;
        
    private:
        TW<A_RenderScene> _Scene;
        
        F_YieldCriticalSection _CriticalSection;
        
        TF_Set<TW<F_GPUDataInstanceSet>> _InstanceSets;
        TF_Map<F_GeneralTypeHashCode, TW<F_GPUDataComponentType>> _ComponentTypes;
        TF_MultiMap<F_GeneralTypeHashCode, TW<F_GPUDataIndexSet>> _ComponentTypeHashCodeToIndexSet;
        
        TF_Map<F_GeneralTypeHashCode, TS<F_GPUDataComponentType>> _GlobalComponentTypes;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetScene() const noexcept
        {
            return _Scene;
        }
        
        ABYTEK_FORCE_INLINE const auto& GetGlobalComponentTypes() const noexcept
        {
            return _GlobalComponentTypes;
        }
        
    public:
        F_GPUData(const TW_Valid<F_WorldRenderResource>& WorldRenderResource, const F_GPUDataBuildParams& BuildParams);
        ~F_GPUData() override;
        
    public:
        void BeginUpdate();
        void EndUpdate();
        void BeginPostUpdate();
        void EndPostUpdate();
        
    private:
        void _RegisterInstanceSet(const TW_Valid<F_GPUDataInstanceSet>& InstanceSet);
        void _UnregisterInstanceSet(const TW_Valid<F_GPUDataInstanceSet>& InstanceSet);
        
    private:
        void _RegisterComponentType(const TW_Valid<F_GPUDataComponentType>& ComponentType);
        void _UnregisterComponentType(const TW_Valid<F_GPUDataComponentType>& ComponentType);
        
    private:
        void _RegisterIndexSet(const TW_Valid<F_GPUDataIndexSet>& IndexSet);
        void _UnregisterIndexSet(const TW_Valid<F_GPUDataIndexSet>& IndexSet);
        
    public:
        B8 HasComponentType(F_GeneralTypeHashCode HashCode) const;
        TW<F_GPUDataComponentType> FindComponentType(F_GeneralTypeHashCode HashCode) const;
        TW_Valid<F_GPUDataComponentType> GetComponentType(F_GeneralTypeHashCode HashCode) const;
        
    public:
        B8 HasGlobalComponentType(F_GeneralTypeHashCode HashCode) const;
        TS<F_GPUDataComponentType> FindGlobalComponentType(F_GeneralTypeHashCode HashCode) const;
        TS<F_GPUDataComponentType> GetGlobalComponentType(F_GeneralTypeHashCode HashCode) const;
        
    public:
        template<typename __F>
        B8 HasGlobalComponentType() const
        {
            return HasComponentType(H_GeneralTypeHashCode::MakeStatic<__F>());
        }
        template<typename __F>
        TS<F_GPUDataComponentType> FindGlobalComponentType() const
        {
            return FindGlobalComponentType(H_GeneralTypeHashCode::MakeStatic<__F>());
        }
        template<typename __F>
        TS<F_GPUDataComponentType> GetGlobalComponentType() const
        {
            return GetGlobalComponentType(H_GeneralTypeHashCode::MakeStatic<__F>());
        }
    };
}
