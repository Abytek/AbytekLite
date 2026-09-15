#pragma once

#include "Abytek/Engine.RHI.prerequisites.hpp"
#include "Abytek/RHIShaderFrequency.hpp"


namespace Abytek
{
    enum class E_RHIResourceCPUAccess : U16
    {
        NONE = 0x0,
        
        READ = 0x1,
        WRITE = 0x2,

        DEFAULT = NONE,
        DEFAULT_CAPABILITIES = NONE
    };
    ABYTEK_DEFINE_FLAG_OPERATORS(E_RHIResourceCPUAccess);

    ABYTEK_FORCE_INLINE B8 RHIHasCPUReadOnlyAccess(E_RHIResourceCPUAccess Access)
    {
        return (
            false
            || FlagHas(Access, E_RHIResourceCPUAccess::READ)
        );
    }
    ABYTEK_FORCE_INLINE B8 RHIHasCPUWriteAccess(E_RHIResourceCPUAccess Access)
    {
        return (
            false
            || FlagHas(Access, E_RHIResourceCPUAccess::WRITE)
        );
    }
    
    inline F_Text RHIResourceCPUAccessToInfoText(E_RHIResourceCPUAccess Access)
    {
        F_Text Result;
        
        TF_Vector<F_Text> Values;
        if (FlagHas(Access, E_RHIResourceCPUAccess::READ))
        {
            Values.push_back(ABYTEK_TEXT("READ"));
        }
        if (FlagHas(Access, E_RHIResourceCPUAccess::WRITE))
        {
            Values.push_back(ABYTEK_TEXT("WRITE"));
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
    
    enum class E_RHIResourceGPUAccess
    {
        NONE = 0x0,
        
        SRV = 0x1,
        UAV = 0x2,
        RTV = 0x4,
        DSV = 0x8,
        CBV = 0x10,
        
        VERTEX_BUFFER = 0x20,
        INDEX_BUFFER = 0x40,
        
        COPY_DEST = 0x80,
        COPY_SOURCE = 0x100,
        
        GENERIC_READ_ = 0x200,
        
        RTAS = 0x400,

        DEFAULT = SRV,
        DEFAULT_CAPABILITIES = SRV | COPY_SOURCE | COPY_DEST
    };
    ABYTEK_DEFINE_FLAG_OPERATORS(E_RHIResourceGPUAccess);

    ABYTEK_FORCE_INLINE B8 RHIHasGPUReadOnlyAccess(E_RHIResourceGPUAccess Access)
    {
        return (
            false
            || FlagHas(Access, E_RHIResourceGPUAccess::SRV)
            || FlagHas(Access, E_RHIResourceGPUAccess::CBV)
            || FlagHas(Access, E_RHIResourceGPUAccess::VERTEX_BUFFER)
            || FlagHas(Access, E_RHIResourceGPUAccess::INDEX_BUFFER)
            || FlagHas(Access, E_RHIResourceGPUAccess::COPY_SOURCE)
            || FlagHas(Access, E_RHIResourceGPUAccess::GENERIC_READ_)
            || FlagHas(Access, E_RHIResourceGPUAccess::RTAS)
        );
    }
    ABYTEK_FORCE_INLINE B8 RHIHasGPUWriteAccess(E_RHIResourceGPUAccess Access)
    {
        return (
            false
            || FlagHas(Access, E_RHIResourceGPUAccess::UAV)
            || FlagHas(Access, E_RHIResourceGPUAccess::RTV)
            || FlagHas(Access, E_RHIResourceGPUAccess::DSV)
            || FlagHas(Access, E_RHIResourceGPUAccess::COPY_DEST)
        );
    }
    
    inline F_Text RHIResourceGPUAccessToInfoText(E_RHIResourceGPUAccess Access)
    {
        F_Text Result;
        
        TF_Vector<F_Text> Values;
        if (FlagHas(Access, E_RHIResourceGPUAccess::SRV))
        {
            Values.push_back(ABYTEK_TEXT("SRV"));
        }
        if (FlagHas(Access, E_RHIResourceGPUAccess::UAV))
        {
            Values.push_back(ABYTEK_TEXT("UAV"));
        }
        if (FlagHas(Access, E_RHIResourceGPUAccess::RTV))
        {
            Values.push_back(ABYTEK_TEXT("RTV"));
        }
        if (FlagHas(Access, E_RHIResourceGPUAccess::DSV))
        {
            Values.push_back(ABYTEK_TEXT("DSV"));
        }
        if (FlagHas(Access, E_RHIResourceGPUAccess::CBV))
        {
            Values.push_back(ABYTEK_TEXT("CBV"));
        }
        if (FlagHas(Access, E_RHIResourceGPUAccess::VERTEX_BUFFER))
        {
            Values.push_back(ABYTEK_TEXT("VERTEX_BUFFER"));
        }
        if (FlagHas(Access, E_RHIResourceGPUAccess::INDEX_BUFFER))
        {
            Values.push_back(ABYTEK_TEXT("INDEX_BUFFER"));
        }
        if (FlagHas(Access, E_RHIResourceGPUAccess::COPY_DEST))
        {
            Values.push_back(ABYTEK_TEXT("COPY_DEST"));
        }
        if (FlagHas(Access, E_RHIResourceGPUAccess::COPY_SOURCE))
        {
            Values.push_back(ABYTEK_TEXT("COPY_SOURCE"));
        }
        if (FlagHas(Access, E_RHIResourceGPUAccess::GENERIC_READ_))
        {
            Values.push_back(ABYTEK_TEXT("GENERIC_READ_"));
        }
        if (FlagHas(Access, E_RHIResourceGPUAccess::RTAS))
        {
            Values.push_back(ABYTEK_TEXT("RTAS"));
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
    
    struct F_RHIResourceAccess
    {
        E_RHIResourceCPUAccess CPU = E_RHIResourceCPUAccess::NONE;
        E_RHIResourceGPUAccess GPU = E_RHIResourceGPUAccess::NONE;
        E_RHIShaderFrequency ShaderFrequency = E_RHIShaderFrequency::DEFAULT;

        friend F_RHIResourceAccess operator ~ (const F_RHIResourceAccess& A)
        {
            F_RHIResourceAccess Result;
            Result.CPU = ~A.CPU;
            Result.GPU = ~A.GPU;
            Result.ShaderFrequency = ~A.ShaderFrequency;
            return Result;
        }
        friend F_RHIResourceAccess operator | (const F_RHIResourceAccess& A, const F_RHIResourceAccess& B)
        {
            F_RHIResourceAccess Result;
            Result.CPU = A.CPU | B.CPU;
            Result.GPU = A.GPU | B.GPU;
            Result.ShaderFrequency = A.ShaderFrequency | B.ShaderFrequency;
            return Result;
        }
        friend F_RHIResourceAccess operator & (const F_RHIResourceAccess& A, const F_RHIResourceAccess& B)
        {
            F_RHIResourceAccess Result;
            Result.CPU = A.CPU & B.CPU;
            Result.GPU = A.GPU & B.GPU;
            Result.ShaderFrequency = A.ShaderFrequency & B.ShaderFrequency;
            return Result;
        }
        friend F_RHIResourceAccess operator << (const F_RHIResourceAccess& A, const F_RHIResourceAccess& B)
        {
            F_RHIResourceAccess Result;
            Result.CPU = A.CPU << B.CPU;
            Result.GPU = A.GPU << B.GPU;
            Result.ShaderFrequency = A.ShaderFrequency << B.ShaderFrequency;
            return Result;
        }
        friend F_RHIResourceAccess& operator >> (const F_RHIResourceAccess& A, const F_RHIResourceAccess& B)
        {
            F_RHIResourceAccess Result;
            Result.CPU = A.CPU >> B.CPU;
            Result.GPU = A.GPU >> B.GPU;
            Result.ShaderFrequency = A.ShaderFrequency >> B.ShaderFrequency;
            return Result;
        }
        F_RHIResourceAccess& operator |= (const F_RHIResourceAccess& X)
        {
            CPU |= X.CPU;
            GPU |= X.GPU;
            ShaderFrequency |= X.ShaderFrequency;
            return *this;
        }
        F_RHIResourceAccess& operator &= (const F_RHIResourceAccess& X)
        {
            CPU &= X.CPU;
            GPU &= X.GPU;
            ShaderFrequency &= X.ShaderFrequency;
            return *this;
        }
        F_RHIResourceAccess& operator <<= (const F_RHIResourceAccess& X)
        {
            CPU <<= X.CPU;
            GPU <<= X.GPU;
            ShaderFrequency <<= X.ShaderFrequency;
            return *this;
        }
        F_RHIResourceAccess& operator >>= (const F_RHIResourceAccess& X)
        {
            CPU >>= X.CPU;
            GPU >>= X.GPU;
            ShaderFrequency >>= X.ShaderFrequency;
            return *this;
        }
        friend B8 operator == (const F_RHIResourceAccess& A, const F_RHIResourceAccess& B)
        {
            return (
                (A.CPU == B.CPU)
                && (A.GPU == B.GPU)
                && (A.ShaderFrequency == B.ShaderFrequency)
            );
        }
        friend B8 operator != (const F_RHIResourceAccess& A, const F_RHIResourceAccess& B)
        {
            return (
                (A.CPU != B.CPU)
                || (A.GPU != B.GPU)
                || (A.ShaderFrequency != B.ShaderFrequency)
            );
        }

        B8 Has(const F_RHIResourceAccess& X) const
        {
            return (
                FlagHas(CPU, X.CPU)
                && FlagHas(GPU, X.GPU)
                && FlagHas(ShaderFrequency, X.ShaderFrequency)
            );
        }
        B8 HasAny(const F_RHIResourceAccess& X) const
        {
            return (
                FlagHasAny(CPU, X.CPU)
                || FlagHasAny(GPU, X.GPU)
                || FlagHasAny(ShaderFrequency, X.ShaderFrequency)
            );
        }
        F_RHIResourceAccess Combine(const F_RHIResourceAccess& X) const
        {
            F_RHIResourceAccess Result;
            Result.CPU = FlagCombine(CPU, X.CPU);
            Result.GPU = FlagCombine(GPU, X.GPU);
            Result.ShaderFrequency = FlagCombine(ShaderFrequency, X.ShaderFrequency);
            return Result;
        }
        F_RHIResourceAccess Remove(const F_RHIResourceAccess& X) const
        {
            F_RHIResourceAccess Result;
            Result.CPU = FlagRemove(CPU, X.CPU);
            Result.GPU = FlagRemove(GPU, X.GPU);
            Result.ShaderFrequency = FlagRemove(ShaderFrequency, X.ShaderFrequency);
            return Result;
        }

        static F_RHIResourceAccess MakeNone(E_RHIShaderFrequency InShaderFrequency = E_RHIShaderFrequency::DEFAULT)
        {
            return {
                E_RHIResourceCPUAccess::NONE,
                E_RHIResourceGPUAccess::NONE,
                InShaderFrequency
            };
        }
        static F_RHIResourceAccess MakeDefaultCapabilities(E_RHIShaderFrequency InShaderFrequency = E_RHIShaderFrequency::DEFAULT)
        {
            return MakeSRVCapabilities(InShaderFrequency);
        }
        static F_RHIResourceAccess MakeDefault(E_RHIShaderFrequency InShaderFrequency = E_RHIShaderFrequency::DEFAULT)
        {
            return MakeSRV(InShaderFrequency);
        }
        static F_RHIResourceAccess MakeBaseCapabilities(E_RHIShaderFrequency InShaderFrequency = E_RHIShaderFrequency::DEFAULT)
        {
            return {
                E_RHIResourceCPUAccess::NONE,
                E_RHIResourceGPUAccess::COPY_DEST | E_RHIResourceGPUAccess::COPY_SOURCE,
                InShaderFrequency
            };
        }
        static F_RHIResourceAccess MakeCBVCapabilities(E_RHIShaderFrequency InShaderFrequency = E_RHIShaderFrequency::DEFAULT)
        {
            return {
                E_RHIResourceCPUAccess::NONE,
                E_RHIResourceGPUAccess::CBV | E_RHIResourceGPUAccess::COPY_DEST | E_RHIResourceGPUAccess::COPY_SOURCE,
                InShaderFrequency
            };
        }
        static F_RHIResourceAccess MakeCBV(E_RHIShaderFrequency InShaderFrequency = E_RHIShaderFrequency::DEFAULT)
        {
            return {
                E_RHIResourceCPUAccess::NONE,
                E_RHIResourceGPUAccess::CBV,
                InShaderFrequency
            };
        }
        static F_RHIResourceAccess MakeSRVCapabilities(E_RHIShaderFrequency InShaderFrequency = E_RHIShaderFrequency::DEFAULT)
        {
            return {
                E_RHIResourceCPUAccess::NONE,
                E_RHIResourceGPUAccess::SRV | E_RHIResourceGPUAccess::COPY_DEST | E_RHIResourceGPUAccess::COPY_SOURCE,
                InShaderFrequency
            };
        }
        static F_RHIResourceAccess MakeSRV(E_RHIShaderFrequency InShaderFrequency = E_RHIShaderFrequency::DEFAULT)
        {
            return {
                E_RHIResourceCPUAccess::NONE,
                E_RHIResourceGPUAccess::SRV,
                InShaderFrequency
            };
        }
        static F_RHIResourceAccess MakeUAVCapabilities(E_RHIShaderFrequency InShaderFrequency = E_RHIShaderFrequency::DEFAULT)
        {
            return {
                E_RHIResourceCPUAccess::NONE,
                E_RHIResourceGPUAccess::UAV | E_RHIResourceGPUAccess::COPY_DEST | E_RHIResourceGPUAccess::COPY_SOURCE,
                InShaderFrequency
            };
        }
        static F_RHIResourceAccess MakeUAV(E_RHIShaderFrequency InShaderFrequency = E_RHIShaderFrequency::DEFAULT)
        {
            return {
                E_RHIResourceCPUAccess::NONE,
                E_RHIResourceGPUAccess::UAV,
                InShaderFrequency
            };
        }
        static F_RHIResourceAccess MakeVertexBufferCapabilities(E_RHIShaderFrequency InShaderFrequency = E_RHIShaderFrequency::DEFAULT)
        {
            return {
                E_RHIResourceCPUAccess::NONE,
                E_RHIResourceGPUAccess::VERTEX_BUFFER | E_RHIResourceGPUAccess::COPY_DEST | E_RHIResourceGPUAccess::COPY_SOURCE,
                InShaderFrequency
            };
        }
        static F_RHIResourceAccess MakeVertexBuffer(E_RHIShaderFrequency InShaderFrequency = E_RHIShaderFrequency::DEFAULT)
        {
            return {
                E_RHIResourceCPUAccess::NONE,
                E_RHIResourceGPUAccess::VERTEX_BUFFER,
                InShaderFrequency
            };
        }
        static F_RHIResourceAccess MakeIndexBufferCapabilities(E_RHIShaderFrequency InShaderFrequency = E_RHIShaderFrequency::DEFAULT)
        {
            return {
                E_RHIResourceCPUAccess::NONE,
                E_RHIResourceGPUAccess::INDEX_BUFFER | E_RHIResourceGPUAccess::COPY_DEST | E_RHIResourceGPUAccess::COPY_SOURCE,
                InShaderFrequency
            };
        }
        static F_RHIResourceAccess MakeIndexBuffer(E_RHIShaderFrequency InShaderFrequency = E_RHIShaderFrequency::DEFAULT)
        {
            return {
                E_RHIResourceCPUAccess::NONE,
                E_RHIResourceGPUAccess::INDEX_BUFFER,
                InShaderFrequency
            };
        }
        static F_RHIResourceAccess MakeRTVCapabilities(E_RHIShaderFrequency InShaderFrequency = E_RHIShaderFrequency::DEFAULT)
        {
            return {
                E_RHIResourceCPUAccess::NONE,
                E_RHIResourceGPUAccess::RTV | E_RHIResourceGPUAccess::COPY_DEST | E_RHIResourceGPUAccess::COPY_SOURCE,
                InShaderFrequency
            };
        }
        static F_RHIResourceAccess MakeRTV(E_RHIShaderFrequency InShaderFrequency = E_RHIShaderFrequency::DEFAULT)
        {
            return {
                E_RHIResourceCPUAccess::NONE,
                E_RHIResourceGPUAccess::RTV,
                InShaderFrequency
            };
        }
        static F_RHIResourceAccess MakeDSVCapabilities(E_RHIShaderFrequency InShaderFrequency = E_RHIShaderFrequency::DEFAULT)
        {
            return {
                E_RHIResourceCPUAccess::NONE,
                E_RHIResourceGPUAccess::DSV | E_RHIResourceGPUAccess::COPY_DEST | E_RHIResourceGPUAccess::COPY_SOURCE,
                InShaderFrequency
            };
        }
        static F_RHIResourceAccess MakeDSV(E_RHIShaderFrequency InShaderFrequency = E_RHIShaderFrequency::DEFAULT)
        {
            return {
                E_RHIResourceCPUAccess::NONE,
                E_RHIResourceGPUAccess::DSV,
                InShaderFrequency
            };
        }
        static F_RHIResourceAccess MakeReadbackCapabilities(E_RHIShaderFrequency InShaderFrequency = E_RHIShaderFrequency::DEFAULT)
        {
            return MakeReadback(InShaderFrequency);
        }
        static F_RHIResourceAccess MakeReadback(E_RHIShaderFrequency InShaderFrequency = E_RHIShaderFrequency::DEFAULT)
        {
            return {
                E_RHIResourceCPUAccess::READ,
                E_RHIResourceGPUAccess::COPY_DEST,
                InShaderFrequency
            };
        }
        static F_RHIResourceAccess MakeUploadCapabilities(E_RHIShaderFrequency InShaderFrequency = E_RHIShaderFrequency::DEFAULT)
        {
            return MakeUpload(InShaderFrequency);
        }
        static F_RHIResourceAccess MakeUpload(E_RHIShaderFrequency InShaderFrequency = E_RHIShaderFrequency::DEFAULT)
        {
            return {
                E_RHIResourceCPUAccess::WRITE,
                E_RHIResourceGPUAccess::GENERIC_READ_,
                InShaderFrequency
            };
        }
        static F_RHIResourceAccess MakeCopySrc(E_RHIShaderFrequency InShaderFrequency = E_RHIShaderFrequency::DEFAULT)
        {
            return {
                E_RHIResourceCPUAccess::NONE,
                E_RHIResourceGPUAccess::COPY_SOURCE,
                InShaderFrequency
            };
        }
        static F_RHIResourceAccess MakeCopyDest(E_RHIShaderFrequency InShaderFrequency = E_RHIShaderFrequency::DEFAULT)
        {
            return {
                E_RHIResourceCPUAccess::NONE,
                E_RHIResourceGPUAccess::COPY_DEST,
                InShaderFrequency
            };
        }
    
        F_Text GetInfoText(U32 NumTabs = 0) const
        {
            F_Text Result;
        
            F_Text Tabs;
            for (U32 Index = 0; Index < NumTabs; ++Index)
            {
                Tabs += ABYTEK_TEXT("\t");
            }
        
            Result += Tabs + ABYTEK_TEXT("CPU Access: ") + RHIResourceCPUAccessToInfoText(CPU);
            Result += ToText(F_NewLine {});
            Result += Tabs + ABYTEK_TEXT("GPU Access: ") + RHIResourceGPUAccessToInfoText(GPU);
            Result += ToText(F_NewLine {});
            Result += Tabs + ABYTEK_TEXT("Shader Frequency: ") + RHIShaderFrequencyToInfoText(ShaderFrequency);
        
            return ABYTEK_MOVE(Result);
        }
        
        ABYTEK_FORCE_INLINE B8 IsNone() const noexcept
        {
            return (
                (CPU == E_RHIResourceCPUAccess::NONE)    
                && (GPU == E_RHIResourceGPUAccess::NONE)    
            );
        }
        ABYTEK_FORCE_INLINE explicit operator B8 () const noexcept
        {
            return !IsNone();
        }
        
        friend F_FeedbackStatus operator << (F_ArchiveReadWriteView& View, const F_RHIResourceAccess& Value) noexcept
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.CPU);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.GPU);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.ShaderFrequency);
            return F_FeedbackStatus::MakeSucceeded();
        }
        friend F_FeedbackStatus operator >> (F_ArchiveReadOnlyView& View, F_RHIResourceAccess& Value) noexcept
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.CPU);
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.GPU);
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.ShaderFrequency);
            return F_FeedbackStatus::MakeSucceeded();
        }
    };
    
    enum class E_RHIResourceAccessCategory
    {
        NONE,
        READ,
        UAV,
        RTV,
        DSV,
        COPY_WRITE,
        CPU_WRITE
    };
    ABYTEK_DEFINE_FLAG_OPERATORS(E_RHIResourceAccessCategory);
    inline E_RHIResourceAccessCategory RHIParseResourceAccessCategory(const F_RHIResourceAccess& Access)
    {
        E_RHIResourceAccessCategory Result = E_RHIResourceAccessCategory::NONE;
        if (FlagHasAny(Access.GPU, E_RHIResourceGPUAccess::UAV))
        {
            Result |= E_RHIResourceAccessCategory::UAV;
        }
        if (FlagHasAny(Access.GPU, E_RHIResourceGPUAccess::DSV))
        {
            Result |= E_RHIResourceAccessCategory::DSV;
        }
        if (FlagHasAny(Access.GPU, E_RHIResourceGPUAccess::RTV))
        {
            Result |= E_RHIResourceAccessCategory::RTV;
        }
        if (FlagHasAny(Access.GPU, E_RHIResourceGPUAccess::SRV))
        {
            Result |= E_RHIResourceAccessCategory::READ;
        }
        if (FlagHasAny(Access.GPU, E_RHIResourceGPUAccess::COPY_SOURCE))
        {
            Result |= E_RHIResourceAccessCategory::READ;
        }
        if (FlagHasAny(Access.GPU, E_RHIResourceGPUAccess::COPY_DEST))
        {
            Result |= E_RHIResourceAccessCategory::COPY_WRITE;
        }
        if (RHIHasCPUWriteAccess(Access.CPU))
        {
            Result |= E_RHIResourceAccessCategory::CPU_WRITE;
        }
        return Result;
    }
}

namespace boost
{
    template<>
    struct hash<Abytek::F_RHIResourceAccess>
    {
        size_t operator ()(const Abytek::F_RHIResourceAccess& X) const
        {
            size_t Result = 0;
            hash_combine(Result, X.CPU);
            hash_combine(Result, X.GPU);
            hash_combine(Result, X.ShaderFrequency);
            return Result;
        }
    };
}