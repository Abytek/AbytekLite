#include "Abytek/GPUData/Render/GPUDataComponentStorage.hpp"
#include "Abytek/GPUData/Render/GPUDataComponentPage.hpp"
#include "Abytek/GPUData/Render/GPUData.hpp"


namespace Abytek
{
    namespace GPUData
    {
        ABYTEK_DEFINE_GLOBAL_RENDER_BINDING(F_GlobalSRVBinding)
        {
            ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::GPUData::F_GlobalSRVBinding"));
        }
        ABYTEK_DEFINE_GLOBAL_RENDER_BINDING(F_GlobalUAVBinding)
        {
            ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::GPUData::F_GlobalUAVBinding"));
        }
    }
    
    F_GPUDataComponentStorage::F_GPUDataComponentStorage(const TW_Valid<F_WorldRenderResource>& WorldRenderResource, const F_GPUDataComponentStorageBuildParams& BuildParams) :
        A_RenderObject(WorldRenderResource),
        _ComponentType(BuildParams.ComponentType)
    {
        _CapacityLimitInBytes = U64(1024) * U64(1024) * U64(1024);
        _MinimalPageSizeInBytes = U64(64) * U64(1024);
        _NextPageSizeInBytes = _MinimalPageSizeInBytes;
        _StrideInBytes = _ComponentType->GetSizeInBytes();
        
        AddNewPage(0);
        Update();
    }
    F_GPUDataComponentStorage::~F_GPUDataComponentStorage()
    {
    }

    void F_GPUDataComponentStorage::BeginUpdate()
    {
    }
    void F_GPUDataComponentStorage::EndUpdate()
    {
    }
    void F_GPUDataComponentStorage::BeginPostUpdate()
    {
        Update();
        FlushDeallocationQueue();
    }
    void F_GPUDataComponentStorage::EndPostUpdate()
    {
    }

    void F_GPUDataComponentStorage::FlushDeallocationQueue()
    {
        F_GPUDataComponentAllocation Allocation;
        while (DeallocationQueue.TryPop(Allocation))
        {
            Allocation.Page->Deallocate(Allocation);
        }
    }

    TF_Optional<F_GPUDataComponentAllocation> F_GPUDataComponentStorage::Allocate(U32 NumComponents)
    {
        TF_Optional<F_GPUDataComponentAllocation> Result;
        _CriticalSection(
            [this, &Result, NumComponents]
            {
                for (const auto& Page : _Pages)
                {
                    if (auto Allocation = Page->Allocate(NumComponents))
                    {
                        Result = ABYTEK_MOVE(Allocation);
                    }
                }
                if (auto Page = AddNewPage(static_cast<U64>(_StrideInBytes) * static_cast<U64>(NumComponents)))
                {
                    if (auto Allocation = Page->Allocate(NumComponents))
                    {
                        Result = ABYTEK_MOVE(Allocation);
                    }
                }
            }
        );
        return Result;
    }
    TS<F_GPUDataComponentPage> F_GPUDataComponentStorage::AddNewPage(U64 SizeInBytes)
    {
        U64 ActualSizeInBytes = SizeInBytes + sizeof(F_Vector4_F32); // with alignment
        
        U64 NewPageSizeInBytes = _NextPageSizeInBytes;
        while (NewPageSizeInBytes <= ActualSizeInBytes)
        {
            NewPageSizeInBytes *= 2;
        }
        
        U64 NewCapacityInBytes = NewPageSizeInBytes + _CapacityInBytes;
        if (NewCapacityInBytes > _CapacityLimitInBytes)
        {
            return {};
        }
        
        _NextPageSizeInBytes = NewPageSizeInBytes * 2;
        _CapacityInBytes = NewCapacityInBytes;
        
        _ShouldRecreateBindGroup = true;
        
        auto Page = TS<F_GPUDataComponentPage>()(
            ABYTEK_WTHIS(),
            static_cast<U32>(_Pages.size()),
            NewPageSizeInBytes
        );
        _Pages.push_back(Page);
        return Page;
    }

    void F_GPUDataComponentStorage::Update()
    {
        if (!_ShouldRecreateBindGroup)
        {
            return;
        }
        _ShouldRecreateBindGroup = false;
        
        _GlobalSRVBindGroup = GPUData::F_GlobalSRVBinding::Instantiate(
            GetRenderRegistryRuntime()    
        ).CreateBindGroup();
        if (_GlobalSRVBindGroup->HasSlot(ABYTEK_NAME("GPUDataPages")))
        {
            F_RHIResourceViewSet GPUDataPages;
            for (const auto& Page : _Pages)
            {
                GPUDataPages.push_back(Page->GetSRV());
            }
            _GlobalSRVBindGroup->BindResourceViewSet(
                ABYTEK_NAME("GPUDataPages"),
                GPUDataPages
            );
        }
        _GlobalSRVBindGroup->Commit();
        
        _GlobalUAVBindGroup = GPUData::F_GlobalUAVBinding::Instantiate(
            GetRenderRegistryRuntime()    
        ).CreateBindGroup();
        if (_GlobalUAVBindGroup->HasSlot(ABYTEK_NAME("GPUDataPages")))
        {
            F_RHIResourceViewSet GPUDataPages;
            for (const auto& Page : _Pages)
            {
                GPUDataPages.push_back(Page->GetUAV());
            }
            _GlobalUAVBindGroup->BindResourceViewSet(
                ABYTEK_NAME("GPUDataPages"),
                GPUDataPages
            );
        }
        _GlobalUAVBindGroup->Commit();
    }

    TF_Optional<F_GPUDataComponentAllocation> F_GPUDataComponentStorage::AddComponent(U32 NumComponents)
    {
        if (auto Allocation = Allocate(NumComponents))
        {
            return Allocation;
        }
        return {};
    }
}
