#include "Abytek/ExecutionGraph.hpp"


namespace Abytek
{
    ABYTEK_REFLECT(F_ExecutionGraphFunction)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::F_ExecutionGraphFunction"));
    }

    F_ExecutionGraphFunction::F_ExecutionGraphFunction(F_ExecutionGraphFunctionBuildParams&& BuildParams) :
        _Graph(BuildParams.Graph),
        _Functor(ABYTEK_MOVE(BuildParams.Functor)),
        _Name(BuildParams.Name)
    {
    }
    F_ExecutionGraphFunction::~F_ExecutionGraphFunction()
    {
        while (_Dependencies.size() > 0)
        {
            RemoveDependency(*_Dependencies.begin());
        }
        while (_ReverseDependencies.size() > 0)
        {
            RemoveReverseDependency(*_ReverseDependencies.begin());
        }
    }

    B8 F_ExecutionGraphFunction::HasDependency(const F_Name& Name) const noexcept
    {
        return _Dependencies.find(Name) != _Dependencies.end();
    }
    void F_ExecutionGraphFunction::AddDependency(const F_Name& Name)
    {
        ABYTEK_ENGINE_CORE_ASSERT(Name != _Name) << "Dependency name cannot be self: " << Name;
        ABYTEK_ENGINE_CORE_ASSERT(!HasDependency(Name)) << "Already added dependency: " << Name;
        
        auto X = _Graph->GetFunction(Name);
        // ABYTEK_ENGINE_CORE_ASSERT(!X->HasDependencyRecursive(_Name)) << "Circular dependency: " << _Name << " <-> " << Name;
        _Dependencies.insert(Name);
        
        if (!X->HasReverseDependency(_Name))
        {
            X->AddReverseDependency(_Name);
        }
        
        _Graph->_IsDirty = true;
    }
    void F_ExecutionGraphFunction::RemoveDependency(const F_Name& Name)
    {
        ABYTEK_ENGINE_CORE_ASSERT(HasDependency(Name)) << "Not added dependency: " << Name;
        _Dependencies.erase(_Dependencies.find(Name));
        
        if (_Graph->HasFunction(Name))
        {
            auto X = _Graph->GetFunction(Name);
            if (X->HasReverseDependency(_Name))
            {
                X->RemoveReverseDependency(_Name);
            }
        }
        
        _Graph->_IsDirty = true;
    }
    
    B8 F_ExecutionGraphFunction::HasReverseDependency(const F_Name& Name) const noexcept
    {
        return _ReverseDependencies.find(Name) != _ReverseDependencies.end();
    }
    void F_ExecutionGraphFunction::AddReverseDependency(const F_Name& Name)
    {
        ABYTEK_ENGINE_CORE_ASSERT(Name != _Name) << "Reverse dependency name cannot be self: " << Name;
        ABYTEK_ENGINE_CORE_ASSERT(!HasReverseDependency(Name)) << "Already added dependency: " << Name;
        
        auto X = _Graph->GetFunction(Name);
        // ABYTEK_ENGINE_CORE_ASSERT(!X->HasReverseDependencyRecursive(_Name)) << "Circular dependency: " << _Name << " <-> " << Name;
        _ReverseDependencies.insert(Name);
        
        if (!X->HasDependency(_Name))
        {
            X->AddDependency(_Name);
        }
        
        _Graph->_IsDirty = true;
    }
    void F_ExecutionGraphFunction::RemoveReverseDependency(const F_Name& Name)
    {
        ABYTEK_ENGINE_CORE_ASSERT(HasReverseDependency(Name)) << "Not added dependency: " << Name;
        _ReverseDependencies.erase(_ReverseDependencies.find(Name));
        
        if (_Graph->HasFunction(Name))
        {
            auto X = _Graph->GetFunction(Name);
            if (X->HasDependency(_Name))
            {
                X->RemoveDependency(_Name);
            }
        }
        
        _Graph->_IsDirty = true;
    }

    void F_ExecutionGraphFunction::Broadcast()
    {
        _Functor();
        Event.Broadcast();
        for (const auto& Command : Queue.PopAll())
        {
            Command();
        }
    }

    B8 F_ExecutionGraphFunction::HasDependencyRecursive(const F_Name& Name) const noexcept
    {
        if (HasDependency(Name))
        {
            return true;
        }
        for (const auto& Dependency : _Dependencies)
        {
            auto X = _Graph->GetFunction(Dependency);
            if (X->HasDependencyRecursive(Name))
            {
                return true;
            }
        }
        return false;
    }
    B8 F_ExecutionGraphFunction::HasReverseDependencyRecursive(const F_Name& Name) const noexcept
    {
        if (HasReverseDependency(Name))
        {
            return true;
        }
        for (const auto& ReverseDependency : _ReverseDependencies)
        {
            auto X = _Graph->GetFunction(ReverseDependency);
            if (X->HasReverseDependencyRecursive(Name))
            {
                return true;
            }
        }
        return false;
    }

    ABYTEK_REFLECT(F_ExecutionGraph)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::F_ExecutionGraph"));
    }

    F_ExecutionGraph::F_ExecutionGraph()
    {
    }
    F_ExecutionGraph::~F_ExecutionGraph()
    {
    }

    F_Name F_ExecutionGraph::GenerateAnonymousFunctionName()
    {
        auto AnonymousFunctionId = _NextAnonymousFunctionId;
        ++_NextAnonymousFunctionId;
        return ABYTEK_TEXT("ExecutionGraphFunction@") + ToText(AnonymousFunctionId);
    }

    TW_Valid<F_ExecutionGraphFunction> F_ExecutionGraph::RegisterFunction(TF_Function<void()>&& Functor, const F_Name& Name)
    {
        F_Name ActualName = Name;
        if (!Name)
        {
            ActualName = GenerateAnonymousFunctionName();
        }
        
        ABYTEK_ENGINE_CORE_ASSERT(!HasFunction(ActualName)) << "Already added function with name: " << ActualName;
        
        F_ExecutionGraphFunctionBuildParams BuildParams;
        BuildParams.Graph = ABYTEK_WTHIS();
        BuildParams.Functor = ABYTEK_MOVE(Functor);
        BuildParams.Name = ActualName;
        auto ExecutionGraphFunction = TS<F_ExecutionGraphFunction>()(ABYTEK_MOVE(BuildParams));
        _Functions[ActualName] = ExecutionGraphFunction;
        _IsDirty = true;
        return ExecutionGraphFunction.Weak();
    }
    void F_ExecutionGraph::UnregisterFunction(const TW_Valid<F_ExecutionGraphFunction>& Function)
    {
        _Functions.erase(_Functions.find(Function->GetName()));
        _IsDirty = true;
    }
    void F_ExecutionGraph::UnregisterFunction(const F_Name& Name)
    {
        ABYTEK_ENGINE_CORE_ASSERT(HasFunction(Name)) << "Not found function with name: " << Name;
        UnregisterFunction(GetFunction(Name));
    }

    void F_ExecutionGraph::_BuildGraph()
    {
        TF_Vector<TS<F_ExecutionGraphFunction>> Result;
        if (_Functions.size() > 0)
        {
            TF_Map<F_Name, U32> Scores;
            for (const auto& [FunctionName, Function] : _Functions)
            {
                Scores[FunctionName] = 0;
            }
        
            TF_Queue<TW<F_ExecutionGraphFunction>> FunctionsToIterate;
            
            for (const auto& [FunctionName, Function] : _Functions)
            {
                U32 Score = 0;
                for (const auto& Dependency : Function->GetDependencies())
                {
                    ++Score;
                }
                Scores[FunctionName] = Score;
                
                if (Score == 0)
                {
                    FunctionsToIterate.Push(Function.Weak());
                }
            }
        
            {
                TW<F_ExecutionGraphFunction> FunctionToIterate;
                while (FunctionsToIterate.TryPop(FunctionToIterate))
                {
                    Result.push_back(ShareObject(FunctionToIterate));
                    for (const auto& ReverseDependency : FunctionToIterate->GetReverseDependencies())
                    {
                        U32 ReverseDependencyScore = Scores[ReverseDependency];
                        --ReverseDependencyScore;
                        Scores[ReverseDependency] = ReverseDependencyScore;
                        
                        if (ReverseDependencyScore == 0)
                        {
                            auto ReverseDependencyFunction = GetFunction(ReverseDependency);
                            FunctionsToIterate.Push(ReverseDependencyFunction);
                        }
                    }
                }
            }
        }
        _SortedFunctions = ABYTEK_MOVE(Result);
        _IsDirty = false;
    }

    void F_ExecutionGraph::Update()
    {
        if (_IsDirty)
        {
            _BuildGraph();
        }
        for (const auto& Function : _SortedFunctions)
        {
            Function->Broadcast();
        }
    }
}
