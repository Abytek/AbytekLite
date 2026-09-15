#pragma once

#include "Abytek/Engine.RHI.prerequisites.hpp"


namespace Abytek
{
    enum class E_RHIShaderFrequency
    {
        NONE = 0x0,
        VERTEX = 0x1,
        PIXEL = 0x2,
        COMPUTE = 0x4,
        RT_RAY_GEN = 0x8,
        RT_CLOSEST_HIT = 0x10,
        RT_ANY_HIT = 0x20,
        RT_MISS = 0x40,
        WORK_GRAPH = 0x80,
        AMPLIFICATION = 0x100,
        MESH = 0x200,
        ALL = VERTEX | PIXEL | COMPUTE | RT_RAY_GEN | RT_CLOSEST_HIT | RT_ANY_HIT | RT_MISS | WORK_GRAPH | AMPLIFICATION | MESH,
        DEFAULT = ALL
    };
    ABYTEK_DEFINE_FLAG_OPERATORS(E_RHIShaderFrequency);
    
    inline F_Text RHIShaderFrequencyToInfoText(E_RHIShaderFrequency Access)
    {
        F_Text Result;
        
        TF_Vector<F_Text> Values;
        if (FlagHas(Access, E_RHIShaderFrequency::VERTEX))
        {
            Values.push_back(ABYTEK_TEXT("VERTEX"));
        }
        if (FlagHas(Access, E_RHIShaderFrequency::PIXEL))
        {
            Values.push_back(ABYTEK_TEXT("PIXEL"));
        }
        if (FlagHas(Access, E_RHIShaderFrequency::COMPUTE))
        {
            Values.push_back(ABYTEK_TEXT("COMPUTE"));
        }
        if (FlagHas(Access, E_RHIShaderFrequency::RT_RAY_GEN))
        {
            Values.push_back(ABYTEK_TEXT("RT_RAY_GEN"));
        }
        if (FlagHas(Access, E_RHIShaderFrequency::RT_CLOSEST_HIT))
        {
            Values.push_back(ABYTEK_TEXT("RT_CLOSEST_HIT"));
        }
        if (FlagHas(Access, E_RHIShaderFrequency::RT_ANY_HIT))
        {
            Values.push_back(ABYTEK_TEXT("RT_ANY_HIT"));
        }
        if (FlagHas(Access, E_RHIShaderFrequency::RT_MISS))
        {
            Values.push_back(ABYTEK_TEXT("RT_MISS"));
        }
        if (FlagHas(Access, E_RHIShaderFrequency::WORK_GRAPH))
        {
            Values.push_back(ABYTEK_TEXT("WORK_GRAPH"));
        }
        if (FlagHas(Access, E_RHIShaderFrequency::AMPLIFICATION))
        {
            Values.push_back(ABYTEK_TEXT("AMPLIFICATION"));
        }
        if (FlagHas(Access, E_RHIShaderFrequency::MESH))
        {
            Values.push_back(ABYTEK_TEXT("MESH"));
        }
        
        Result += ABYTEK_TEXT("[");
        
        U32 NumValues = Values.size();
        for (U32 Idx = 0; Idx < NumValues; ++Idx)
        {
            Result += Values[Idx];
            if (Idx + 1 < NumValues)
            {
                Result += ABYTEK_TEXT(", ");
            }
        }
        
        Result += ABYTEK_TEXT("]");
        return ABYTEK_MOVE(Result);
    }
}
