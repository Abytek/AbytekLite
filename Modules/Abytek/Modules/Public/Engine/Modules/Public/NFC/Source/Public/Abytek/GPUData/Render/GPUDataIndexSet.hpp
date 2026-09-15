#pragma once

#include "Abytek/RenderBase/RenderObject.hpp"
#include "Abytek/GPUData/GPUDataCommon.hpp"


namespace Abytek
{
    class F_GPUData;
    class F_GPUDataInstanceSet;
    class F_GPUDataComponentType;

    struct F_GPUDataIndexSetBuildParams
    {
        TW<F_GPUData> GPUData;
        TF_Vector<TS<F_GPUDataComponentType>> ComponentTypes;
    };
    class ABYTEK_ENGINE_NFC_API F_GPUDataIndexSet final : public A_RenderObject
    {
    private:
        TW<F_GPUData> _GPUData;
        TF_Vector<TS<F_GPUDataComponentType>> _ComponentTypes;
        U32 _StrideInBytes = 0;
        
        TF_Vector<TW<F_GPUDataInstanceSet>> _InstanceSets;
        TF_Set<TW<F_GPUDataInstanceSet>> _DirtyInstanceSets;
        TF_Map<TW<F_GPUDataInstanceSet>, U32> _InstanceSetToIndex;
        
        TS<A_RHIResource> _Buffer;
        U32 _LastBufferSize = 0;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetGPUData() const noexcept
        {
            return _GPUData;
        }
        ABYTEK_FORCE_INLINE const auto& GetComponentTypes() const noexcept
        {
            return _ComponentTypes;
        }
        ABYTEK_FORCE_INLINE auto GetStrideInBytes() const noexcept
        {
            return _StrideInBytes;
        }
        
        ABYTEK_FORCE_INLINE const auto& GetBuffer() const noexcept
        {
            return _Buffer;
        }
        
    public:
        F_GPUDataIndexSet(const TW_Valid<F_WorldRenderResource>& WorldRenderResource, const F_GPUDataIndexSetBuildParams& BuildParams);
        ~F_GPUDataIndexSet() override;
        
    public:
        B8 HasInstanceSet(const TW_Valid<F_GPUDataInstanceSet>& InstanceSet) const;
        U32 GetInstanceSetIndex(const TW_Valid<F_GPUDataInstanceSet>& InstanceSet) const;
        TW_Valid<F_GPUDataInstanceSet> GetInstanceSetAtIndex(U32 Index) const;
        void AddInstanceSet(const TW_Valid<F_GPUDataInstanceSet>& InstanceSet);
        void RemoveInstanceSet(const TW_Valid<F_GPUDataInstanceSet>& InstanceSet);
        
    private:
        void _UpdateBufferSize();
        void _UploadDirtyInstanceSets();
        
    public:
        void Update();
    };
}
