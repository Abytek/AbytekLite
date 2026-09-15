#pragma once

#include "Abytek/Engine.Core.prerequisites.hpp"


namespace Abytek
{
    class F_ExecutionGraph;
    
    using F_ExecutionGraphFunctionEvent = TF_Event<void()>;
    
    struct F_ExecutionGraphFunctionBuildParams
    {
        TW<F_ExecutionGraph> Graph;
        TF_Function<void()> Functor;
        F_Name Name;
    };
    class ABYTEK_ENGINE_CORE_API F_ExecutionGraphFunction final : public A_Object
    {
    public:
        ABYTEK_BEGIN_REFLECTOR()
        ABYTEK_END_REFLECTOR(F_ExecutionGraphFunction)

    private:
        TW<F_ExecutionGraph> _Graph;
        TF_Function<void()> _Functor;
        F_Name _Name;
        
        TF_Set<F_Name> _Dependencies;
        TF_Set<F_Name> _ReverseDependencies;

    public:
        F_ExecutionGraphFunctionEvent Event;
        TF_ConcurrentQueue<TF_Function<void()>> Queue;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetGraph() const noexcept
        {
            return _Graph;
        }
        ABYTEK_FORCE_INLINE const auto& GetFunctor() const noexcept
        {
            return _Functor;
        }
        ABYTEK_FORCE_INLINE const auto& GetName() const noexcept
        {
            return _Name;
        }

        ABYTEK_FORCE_INLINE const auto& GetDependencies() const noexcept
        {
            return _Dependencies;
        }
        ABYTEK_FORCE_INLINE const auto& GetReverseDependencies() const noexcept
        {
            return _ReverseDependencies;
        }
        
    public:
        F_ExecutionGraphFunction(F_ExecutionGraphFunctionBuildParams&& BuildParams);
        ~F_ExecutionGraphFunction() override;
        
    public:
        B8 HasDependency(const F_Name& Name) const noexcept;
        void AddDependency(const F_Name& Name);
        void RemoveDependency(const F_Name& Name);
        void EnsureDependency(const F_Name& Name)
        {
            if (HasDependency(Name))
            {
                return;
            }
            AddDependency(Name);
        }
        
    public:
        B8 HasReverseDependency(const F_Name& Name) const noexcept;
        void AddReverseDependency(const F_Name& Name);
        void RemoveReverseDependency(const F_Name& Name);
        void EnsureReverseDependency(const F_Name& Name)
        {
            if (HasReverseDependency(Name))
            {
                return;
            }
            AddReverseDependency(Name);
        }
        
    public:
        void Broadcast();
        
    public:
        B8 HasDependencyRecursive(const F_Name& Name) const noexcept;
        B8 HasReverseDependencyRecursive(const F_Name& Name) const noexcept;
    };
    
    class ABYTEK_ENGINE_CORE_API F_ExecutionGraph : public A_Object
    {
    public:
        friend class F_ExecutionGraphFunction;
        
    public:
        ABYTEK_BEGIN_REFLECTOR()
        ABYTEK_END_REFLECTOR(F_ExecutionGraph)

    private:
        TF_Map<F_Name, TS<F_ExecutionGraphFunction>> _Functions;
        U64 _NextAnonymousFunctionId = 0;
        B8 _IsDirty = false;
        TF_Vector<TS<F_ExecutionGraphFunction>> _SortedFunctions;

    public:
        ABYTEK_FORCE_INLINE const auto& GetFunctions() const noexcept
        {
            return _Functions;
        }
        ABYTEK_FORCE_INLINE B8 IsDirty() const noexcept
        {
            return _IsDirty;
        }
        ABYTEK_FORCE_INLINE const auto& GetSortedFunctions() const noexcept
        {
            return _SortedFunctions;
        }
        
    public:
        F_ExecutionGraph();
        ~F_ExecutionGraph() override;
        
    public:
        F_Name GenerateAnonymousFunctionName();
        
    public:
        B8 HasFunction(const F_Name& Name) const
        {
            return _Functions.find(Name) != _Functions.end();
        }
        TW_Valid<F_ExecutionGraphFunction> GetFunction(const F_Name& Name) const
        {
            ABYTEK_ENGINE_CORE_ASSERT(HasFunction(Name)) << "Not found function: " << Name;
            return _Functions.find(Name)->second.Weak();
        }
        TW<F_ExecutionGraphFunction> FindFunction(const F_Name& Name) const
        {
            auto It = _Functions.find(Name);
            if (It == _Functions.end())
            {
                return {};
            }
            return It->second.Weak();
        }
        TW_Valid<F_ExecutionGraphFunction> RegisterFunction(TF_Function<void()>&& Functor, const F_Name& Name = {});
        TW_Valid<F_ExecutionGraphFunction> RegisterFunction(const F_Name& Name = {})
        {
            return RegisterFunction([]{}, Name);
        }
        void UnregisterFunction(const TW_Valid<F_ExecutionGraphFunction>& Function);
        void UnregisterFunction(const F_Name& Name);
        
    private:
        void _BuildGraph();
        
    public:
        void Update();
    };
}
