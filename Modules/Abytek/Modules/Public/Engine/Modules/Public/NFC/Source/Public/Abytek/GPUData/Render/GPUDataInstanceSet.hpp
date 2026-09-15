#pragma once

#include "Abytek/RenderBase/RenderObject.hpp"


namespace Abytek
{
    class F_GPUData;
    class F_GPUDataComponentType;

    struct F_GPUDataInstanceSetBuildParams
    {
        TW<F_GPUData> GPUData;
        TF_Vector<TS<F_GPUDataComponentType>> ComponentTypes;
        U32 Num = 0;
    };
    class ABYTEK_ENGINE_NFC_API F_GPUDataInstanceSet final : public A_RenderObject
    {
    private:
        TW<F_GPUData> _GPUData;
        TF_Vector<TS<F_GPUDataComponentType>> _ComponentTypes;
        TF_Vector<F_GPUDataComponentAllocation> _ComponentAllocations;
        U32 _Num = 0;
        B8 _IsUsable = true;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetGPUData() const noexcept
        {
            return _GPUData;
        }
        ABYTEK_FORCE_INLINE const auto& GetComponentTypes() const noexcept
        {
            return _ComponentTypes;
        }
        ABYTEK_FORCE_INLINE const auto& GetComponentAllocations() const noexcept
        {
            return _ComponentAllocations;
        }
        ABYTEK_FORCE_INLINE auto GetNum() const noexcept
        {
            return _Num;
        }
        ABYTEK_FORCE_INLINE auto IsUsable() const noexcept
        {
            return _IsUsable;
        }
        
    public:
        F_GPUDataInstanceSet(const TW_Valid<F_WorldRenderResource>& WorldRenderResource, const F_GPUDataInstanceSetBuildParams& BuildParams);
        ~F_GPUDataInstanceSet() override;
        
    private:
        void _AllocateComponents();
        void _DeallocateComponents();
        
    public:
        B8 HasComponentType(const TW_Valid<F_GPUDataComponentType>& ComponentType) const;
        U32 GetComponentTypeIndex(const TW_Valid<F_GPUDataComponentType>& ComponentType) const;
        
    public:
        void UploadComponent(U32 Index, const void* DataPtr);
        void UploadComponent(const TW_Valid<F_GPUDataComponentType>& ComponentType, const void* DataPtr);
    };
}
