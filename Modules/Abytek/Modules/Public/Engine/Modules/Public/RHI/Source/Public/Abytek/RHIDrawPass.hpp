#pragma once

#include "Abytek/RHIPass.hpp"
#include "Abytek/RHIHasGeneralPipeline.hpp"
#include "Abytek/RHIIndirectConfig.hpp"


namespace Abytek
{
    enum class E_RHIDrawType : U8
    {
        NONE,
        INDEXED,
        NON_INDEXED,
        DISPATCH_MESH,
        DEFAULT = INDEXED
    };
    
    struct F_RHIDrawNonIndexedConfig
    {
        U32 NumVerticesPerInstance = 0;
        U32 NumInstances = 0;
        U32 VertexOffset = 0;
        U32 InstanceOffset = 0;
        
        static F_RHIDrawNonIndexedConfig Make(
            U32 InNumVerticesPerInstance,
            U32 InNumInstances = 1,
            U32 InVertexOffset = 0,
            U32 InInstanceOffset = 0
        )
        {
            F_RHIDrawNonIndexedConfig Result;
            Result.NumVerticesPerInstance = InNumVerticesPerInstance;
            Result.NumInstances = InNumInstances;
            Result.VertexOffset = InVertexOffset;
            Result.InstanceOffset = InInstanceOffset;
            return Result;
        }
    };
    struct F_RHIDrawIndexedConfig
    {
        U32 NumIndicesPerInstance = 0;
        U32 NumInstances = 0;
        U32 IndexOffset = 0;
        I32 VertexOffset = 0;
        U32 InstanceOffset = 0;
        
        static F_RHIDrawIndexedConfig Make(
            U32 InNumIndicesPerInstance,
            U32 InNumInstances = 1,
            U32 InIndexOffset = 0,
            I32 InVertexOffset = 0,
            U32 InInstanceOffset = 0
        )
        {
            F_RHIDrawIndexedConfig Result;
            Result.NumIndicesPerInstance = InNumIndicesPerInstance;
            Result.NumInstances = InNumInstances;
            Result.IndexOffset = InIndexOffset;
            Result.VertexOffset = InVertexOffset;
            Result.InstanceOffset = InInstanceOffset;
            return Result;
        }
    };
    struct F_RHIDrawDispatchMeshConfig
    {
        F_Vector3_U32 NumThreadGroups = F_Vector3_U32::Zero();
        
        static F_RHIDrawDispatchMeshConfig Make(
            const F_Vector3_U32& InNumThreadGroups = F_Vector3_U32::Zero()
        )
        {
            F_RHIDrawDispatchMeshConfig Result;
            Result.NumThreadGroups = InNumThreadGroups;
            return Result;
        }
    };
    
    struct F_RHIViewportConfig
    {
        F_Vector3_F32 Min = F_Vector3_F32::Zero();
        F_Vector3_F32 Max = F_Vector3_F32::Zero();
        
        static F_RHIViewportConfig Make(
            const F_Vector3_F32& InMin = F_Vector3_F32::Zero(),
            const F_Vector3_F32& InMax = F_Vector3_F32::Zero()
        )
        {
            F_RHIViewportConfig Result;
            Result.Min = InMin;
            Result.Max = InMax;
            return Result;
        }
        
        friend ABYTEK_FORCE_INLINE B8 operator == (const F_RHIViewportConfig& A, const F_RHIViewportConfig& B) noexcept
        {
            return (
                (A.Min == B.Min)    
                && (A.Max == B.Max)    
            );
        }
        friend ABYTEK_FORCE_INLINE B8 operator != (const F_RHIViewportConfig& A, const F_RHIViewportConfig& B) noexcept
        {
            return (
                (A.Min != B.Min)    
                || (A.Max != B.Max)    
            );
        }
    };
    
    struct F_RHIViewportScissorConfig
    {
        TF_SmallVector<F_RHIViewportConfig, 1> Viewports;
        TF_SmallVector<F_Rect_F32, 1> Scissors;
        
