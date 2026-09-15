#pragma once

#include "Abytek/RHITemplate.hpp"
#include "Abytek/RHIResourceAccess.hpp"
#include "Abytek/RHISamplerAccess.hpp"
#include "Abytek/RHIBindGroup.hpp"
#include "Abytek/RHIFeature.hpp"



namespace Abytek
{
    class A_RHITemplateDatabase;
    
    struct F_RHIBindGroupTemplateSlot
    {
        F_Name Name;
        E_RHIBindGroupSlotType Type = E_RHIBindGroupSlotType::NONE;    
        F_RHIResourceAccess ResourceAccess;
        F_RHISamplerAccess SamplerAccess;
        union
        {
            E_RHIFormat RTVFormat = E_RHIFormat::NONE;
            E_RHIFormat DSVFormat;
        };
        union
        {
            U32 NumResourceViews = 0;
            U32 NumSamplers;
        };
        F_RHIVertexInputLayout VertexInputLayout;
        U16 UniformDataSizeInBytes = 0;
        U16 UniformDataAlignmentInBytes = 0;
        E_RHIUniformDataFlag UniformDataFlags = E_RHIUniformDataFlag::DEFAULT;

        static F_RHIBindGroupTemplateSlot MakeResourceView(
            const F_Name& InName,
            const F_RHIResourceAccess& InAccess = F_RHIResourceAccess::MakeDefault()
        )
        {
            F_RHIBindGroupTemplateSlot Result;
            Result.Name = InName;
            Result.Type = E_RHIBindGroupSlotType::RESOURCE_VIEW;
            Result.ResourceAccess = InAccess;
            Result.NumResourceViews = 1;
            return Result;
        }
        static F_RHIBindGroupTemplateSlot MakeSampler(
            const F_Name& InName,
            const F_RHISamplerAccess& InAccess = F_RHISamplerAccess::MakeDefault()
        )
        {
            F_RHIBindGroupTemplateSlot Result;
            Result.Name = InName;
            Result.Type = E_RHIBindGroupSlotType::SAMPLER;
            Result.SamplerAccess = InAccess;
            Result.NumSamplers = 1;
            return Result;
        }
        static F_RHIBindGroupTemplateSlot MakeResourceViewSet(
            const F_Name& InName,
            U32 InNumResourceViews,
            const F_RHIResourceAccess& InAccess = F_RHIResourceAccess::MakeDefault()
        )
        {
            F_RHIBindGroupTemplateSlot Result;
            Result.Name = InName;
            Result.Type = E_RHIBindGroupSlotType::RESOURCE_VIEW_SET;
            Result.ResourceAccess = InAccess;
            Result.NumResourceViews = InNumResourceViews;
            return Result;
        }
        static F_RHIBindGroupTemplateSlot MakeSamplerSet(
            const F_Name& InName,
            U32 InNumSamplers,
            const F_RHISamplerAccess& InAccess = F_RHISamplerAccess::MakeDefault()
        )
        {
            F_RHIBindGroupTemplateSlot Result;
            Result.Name = InName;
            Result.Type = E_RHIBindGroupSlotType::SAMPLER_SET;
            Result.SamplerAccess = InAccess;
            Result.NumSamplers = InNumSamplers;
            return Result;
        }
        static F_RHIBindGroupTemplateSlot MakeVertexBuffer(
            const F_Name& InName,
            const F_RHIVertexInputLayout& InVertexInputLayout,
            const F_RHIResourceAccess& InAccess = F_RHIResourceAccess::MakeVertexBuffer()
        )
        {
            F_RHIBindGroupTemplateSlot Result;
            Result.Name = InName;
            Result.Type = E_RHIBindGroupSlotType::VERTEX_BUFFER;
            Result.ResourceAccess = InAccess;
            Result.VertexInputLayout = InVertexInputLayout;
            return Result;
        }
        static F_RHIBindGroupTemplateSlot MakeIndexBuffer(
            const F_Name& InName,
            const F_RHIResourceAccess& InAccess = F_RHIResourceAccess::MakeIndexBuffer()
        )
        {
            F_RHIBindGroupTemplateSlot Result;
            Result.Name = InName;
            Result.Type = E_RHIBindGroupSlotType::INDEX_BUFFER;
            Result.ResourceAccess = InAccess;
            return Result;
        }
        static F_RHIBindGroupTemplateSlot MakeRTV(
            const F_Name& InName,
            E_RHIFormat InFormat,
            const F_RHIResourceAccess& InAccess = F_RHIResourceAccess::MakeRTV()
        )
        {
            F_RHIBindGroupTemplateSlot Result;
            Result.Name = InName;
            Result.Type = E_RHIBindGroupSlotType::RTV;
            Result.ResourceAccess = InAccess;
            Result.RTVFormat = InFormat;
            return Result;
        }
        static F_RHIBindGroupTemplateSlot MakeDSV(
            const F_Name& InName,
            E_RHIFormat InFormat,
            const F_RHIResourceAccess& InAccess = F_RHIResourceAccess::MakeDSV()
        )
        {
            F_RHIBindGroupTemplateSlot Result;
            Result.Name = InName;
            Result.Type = E_RHIBindGroupSlotType::DSV;
            Result.ResourceAccess = InAccess;
            Result.DSVFormat = InFormat;
            return Result;
        }
        static F_RHIBindGroupTemplateSlot MakeUniformData(
            const F_Name& InName,
            U16 InUniformDataSizeInBytes,
            U16 InUniformDataAlignmentInBytes,
            E_RHIUniformDataFlag InUniformDataFlags = E_RHIUniformDataFlag::DEFAULT
        )
        {
            F_RHIBindGroupTemplateSlot Result;
            Result.Name = InName;
            Result.Type = E_RHIBindGroupSlotType::UNIFORM_DATA;
            Result.UniformDataSizeInBytes = InUniformDataSizeInBytes;
            Result.UniformDataAlignmentInBytes = InUniformDataAlignmentInBytes;
            Result.UniformDataFlags = InUniformDataFlags;
            return Result;
        }
        template<typename __F_UniformData>
        static F_RHIBindGroupTemplateSlot MakeUniformData(
            const F_Name& InName,
            E_RHIUniformDataFlag InUniformDataFlags = E_RHIUniformDataFlag::DEFAULT
        )
        {
            return MakeUniformData(
                InName, 
                sizeof(__F_UniformData), 
                ABYTEK_ALIGNOF(__F_UniformData), 
                InUniformDataFlags
            );
        }
        
