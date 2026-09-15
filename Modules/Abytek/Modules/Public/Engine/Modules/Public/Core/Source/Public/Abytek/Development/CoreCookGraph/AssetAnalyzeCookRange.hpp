#pragma once

#include "Abytek/Engine.Core.prerequisites.hpp"


#ifdef ABYTEK_ENABLE_DEVELOPMENT_BUILD
namespace Abytek
{
    struct F_AssetAnalyzedObjectInfo
    {
        F_Name Name;
        TF_ReflectionTypeHandle<A_SerializableObject> Type;
    };
    struct F_AssetAnalyzedPackageInfo
    {
        F_Name Name;
        // Map object name to object info
        TF_Map<F_Name, F_AssetAnalyzedObjectInfo> Objects;
    };
    
    struct ABYTEK_ENGINE_CORE_API F_AssetAnalyzeCookRange : A_Object
    {
    public:
        ABYTEK_DECLARE_OBJECT_SINGLETON_CRT(F_AssetAnalyzeCookRange);
        
    public:
        static F_Name GetBeginFunctionName();
        static F_Name GetEndFunctionName();
        static F_Name GetTaskTag();
        
    private:
        F_YieldSynchronizationSection _SynchronizationSection;
        TF_ConcurrentQueue<TF_Function<void()>> _Queue;
        
        // Map package name to object info
        TF_Map<F_Name, F_AssetAnalyzedPackageInfo> _PackageInfos;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetQueue() const noexcept
        {
            return _Queue;
        }
        
        ABYTEK_FORCE_INLINE const auto& GetPackageInfos() const noexcept
        {
            return _PackageInfos;
        }
        
    public:
        F_AssetAnalyzeCookRange();
        ~F_AssetAnalyzeCookRange() override;
        
    protected:
        void Begin();
        void End();
        
    public:
        static void EnqueueCommand(TF_Function<void()>&& Command);
        template<typename __F_Callback>
        static void Ensure(__F_Callback&& Callback)
        {
            GetInstance()->_SynchronizationSection.Ensure(
                ABYTEK_FORWARD(Callback)  
            );
        }
        
    private:
        void _Analyze();
    };
}
#endif

#define ABYTEK_CHECK_ASSET_ANALYZE_COOK_RANGE() ABYTEK_CHECK_TASK_TAG(Abytek::F_AssetAnalyzeCookRange::GetTaskTag())
#define ABYTEK_CHECK_NOT_ASSET_ANALYZE_COOK_RANGE() ABYTEK_CHECK_NOT_TASK_TAG(Abytek::F_AssetAnalyzeCookRange::GetTaskTag())