#pragma once

#include "Abytek/Engine.RenderCore.prerequisites.hpp"


namespace Abytek
{
    struct H_RenderFeatureSets
    {
        static F_RHIFeatureSet MakeDefault()
        {
            F_RHIFeatureSet Result;
            Result.RayTracing = F_RHIFeatureSet::E_RayTracing::NONE;
            Result.MeshShader = F_RHIFeatureSet::E_MeshShader::L1;
            Result.VariableRateShading = F_RHIFeatureSet::E_VariableRateShading::L1;
            Result.SamplerFeedback = F_RHIFeatureSet::E_SamplerFeedback::L1;
            Result.ConservativeRaster = F_RHIFeatureSet::E_ConservativeRaster::L1;
            Result.Binding = F_RHIFeatureSet::E_Binding::L3;
            Result.WaveOps = F_RHIFeatureSet::E_WaveOps::L1;
            Result.AtomicOps = F_RHIFeatureSet::E_AtomicOps::L1;
            Result.PrimitiveTypes = F_RHIFeatureSet::E_PrimitiveTypes::L1;
            Result.MultiView = F_RHIFeatureSet::E_MultiView::NONE;
            return Result;
        }
    };
}
