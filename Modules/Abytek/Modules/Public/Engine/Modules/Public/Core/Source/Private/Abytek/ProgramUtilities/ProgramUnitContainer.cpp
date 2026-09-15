#include "Abytek/ProgramUtilities/ProgramUnitContainer.hpp"
#include "Abytek/ProgramUtilities/ProgramUnit.hpp"
#include "Abytek/Console.hpp"
#include "Abytek/TaskUtilities.hpp"


namespace Abytek
{
    ABYTEK_REFLECT(F_ProgramUnitContainer)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::F_ProgramUnitContainer"));    
    }
    
    F_ProgramUnitContainer::F_ProgramUnitContainer(const F_ProgramUnitContainerBuildParams& BuildParams) :
        _Name(BuildParams.Name),
        _Parent(BuildParams.Parent),
        _Console(BuildParams.Console)
    {
        if (!_Name)
        {
            _Name = ToText((TF_UInt<sizeof(void*)>)this);
        }
        if(_Parent)
        {
            _Parent->_Childs.push_back(ABYTEK_WTHIS());
            _FullName = ToText(_Parent->GetFullName()->c_str()) + ABYTEK_TEXT("->") + _Name->c_str();
        }
        else
        {
            _FullName = _Name;
        }
    }
    F_ProgramUnitContainer::~F_ProgramUnitContainer()
    {
        EnqueueRemoveAllUnits();
        if (auto Status = Update(); !Status)
        {
            ABYTEK_LOG_FATAL() << "Failed to update program scope: " << Status.ErrorMessage;
        }
        
        if(_Parent)
        {
            auto WThis = ABYTEK_WTHIS();
            for (auto It = _Parent->_Childs.begin(); It != _Parent->_Childs.end(); ++It)
            {
                if (*It == WThis)
                {
                    _Parent->_Childs.erase(It);
                    return;
                }
            }
        }
    }
    
    void F_ProgramUnitContainer::SetupUnits(const TF_Vector<TW<F_ProgramUnit>>& Units)
    {
        {
            for (const auto& Unit : Units)
            {
                Unit->OnInit();
                H_TaskUtilities::Switch();
            }
            for (const auto& Unit : Units)
            {
                Unit->OnPostInit();
                H_TaskUtilities::Switch();
            }
        }
    }
    void F_ProgramUnitContainer::CleanUpUnits(const TF_Vector<TW<F_ProgramUnit>>& Units)
    {
        {
            for (const auto& Unit : Units)
            {
                Unit->OnPreRelease();
                H_TaskUtilities::Switch();
            }
            for (const auto& Unit : Units)
            {
                Unit->OnRelease();
                H_TaskUtilities::Switch();
            }
        }
    }

    F_FeedbackStatus F_ProgramUnitContainer::ValidateUnit(const TW_Valid<F_ProgramUnit>& Unit)
    {
        return F_FeedbackStatus::MakeSucceeded();
    }

    void F_ProgramUnitContainer::EnqueueRemoveAllUnits()
    {
        for (const auto& Unit : _Units)
        {
            EnqueueRemoveUnit(Unit->GetName());
        }
        for (const auto& Unit : _Units)
        {
            EnqueueRemoveUnit(Unit->GetName());
        }
    }
    F_FeedbackStatus F_ProgramUnitContainer::Update(const F_UpdateParams& Params)
    {
        struct F_Frame
        {
            TF_Vector<TW<F_ProgramUnit>> NewlyAddedUnits;
            TF_Vector<TW<F_ProgramUnit>> UnitsToRemove;
        };
       
        F_Frame Frame;
        Frame.NewlyAddedUnits = Queues.NewlyAddedUnits.PopAll();
        Frame.UnitsToRemove = Queues.UnitsToRemove.PopAll();
        
        if (Frame.NewlyAddedUnits.size() > 0)
        {
            for (const auto& Unit : Frame.NewlyAddedUnits)
            {
                ABYTEK_FEEDBACK_STATUS_CHECK_HARD(ValidateUnit(Unit));
            }
            {
                if (Params.OnReflect)
                {
                    Params.OnReflect();
                }
                for (auto& Unit : Frame.NewlyAddedUnits)
                {
                    Unit->_IsOnReflect = true;
                    auto ReflectionSession = Unit->GetReflectionSession();
                    ReflectionSession->Begin();
                    Unit->RegisterReflectionType();
                    Unit->OnReflect();
                    ReflectionSession->End();
                    Unit->_IsOnReflect = false;
                    H_TaskUtilities::Switch();
                }
            }
            {
                if (Params.OnPostReflect)
                {
                    Params.OnPostReflect();
                }
                for (auto& Unit : Frame.NewlyAddedUnits)
                {
                    Unit->OnPostReflect();
                    H_TaskUtilities::Switch();
                }
            }
            
            {
                if (Params.OnConfig)
                {
                    Params.OnConfig();
                }
                for (auto& Unit : Frame.NewlyAddedUnits)
                {
                    Unit->_IsOnConfig = true;
                    Unit->OnConfig();
                    Unit->_IsOnConfig = false;
                    H_TaskUtilities::Switch();
                }
            }
            _Console->Update();
            {
                if (Params.OnPostConfig)
                {
                    Params.OnPostConfig();
                }
                for (auto& Unit : Frame.NewlyAddedUnits)
                {
                    Unit->OnPostConfig();
                    H_TaskUtilities::Switch();
                }
            }
            
            {
                SetupUnits(Frame.NewlyAddedUnits);
                H_TaskUtilities::Switch();
            }
        }
        
        if (Frame.UnitsToRemove.size() > 0)
        {
            TF_Set<TW<F_ProgramUnit>> UnitsToRemove;
            
            TF_Set<TW<F_ProgramUnit>> UnitsToProcess;
            for (const auto& Unit : Frame.UnitsToRemove)
            {
                if (UnitsToProcess.find(Unit) != UnitsToProcess.end())
                {
                    continue;
                }
                UnitsToProcess.insert(Unit);
            }
            
            TF_Set<TW<F_ProgramUnit>> UnitsToCheck;
            
            while ((UnitsToProcess.size() > 0) || (UnitsToCheck.size() > 0))
            {
                {
                    auto CachedUnitsToProcess = ABYTEK_MOVE(UnitsToProcess);
                    for (const auto& Unit : CachedUnitsToProcess)
                    {
                        UnitsToRemove.insert(Unit);
                        for (const auto& Dependency : Unit->_Dependencies)
                        {
                            if (UnitsToRemove.find(Dependency) != UnitsToRemove.end())
                            {
                                continue;
                            }
                            if (UnitsToCheck.find(Dependency) != UnitsToCheck.end())
                            {
                                continue;
                            }
                            UnitsToCheck.insert(Dependency);
                        }
                    }
                    for (const auto& Unit : CachedUnitsToProcess)
                    {
                        for (const auto& Dependent : Unit->_Dependents)
                        {
                            if (UnitsToRemove.find(Dependent) != UnitsToRemove.end())
                            {
                                continue;
                            }
                            if (UnitsToProcess.find(Dependent) != UnitsToProcess.end())
                            {
                                continue;
                            }
                            UnitsToProcess.insert(Dependent);
                        }
                    }
                }
                {
                    auto CachedUnitsToCheck = ABYTEK_MOVE(UnitsToCheck);
                    for (const auto& Unit : CachedUnitsToCheck)
                    {
                        B8 ShouldRemove = !Unit->IsPersistent();
                        if (ShouldRemove)
                        {
                            for (const auto& Dependent : Unit->_Dependents)
                            {
                                if (UnitsToRemove.find(Dependent) == UnitsToRemove.end())
                                {
                                    ShouldRemove = false;
                                    break;
                                }
                            }
                        }
                        if (ShouldRemove)
                        {
                            if (UnitsToProcess.find(Unit) == UnitsToProcess.end())
                            {
                                UnitsToProcess.insert(Unit);
                                break;
                            }
                        }
                    }
                }
            }
            
            TF_Vector<TW<F_ProgramUnit>> SortedUnitsToRemove;
            for (const auto& Unit : UnitsToRemove)
            {
                SortedUnitsToRemove.push_back(Unit);
            }
            
            boost::sort(
                SortedUnitsToRemove,
                [](const TW_Valid<F_ProgramUnit>& A, const TW_Valid<F_ProgramUnit>& B)
                {
                    return A->GetPriority() > B->GetPriority();
                }
            );
            
            CleanUpUnits(SortedUnitsToRemove);
            
            {
                for (const auto& Unit : SortedUnitsToRemove)
                {
                    for (const auto& ConsoleVariableName : Unit->_ConsoleVariableNames)
                    {
                        _Console->UnregisterVariable(ConsoleVariableName);
                    }
                }
            }
            {
                if (Params.OnPreCleanUpReflectionSession)
                {
                    Params.OnPreCleanUpReflectionSession();
                }
                for (const auto& Unit : SortedUnitsToRemove)
                {
                    Unit->GetReflectionSession()->GBMUnmarkAsRoot();
                    F_ReflectionContext::GetGlobal()->Flush();
                }
            }
            for (const auto& Unit : SortedUnitsToRemove)
            {
                _Units.Remove(_Units.Find(Unit));
            }
        }
        return F_FeedbackStatus::MakeSucceeded();
    }

    TF_Vector<TW<F_ReflectionSession>> F_ProgramUnitContainer::GatherReflectionSessions() const
    {
        TF_Vector<TW<F_ReflectionSession>> Result;
        for (const auto& Unit : _Units)
        {
            auto UnitReflectionSessions = Unit->GatherReflectionSessions();
            Result.insert(Result.end(), UnitReflectionSessions.begin(), UnitReflectionSessions.end());
        }
        return ABYTEK_MOVE(Result);
    }
}
