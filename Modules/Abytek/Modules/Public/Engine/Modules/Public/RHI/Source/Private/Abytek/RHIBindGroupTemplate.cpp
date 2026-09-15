#include "Abytek/RHIBindGroupTemplate.hpp"
#include "Abytek/RHIBindGroupTemplateRuntime.hpp"


namespace Abytek
{
    A_RHIBindGroupTemplate::A_RHIBindGroupTemplate(const F_RHIBindGroupTemplateBuildParams& BuildParams) :
        A_RHITemplate(BuildParams),
        _Config(static_cast<const F_RHIBindGroupTemplateConfig&>(BuildParams))
    {
    }
    A_RHIBindGroupTemplate::~A_RHIBindGroupTemplate()
    {
    }

    B8 A_RHIBindGroupTemplate::HasSlot(const F_Name& Name) const
    {
        for (const auto& Slot : _Config.Slots)
        {
            if (Slot.Name == Name)
            {
                return true;
            }
        }
        return false;
    }
    TF_Optional<U32> A_RHIBindGroupTemplate::FindSlotIndex(const F_Name& Name) const
    {
        U32 NumSlots = _Config.Slots.size();
        for (U32 SlotIndex = 0; SlotIndex < NumSlots; ++SlotIndex)
        {
            if (_Config.Slots[SlotIndex].Name == Name)
            {
                return SlotIndex;
            }
        }
        return {};
    }

    TS_Valid<A_RHITemplateRuntime> A_RHIBindGroupTemplate::CreateAndBuildRuntime(const TW_Valid<A_RHIContext>& Context)
    {
        F_RHIBindGroupTemplateRuntimeBuildParams BuildParams;
        BuildParams.Context = Context;
        BuildParams.Template = ABYTEK_STHIS();
        return RACreateAndBuildShared<A_RHIBindGroupTemplateRuntime>(BuildParams);
    }

    U32 A_RHIBindGroupTemplate::GetEncodedDataSizeInBytes()
    {
        return 0;
    }
    U32 A_RHIBindGroupTemplate::GetEncodedDataAlignmentInBytes()
    {
        return 0;
    }
}
