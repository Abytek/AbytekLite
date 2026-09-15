#include "Abytek/DirectX12Shared/RHICompiler.hpp"
#include "Abytek/RHIBindGroup.hpp"
#include "Abytek/RHIBindGroupTemplate.hpp"
#include "Abytek/RHISlang.hpp"
#include "Abytek/RHISlangSubsystem.hpp"
#include "Abytek/RHITemplateDatabase.hpp"


#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
namespace Abytek
{
    F_DirectX12SharedRHICompiler::F_DirectX12SharedRHICompiler() :
        A_RHICompiler(E_RHIAPI::DIRECTX12)
    {
    }
    F_DirectX12SharedRHICompiler::~F_DirectX12SharedRHICompiler()
    {
    }

    F_String F_DirectX12SharedRHICompiler::D3DParseProfile(const F_RHIFeatureSupports& FeatureSupports, E_RHIShaderFrequency ShaderFrequency)
    {
        auto GetRequiredShaderModel = [](
            E_RHIShaderFrequency Frequency,
            const F_RHIFeatureSupports& FTemplate
        ) -> U32
        {
            U32 SM = 60; // baseline SM6_0

            const auto& F = FTemplate.ExtendedSet;

            // ---- general shader features ----

            if (F.PrimitiveTypes >= F_RHIFeatureSet::E_PrimitiveTypes::FEATURE_F16)
                SM = Max<U32>(SM, 62u);

            if (F.VariableRateShading >= F_RHIFeatureSet::E_VariableRateShading::L1)
                SM = Max<U32>(SM, 64u);

            if (F.MeshShader >= F_RHIFeatureSet::E_MeshShader::L1)
                SM = Max<U32>(SM, 65u);

            if (F.SamplerFeedback >= F_RHIFeatureSet::E_SamplerFeedback::L1)
                SM = Max<U32>(SM, 65u);

            if (F.AtomicOps >= F_RHIFeatureSet::E_AtomicOps::FEATURE_I64)
                SM = Max<U32>(SM, 66u);

            if (F.Binding >= F_RHIFeatureSet::E_Binding::L3)
                SM = Max<U32>(SM, 66u);

            // ---- ray tracing ----

            if (
                Frequency == E_RHIShaderFrequency::RT_RAY_GEN ||
                Frequency == E_RHIShaderFrequency::RT_ANY_HIT ||
                Frequency == E_RHIShaderFrequency::RT_CLOSEST_HIT ||
                Frequency == E_RHIShaderFrequency::RT_MISS
            )
            {
                if (F.RayTracing >= F_RHIFeatureSet::E_RayTracing::L1)
                    SM = Max<U32>(SM, 63u);

                if (F.RayTracing >= F_RHIFeatureSet::E_RayTracing::L2)
                    SM = Max<U32>(SM, 66u);
            }

            // ---- work graphs ----

            if (Frequency == E_RHIShaderFrequency::WORK_GRAPH)
            {
                SM = Max<U32>(SM, 68u); // SM 6.8 required
            }

            // ---- clamp invalid combinations ----

            if (
                Frequency != E_RHIShaderFrequency::WORK_GRAPH &&
                Frequency != E_RHIShaderFrequency::RT_RAY_GEN &&
                Frequency != E_RHIShaderFrequency::RT_ANY_HIT &&
                Frequency != E_RHIShaderFrequency::RT_CLOSEST_HIT &&
                Frequency != E_RHIShaderFrequency::RT_MISS
            )
            {
                // SM 6.8 currently not valid for standard shaders
                if (SM > 66)
                    SM = 66;
            }

            return SM;
        };

        auto GetShaderPrefix = [](E_RHIShaderFrequency Freq) -> const char*
        {
            switch (Freq)
            {
            case E_RHIShaderFrequency::VERTEX:        return "vs";
            case E_RHIShaderFrequency::PIXEL:         return "ps";
            case E_RHIShaderFrequency::MESH:          return "ms";
            case E_RHIShaderFrequency::AMPLIFICATION: return "as";
            case E_RHIShaderFrequency::COMPUTE:       return "cs";

            case E_RHIShaderFrequency::RT_RAY_GEN:
            case E_RHIShaderFrequency::RT_ANY_HIT:
            case E_RHIShaderFrequency::RT_CLOSEST_HIT:
            case E_RHIShaderFrequency::RT_MISS:
            case E_RHIShaderFrequency::WORK_GRAPH:
                return "lib";

            default:
                return "unknown";
            }
        };

        U32 SM = GetRequiredShaderModel(ShaderFrequency, FeatureSupports);

        U32 Major = SM / 10;
        U32 Minor = SM % 10;

        const char* Prefix = GetShaderPrefix(ShaderFrequency);

        char Buffer[32];
        std::snprintf(Buffer, sizeof(Buffer), "%s_%u_%u", Prefix, Major, Minor);

        return ToString(Buffer);
    }