        static F_RHIViewportScissorConfig Make(const F_Vector2_F32& Size, const F_Vector2_F32& Offset = F_Vector2_F32::Zero())
        {
            F_RHIViewportScissorConfig Result;
            {
                F_RHIViewportConfig ViewportConfig;
                ViewportConfig.Min.X = Offset.X;
                ViewportConfig.Min.Y = Offset.Y;
                ViewportConfig.Min.Z = 0;
                ViewportConfig.Max.X = Size.X;
                ViewportConfig.Max.Y = Size.Y;
                ViewportConfig.Max.Z = 1;
                Result.Viewports.push_back(ViewportConfig);
            }
            {
                F_Rect_F32 ScissorRect;
                ScissorRect.Min = Offset;
                ScissorRect.Max = Size;
                Result.Scissors.push_back(ScissorRect);
            }
            return Result;
        }
    };
    
    struct F_RHIDrawPassBuildParams : F_RHIPassBuildParams, F_RHIHasGeneralPipelineBuildParams
    {
        E_RHIDrawType DrawType = E_RHIDrawType::DEFAULT;
        F_RHIDrawNonIndexedConfig NonIndexed;
        F_RHIDrawIndexedConfig Indexed;
        F_RHIDrawDispatchMeshConfig DispatchMesh;
        
        F_RHIViewportScissorConfig ViewportScissor;
        
        TF_Optional<F_RHIIndirectConfig> Indirect;
    };
    
    class ABYTEK_ENGINE_RHI_API A_RHIDrawPass : public A_RHIPass, public A_RHIHasGeneralPipeline
    {
    private:        
        E_RHIDrawType _DrawType = E_RHIDrawType::NONE;
        F_RHIDrawNonIndexedConfig _NonIndexed;
        F_RHIDrawIndexedConfig _Indexed;
        F_RHIDrawDispatchMeshConfig _DispatchMesh;
        
        F_RHIViewportScissorConfig _ViewportScissor;
        
        TF_Optional<F_RHIIndirectConfig> _Indirect;

    public:
        ABYTEK_FORCE_INLINE const auto& GetDrawType() const noexcept
        {
            return _DrawType;
        }
        ABYTEK_FORCE_INLINE auto& InjectDrawType() noexcept
        {
            return _DrawType;
        }
        ABYTEK_FORCE_INLINE const auto& GetNonIndexed() const noexcept
        {
            return _NonIndexed;
        }
        ABYTEK_FORCE_INLINE auto& InjectNonIndexed() noexcept
        {
            return _NonIndexed;
        }
        ABYTEK_FORCE_INLINE const auto& GetIndexed() const noexcept
        {
            return _Indexed;
        }
        ABYTEK_FORCE_INLINE auto& InjectIndexed() noexcept
        {
            return _Indexed;
        }
        ABYTEK_FORCE_INLINE const auto& GetDispatchMesh() const noexcept
        {
            return _DispatchMesh;
        }
        ABYTEK_FORCE_INLINE auto& InjectDispatchMesh() noexcept
        {
            return _DispatchMesh;
        }

        ABYTEK_FORCE_INLINE const auto& GetViewportScissor() const noexcept
        {
            return _ViewportScissor;
        }
        ABYTEK_FORCE_INLINE auto& InjectViewportScissor() noexcept
        {
            return _ViewportScissor;
        }

        ABYTEK_FORCE_INLINE const auto& GetIndirect() const noexcept
        {
            return _Indirect;
        }
        ABYTEK_FORCE_INLINE auto& InjectIndirect() noexcept
        {
            return _Indirect;
        }
        
    public:
        ABYTEK_RA_DECLARE_OBJECT_CREATABLE(A_RHIDrawPass);
        virtual void Build(const F_RHIDrawPassBuildParams& BuildParams);
        virtual void Release() override;
        
    public:
        virtual E_RHIPassClass GetPassClass() override
        {
            return E_RHIPassClass::GRAPHICS;
        }
        
    public:
        virtual TS_Valid<A_RHIPassProxy> CreateProxy() override;
    };
}