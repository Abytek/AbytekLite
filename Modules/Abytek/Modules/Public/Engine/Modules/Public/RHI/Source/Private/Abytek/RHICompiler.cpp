#include "Abytek/RHICompiler.hpp"
#include "Abytek/DirectX12Shared/RHICompiler.hpp"

#include "Abytek/RHITemplateDatabase.hpp"


#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
namespace Abytek
{
    A_RHICompiler::A_RHICompiler(E_RHIAPI API) :
        _API(API)
    {
    }
    A_RHICompiler::~A_RHICompiler()
    {
    }

    F_RHICommonCompilationStatus A_RHICompiler::ValidateBindGroupTemplate(
        const F_RHIBindGroupTemplateCompileParams& CompileParams,
        const TW_Valid<A_RHITemplateDatabase>& TemplateDatabase
    )
    {
        if (!CompileParams.Database)
        {
            return F_RHICommonCompilationStatus::MakeFailed(
                ABYTEK_TEXT("Invalid template database")
            );
        }
        
        // Validate duplicated slot names
        {
            TF_Set<F_Name> SlotNames;
            for (U32 SlotIndex = 0; SlotIndex < CompileParams.Slots.size(); ++SlotIndex)
            {
                const auto& Slot = CompileParams.Slots[SlotIndex];
                switch (Slot.Type)
                {
                case E_RHIBindGroupSlotType::RESOURCE_VIEW:
                case E_RHIBindGroupSlotType::RESOURCE_VIEW_SET:
                    if (Slot.ResourceAccess.GPU == E_RHIResourceGPUAccess::NONE)
                    {
                        return F_RHICommonCompilationStatus::MakeFailed(
                            ABYTEK_TEXT("Invalid resource access at slot index: ")
                            + ToText(SlotIndex)
                        );
                    }
                    break;
                }

                if (SlotNames.find(Slot.Name) != SlotNames.end())
                {
                    return F_RHICommonCompilationStatus::MakeFailed(
                        ABYTEK_TEXT("Duplicated slot name at index: ")
                        + ToText(SlotIndex)
                    );
                }
                SlotNames.insert(Slot.Name);
            }
        }
        return F_RHICommonCompilationStatus::MakeSucceeded();
    }
    F_RHICommonCompilationStatus A_RHICompiler::ValidatePipelineStateTemplate(
        const F_RHIPipelineStateTemplateCompileParams& CompileParams,
        const TW_Valid<A_RHITemplateDatabase>& TemplateDatabase
    )
    {
        if (!CompileParams.Database)
        {
            return F_RHICommonCompilationStatus::MakeFailed(
                ABYTEK_TEXT("Invalid template database")
            );
        }
        if (CompileParams.Type == E_RHIPipelineStateType::NONE)
        {
            return F_RHICommonCompilationStatus::MakeFailed(
                ABYTEK_TEXT("Invalid pipeline state type")
            );
        }
        
        // Check for duplicated names error
        {
            TF_Set<F_Name> SlotNames;
            for (U32 BindGroupIndex = 0; BindGroupIndex < CompileParams.BindGroups.size(); ++BindGroupIndex)
            {
                const auto& BindGroupSlot = CompileParams.BindGroups[BindGroupIndex];
                if (!TemplateDatabase->HasTemplate(BindGroupSlot.TemplateHashCode))
                {
                    return F_RHICommonCompilationStatus::MakeFailed(
                        ABYTEK_TEXT("Not found bind group template with hash code ")
                        + ToText(BindGroupSlot.TemplateHashCode)
                        + ABYTEK_TEXT(" at index ")
                        + ToText(BindGroupIndex)
                    );
                }
                auto BindGroupTemplate = TemplateDatabase->GetTemplate(BindGroupSlot.TemplateHashCode)
                    .FastCast<A_RHIBindGroupTemplate>();
                const auto& Slots = BindGroupTemplate->GetSlots();
            
                for (U32 SlotIndex = 0; SlotIndex < Slots.size(); ++SlotIndex)
                {
                    const auto& Slot = Slots[SlotIndex];
                    if (SlotNames.find(Slot.Name) != SlotNames.end())
                    {
                        return F_RHICommonCompilationStatus::MakeFailed(
                            ABYTEK_TEXT("Duplicated slot name at bind group index: ")
                            + ToText(BindGroupIndex)
                            + ABYTEK_TEXT(", local slot index: ")
                            + ToText(SlotIndex)
                        );
                    }
                    SlotNames.insert(Slot.Name);
                }
            }
        }

        // Check for invalid shaders
        if (CompileParams.VertexShader)
        {
            if (CompileParams.Type != E_RHIPipelineStateType::GRAPHICS)
            {
                return F_RHICommonCompilationStatus::MakeFailed(ABYTEK_TEXT("Vertex shader is not allowed in non-graphics pipeline state"));
            }
        }
        if (CompileParams.PixelShader)
        {
            if (CompileParams.Type != E_RHIPipelineStateType::GRAPHICS)
            {
                return F_RHICommonCompilationStatus::MakeFailed(ABYTEK_TEXT("Pixel shader is not allowed in non-graphics pipeline state"));
            }
        }
        if (CompileParams.AmplificationShader)
        {
            if (CompileParams.Type != E_RHIPipelineStateType::GRAPHICS)
            {
                return F_RHICommonCompilationStatus::MakeFailed(ABYTEK_TEXT("Amplification shader is not allowed in non-graphics pipeline state"));
            }
        }
        if (CompileParams.MeshShader)
        {
            if (CompileParams.Type != E_RHIPipelineStateType::GRAPHICS)
            {
                return F_RHICommonCompilationStatus::MakeFailed(ABYTEK_TEXT("Mesh shader is not allowed in non-graphics pipeline state"));
            }
        }
        if (CompileParams.ComputeShader)
        {
            if (CompileParams.Type != E_RHIPipelineStateType::COMPUTE)
            {
                return F_RHICommonCompilationStatus::MakeFailed(ABYTEK_TEXT("Compute shader is not allowed in non-compute pipeline state"));
            }
        }
        return F_RHICommonCompilationStatus::MakeSucceeded();
    }

    F_RHICommonCompilationStatus A_RHICompiler::CompileBindGroupTemplate(
        const F_RHIBindGroupTemplateCompileParams& CompileParams,
        const TW_Valid<A_RHITemplateDatabase>& TemplateDatabase,
        TS<A_RHIBindGroupTemplate>& OutCompiledObject
    )
    {
        return ValidateBindGroupTemplate(CompileParams, TemplateDatabase);
    }
    F_RHICommonCompilationStatus A_RHICompiler::CompilePipelineStateTemplate(
        const F_RHIPipelineStateTemplateCompileParams& CompileParams,
        const TW_Valid<A_RHITemplateDatabase>& TemplateDatabase,
        TS<A_RHIPipelineStateTemplate>& OutCompiledObject
    )
    {
        return ValidatePipelineStateTemplate(CompileParams, TemplateDatabase);
    }

    TU<A_RHICompiler> A_RHICompiler::Create(E_RHIAPI API)
    {
        switch (API)
        {
        case E_RHIAPI::DIRECTX12:
            return TU<F_DirectX12SharedRHICompiler>()();
        default:
            ABYTEK_ENGINE_RHI_ASSERT(false) << "Invalid RHI API, cannot create compiler";
        }
        return {};
    }
}
#endif