    F_RHICommonCompilationStatus F_DirectX12SharedRHICompiler::D3DCompileBindGroupTemplate(
        const F_RHIBindGroupTemplateCompileParams& CompileParams,
        const TW_Valid<A_RHITemplateDatabase>& TemplateDatabase,
        TS<A_RHIBindGroupTemplate>& OutCompiledObject
    )
    {
        F_DirectX12SharedRHIBindGroupTemplateCompiledData OutCompiledData;
        
        const auto& Slots = CompileParams.Slots;
        U32 NumSlots = Slots.size();

        // Create initial slot maps
        TF_SmallVector<F_DirectX12SharedRHIBindGroupTemplateCompiledData::F_SlotMap, 16> InitialSlotMaps;
        InitialSlotMaps.reserve(NumSlots);
        {
            for (U32 SlotIndex = 0; SlotIndex < NumSlots; ++SlotIndex)
            {
                const auto& Slot = Slots[SlotIndex];
                F_DirectX12SharedRHIBindGroupTemplateCompiledData::F_SlotMap SlotMap;
                InitialSlotMaps.push_back(SlotMap);
            }
        }

        // Initial root parameters
        TF_SmallVector<DirectX12SharedAPIWrapper::F_RootParameter, 4> InitialRootParameters;
        TF_SmallVector<U32, 16> InitialRootParameterIndexToSlotIndex;
        {
            for (U32 SlotIndex = 0; SlotIndex < NumSlots; ++SlotIndex)
            {
                const auto& Slot = Slots[SlotIndex];
                auto& SlotMap = InitialSlotMaps[SlotIndex];
                switch (Slot.Type)
                {
                case E_RHIBindGroupSlotType::RESOURCE_VIEW:
                case E_RHIBindGroupSlotType::RESOURCE_VIEW_SET:
                    {
                        DirectX12SharedAPIWrapper::F_RootParameter RootParameter;
                        RootParameter.Type = DirectX12SharedAPIWrapper::E_RootParameterType::DESCRIPTOR_TABLE;
                        RootParameter.DescriptorTable.Ranges.resize(1);

                        auto& Range = RootParameter.DescriptorTable.Ranges[0];
                        if (FlagHasAny(Slot.ResourceAccess.GPU, E_RHIResourceGPUAccess::CBV))
                        {
                            Range.Type = DirectX12SharedAPIWrapper::E_DescriptorRangeType::CBV;
                        }
                        if (FlagHasAny(Slot.ResourceAccess.GPU, E_RHIResourceGPUAccess::SRV))
                        {
                            Range.Type = DirectX12SharedAPIWrapper::E_DescriptorRangeType::SRV;
                        }
                        if (FlagHasAny(Slot.ResourceAccess.GPU, E_RHIResourceGPUAccess::UAV))
                        {
                            Range.Type = DirectX12SharedAPIWrapper::E_DescriptorRangeType::UAV;
                        }
                        if (Slot.NumResourceViews == ~U32(0))
                        {
                            Range.NumDescriptors = ~U32(0);
                        }
                        else
                        {
                            Range.NumDescriptors = Slot.NumResourceViews;
                        }
                        Range.BaseShaderRegister = 0;
                        Range.RegisterSpace = 0;
                        Range.OffsetInDescriptorsFromTableStart = 0;

                        RootParameter.ShaderVisibility = DirectX12SharedAPIWrapper::Conversions::RHIShaderFrequencyToShaderVisibility(Slot.ResourceAccess.ShaderFrequency);

                        SlotMap.RootParameterIndex = InitialRootParameters.size();
                        SlotMap.DescriptorIndexFromRootParameterStart = 0;
                        
                        SlotMap.DescriptorRangeIndex = 0;
                        SlotMap.DescriptorIndexFromRangeStart = 0;
                        
                        SlotMap.NumDescriptors = Range.NumDescriptors;
                        
                        InitialRootParameters.push_back(RootParameter);
                        InitialRootParameterIndexToSlotIndex.push_back(SlotIndex);
                    }
                    break;
                case E_RHIBindGroupSlotType::SAMPLER:
                case E_RHIBindGroupSlotType::SAMPLER_SET:
                    {
                        DirectX12SharedAPIWrapper::F_RootParameter RootParameter;
                        RootParameter.Type = DirectX12SharedAPIWrapper::E_RootParameterType::DESCRIPTOR_TABLE;
                        RootParameter.DescriptorTable.Ranges.resize(1);

                        auto& Range = RootParameter.DescriptorTable.Ranges[0];
                        Range.Type = DirectX12SharedAPIWrapper::E_DescriptorRangeType::SAMPLER;
                        if (Slot.NumSamplers == ~U32(0))
                        {
                            Range.NumDescriptors = ~U32(0);
                        }
                        else
                        {
                            Range.NumDescriptors = Slot.NumSamplers;
                        }
                        Range.BaseShaderRegister = 0;
                        Range.RegisterSpace = 0;
                        Range.OffsetInDescriptorsFromTableStart = 0;

                        RootParameter.ShaderVisibility = DirectX12SharedAPIWrapper::Conversions::RHIShaderFrequencyToShaderVisibility(Slot.SamplerAccess.ShaderFrequency);

                        SlotMap.RootParameterIndex = InitialRootParameters.size();
                        SlotMap.DescriptorIndexFromRootParameterStart = 0;
                        
                        SlotMap.DescriptorRangeIndex = 0;
                        SlotMap.DescriptorIndexFromRangeStart = 0;
                        
                        SlotMap.NumDescriptors = Range.NumDescriptors;

                        InitialRootParameters.push_back(RootParameter);
                        InitialRootParameterIndexToSlotIndex.push_back(SlotIndex);
                    }
                    break;
                }
            }
        }
        U32 NumInitialRootParameters = InitialRootParameters.size();

        // Make SortedRootParameterRemap
        TF_SmallVector<U32, 16> SortedRootParameterRemap;
        {
            SortedRootParameterRemap.resize(NumInitialRootParameters);
            for (U32 RootParameterIndex = 0; RootParameterIndex < NumInitialRootParameters; ++RootParameterIndex)
            {
                SortedRootParameterRemap[RootParameterIndex] = RootParameterIndex;
            }

            std::sort(
                SortedRootParameterRemap.begin(),
                SortedRootParameterRemap.end(),
                [&](U32 A, U32 B)
                {
                    const auto& RootParameterA = InitialRootParameters[A];
                    const auto& RootParameterB = InitialRootParameters[B];

                    // Shader visibility
                    {
                        using F_ShaderVisibilityIndex = TF_UInt<sizeof(DirectX12SharedAPIWrapper::E_ShaderVisibility)>;
                        
                        auto ScoreA = static_cast<F_ShaderVisibilityIndex>(RootParameterA.ShaderVisibility);
                        auto ScoreB = static_cast<F_ShaderVisibilityIndex>(RootParameterB.ShaderVisibility);

                        if (ScoreA != ScoreB)
                        {
                            return ScoreA < ScoreB;
                        }
                    }

                    // Root parameter type
                    {
                        using F_RootParameterTypeIndex = TF_UInt<sizeof(DirectX12SharedAPIWrapper::E_RootParameterType)>;
                        
                        auto ScoreA = static_cast<F_RootParameterTypeIndex>(RootParameterA.Type);
                        auto ScoreB = static_cast<F_RootParameterTypeIndex>(RootParameterB.Type);

                        if (ScoreA != ScoreB)
                        {
                            return ScoreA < ScoreB;
                        }
                    }
                    if (RootParameterA.Type != DirectX12SharedAPIWrapper::E_RootParameterType::DESCRIPTOR_TABLE)
                    {
                        return false;
                    }

                    // Descriptor heap type
                    {
                        using F_DescriptorHeapTypeIndex = TF_UInt<sizeof(DirectX12SharedAPIWrapper::E_DescriptorHeapType)>;
                        
                        auto ScoreA = static_cast<F_DescriptorHeapTypeIndex>(
                            DirectX12SharedAPIWrapper::Conversions::DescriptorRangeTypeToDescriptorHeapType(
                                RootParameterA.DescriptorTable.Ranges[0].Type
                            )
                        );
                        auto ScoreB = static_cast<F_DescriptorHeapTypeIndex>(
                            DirectX12SharedAPIWrapper::Conversions::DescriptorRangeTypeToDescriptorHeapType(
                                RootParameterB.DescriptorTable.Ranges[0].Type
                            )
                        );

                        if (ScoreA != ScoreB)
                        {
                            return ScoreA < ScoreB;
                        }
                    }

                    // Number of descriptors
                    {
                        auto ScoreA = (RootParameterA.DescriptorTable.Ranges[0].NumDescriptors == ~U32(0));
                        auto ScoreB = (RootParameterB.DescriptorTable.Ranges[0].NumDescriptors == ~U32(0));

                        if (ScoreA != ScoreB)
                        {
                            return ScoreA < ScoreB;
                        }
                    }

                    // Descriptor range type
                    {
                        using F_DescriptorRangeTypeIndex = TF_UInt<sizeof(DirectX12SharedAPIWrapper::E_DescriptorRangeType)>;
                        
                        auto ScoreA = static_cast<F_DescriptorRangeTypeIndex>(
                            RootParameterA.DescriptorTable.Ranges[0].Type
                        );
                        auto ScoreB = static_cast<F_DescriptorRangeTypeIndex>(
                            RootParameterB.DescriptorTable.Ranges[0].Type
                        );

                        if (ScoreA != ScoreB)
                        {
                            return ScoreA < ScoreB;
                        }
                    }
                    return false;
                }
            );
        }

        // Merge root parameters;
        auto MergedSlotMaps = InitialSlotMaps;
        TF_SmallVector<DirectX12SharedAPIWrapper::F_RootParameter, 4> MergedRootParameters;
        {
            U32 NumDescriptorsInCurrentMergedRootParameter = 0;
            
            for (U32 RootParameterIndex = 0; RootParameterIndex < NumInitialRootParameters; ++RootParameterIndex)
            {
                U32 RootParameterIndex2 = SortedRootParameterRemap[RootParameterIndex];
                const auto& RootParameter2 = InitialRootParameters[RootParameterIndex2];

                if (RootParameter2.Type == DirectX12SharedAPIWrapper::E_RootParameterType::DESCRIPTOR_TABLE)
                {
                    const auto& RootDescriptorTable2 = RootParameter2.DescriptorTable;
                    const auto& LastDescriptorRange2 = RootDescriptorTable2.Ranges.back();
                    
                    B8 HasNewRootParameter = false;
                    if (RootParameterIndex > 0)
                    {
                        U32 RootParameterIndex3 = SortedRootParameterRemap[RootParameterIndex - 1];
                        const auto& RootParameter3 = InitialRootParameters[RootParameterIndex3];
                        if (RootParameter3.Type != DirectX12SharedAPIWrapper::E_RootParameterType::DESCRIPTOR_TABLE)
                        {
                            HasNewRootParameter = true;
                        }
                        else
                        {
                            const auto& RootDescriptorTable3 = RootParameter3.DescriptorTable;
                            const auto& LastDescriptorRange3 = RootDescriptorTable3.Ranges.back();

                            if (
                                (
                                    DirectX12SharedAPIWrapper::Conversions::DescriptorRangeTypeToDescriptorHeapType(LastDescriptorRange2.Type)
                                    != DirectX12SharedAPIWrapper::Conversions::DescriptorRangeTypeToDescriptorHeapType(LastDescriptorRange3.Type)
                                )
                                || (LastDescriptorRange3.NumDescriptors == ~U32(0))
                                || (RootParameter3.ShaderVisibility != RootParameter2.ShaderVisibility)
                            )
                            {
                                HasNewRootParameter = true;
                            }
                        }
                    }
                    else
                    {
                        HasNewRootParameter = true;
                    }
                    
                    U32 SlotIndex = InitialRootParameterIndexToSlotIndex[RootParameterIndex2];
                    
                    auto& MergedSlotMap = MergedSlotMaps[SlotIndex];

                    if (HasNewRootParameter)
                    {
                        NumDescriptorsInCurrentMergedRootParameter = 0;
                        
                        DirectX12SharedAPIWrapper::F_RootParameter MergedRootParameter;
                        MergedRootParameter.Type = DirectX12SharedAPIWrapper::E_RootParameterType::DESCRIPTOR_TABLE;
                        MergedRootParameter.ShaderVisibility = RootParameter2.ShaderVisibility;
                        MergedRootParameters.push_back(MergedRootParameter);
                    }

                    U32 MergedRootParameterIndex = MergedRootParameters.size() - 1;

                    auto& MergedRootParameter = MergedRootParameters.back();
                    MergedSlotMap.RootParameterIndex = MergedRootParameterIndex;
                    MergedSlotMap.DescriptorIndexFromRootParameterStart = NumDescriptorsInCurrentMergedRootParameter;
                    MergedSlotMap.DescriptorRangeIndex = MergedRootParameter.DescriptorTable.Ranges.size();

                    const auto& InitialDescriptorRange = RootParameter2.DescriptorTable.Ranges[0];
                    NumDescriptorsInCurrentMergedRootParameter += InitialDescriptorRange.NumDescriptors;
                    
                    MergedRootParameter.DescriptorTable.Ranges.push_back(InitialDescriptorRange);
                } 
            }
        }
        U32 NumMergedRootParameters = MergedRootParameters.size();

        // Merge descriptor ranges
        auto SecondMergedSlotMaps = MergedSlotMaps;
        TF_SmallVector<DirectX12SharedAPIWrapper::F_RootParameter, 4> SecondMergedRootParameters = MergedRootParameters;
        {
            struct F_AnalyzedElement
            {
                U32 NumMerges = 0;
                U32 NumDescriptors = 0;
                TF_SmallVector<U32, 16> NewRangeIndexToNumOriginalRanges;
                TF_SmallVector<U32, 16> OriginalRangeIndexToNewRangeIndex;
                TF_SmallVector<U32, 16> OriginalRangeIndexToDescriptorIndexOffsetFromRangeStart;
            };
            TF_SmallVector<F_AnalyzedElement, 16> AnalyzedElements(NumMergedRootParameters);

            // Analyze
            for (U32 RootParameterIndex = 0; RootParameterIndex < NumMergedRootParameters; ++RootParameterIndex)
            {
                auto& SecondMergedRootParameter = SecondMergedRootParameters[RootParameterIndex];

                F_AnalyzedElement AnalyzedElement;
                
                if (SecondMergedRootParameter.Type == DirectX12SharedAPIWrapper::E_RootParameterType::DESCRIPTOR_TABLE)
                {
                    auto& Ranges = SecondMergedRootParameter.DescriptorTable.Ranges;
                    U32 NumRanges = Ranges.size();

                    for (U32 RangeIndex = 0; RangeIndex < NumRanges;)
                    {
                        U32 BeginRangeIndex = RangeIndex;
                        U32 EndRangeIndex = RangeIndex;

                        const auto& BeginRange = Ranges[BeginRangeIndex];
                        for (; EndRangeIndex < NumRanges; ++EndRangeIndex)
                        {
                            const auto& Range = Ranges[EndRangeIndex];

                            if (BeginRange.Type != Range.Type)
                            {
                                break;
                            }
                        }

                        {
                            U32 RangeMergeIndex = AnalyzedElement.NumMerges;
                            
                            U32 DescriptorIndexOffsetFromRootParameterStart = AnalyzedElement.NumDescriptors;

                            AnalyzedElement.NewRangeIndexToNumOriginalRanges.push_back(EndRangeIndex - BeginRangeIndex);
                            
                            for (U32 RangeIndex2 = BeginRangeIndex; RangeIndex2 < EndRangeIndex; ++RangeIndex2)
                            {
                                const auto& Range = Ranges[RangeIndex2];
                            
                                AnalyzedElement.OriginalRangeIndexToNewRangeIndex.push_back(RangeMergeIndex);
                                AnalyzedElement.OriginalRangeIndexToDescriptorIndexOffsetFromRangeStart.push_back(
                                    AnalyzedElement.NumDescriptors
                                    - DescriptorIndexOffsetFromRootParameterStart
                                );
                                
                                AnalyzedElement.NumDescriptors += Range.NumDescriptors;
                            }
                            
                            ++AnalyzedElement.NumMerges;
                        }
                        
                        RangeIndex = EndRangeIndex;
                    }
                } 

                AnalyzedElements[RootParameterIndex] = AnalyzedElement;
            }
            
            // Update slot maps
            for (U32 SlotIndex = 0; SlotIndex < NumSlots; ++SlotIndex)
            {
                auto& SecondMergedSlotMap = SecondMergedSlotMaps[SlotIndex];
                if (SecondMergedSlotMap.RootParameterIndex == ~U32(0))
                {
                    continue;
                }
                
                auto& AnalyzedElement = AnalyzedElements[SecondMergedSlotMap.RootParameterIndex];

                U32 NewRangeIndex =  AnalyzedElement.OriginalRangeIndexToNewRangeIndex[SecondMergedSlotMap.DescriptorRangeIndex];
                U32 NewDescriptorIndexFromRangeStart =  AnalyzedElement.OriginalRangeIndexToDescriptorIndexOffsetFromRangeStart[
                    SecondMergedSlotMap.DescriptorRangeIndex
                ];
            
                SecondMergedSlotMap.DescriptorRangeIndex = NewRangeIndex;
                SecondMergedSlotMap.DescriptorIndexFromRangeStart = NewDescriptorIndexFromRangeStart;
            }

            // Update ranges
            for (U32 RootParameterIndex = 0; RootParameterIndex < NumMergedRootParameters; ++RootParameterIndex)
            {
                auto& MergedRootParameter = MergedRootParameters[RootParameterIndex];
                auto& SecondMergedRootParameter = SecondMergedRootParameters[RootParameterIndex];

                F_AnalyzedElement& AnalyzedElement = AnalyzedElements[RootParameterIndex];
                
                if (SecondMergedRootParameter.Type == DirectX12SharedAPIWrapper::E_RootParameterType::DESCRIPTOR_TABLE)
                {
                    SecondMergedRootParameter.DescriptorTable.Ranges = {};
                    
                    U32 OriginalRangeIndex = 0;
                    for (U32 NewRangeIndex = 0; NewRangeIndex < AnalyzedElement.NumMerges; ++NewRangeIndex)
                    {
                        U32 NumOriginalRanges = AnalyzedElement.NewRangeIndexToNumOriginalRanges[NewRangeIndex];
                        
                        const auto& OriginalFirstRange = MergedRootParameter.DescriptorTable.Ranges[OriginalRangeIndex];
                        const auto& OriginalLastRange = MergedRootParameter.DescriptorTable.Ranges[OriginalRangeIndex + NumOriginalRanges - 1];

                        DirectX12SharedAPIWrapper::F_DescriptorRange NewRange = OriginalFirstRange;
                        if (OriginalLastRange.NumDescriptors == ~U32(0))
                        {
                            NewRange.NumDescriptors = ~U32(0);
                        }
                        else
                        {
                            NewRange.NumDescriptors = 0;
                            
                            U32 BeginOriginalRangeIndex2 = OriginalRangeIndex;
                            U32 EndOriginalRangeIndex2 = OriginalRangeIndex + NumOriginalRanges;
                            for (U32 OriginalRangeIndex2 = BeginOriginalRangeIndex2; OriginalRangeIndex2 < EndOriginalRangeIndex2; ++OriginalRangeIndex2)
                            {
                                const auto& OriginalRange2 = MergedRootParameter.DescriptorTable.Ranges[OriginalRangeIndex2];
                                NewRange.NumDescriptors += OriginalRange2.NumDescriptors;
                            }
                        }

                        SecondMergedRootParameter.DescriptorTable.Ranges.push_back(NewRange);
                        
                        OriginalRangeIndex += NumOriginalRanges;
                    }
                }
            }
        }

        // Generate descriptor bindings
        TF_SmallVector<F_DirectX12SharedRHIBindGroupTemplateCompiledData::F_DescriptorBinding, 16> DescriptorBindings;
        {
            for (U32 RootParameterIndex = 0; RootParameterIndex < NumMergedRootParameters; ++RootParameterIndex)
            {
                auto& SecondMergedRootParameter = SecondMergedRootParameters[RootParameterIndex];
                if (SecondMergedRootParameter.Type == DirectX12SharedAPIWrapper::E_RootParameterType::DESCRIPTOR_TABLE)
                {
                    const auto& Ranges = SecondMergedRootParameter.DescriptorTable.Ranges;
                    U32 NumRanges = Ranges.size();

                    // Calculate num fixed descriptors
                    U32 NumFixedDescriptors = 0;
                    B8 IsDynamic = false;
                    for (U32 SlotIndex = 0; SlotIndex < NumSlots; ++SlotIndex)
                    {
                        auto& SlotMap = SecondMergedSlotMaps[SlotIndex];
                        if (SlotMap.RootParameterIndex == RootParameterIndex)
                        {
                            if (SlotMap.NumDescriptors != ~U32(0))
                            {
                                NumFixedDescriptors += SlotMap.NumDescriptors;
                            }
                        }
                    }
                    for (U32 RangeIndex = 0; RangeIndex < NumRanges; ++RangeIndex)
                    {
                        const auto& Range = Ranges[RangeIndex];
                        if (Range.NumDescriptors == ~U32(0))
                        {
                            IsDynamic = true;
                            break;
                        }
                    }

                    F_DirectX12SharedRHIBindGroupTemplateCompiledData::F_DescriptorBinding DescriptorBinding;
                    DescriptorBinding.RootParameterIndex = RootParameterIndex;
                    DescriptorBinding.NumFixedDescriptors = NumFixedDescriptors;
                    DescriptorBinding.DynamicSizeSlotIndex = ~U32(0);
                    DescriptorBinding.HeapType = DirectX12SharedAPIWrapper::Conversions::DescriptorRangeTypeToDescriptorHeapType(
                        Ranges[0].Type
                    );
                    for (U32 SlotIndex = 0; SlotIndex < NumSlots; ++SlotIndex)
                    {
                        auto& SlotMap = SecondMergedSlotMaps[SlotIndex];
                        if (SlotMap.RootParameterIndex == RootParameterIndex)
                        {
                            SlotMap.DescriptorBindingIndex = DescriptorBindings.size();
                        }
                    }
                    if (IsDynamic)
                    {
                        U32 MaxDescriptorIndexFromRootParameterStart = 0;
                        for (U32 SlotIndex = 0; SlotIndex < NumSlots; ++SlotIndex)
                        {
                            auto& SlotMap = SecondMergedSlotMaps[SlotIndex];
                            if (SlotMap.RootParameterIndex == RootParameterIndex)
                            {
                                MaxDescriptorIndexFromRootParameterStart = Max<U32>(
                                    MaxDescriptorIndexFromRootParameterStart,
                                    SlotMap.DescriptorIndexFromRootParameterStart
                                );
                            }
                        }
                        for (U32 SlotIndex = 0; SlotIndex < NumSlots; ++SlotIndex)
                        {
                            auto& SlotMap = SecondMergedSlotMaps[SlotIndex];
                            if (SlotMap.RootParameterIndex == RootParameterIndex)
                            {
                                if (SlotMap.DescriptorIndexFromRootParameterStart == MaxDescriptorIndexFromRootParameterStart)
                                {
                                    DescriptorBinding.DynamicSizeSlotIndex = SlotIndex;
                                    break;
                                }
                            }
                        }
                    }

                    DescriptorBindings.push_back(DescriptorBinding);
                }
            }
        }

        // Generate input element descs and vertex buffer bindings
        TF_SmallVector<DirectX12SharedAPIWrapper::F_InputElementDesc, 4> InputElementDescs;
        TF_SmallVector<F_DirectX12SharedRHIBindGroupTemplateCompiledData::F_VertexBufferBinding, 16> VertexBufferBindings;
        {
            for (U32 SlotIndex = 0; SlotIndex < NumSlots; ++SlotIndex)
            {
                auto& Slot = Slots[SlotIndex];

                if (Slot.Type != E_RHIBindGroupSlotType::VERTEX_BUFFER)
                {
                    continue;
                }
                
                auto& SlotMap = SecondMergedSlotMaps[SlotIndex];
                const auto& VertexInputLayout = Slot.VertexInputLayout;

                SlotMap.VertexBufferBindingIndex = VertexBufferBindings.size();

                F_DirectX12SharedRHIBindGroupTemplateCompiledData::F_VertexBufferBinding VertexBufferBinding;
                VertexBufferBinding.SlotIndex = SlotIndex;
                VertexBufferBinding.NumInputElements = VertexInputLayout.Elements.size();
                VertexBufferBinding.InputElementIndex = InputElementDescs.size();
                VertexBufferBindings.push_back(VertexBufferBinding);
                
                F_Name LastSemanticName;
                U32 SemanticIndex = 0;
                
                for (U32 InputElementIndex = 0; InputElementIndex < VertexInputLayout.Elements.size(); ++InputElementIndex)
                {
                    DirectX12SharedAPIWrapper::F_InputElementDesc InputElementDesc;

                    F_Name SemanticName = VertexInputLayout.Elements[InputElementIndex].Name;
                    if (LastSemanticName != SemanticName)
                    {
                        SemanticIndex = 0;
                        LastSemanticName = SemanticName;
                    }

                    const auto& SourceElement = VertexInputLayout.Elements[InputElementIndex];
                    InputElementDesc.SemanticName = SemanticName;
                    InputElementDesc.SemanticIndex = SemanticIndex;
                    InputElementDesc.Format = SourceElement.Format;
                    InputElementDesc.InputSlot = SlotMap.VertexBufferBindingIndex;
                    InputElementDesc.AlignedByteOffset = SourceElement.OffsetInBytes;
                    InputElementDesc.Class = SourceElement.Class;
                    
                    InputElementDescs.push_back(InputElementDesc);

                    ++SemanticIndex;
                }
            }
        }
        
        // Generate index buffer binding
        TF_Optional<F_DirectX12SharedRHIBindGroupTemplateCompiledData::F_IndexBufferBinding> IndexBufferBinding;
        {
            for (U32 SlotIndex = 0; SlotIndex < NumSlots; ++SlotIndex)
            {
                auto& Slot = Slots[SlotIndex];

                if (Slot.Type != E_RHIBindGroupSlotType::INDEX_BUFFER)
                {
                    continue;
                }

                if (IndexBufferBinding)
                {
                    return F_RHICommonCompilationStatus::MakeFailed(ABYTEK_TEXT("Only 1 index buffer slot is allowed in a bind group"));
                }
                
                auto& SlotMap = SecondMergedSlotMaps[SlotIndex];

                SlotMap.HasIndexBufferBinding = true;

                F_DirectX12SharedRHIBindGroupTemplateCompiledData::F_IndexBufferBinding TempIndexBufferBinding;
                TempIndexBufferBinding.SlotIndex = SlotIndex;
                IndexBufferBinding = TempIndexBufferBinding;
            }
        }

        // Generate RTV formats and RTV bindings
        TF_SmallVector<E_RHIFormat, 8> RTVFormats;
        TF_SmallVector<F_DirectX12SharedRHIBindGroupTemplateCompiledData::F_RTVBinding, 8> RTVBindings;
        {
            for (U32 SlotIndex = 0; SlotIndex < NumSlots; ++SlotIndex)
            {
                auto& Slot = Slots[SlotIndex];

                if (Slot.Type != E_RHIBindGroupSlotType::RTV)
                {
                    continue;
                }
                
                auto& SlotMap = SecondMergedSlotMaps[SlotIndex];
                RTVFormats.push_back(Slot.RTVFormat);

                SlotMap.RTVBindingIndex = RTVBindings.size();

                F_DirectX12SharedRHIBindGroupTemplateCompiledData::F_RTVBinding RTVBinding;
                RTVBinding.SlotIndex = SlotIndex;
                RTVBindings.push_back(RTVBinding);
            }
        }

        // Generate DSV formats and DSV binding
        TF_Optional<E_RHIFormat> DSVFormat;
        TF_Optional<F_DirectX12SharedRHIBindGroupTemplateCompiledData::F_DSVBinding> DSVBinding;
        {
            for (U32 SlotIndex = 0; SlotIndex < NumSlots; ++SlotIndex)
            {
                auto& Slot = Slots[SlotIndex];

                if (Slot.Type != E_RHIBindGroupSlotType::DSV)
                {
                    continue;
                }

                if (DSVBinding)
                {
                    return F_RHICommonCompilationStatus::MakeFailed(ABYTEK_TEXT("Only 1 DSV slot is allowed in a bind group"));
                }
                
                auto& SlotMap = SecondMergedSlotMaps[SlotIndex];
                DSVFormat = Slot.DSVFormat;

                SlotMap.HasDSVBinding = true;

                F_DirectX12SharedRHIBindGroupTemplateCompiledData::F_DSVBinding TempDSVBinding;
                TempDSVBinding.SlotIndex = SlotIndex;
                DSVBinding = TempDSVBinding;
            }
        }
        
        //
        auto FinalSlotMaps = SecondMergedSlotMaps;
        TF_SmallVector<DirectX12SharedAPIWrapper::F_RootParameter, 4> FinalRootParameters = SecondMergedRootParameters;
        
        //Setup uniform data
        TF_SmallVector<F_DirectX12SharedRHIBindGroupTemplateCompiledData::F_UniformDataBinding, 2> UniformDataBindings;
        B8 UseConstantBuffer = false;
        {
            U32 TotalUniformDataSizeInBytes = 0;
            for (U32 SlotIndex = 0; SlotIndex < NumSlots; ++SlotIndex)
            {
                const auto& Slot = Slots[SlotIndex];
                auto& SlotMap = FinalSlotMaps[SlotIndex];
                switch (Slot.Type)
                {
                case E_RHIBindGroupSlotType::UNIFORM_DATA:
                    {
                        DirectX12SharedAPIWrapper::F_RootParameter RootParameter; 
                        
                        F_DirectX12SharedRHIBindGroupTemplateCompiledData::F_UniformDataBinding UniformDataBinding;
                        UniformDataBinding.SlotIndex = SlotIndex;
                        
                        B8 ForcePushConstants = FlagHas(Slot.UniformDataSizeInBytes, E_RHIUniformDataFlag::FORCE_PUSH_CONSTANTS);
                        B8 DenyPushConstants = FlagHas(Slot.UniformDataSizeInBytes, E_RHIUniformDataFlag::DENY_PUSH_CONSTANTS);
                        ABYTEK_ENGINE_RHI_ASSERT(!(ForcePushConstants && DenyPushConstants)) << "E_RHIUniformDataFlag::FORCE_PUSH_CONSTANTS and E_RHIUniformDataFlag::DENY_PUSH_CONSTANTS cannot be combined";
                        
                        TotalUniformDataSizeInBytes += Slot.UniformDataSizeInBytes;
                        
                        if (
                            (
                                (TotalUniformDataSizeInBytes <= DirectX12SharedAPIWrapper::DefaultMaxRootConstantsSizeInBytes)
                                || ForcePushConstants
                            )
                            && !DenyPushConstants
                        )
                        {
                            UniformDataBinding.UseRootConstants = true;
                            UniformDataBinding.SizeInBytes = static_cast<U32>(AlignSize_PO2(Slot.UniformDataSizeInBytes, sizeof(U32)));
                            
                            RootParameter.Type = DirectX12SharedAPIWrapper::E_RootParameterType::CONSTANTS;
                            RootParameter.Constants.Num32BitValues = UniformDataBinding.SizeInBytes / sizeof(U32);
                        }
                        else
                        {
                            UniformDataBinding.UseRootConstants = false;
                            UniformDataBinding.SizeInBytes = AlignSize_PO2(Slot.UniformDataSizeInBytes, DirectX12SharedAPIWrapper::MinConstantBufferRegionAlignment);
                            
                            RootParameter.Type = DirectX12SharedAPIWrapper::E_RootParameterType::CBV;
                            UseConstantBuffer = true;
                        }

                        RootParameter.ShaderVisibility = DirectX12SharedAPIWrapper::Conversions::RHIShaderFrequencyToShaderVisibility(Slot.SamplerAccess.ShaderFrequency);

                        SlotMap.UniformDataBindingIndex = UniformDataBindings.size();
                        SlotMap.RootParameterIndex = FinalRootParameters.size();
                        
                        UniformDataBinding.RootParameterIndex = SlotMap.RootParameterIndex;
                        
                        UniformDataBindings.push_back(UniformDataBinding);
                        FinalRootParameters.push_back(RootParameter);
                    }
                    break;
                }
            }
        }
        
        // Bindless
        U32 EncodedDataSizeInBytes = ~U32(0);
        U32 EncodedDataAlignmentInBytes = ~U32(0);
        if (CompileParams.AllowBindless)
        {
            EncodedDataSizeInBytes = 0;
            EncodedDataAlignmentInBytes = sizeof(U32);
            
            for (auto& UniformDataBinding : UniformDataBindings)
            {
                const auto& Slot = Slots[UniformDataBinding.SlotIndex];
                U32 EncodedDataOffsetInBytes = static_cast<U32>(AlignAddress_PO2(EncodedDataSizeInBytes, sizeof(U32)));
                
                UniformDataBinding.EncodedDataOffsetInBytes = EncodedDataSizeInBytes;
                
                EncodedDataSizeInBytes = EncodedDataOffsetInBytes + Slot.UniformDataSizeInBytes;
                EncodedDataAlignmentInBytes = Max<U32>(EncodedDataAlignmentInBytes, Slot.UniformDataAlignmentInBytes);
            }
            for (auto& DescriptorBinding : DescriptorBindings)
            {
                U32 EncodedDataOffsetInBytes = static_cast<U32>(AlignAddress_PO2(EncodedDataSizeInBytes, sizeof(U32)));
                
                DescriptorBinding.EncodedDataOffsetInBytes = EncodedDataOffsetInBytes;
                
                EncodedDataSizeInBytes = EncodedDataOffsetInBytes + sizeof(U32) * 2 /* Begin Index and End Index */;
                EncodedDataAlignmentInBytes = Max<U32>(EncodedDataAlignmentInBytes, ABYTEK_ALIGNOF(U32));
            }
        }
        EncodedDataSizeInBytes = static_cast<U32>(AlignSize_PO2(EncodedDataSizeInBytes, EncodedDataAlignmentInBytes));
        
        // 
        F_DirectX12SharedRHIBindGroupTemplateCompiledData CompiledData;
        
        // Compiled data for root signature
        CompiledData.RootParameters = ABYTEK_MOVE(FinalRootParameters);
        CompiledData.InputElementDescs = ABYTEK_MOVE(InputElementDescs);
        CompiledData.HasIndexBuffer = IndexBufferBinding.has_value();
        CompiledData.RTVFormats = ABYTEK_MOVE(RTVFormats);
        CompiledData.DSVFormat = ABYTEK_MOVE(DSVFormat);
        
        // Compiled data for slot-related mapping
        CompiledData.SlotMaps = ABYTEK_MOVE(FinalSlotMaps);

        // Compiled data for bindings
        CompiledData.DescriptorBindings = ABYTEK_MOVE(DescriptorBindings);
        CompiledData.VertexBufferBindings = ABYTEK_MOVE(VertexBufferBindings);
        CompiledData.IndexBufferBinding = ABYTEK_MOVE(IndexBufferBinding);
        CompiledData.RTVBindings = ABYTEK_MOVE(RTVBindings); 
        CompiledData.DSVBinding = ABYTEK_MOVE(DSVBinding);
        CompiledData.UniformDataBindings = ABYTEK_MOVE(UniformDataBindings);
        CompiledData.UseConstantBuffer = UseConstantBuffer;
        
        // Compiled data for bindless
        CompiledData.EncodedDataSizeInBytes = EncodedDataSizeInBytes;
        CompiledData.EncodedDataAlignmentInBytes = EncodedDataAlignmentInBytes;

        OutCompiledData = ABYTEK_MOVE(CompiledData);
        
        OutCompiledObject = TS<F_DirectX12SharedRHIBindGroupTemplate>()(
            CompileParams,
            ABYTEK_MOVE(OutCompiledData)
        );
        OutCompiledObject->SetCompileConfig(CompileParams);
        return F_RHICommonCompilationStatus::MakeSucceeded();
    }
    F_RHICommonCompilationStatus F_DirectX12SharedRHICompiler::D3DCompilePipelineStateTemplate(
        const F_RHIPipelineStateTemplateCompileParams& CompileParams,
        const TW_Valid<A_RHITemplateDatabase>& TemplateDatabase,
        TS<A_RHIPipelineStateTemplate>& OutCompiledObject
    ) 
    {
        F_DirectX12SharedRHIPipelineStateTemplateCompiledData OutCompiledData;
        
        const auto& FeatureSupports = CompileParams.Database->GetFeatureSupports();
        
        const auto& BindGroups = CompileParams.BindGroups;  
        U32 NumBindGroups = BindGroups.size();

        TF_SmallVector<TS<F_DirectX12SharedRHIBindGroupTemplate>, 6> BindGroupTemplates;
        for (U32 BindGroupIndex = 0; BindGroupIndex < NumBindGroups; ++BindGroupIndex)
        {
            const auto& BindGroup = BindGroups[BindGroupIndex];
            if (!TemplateDatabase->HasTemplate(BindGroup.TemplateHashCode))
            {
                return F_RHICommonCompilationStatus::MakeFailed(
                    ABYTEK_TEXT("Not found bind group template with hash code ")
                    + ToText(BindGroup.TemplateHashCode)
                    + ABYTEK_TEXT(" at index ")
                    + ToText(BindGroupIndex)
                );
            }
            auto BindGroupTemplate = TemplateDatabase->GetTemplate(BindGroup.TemplateHashCode)
                .FastCast<A_RHIBindGroupTemplate>();
            BindGroupTemplates.push_back(BindGroupTemplate.FastCast<F_DirectX12SharedRHIBindGroupTemplate>());
        }
        
        TF_Set<F_Text> GatheredSlangShaderFilePaths;
        
        auto GlobalSession = F_RHISlangSubsystem::GetInstance()->PopGlobalSession();
        ABYTEK_DEFER(GlobalSession)
        {
            F_RHISlangSubsystem::GetInstance()->PushGlobalSession(GlobalSession);
        };

#pragma region Generate Data for Root Signature Creation
        // For root signature creation
        TF_SmallVector<DirectX12SharedAPIWrapper::F_RootParameter, 4> RootParameters;
        TF_SmallVector<DirectX12SharedAPIWrapper::F_InputElementDesc, 4> InputElementDescs;
        B8 HasIndexBuffer = false;
        B8 AllowBindless = CompileParams.AllowBindless;

        // For slot-related mapping
        TF_Vector<F_DirectX12SharedRHIPipelineStateTemplateCompiledData::F_BindGroupMap> BindGroupMaps;
        BindGroupMaps.resize(NumBindGroups);

        {
            for (U32 BindGroupIndex = 0; BindGroupIndex < NumBindGroups; ++BindGroupIndex)
            {
                const auto& BindGroupTemplate = BindGroupTemplates[BindGroupIndex];
                const auto& BindGroupTemplaleData = BindGroupTemplate->GetCompiledData();
                auto& BindGroupMap = BindGroupMaps[BindGroupIndex];
                
                BindGroupMap.RootParameterIndex = RootParameters.size();
                BindGroupMap.NumRootParameters = BindGroupTemplaleData.RootParameters.size();
                
                AllowBindless |= BindGroupTemplate->GetAllowBindless();

                RootParameters.insert(
                    RootParameters.end(),
                    BindGroupTemplaleData.RootParameters.begin(),
                    BindGroupTemplaleData.RootParameters.end()
                );
                if (BindGroupTemplaleData.InputElementDescs.size())
                {
                    if (CompileParams.Type != E_RHIPipelineStateType::GRAPHICS)
                    {
                        return F_RHICommonCompilationStatus::MakeFailed(ABYTEK_TEXT("Vertex buffer is not allowed on non-graphics pipeline state"));
                    }
                    InputElementDescs.insert(
                        InputElementDescs.end(),
                        BindGroupTemplaleData.InputElementDescs.begin(),
                        BindGroupTemplaleData.InputElementDescs.end()
                    );
                }
                if (BindGroupTemplaleData.HasIndexBuffer)
                {
                    if (HasIndexBuffer)
                    {
                        return F_RHICommonCompilationStatus::MakeFailed(ABYTEK_TEXT("Only 1 index buffer is allowed"));
                    }
                    if (CompileParams.Type != E_RHIPipelineStateType::GRAPHICS)
                    {
                        return F_RHICommonCompilationStatus::MakeFailed(ABYTEK_TEXT("Index buffer is not allowed on non-graphics pipeline state"));
                    }
                }
                HasIndexBuffer |= BindGroupTemplaleData.HasIndexBuffer;
            }
        }

        // Bind shader registers and register spaces
        {
            struct F_BindingAllocator
            {
                U32 ShaderRegister = 0;
                U32 RegisterSpace = 0;

                void Allocate(U32 Count, U32& OutShaderRegister, U32& OutRegisterSpace)
                {
                    OutShaderRegister = ShaderRegister;
                    OutRegisterSpace = RegisterSpace;
                    
                    if (Count == ~U32(0))
                    {
                        ShaderRegister = 0;
                        ++RegisterSpace;
                    }
                    else
                    {
                        ShaderRegister += Count;
                    }
                }
            };
            F_BindingAllocator BindingAllocator_CBV;
            F_BindingAllocator BindingAllocator_SRV;
            F_BindingAllocator BindingAllocator_UAV;
            F_BindingAllocator BindingAllocator_SAMPLER;

            for (U32 RootParameterIndex = 0; RootParameterIndex < RootParameters.size(); ++RootParameterIndex)
            {
                auto& RootParameter = RootParameters[RootParameterIndex];
                    
                switch (RootParameter.Type)
                {
                case DirectX12SharedAPIWrapper::E_RootParameterType::CBV:
                    BindingAllocator_CBV.Allocate(1, RootParameter.Descriptor.ShaderRegister, RootParameter.Descriptor.RegisterSpace);
                    break;
                case DirectX12SharedAPIWrapper::E_RootParameterType::CONSTANTS:
                    BindingAllocator_CBV.Allocate(1, RootParameter.Constants.ShaderRegister, RootParameter.Constants.RegisterSpace);
                    break;
                case DirectX12SharedAPIWrapper::E_RootParameterType::DESCRIPTOR_TABLE:
                    {
                        auto& DescriptorTable = RootParameter.DescriptorTable;
                        for (auto& Range : DescriptorTable.Ranges)
                        {
                            switch (Range.Type)
                            {
                            case DirectX12SharedAPIWrapper::E_DescriptorRangeType::CBV:
                                BindingAllocator_CBV.Allocate(Range.NumDescriptors, Range.BaseShaderRegister, Range.RegisterSpace);
                                break;
                            case DirectX12SharedAPIWrapper::E_DescriptorRangeType::SRV:
                                BindingAllocator_SRV.Allocate(Range.NumDescriptors, Range.BaseShaderRegister, Range.RegisterSpace);
                                break;
                            case DirectX12SharedAPIWrapper::E_DescriptorRangeType::UAV:
                                BindingAllocator_UAV.Allocate(Range.NumDescriptors, Range.BaseShaderRegister, Range.RegisterSpace);
                                break;
                            case DirectX12SharedAPIWrapper::E_DescriptorRangeType::SAMPLER:
                                BindingAllocator_SAMPLER.Allocate(Range.NumDescriptors, Range.BaseShaderRegister, Range.RegisterSpace);
                                break;
                            }
                        }
                    }
                    break;
                default:
                    ABYTEK_ENGINE_RHI_ASSERT(false) << "Unknown root parameter type";
                    break;
                }
            }
        }

        OutCompiledData.RootParameters = RootParameters;
        OutCompiledData.InputElementDescs = InputElementDescs;
        OutCompiledData.HasIndexBuffer = HasIndexBuffer;
        OutCompiledData.AllowBindless = AllowBindless;
        
        OutCompiledData.BindGroupMaps = BindGroupMaps;
#pragma endregion

#pragma region Save RTVs + DSV format to compiled data
        for (const auto& BindGroupTemplate : BindGroupTemplates)
        {
            const auto& BindGroupTemplateCompiledData = BindGroupTemplate->GetCompiledData();
            
            if (BindGroupTemplateCompiledData.RTVFormats.size() > 0)
            {
                ABYTEK_ENGINE_RHI_ASSERT(OutCompiledData.RTVFormats.size() == 0) << "Only 1 bind group in a pipeline state is allowed to have RTVs";
                OutCompiledData.RTVFormats = BindGroupTemplateCompiledData.RTVFormats;
            }
            if (BindGroupTemplateCompiledData.DSVFormat)
            {
                ABYTEK_ENGINE_RHI_ASSERT(!OutCompiledData.DSVFormat) << "Only 1 bind group in a pipeline state is allowed to have DSV";
                OutCompiledData.DSVFormat = BindGroupTemplateCompiledData.DSVFormat;
            }
        }
#pragma endregion
        
#pragma region Generate Customized Parameter Binding Map
        F_RHISlangCustomizedParameterBindingMap SlangCustomizedParameterBindingMap;

        for (U32 BindGroupIndex = 0; BindGroupIndex < BindGroups.size(); ++BindGroupIndex)
        {
            const auto& BindGroupMap = BindGroupMaps[BindGroupIndex];
            const auto& BindGroupTemplate = BindGroupTemplates[BindGroupIndex];
            const auto& BindGroupTemplateSlots = BindGroupTemplate->GetSlots();
            const auto& BindGroupTemplateData = BindGroupTemplate->GetCompiledData();
            U32 NumSlots_InBindGroupTemplate = BindGroupTemplateSlots.size();
            for (U32 SlotIndex_InBindGroupTemplate = 0; SlotIndex_InBindGroupTemplate < NumSlots_InBindGroupTemplate; ++SlotIndex_InBindGroupTemplate)
            {
                const auto& Slot_InBindGroupTemplate = BindGroupTemplateSlots[SlotIndex_InBindGroupTemplate];
                const auto& SlotMap_InBindGroupTemplate = BindGroupTemplateData.SlotMaps[SlotIndex_InBindGroupTemplate];

                if (SlotMap_InBindGroupTemplate.RootParameterIndex != ~U32(0))
                {
                    const auto& RootParameter = RootParameters[
                        BindGroupMap.RootParameterIndex
                        + SlotMap_InBindGroupTemplate.RootParameterIndex
                    ];
                    
                    F_RHISlangCustomizedParameterBinding ParameterBinding; 
                    
                    switch (RootParameter.Type)
                    {
                    case DirectX12SharedAPIWrapper::E_RootParameterType::CBV:
                        {
                            ParameterBinding.count = 1; 
                            ParameterBinding.shaderRegister = RootParameter.Descriptor.ShaderRegister;
                            ParameterBinding.registerSpace = RootParameter.Descriptor.RegisterSpace;
                        }
                        break;
                    case DirectX12SharedAPIWrapper::E_RootParameterType::CONSTANTS:
                        {
                            ParameterBinding.count = 1; 
                            ParameterBinding.shaderRegister = RootParameter.Constants.ShaderRegister;
                            ParameterBinding.registerSpace = RootParameter.Constants.RegisterSpace;
                        }
                        break;
                    case DirectX12SharedAPIWrapper::E_RootParameterType::DESCRIPTOR_TABLE:
                        {
                            const auto& DescriptorTable = RootParameter.DescriptorTable;
                            const auto& Range = DescriptorTable.Ranges[SlotMap_InBindGroupTemplate.DescriptorRangeIndex];

                            ParameterBinding.count = SlotMap_InBindGroupTemplate.NumDescriptors; 
                            ParameterBinding.shaderRegister = Range.BaseShaderRegister + SlotMap_InBindGroupTemplate.DescriptorIndexFromRangeStart;
                            ParameterBinding.registerSpace = Range.RegisterSpace;
                        }
                        break;
                    default:
                        ABYTEK_ENGINE_RHI_ASSERT(false) << "Unknown root parameter type";
                        break;
                    }
                    SlangCustomizedParameterBindingMap[ToString(*Slot_InBindGroupTemplate.Name).c_str()] = ParameterBinding;
                    SlangCustomizedParameterBindingMap[("SLANG_parameterGroup_" + ToString(*Slot_InBindGroupTemplate.Name)).c_str()] = ParameterBinding;
                    SlangCustomizedParameterBindingMap[("SLANG_parameterGroup____CBuffer___" + ToString(*Slot_InBindGroupTemplate.Name)).c_str()] = ParameterBinding;
                }
            }
        }
#pragma endregion
        
#pragma region Compile Slang Shader Source
        struct F_CompileSlangShaderSourceCallbackParams
        {
            Slang::ComPtr<slang::ISession> Session;
            TF_Vector<Slang::ComPtr<slang::IModule>>& Modules;
            Slang::ComPtr<slang::IEntryPoint> EntryPoint;
            Slang::ComPtr<slang::IComponentType> Program;
            Slang::ComPtr<slang::IComponentType> LinkedProgram;
            Slang::ComPtr<slang::IBlob> KernelBlob_DXIL;
            Slang::ComPtr<slang::IBlob> KernelBlob_HLSL;
            TF_Set<F_Text> SlangShaderFilePaths;
        };
        auto CompileSlangShaderSource_Core = [&](
            const F_RHIShaderSourceSlang& SlangShaderSource, 
            const F_String& Profile,
            auto&& Callback
        ) -> F_RHICommonCompilationStatus
        {
            Slang::ComPtr<slang::IBlob> DiagnosticBlob;
            
            SlangResult HR;

            SlangProfileID ProfileID = GlobalSession->findProfile(Profile.c_str());

            B8 ShouldCompileHLSL = CompileParams.DebugGeneratedShaders;

            int TargetIndex_DXIL = 0;
            int TargetIndex_HLSL = 1;
            
            slang::TargetDesc TargetDescs[2];
            TargetDescs[TargetIndex_DXIL].format = SLANG_DXIL;
            TargetDescs[TargetIndex_DXIL].profile = ProfileID;
            TargetDescs[TargetIndex_HLSL].format = SLANG_HLSL;
            TargetDescs[TargetIndex_HLSL].profile = ProfileID;

            TF_Vector<F_String> SearchPaths_String;
            TF_Vector<const char*> SearchPaths_CStrArray;
            const char** SearchPaths_CStrArrayRaw = nullptr;
            {
                for (const auto& SearchPath : SlangShaderSource.SearchPaths)
                {
                    SearchPaths_String.push_back(ToString(SearchPath));
                }
                for (const auto& SearchPath_String : SearchPaths_String)
                {
                    SearchPaths_CStrArray.push_back(SearchPath_String.c_str());
                }
                if (SearchPaths_CStrArray.size())
                {
                    SearchPaths_CStrArrayRaw = SearchPaths_CStrArray.data();
                }
            }

            TF_Vector<std::pair<F_String, F_String>> Definitions_String;
            TF_Vector<slang::PreprocessorMacroDesc> Definitions_DescArray;
            const slang::PreprocessorMacroDesc* Definitions_DescArrayRaw = nullptr;
            {
                for (const auto& Definition : SlangShaderSource.Definitions)
                {
                    Definitions_String.push_back({ ToString(*Definition.first), ToString(Definition.second) });
                }
                for (const auto& Definition_String : Definitions_String)
                {
                    slang::PreprocessorMacroDesc Desc;
                    Desc.name = Definition_String.first.c_str();
                    Desc.value = Definition_String.second.c_str();
                    Definitions_DescArray.push_back(Desc);
                }
                if (Definitions_DescArray.size())
                {
                    Definitions_DescArrayRaw = Definitions_DescArray.data();
                }
            }
            
            slang::SessionDesc SessionDesc;
            SessionDesc.targets = TargetDescs;
            SessionDesc.targetCount = sizeof(TargetDescs) / sizeof(slang::TargetDesc);
            SessionDesc.searchPaths = SearchPaths_CStrArrayRaw;
            SessionDesc.searchPathCount = SearchPaths_CStrArray.size();
            SessionDesc.preprocessorMacros = Definitions_DescArrayRaw;
            SessionDesc.preprocessorMacroCount = Definitions_DescArray.size();
            SessionDesc.defaultMatrixLayoutMode = SLANG_MATRIX_LAYOUT_COLUMN_MAJOR;
            SessionDesc.abytekCustomizationConfig.overrideParameterBindings = SlangCustomizedParameterBindingMap;
            Slang::ComPtr<slang::ISession> Session;
            HR = GlobalSession->createSession(SessionDesc, Session.writeRef());
            if (SLANG_FAILED(HR))
            {
                return F_RHICommonCompilationStatus::MakeFailed(ABYTEK_TEXT("Failed to create slang session"));
            }

            TF_Set<F_Text> SlangShaderFilePaths;
            TF_Vector<Slang::ComPtr<slang::IModule>> Modules;
            for (const auto& ModuleName : SlangShaderSource.ModuleNames)
            {
                Slang::ComPtr<slang::IModule> Module(
                    Session->loadModule(
                        ToString(*ModuleName).c_str(),
                        DiagnosticBlob.writeRef()
                    )
                );
                if (DiagnosticBlob)
                {
                    return F_RHICommonCompilationStatus::MakeFailed(
                        ToText(ABYTEK_TEXT("Failed to load module \""))
                        + *ModuleName
                        + ABYTEK_TEXT("\": ")
                        + ToText((const char*)DiagnosticBlob->getBufferPointer())
                    );
                }
                Modules.push_back(Module);
                {
                    const SlangInt32 DependencyCount = Module->getDependencyFileCount();
                    for (SlangInt32 Index = 0; Index < DependencyCount; ++Index)
                    {
                        if (const char* DependencyPath = Module->getDependencyFilePath(Index))
                        {
                            auto Path = ToText(DependencyPath);
                            if (SlangShaderFilePaths.find(Path) != SlangShaderFilePaths.end())
                            {
                                continue;
                            }
                            SlangShaderFilePaths.insert(Path);
                        }
                    }
                }
            }

            Slang::ComPtr<slang::IEntryPoint> EntryPoint;
            F_String EntryPointName_String = ToString(*SlangShaderSource.EntryPointName);
            for (const auto& Module : Modules)
            {
                SlangResult TempHR = Module->findEntryPointByName(
                    EntryPointName_String.c_str(),
                    EntryPoint.writeRef()
                );
                if (SLANG_SUCCEEDED(TempHR))
                {
                    break;
                }
            }
            if (!EntryPoint)
            {
                return F_RHICommonCompilationStatus::MakeFailed(
                    ToText(ABYTEK_TEXT("Not found entry point \""))
                    + ToText(EntryPointName_String)
                    + ABYTEK_TEXT("\"")
                );
            }

            TF_Vector<slang::IComponentType*> ComponentTypes;
            {
                for (const auto& Module : Modules)
                {
                    ComponentTypes.push_back(Module.get());
                }
                ComponentTypes.push_back(EntryPoint.get());
            }
            Slang::ComPtr<slang::IComponentType> Program;
            HR = Session->createCompositeComponentType(
                ComponentTypes.data(),
                ComponentTypes.size(),
                Program.writeRef(),
                DiagnosticBlob.writeRef()
            );
            if (SLANG_FAILED(HR))
            {
                return F_RHICommonCompilationStatus::MakeFailed(
                    ToText(ABYTEK_TEXT("Cannot create slang program: "))
                    + ToText((const char*)DiagnosticBlob->getBufferPointer())
                );
            }

            Slang::ComPtr<slang::IComponentType> LinkedProgram;
            HR = Program->link(
                LinkedProgram.writeRef(),
                DiagnosticBlob.writeRef()
            );
            if (SLANG_FAILED(HR))
            {
                return F_RHICommonCompilationStatus::MakeFailed(
                    ToText(ABYTEK_TEXT("Cannot link slang program: "))
                    + ToText((const char*)DiagnosticBlob->getBufferPointer())
                );
            }

            Slang::ComPtr<slang::IBlob> KernelBlob_DXIL;
            Slang::ComPtr<slang::IBlob> KernelBlob_HLSL;
            
            {
                F_FeedbackStatus ParallelFeedbackStatus = F_FeedbackStatus::MakeSucceeded();
                
                F_TaskInstanceSet TaskInstanceSet;
                TaskInstanceSet.Name = ABYTEK_NAME("CompileSlangShader");
                TaskInstanceSet.StackSize = E_TaskStackSize::EXTREME;
                TaskInstanceSet.Functor = [&]
                {
                    HR = LinkedProgram->getEntryPointCode(
                        0,
                        TargetIndex_DXIL,
                        KernelBlob_DXIL.writeRef(),
                        DiagnosticBlob.writeRef()
                    );
                    if (SLANG_FAILED(HR))
                    {
                        ParallelFeedbackStatus = F_RHICommonCompilationStatus::MakeFailed(
                            ToText(ABYTEK_TEXT("Failed to get entry point code for DXIL: "))
                            + ToText((const char*)DiagnosticBlob->getBufferPointer())
                        );
                        return;
                    }
                    if (ShouldCompileHLSL)
                    {
                        HR = LinkedProgram->getEntryPointCode(
                            0,
                            TargetIndex_HLSL,
                            KernelBlob_HLSL.writeRef(),
                            DiagnosticBlob.writeRef()
                        );
                        if (SLANG_FAILED(HR))
                        {
                            ParallelFeedbackStatus = F_RHICommonCompilationStatus::MakeFailed(
                                ToText(ABYTEK_TEXT("Failed to get entry point code for HLSL: "))
                                + ToText((const char*)DiagnosticBlob->getBufferPointer())
                            );
                            return;
                        }
                    }
                };
                ABYTEK_AWAIT H_TaskUtilities::Schedule(ABYTEK_MOVE(TaskInstanceSet));
                
                ABYTEK_FEEDBACK_STATUS_CHECK(ParallelFeedbackStatus);
            }
            
            F_CompileSlangShaderSourceCallbackParams CallbackParams = {
                Session,
                Modules,
                EntryPoint,
                Program,
                LinkedProgram,
                KernelBlob_DXIL,
                KernelBlob_HLSL,
                SlangShaderFilePaths
            };
            Callback(CallbackParams);
            return F_RHICommonCompilationStatus::MakeSucceeded();
        };
        auto CompileSlangShaderSource = [&](
            const F_RHIShaderSourceSlang& SlangShaderSource,
            const F_String& Profile,
            TF_Vector<F_RHIShaderTarget>& OutTargets,
            TF_Set<F_Text>& OutSlangShaderFilePaths
        ) -> F_RHICommonCompilationStatus
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(
                CompileSlangShaderSource_Core(
                    SlangShaderSource,
                    Profile,
                    [&](const F_CompileSlangShaderSourceCallbackParams& CallbackParams)
                    {
                        {
                            TF_Vector<U8> Binary;
                            Binary.resize(CallbackParams.KernelBlob_DXIL->getBufferSize());
                            memcpy(
                                Binary.data(),
                                CallbackParams.KernelBlob_DXIL->getBufferPointer(),
                                Binary.size()
                            );
                            OutTargets.push_back(
                                F_RHIShaderTarget::Make(
                                    F_RHIShaderTargetDXIL::Make(Binary)
                                )
                            );
                        }
                        if (CallbackParams.KernelBlob_HLSL)
                        {
                            OutTargets.push_back(
                                F_RHIShaderTarget::Make(
                                    F_RHIShaderTargetHLSL::Make(
                                        ToText((const char*)CallbackParams.KernelBlob_HLSL->getBufferPointer())
                                    )
                                )
                            );
                        }
                        for (const auto& SlangShaderFilePath : CallbackParams.SlangShaderFilePaths)
                        {
                            OutSlangShaderFilePaths.insert(SlangShaderFilePath);
                        }
                    }
                )
            );
            return F_RHICommonCompilationStatus::MakeSucceeded();
        };
#pragma endregion 
        
#pragma region Vertex Shader
        if (CompileParams.VertexShader)
        {
            const auto& ShaderSource = CompileParams.VertexShader->Source;
            ABYTEK_ENGINE_RHI_ASSERT(ShaderSource.Type == E_RHIShaderSourceType::SLANG);

            const auto& SlangShaderSource = ShaderSource.Slang;

            F_RHIPipelineStateTemplateCompiledVertexShader CompiledShader;

            ABYTEK_FEEDBACK_STATUS_CHECK(
                CompileSlangShaderSource(
                    SlangShaderSource,
                    D3DParseProfile(FeatureSupports, E_RHIShaderFrequency::VERTEX),
                    CompiledShader.Targets,
                    GatheredSlangShaderFilePaths
                )
            );
            OutCompiledData.VertexShader = CompiledShader;
        }
#pragma endregion 
        
#pragma region Pixel Shader
        if (CompileParams.PixelShader)
        {
            const auto& ShaderSource = CompileParams.PixelShader->Source;
            ABYTEK_ENGINE_RHI_ASSERT(ShaderSource.Type == E_RHIShaderSourceType::SLANG);

            const auto& SlangShaderSource = ShaderSource.Slang;

            F_RHIPipelineStateTemplateCompiledPixelShader CompiledShader;

            ABYTEK_FEEDBACK_STATUS_CHECK(
                CompileSlangShaderSource(
                    SlangShaderSource,
                    D3DParseProfile(FeatureSupports, E_RHIShaderFrequency::PIXEL),
                    CompiledShader.Targets,
                    GatheredSlangShaderFilePaths
                )
            );
            OutCompiledData.PixelShader = CompiledShader;
        }
#pragma endregion 
        
#pragma region Mesh Shader
        if (CompileParams.MeshShader)
        {
            const auto& ShaderSource = CompileParams.MeshShader->Source;
            ABYTEK_ENGINE_RHI_ASSERT(ShaderSource.Type == E_RHIShaderSourceType::SLANG);

            const auto& SlangShaderSource = ShaderSource.Slang;

            F_RHIPipelineStateTemplateCompiledMeshShader CompiledShader;

            ABYTEK_FEEDBACK_STATUS_CHECK(
                CompileSlangShaderSource(
                    SlangShaderSource,
                    D3DParseProfile(FeatureSupports, E_RHIShaderFrequency::MESH),
                    CompiledShader.Targets,
                    GatheredSlangShaderFilePaths
                )
            );
            OutCompiledData.MeshShader = CompiledShader;
        }
#pragma endregion
        
#pragma region Amplification Shader
        if (CompileParams.AmplificationShader)
        {
            const auto& ShaderSource = CompileParams.AmplificationShader->Source;
            ABYTEK_ENGINE_RHI_ASSERT(ShaderSource.Type == E_RHIShaderSourceType::SLANG);

            const auto& SlangShaderSource = ShaderSource.Slang;

            F_RHIPipelineStateTemplateCompiledAmplificationShader CompiledShader;

            ABYTEK_FEEDBACK_STATUS_CHECK(
                CompileSlangShaderSource(
                    SlangShaderSource,
                    D3DParseProfile(FeatureSupports, E_RHIShaderFrequency::AMPLIFICATION),
                    CompiledShader.Targets,
                    GatheredSlangShaderFilePaths
                )
            );
            OutCompiledData.AmplificationShader = CompiledShader;
        }
#pragma endregion 
        
#pragma region Compute Shader
        if (CompileParams.ComputeShader)
        {
            const auto& ShaderSource = CompileParams.ComputeShader->Source;
            ABYTEK_ENGINE_RHI_ASSERT(ShaderSource.Type == E_RHIShaderSourceType::SLANG);

            const auto& SlangShaderSource = ShaderSource.Slang;

            F_RHIPipelineStateTemplateCompiledComputeShader CompiledShader;

            ABYTEK_FEEDBACK_STATUS_CHECK(
                CompileSlangShaderSource(
                    SlangShaderSource,
                    D3DParseProfile(FeatureSupports, E_RHIShaderFrequency::COMPUTE),
                    CompiledShader.Targets,
                    GatheredSlangShaderFilePaths
                )
            );
            OutCompiledData.ComputeShader = CompiledShader;
        }
#pragma endregion

#pragma region Root Signature
        TS<F_DirectX12SharedRHIRootSignatureTemplate> RootSignatureTemplate;
        {
            F_DirectX12SharedRHIRootSignatureTemplateCompileParams RootSignatureTemplateCompileParams;
            RootSignatureTemplateCompileParams.Database = CompileParams.Database;
            RootSignatureTemplateCompileParams.RootParameters = OutCompiledData.RootParameters;
            RootSignatureTemplateCompileParams.AllowVertexInputLayout = OutCompiledData.InputElementDescs.size() > 0;
            RootSignatureTemplateCompileParams.AllowBindless = OutCompiledData.AllowBindless;

            auto HashCode = RHITemplateConfigBasedHashCode<F_DirectX12SharedRHIRootSignatureTemplateConfig>(
                RootSignatureTemplateCompileParams,
                CompileParams.CustomBaseDependencyHashCode
            );
            RootSignatureTemplateCompileParams.CustomHashCode = HashCode;

            F_RHICommonCompilationStatus Status = F_FeedbackStatus::MakeSucceeded();
            TS<A_RHITemplate> TempRootSignatureTemplate1 = CompileParams.Database->LockedCreateTemplate(
                HashCode,
                [&]
                {
                    TS<F_DirectX12SharedRHIRootSignatureTemplate> TempRootSignatureTemplate2;
                    Status = CompileRootSignatureTemplate(RootSignatureTemplateCompileParams, TemplateDatabase, TempRootSignatureTemplate2);
                    return TempRootSignatureTemplate2;
                }
            );
            if (!Status)
            {
                return Status;
            }
            RootSignatureTemplate = TempRootSignatureTemplate1.FastCast<F_DirectX12SharedRHIRootSignatureTemplate>();
        }
        OutCompiledData.RootSignatureTemplateHashCode = RootSignatureTemplate->GetHashCode();
#pragma endregion
        
