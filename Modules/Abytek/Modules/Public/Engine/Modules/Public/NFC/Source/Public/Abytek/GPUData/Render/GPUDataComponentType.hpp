#pragma once

#include "Abytek/RenderBase/RenderObject.hpp"
#include "Abytek/GPUData/GPUDataCommon.hpp"


namespace Abytek
{
    class F_GPUData;
    class F_GPUDataComponentStorage;
    class F_GPUDataInstanceSet;

    struct F_GPUDataComponentTypeBuildParams
    {
        TW<F_GPUData> GPUData;
        F_GeneralTypeHashCode HashCode;
        U32 SizeInBytes = 0;
        U32 AlignmentInBytes = 0;
    };
    class ABYTEK_ENGINE_NFC_API F_GPUDataComponentType final : public A_RenderObject
    {
    private:
        TW<F_GPUData> _GPUData;
        TS<F_GPUDataComponentStorage> _Storage;
        F_GeneralTypeHashCode _HashCode;
        U32 _SizeInBytes = 0;
        U32 _AlignmentInBytes = 0;
        TF_Set<TW<F_GPUDataInstanceSet>> _InstanceSets;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetGPUData() const noexcept
        {
            return _GPUData;
        }
        ABYTEK_FORCE_INLINE const auto& GetStorage() const noexcept
        {
            return _Storage;
        }
        ABYTEK_FORCE_INLINE auto GetHashCode() const noexcept
        {
            return _HashCode;
        }
        ABYTEK_FORCE_INLINE auto GetSizeInBytes() const noexcept
        {
            return _SizeInBytes;
        }
        ABYTEK_FORCE_INLINE auto GetAlignmentInBytes() const noexcept
        {
            return _AlignmentInBytes;
        } 
        ABYTEK_FORCE_INLINE const auto& GetInstanceSets() const noexcept
        {
            return _InstanceSets;
        }
        
    public:
        F_GPUDataComponentType(const TW_Valid<F_WorldRenderResource>& WorldRenderResource, const F_GPUDataComponentTypeBuildParams& BuildParams);
        ~F_GPUDataComponentType() override;
        
    public:
        B8 HasInstanceSet(const TW_Valid<F_GPUDataInstanceSet>& InstanceSet) const;
        void AddInstanceSet(const TW_Valid<F_GPUDataInstanceSet>& InstanceSet);
        void RemoveInstanceSet(const TW_Valid<F_GPUDataInstanceSet>& InstanceSet);
        
    public:
        TF_Optional<F_GPUDataComponentAllocation> New(U32 NumComponents = 1);
        void Delete(const F_GPUDataComponentAllocation& ComponentAllocation);
    };
}