        friend F_FeedbackStatus operator << (F_ArchiveReadWriteView& View, const F_RHIBindGroupTemplateSlot& Value) noexcept
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.Name);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.Type);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.ResourceAccess);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.SamplerAccess);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.RTVFormat);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.DSVFormat);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.NumResourceViews);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.NumSamplers);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.VertexInputLayout);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.UniformDataSizeInBytes);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.UniformDataAlignmentInBytes);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.UniformDataFlags);
            return F_FeedbackStatus::MakeSucceeded();
        }
        friend F_FeedbackStatus operator >> (F_ArchiveReadOnlyView& View, F_RHIBindGroupTemplateSlot& Value) noexcept
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.Name);
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.Type);
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.ResourceAccess);
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.SamplerAccess);
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.RTVFormat);
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.DSVFormat);
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.NumResourceViews);
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.NumSamplers);
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.VertexInputLayout);
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.UniformDataSizeInBytes);
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.UniformDataAlignmentInBytes);
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.UniformDataFlags);
            return F_FeedbackStatus::MakeSucceeded();
        }
        
        friend B8 operator == (const F_RHIBindGroupTemplateSlot& A, const F_RHIBindGroupTemplateSlot& B)
        {
            if (A.Type != B.Type)
            {
                return false;
            }
            switch (A.Type)
            {
            case E_RHIBindGroupSlotType::NONE:
                break;
            case E_RHIBindGroupSlotType::RESOURCE_VIEW:
            case E_RHIBindGroupSlotType::RESOURCE_VIEW_SET:
                if (
                    (A.ResourceAccess != B.ResourceAccess)
                    || (A.NumResourceViews != B.NumResourceViews)
                )
                {
                    return false;
                }
                break;
            case E_RHIBindGroupSlotType::VERTEX_BUFFER:
                if (
                    (A.ResourceAccess != B.ResourceAccess)
                    || (A.VertexInputLayout != B.VertexInputLayout)
                )
                {
                    return false;
                }
                break;
            case E_RHIBindGroupSlotType::INDEX_BUFFER:
                if (A.ResourceAccess != B.ResourceAccess)
                {
                    return false;
                }
                break;
            case E_RHIBindGroupSlotType::RTV:
                if (
                    (A.ResourceAccess != B.ResourceAccess)
                    || (A.RTVFormat != B.RTVFormat)
                )
                {
                    return false;
                }
                break;
            case E_RHIBindGroupSlotType::DSV:
                if (
                    (A.ResourceAccess != B.ResourceAccess)
                    || (A.DSVFormat != B.DSVFormat)
                )
                {
                    return false;
                }
                break;
            case E_RHIBindGroupSlotType::SAMPLER:
            case E_RHIBindGroupSlotType::SAMPLER_SET:
                if (
                    (A.SamplerAccess != B.SamplerAccess)
                    || (A.NumSamplers != B.NumSamplers)
                )
                {
                    return false;
                }
                break;
            case E_RHIBindGroupSlotType::UNIFORM_DATA:
                if (
                    (A.UniformDataSizeInBytes != B.UniformDataSizeInBytes)
                    || (A.UniformDataAlignmentInBytes != B.UniformDataAlignmentInBytes)
                    || (A.UniformDataFlags != B.UniformDataFlags)
                )
                {
                    return false;
                }
                break;
            default:
                ABYTEK_LOG_FATAL() << "Invalid bind group template slot type: " << static_cast<U32>(A.Type);
                break;
            }
            return (
                (A.Name == B.Name)
            );
        }
        friend B8 operator != (const F_RHIBindGroupTemplateSlot& A, const F_RHIBindGroupTemplateSlot& B)
        {
            return !(A == B);
        }
    };

    struct F_RHIBindGroupTemplateConfig
    {
        TF_Vector<F_RHIBindGroupTemplateSlot> Slots;
        B8 AllowBindless = false;
        
        friend F_FeedbackStatus operator << (F_ArchiveReadWriteView& View, const F_RHIBindGroupTemplateConfig& Value) noexcept
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.Slots);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.AllowBindless);
            return F_FeedbackStatus::MakeSucceeded();
        }
        friend F_FeedbackStatus operator >> (F_ArchiveReadOnlyView& View, F_RHIBindGroupTemplateConfig& Value) noexcept
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.Slots);
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.AllowBindless);
            return F_FeedbackStatus::MakeSucceeded();
        }
        
        friend B8 operator == (const F_RHIBindGroupTemplateConfig& A, const F_RHIBindGroupTemplateConfig& B)
        {
            return (
                (A.Slots == B.Slots)
                && (A.AllowBindless == B.AllowBindless)
            );
        }
        friend B8 operator != (const F_RHIBindGroupTemplateConfig& A, const F_RHIBindGroupTemplateConfig& B)
        {
            return !(A == B);
        }
    };
    struct F_RHIBindGroupTemplateBuildParams : F_RHITemplateBuildParamsBase, F_RHIBindGroupTemplateConfig
    {
    };

    struct F_RHIBindGroupTemplateCompileConfig
    {
        friend F_FeedbackStatus operator << (F_ArchiveReadWriteView& View, const F_RHIBindGroupTemplateCompileConfig& Value) noexcept
        {
            return F_FeedbackStatus::MakeSucceeded();
        }
        friend F_FeedbackStatus operator >> (F_ArchiveReadOnlyView& View, F_RHIBindGroupTemplateCompileConfig& Value) noexcept
        {
            return F_FeedbackStatus::MakeSucceeded();
        }
        
        friend B8 operator == (const F_RHIBindGroupTemplateCompileConfig& A, const F_RHIBindGroupTemplateCompileConfig& B)
        {
            return true;
        }
        friend B8 operator != (const F_RHIBindGroupTemplateCompileConfig& A, const F_RHIBindGroupTemplateCompileConfig& B)
        {
            return !(A == B);
        }
    };
    struct F_RHIBindGroupTemplateCompileParams : F_RHIBindGroupTemplateBuildParams, F_RHIBindGroupTemplateCompileConfig
    {
    };
    
    class ABYTEK_ENGINE_RHI_API A_RHIBindGroupTemplate : public A_RHITemplate
    {
    private:
        F_RHIBindGroupTemplateConfig _Config;
#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
        F_RHIBindGroupTemplateCompileConfig _CompileConfig;
#endif

    public:
        ABYTEK_FORCE_INLINE const auto& GetConfig() const noexcept
        {
            return _Config;
        }
#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
        ABYTEK_FORCE_INLINE const auto& GetCompileConfig() const noexcept
        {
            return _CompileConfig;
        }
#endif
        
        ABYTEK_FORCE_INLINE const auto& GetSlots() const noexcept
        {
            return _Config.Slots;
        }
        ABYTEK_FORCE_INLINE auto GetAllowBindless() const noexcept
        {
            return _Config.AllowBindless;
        }
        
    protected:
        A_RHIBindGroupTemplate(const F_RHIBindGroupTemplateBuildParams& BuildParams);

    public:
        ~A_RHIBindGroupTemplate() override;

    public:
        B8 HasSlot(const F_Name& Name) const;
        TF_Optional<U32> FindSlotIndex(const F_Name& Name) const;

    protected:
        TS_Valid<A_RHITemplateRuntime> CreateAndBuildRuntime(const TW_Valid<A_RHIContext>& Context) override;
        
    public:
        virtual U32 GetEncodedDataSizeInBytes();
        virtual U32 GetEncodedDataAlignmentInBytes();
        
#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
    public:
        void SetCompileConfig(const F_RHIBindGroupTemplateCompileConfig& X)
        {
            _CompileConfig = X;
        }
#endif
    };
}