        OutCompiledObject = TS<F_DirectX12SharedRHIPipelineStateTemplate>()(
            CompileParams,
            ABYTEK_MOVE(OutCompiledData)
        );
        OutCompiledObject->SetCompileConfig(CompileParams);
        for (const auto& SlangShaderFilePath : GatheredSlangShaderFilePaths)
        {
            F_RHISlangShaderFileVersion ShaderFileVersion = F_RHISlangShaderFileVersion::Make(SlangShaderFilePath);
            ShaderFileVersion.LoadCurrent();
            OutCompiledObject->AddSlangShaderFileVersion(ShaderFileVersion);
        }
        return F_RHICommonCompilationStatus::MakeSucceeded();
    }
    F_RHICommonCompilationStatus F_DirectX12SharedRHICompiler::D3DCompileRootSignatureTemplate(
        const F_DirectX12SharedRHIRootSignatureTemplateCompileParams& CompileParams,
        const TW_Valid<A_RHITemplateDatabase>& TemplateDatabase,
        TS<F_DirectX12SharedRHIRootSignatureTemplate>& OutCompiledObject
    )
    {
        F_DirectX12SharedRHIRootSignatureTemplateCompiledData OutCompiledData;
        
        TF_SmallVector<D3D12_DESCRIPTOR_RANGE, 32> D3D12DescriptorRanges;
        TF_SmallVector<D3D12_ROOT_PARAMETER, 32> D3D12RootParameters;

        U32 NumDescriptorRanges = 0;
        U32 NumRootParameters = CompileParams.RootParameters.size();
        
        for (const auto& RootParameter : CompileParams.RootParameters)
        {
            if (RootParameter.Type == DirectX12SharedAPIWrapper::E_RootParameterType::DESCRIPTOR_TABLE)
            {
                NumDescriptorRanges += RootParameter.DescriptorTable.Ranges.size();
            }
        }
        D3D12DescriptorRanges.resize(NumDescriptorRanges);
        D3D12RootParameters.resize(NumRootParameters);

        U32 DescriptorRangeIndexToWrite = 0;
        U32 RootParameterIndexToWrite = 0;
        for (const auto& RootParameter : CompileParams.RootParameters)
        {
            D3D12_ROOT_PARAMETER D3D12RootParameter;
            D3D12RootParameter.ShaderVisibility = DirectX12SharedAPIWrapper::Conversions::ShaderVisibilityToD3D12ShaderVisibility(RootParameter.ShaderVisibility);
            switch (RootParameter.Type)
            {
            case DirectX12SharedAPIWrapper::E_RootParameterType::CBV:
                D3D12RootParameter.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
                D3D12RootParameter.Descriptor.ShaderRegister = RootParameter.Descriptor.ShaderRegister;
                D3D12RootParameter.Descriptor.RegisterSpace = RootParameter.Descriptor.RegisterSpace;
                break;
            case DirectX12SharedAPIWrapper::E_RootParameterType::SRV:
                D3D12RootParameter.ParameterType = D3D12_ROOT_PARAMETER_TYPE_SRV;
                D3D12RootParameter.Descriptor.ShaderRegister = RootParameter.Descriptor.ShaderRegister;
                D3D12RootParameter.Descriptor.RegisterSpace = RootParameter.Descriptor.RegisterSpace;
                break;
            case DirectX12SharedAPIWrapper::E_RootParameterType::UAV:
                D3D12RootParameter.ParameterType = D3D12_ROOT_PARAMETER_TYPE_UAV;
                D3D12RootParameter.Descriptor.ShaderRegister = RootParameter.Descriptor.ShaderRegister;
                D3D12RootParameter.Descriptor.RegisterSpace = RootParameter.Descriptor.RegisterSpace;
                break;
            case DirectX12SharedAPIWrapper::E_RootParameterType::CONSTANTS:
                D3D12RootParameter.ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
                D3D12RootParameter.Constants.ShaderRegister = RootParameter.Constants.ShaderRegister;
                D3D12RootParameter.Constants.RegisterSpace = RootParameter.Constants.RegisterSpace;
                D3D12RootParameter.Constants.Num32BitValues = RootParameter.Constants.Num32BitValues;
                break;
            case DirectX12SharedAPIWrapper::E_RootParameterType::DESCRIPTOR_TABLE:
                D3D12RootParameter.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
                D3D12RootParameter.DescriptorTable.NumDescriptorRanges = RootParameter.DescriptorTable.Ranges.size();
                D3D12RootParameter.DescriptorTable.pDescriptorRanges = &D3D12DescriptorRanges[DescriptorRangeIndexToWrite];
                for (const auto& Range : RootParameter.DescriptorTable.Ranges)
                {
                    D3D12_DESCRIPTOR_RANGE D3D12DescriptorRange;
                    switch (Range.Type)
                    {
                    case DirectX12SharedAPIWrapper::E_DescriptorRangeType::CBV:
                        D3D12DescriptorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
                        break;
                    case DirectX12SharedAPIWrapper::E_DescriptorRangeType::SRV:
                        D3D12DescriptorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
                        break;
                    case DirectX12SharedAPIWrapper::E_DescriptorRangeType::UAV:
                        D3D12DescriptorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_UAV;
                        break;
                    case DirectX12SharedAPIWrapper::E_DescriptorRangeType::SAMPLER:
                        D3D12DescriptorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER;
                        break;
                    }
                    D3D12DescriptorRange.NumDescriptors = Range.NumDescriptors;
                    D3D12DescriptorRange.BaseShaderRegister = Range.BaseShaderRegister;
                    D3D12DescriptorRange.RegisterSpace = Range.RegisterSpace;
                    D3D12DescriptorRange.OffsetInDescriptorsFromTableStart = Range.OffsetInDescriptorsFromTableStart;
                    D3D12DescriptorRanges[DescriptorRangeIndexToWrite] = D3D12DescriptorRange;
                    ++DescriptorRangeIndexToWrite;
                }
                break;
            }
            D3D12RootParameters[RootParameterIndexToWrite] = D3D12RootParameter;
            ++RootParameterIndexToWrite;
        }

        D3D12_ROOT_SIGNATURE_DESC D3D12RootSignatureDesc = {};
        D3D12RootSignatureDesc.NumStaticSamplers = 0;
        D3D12RootSignatureDesc.pStaticSamplers = nullptr;
        D3D12RootSignatureDesc.NumParameters = D3D12RootParameters.size();
        D3D12RootSignatureDesc.pParameters = D3D12RootParameters.size() ? D3D12RootParameters.data() : nullptr;
        D3D12RootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_NONE;
        if (CompileParams.AllowVertexInputLayout)
        {
            D3D12RootSignatureDesc.Flags |= D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
        }
        if (CompileParams.AllowBindless)
        {
            D3D12RootSignatureDesc.Flags |= D3D12_ROOT_SIGNATURE_FLAG_CBV_SRV_UAV_HEAP_DIRECTLY_INDEXED;
            D3D12RootSignatureDesc.Flags |= D3D12_ROOT_SIGNATURE_FLAG_SAMPLER_HEAP_DIRECTLY_INDEXED;
        }

        ID3DBlob* SerializedBlob = nullptr;
        ID3DBlob* ErrorBlob = nullptr;
        HRESULT HR = D3D12SerializeRootSignature(
            &D3D12RootSignatureDesc,
            D3D_ROOT_SIGNATURE_VERSION_1,
            &SerializedBlob,
            &ErrorBlob
        );
        if (FAILED(HR))
        {
            F_Text ErrorMessage = ToText((const char*)ErrorBlob->GetBufferPointer());
            ErrorBlob->Release();
            return F_RHICommonCompilationStatus::MakeFailed(ErrorMessage);
        }

        OutCompiledData.Binary.resize(SerializedBlob->GetBufferSize());
        memcpy(
            OutCompiledData.Binary.data(),
            SerializedBlob->GetBufferPointer(),
            SerializedBlob->GetBufferSize()
        );

        SerializedBlob->Release();
        
        OutCompiledObject = TS<F_DirectX12SharedRHIRootSignatureTemplate>()(
            CompileParams,
            ABYTEK_MOVE(OutCompiledData)
        );
        OutCompiledObject->SetCompileConfig(CompileParams);
        return F_RHICommonCompilationStatus::MakeSucceeded();
    }

    F_RHICommonCompilationStatus F_DirectX12SharedRHICompiler::ValidateBindGroupTemplate(
        const F_RHIBindGroupTemplateCompileParams& CompileParams,
        const TW_Valid<A_RHITemplateDatabase>& TemplateDatabase
    )
    {
        ABYTEK_FEEDBACK_STATUS_CHECK(
            A_RHICompiler::ValidateBindGroupTemplate(CompileParams, TemplateDatabase)
        );
        return F_RHICommonCompilationStatus::MakeSucceeded();
    }
    F_RHICommonCompilationStatus F_DirectX12SharedRHICompiler::ValidatePipelineStateTemplate(
        const F_RHIPipelineStateTemplateCompileParams& CompileParams,
        const TW_Valid<A_RHITemplateDatabase>& TemplateDatabase
    )
    {
        ABYTEK_FEEDBACK_STATUS_CHECK(
            A_RHICompiler::ValidatePipelineStateTemplate(CompileParams, TemplateDatabase)
        );
        return F_RHICommonCompilationStatus::MakeSucceeded();
    }
    F_RHICommonCompilationStatus F_DirectX12SharedRHICompiler::ValidateRootSignatureTemplate(
        const F_DirectX12SharedRHIRootSignatureTemplateCompileParams& CompileParams,
        const TW_Valid<A_RHITemplateDatabase>& TemplateDatabase
    )
    {
        return F_RHICommonCompilationStatus::MakeSucceeded();
    }

    F_RHICommonCompilationStatus F_DirectX12SharedRHICompiler::CompileBindGroupTemplate(
        const F_RHIBindGroupTemplateCompileParams& CompileParams,
        const TW_Valid<A_RHITemplateDatabase>& TemplateDatabase,
        TS<A_RHIBindGroupTemplate>& OutCompiledObject
    )
    {
        ABYTEK_FEEDBACK_STATUS_CHECK(
            A_RHICompiler::CompileBindGroupTemplate(CompileParams, TemplateDatabase, OutCompiledObject)
        );
        return D3DCompileBindGroupTemplate(CompileParams, TemplateDatabase, OutCompiledObject);
    }
    F_RHICommonCompilationStatus F_DirectX12SharedRHICompiler::CompilePipelineStateTemplate(
        const F_RHIPipelineStateTemplateCompileParams& CompileParams,
        const TW_Valid<A_RHITemplateDatabase>& TemplateDatabase,
        TS<A_RHIPipelineStateTemplate>& OutCompiledObject
    )
    {
        ABYTEK_FEEDBACK_STATUS_CHECK(
            A_RHICompiler::CompilePipelineStateTemplate(CompileParams, TemplateDatabase, OutCompiledObject)
        );
        return D3DCompilePipelineStateTemplate(CompileParams, TemplateDatabase, OutCompiledObject);
    }
    F_RHICommonCompilationStatus F_DirectX12SharedRHICompiler::CompileRootSignatureTemplate(
        const F_DirectX12SharedRHIRootSignatureTemplateCompileParams& CompileParams,
        const TW_Valid<A_RHITemplateDatabase>& TemplateDatabase,
        TS<F_DirectX12SharedRHIRootSignatureTemplate>& OutCompiledObject
    )
    {
        ABYTEK_FEEDBACK_STATUS_CHECK(
            ValidateRootSignatureTemplate(CompileParams, TemplateDatabase)
        );
        return D3DCompileRootSignatureTemplate(CompileParams, TemplateDatabase, OutCompiledObject);
    }
}
#endif