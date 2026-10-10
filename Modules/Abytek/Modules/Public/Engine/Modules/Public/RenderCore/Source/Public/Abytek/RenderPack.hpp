#pragma once

#include "Abytek/Engine.RenderCore.prerequisites.hpp"
#include "Abytek/World/WorldContext.hpp"
#include "Abytek/RenderCoreHelper.hpp"
#include "Abytek/Cookable.hpp"
#include "Abytek/Renderable.hpp"


namespace Abytek
{
    class F_RenderRegistry;
    class A_RenderRegistryPortData;
    
    struct F_RenderPackBulkHeader
    {
        U64 PayloadOffsetInBytes = 0;
        U64 PayloadSizeInBytes = 0;
        
        friend F_FeedbackStatus operator << (F_ArchiveReadWriteView& View, const F_RenderPackBulkHeader& Value)
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.PayloadOffsetInBytes);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.PayloadSizeInBytes);
            return F_FeedbackStatus::MakeSucceeded();
        }
        friend F_FeedbackStatus operator >> (F_ArchiveReadOnlyView& View, F_RenderPackBulkHeader& Value)
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.PayloadOffsetInBytes);
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.PayloadSizeInBytes);
            return F_FeedbackStatus::MakeSucceeded();
        }
    };
    
    class ABYTEK_ENGINE_RENDER_CORE_API A_RenderPackTemplateMap
    {
    private:
        TF_Map<F_RHITemplateHashCode, TS<A_RHITemplate>> _Templates;
        
        // Slow remove because this one is typically used for render packs, which have only template additions in cooked build! 
        TF_Vector<TW<A_RHITemplate>> _SortedTemplates;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetTemplates() const noexcept
        {
            return _Templates;
        }
        ABYTEK_FORCE_INLINE auto& GetSortedTemplates() noexcept
        {
            return _SortedTemplates;
        }
        
    protected:
        A_RenderPackTemplateMap();
        
    public:
        virtual ~A_RenderPackTemplateMap();
        
    public:
        B8 HasTemplate(F_RHITemplateHashCode HashCode) const;
        TS<A_RHITemplate> FindTemplate(F_RHITemplateHashCode HashCode);
        TS<A_RHITemplate> GetTemplate(F_RHITemplateHashCode HashCode);
        void AddTemplate(const TS<A_RHITemplate>& Template);
        void RemoveTemplate(F_RHITemplateHashCode HashCode);
        void EnsureTemplate(const TS<A_RHITemplate>& Template);
        void RemoveUnusedTemplates();
        
    protected:
        virtual void OnAddTemplate(const TS<A_RHITemplate>& Template);
        virtual void OnRemoveTemplate(F_RHITemplateHashCode HashCode);
        virtual void OnModifyTemplates();
        
    public:
        B8 ShouldCompile(
            const F_RHIPipelineStateTemplateCompileParams& CompileParams,
            const TF_Set<F_RHITemplateHashCode>& TemplateHashCodesToCompile = {},
            const F_Name& Name = {}
        );
        B8 ShouldCompile(
            const F_RHIBindGroupTemplateCompileParams& CompileParams,
            const TF_Set<F_RHITemplateHashCode>& TemplateHashCodesToCompile = {},
            const F_Name& Name = {}
        );
        
    public:
        B8 TryBuildCommand(
            const TS<F_RenderRegistry>& RenderRegistry,
            B8 ShouldCompileByDefault,
            const F_RHIPipelineStateTemplateCompileParams& CompileParams,
            TF_Vector<TF_Function<void(TF_Vector<TS<A_RHITemplate>>& OutTemplates)>>& OutCommands,
            TF_Set<F_RHITemplateHashCode>& OutTemplateHashCodesToCompile,
            TF_Set<F_RHITemplateHashCode>& OutTemplateHashCodes,
            const F_Name& Name = {}
        );
        B8 TryBuildCommand(
            const TS<F_RenderRegistry>& RenderRegistry,
            B8 ShouldCompileByDefault,
            const F_RHIBindGroupTemplateCompileParams& CompileParams,
            TF_Vector<TF_Function<void(TF_Vector<TS<A_RHITemplate>>& OutTemplates)>>& OutCommands,
            TF_Set<F_RHITemplateHashCode>& OutTemplateHashCodesToCompile,
            TF_Set<F_RHITemplateHashCode>& OutTemplateHashCodes,
            const F_Name& Name = {}
        );
    };
    
    class ABYTEK_ENGINE_RENDER_CORE_API F_RenderPackTemplateMap : public A_Object, public A_RenderPackTemplateMap
    {
    public:
        F_RenderPackTemplateMap();
        ~F_RenderPackTemplateMap() override;
    };
    
    class ABYTEK_ENGINE_RENDER_CORE_API F_RenderPackData : public A_Object, public A_RenderPackTemplateMap
    {
    public:
        friend class F_RenderPack;
        
    private:
        TW<F_RenderPack> _Pack;
        TS<A_RenderRegistryPortData> _PortData;
        TF_Map<F_RHITemplateHashCode, TS<A_RHITemplateRuntime>> _TemplateRuntimes;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetPack() const noexcept
        {
            return _Pack;
        }
        ABYTEK_FORCE_INLINE const auto& GetPortData() const noexcept
        {
            return _PortData;
        }
        ABYTEK_FORCE_INLINE const auto& GetTemplateRuntimes() const noexcept
        {
            return _TemplateRuntimes;
        }
        
    public:
        F_RenderPackData(
            const TW_Valid<F_RenderPack>& Pack,
            const TS<A_RenderRegistryPortData>& PortData
        );
        ~F_RenderPackData() override;
        
    protected:
        void OnAddTemplate(const TS<A_RHITemplate>& Template) override;
        void OnRemoveTemplate(F_RHITemplateHashCode HashCode) override;
        
    public:
        void EnqueueCommand(TF_Function<void()>&& Command);
    };
    
    class ABYTEK_ENGINE_RENDER_CORE_API F_RenderPack : public A_WorldContext, public A_RenderPackTemplateMap, public I_Cookable
    {
    public:
        ABYTEK_BEGIN_REFLECTOR(A_WorldContext)
        ABYTEK_END_REFLECTOR(F_RenderPack);
    
    private:
        TS<F_RenderRegistry> _Registry;
        
        B8 _IsTemplatesLoaded = false;
        B8 _IsInitialTemplateAdding = false;
        
#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
        TS<F_RenderPackTemplateMap> _CookedTemplateMap;
#endif
        
        TF_Vector<TS<F_RenderPackData>> _DataList;
        TS<F_RenderPackData> _MainData;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetRegistry() const noexcept
        {
            return _Registry;
        }
        F_RHITemplateHashCode GetBaseDependencyHashCodeForTemplates() const noexcept
        {
            return H_RenderCore::GenerateBaseDependencyHashCodeForTemplates<F_RenderPack>(
                GetName().GetHashCode()
            );
        }
        
#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
        ABYTEK_FORCE_INLINE const auto& GetCookedTemplateMap() const noexcept
        {
            return _CookedTemplateMap;
        }
#endif
        
        ABYTEK_FORCE_INLINE const auto& GetDataList() const noexcept
        {
            return _DataList;
        }
        ABYTEK_FORCE_INLINE const auto& GetMainData() const noexcept
        {
            return _MainData;
        }
        TS<F_RenderPackData> FindData(const TW_Valid<A_RenderRegistryPortData> PortData) const noexcept
        {
            for (const auto& Data : _DataList)
            {
                if (Data->GetPortData().Weak() == PortData)
                {
                    return Data;
                }
            }
            return {};
        }
        
    public:
        F_RenderPack(const F_SerializableObjectInitParams& InitParams);
        ~F_RenderPack() override;
        
    protected:
        void OnLoad() override;
        void OnUnload() override;
        
    protected:
        F_FeedbackStatus BinarySerialize(F_SerializableObjectBinarySerializeParams& Params) override;
        F_FeedbackStatus BinaryDeserialize(F_SerializableObjectBinaryDeserializeParams& Params) override;
        
#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
    public:
        virtual void PrepareTemplates(
            const TS<F_RenderRegistry>& RenderRegistry,
            const TW_Valid<A_RenderPackTemplateMap>& RenderPackTemplateMap
        );
#endif
        
    protected:
        void OnAddTemplate(const TS<A_RHITemplate>& Template) override;
        void OnRemoveTemplate(F_RHITemplateHashCode HashCode) override;
        void OnModifyTemplates() override;
        
#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
    protected:
        void PrepareForCooking() override;
        void Cook() override;
        void CleanUpAfterCooking() override;
#endif
        
    public:
        void AddData(const TS<A_RenderRegistryPortData>& PortData);
        void RemoveData(const TS<A_RenderRegistryPortData>& PortData);
        
    public:
#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
        // Compile new templates.
        // Notes that this function will remove all the unused templates.
        static void ExecuteExclusiveTemplateCompilation(
            const TS<F_RenderRegistry>& RenderRegistry,
            const TW_Valid<A_RenderPackTemplateMap>& RenderPackTemplateMap,
            const TF_Set<F_RHITemplateHashCode>& TemplateHashCodesToCompile,
            const TF_Set<F_RHITemplateHashCode>& RootTemplateHashCodes,
            TF_Function<void(TF_Vector<TS<A_RHITemplate>>& OutNewTemplates)>&& MainWork
        );
        // Execute compile commands in parallel
        static void ExecuteParallelCompileCommands(
            const TF_Vector<TF_Function<void(TF_Vector<TS<A_RHITemplate>>& OutTemplates)>>& Commands,
            TF_Vector<TS<A_RHITemplate>>& OutNewTemplates
        );
#endif
    };
}