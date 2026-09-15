#pragma once

#include "Abytek/Engine.Core.prerequisites.hpp"
#include "Abytek/ProgramUtilities/ProgramUnit.hpp"


namespace Abytek
{
    class F_Console;
    
    namespace Internal::ProgramUnit
    {
        template<typename __F_Unit>
        struct TH_UnitUtils
        {
            static TW<__F_Unit> FindUnit(const TW_Valid<F_ProgramUnitContainer>& UnitContainer, const F_Name& Name);
            static void MarkUnitAsPersistent(const TW_Valid<F_ProgramUnitContainer>& , const F_Name& Name);
            static TW_Valid<__F_Unit> EnsureUnit(const TW_Valid<F_ProgramUnitContainer>& UnitContainer, const F_Name& Name, F_UniqueObjectCreator&& UnitCreator, B8 IsPersistent);
        };
    }
    
    namespace ProgramUnitContainerCommands
    {
        using F_OnRemoveUnit = TF_Function<F_FeedbackStatus()>;
    }
    struct F_ProgramUnitContainerQueues
    {
        TF_ConcurrentQueue<TW<F_ProgramUnit>> NewlyAddedUnits;
        TF_ConcurrentQueue<TW<F_ProgramUnit>> UnitsToRemove;
    };
    
    struct F_ProgramUnitContainerBuildParams
    {
        F_Name Name;
        TW<F_Console> Console;
        TW<F_ProgramUnitContainer> Parent;
    };
    
    class ABYTEK_ENGINE_CORE_API F_ProgramUnitContainer : public A_Object
    {
    public:
        ABYTEK_BEGIN_REFLECTOR()
        ABYTEK_END_REFLECTOR(F_ProgramUnitContainer)
        
    private:
        F_Name _Name;
        TW<F_ProgramUnitContainer> _Parent;
        F_Name _FullName;
        
        TW<F_Console> _Console;
        
        TF_UniqueObjectSet<F_ProgramUnit> _Units;
        TF_Map<F_Name, TW<F_ProgramUnit>> _NameToUnitMap;
        TF_Vector<TW<F_ProgramUnitContainer>> _Childs;
        
        U64 _NextUnitPriority = 0;
        
    public:
        F_ProgramUnitContainerQueues Queues;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetName() const noexcept
        {
            return _Name;
        }
        ABYTEK_FORCE_INLINE const auto& GetFullName() const noexcept
        {
            return _FullName;
        }
        ABYTEK_FORCE_INLINE const auto& GetParent() const noexcept
        {
            return _Parent;
        }
        
        ABYTEK_FORCE_INLINE const auto& GetConsole() const noexcept
        {
            return _Console;
        }
        
        ABYTEK_FORCE_INLINE const auto& GetUnits() const noexcept
        {
            return _Units;
        }
        ABYTEK_FORCE_INLINE const auto& GetNameToUnitMap() const noexcept
        {
            return _NameToUnitMap;
        }
        ABYTEK_FORCE_INLINE const auto& GetChilds() const noexcept
        {
            return _Childs;
        }
        
    public:
        F_ProgramUnitContainer(const F_ProgramUnitContainerBuildParams& BuildParams);
        ~F_ProgramUnitContainer() override;

    public:
        template<typename __F_Unit = F_ProgramUnit>
        B8 HasUnit(const F_Name& Name = __F_Unit::GetStaticName()) const noexcept
        {
            auto It = _NameToUnitMap.find(Name);
            if (It == _NameToUnitMap.end())
            {
                return false;
            }
            return It->second.CheckPolymorphism<__F_Unit>();
        }
        template<typename __F_Unit = F_ProgramUnit>
        TW_Valid<__F_Unit> GetUnit(const F_Name& Name = __F_Unit::GetStaticName()) const noexcept
        {
            return _NameToUnitMap.find(Name)->second.WithValidation().FastCast<__F_Unit>();
        }
        template<typename __F_Unit = F_ProgramUnit>
        TW<__F_Unit> FindUnit(const F_Name& Name = __F_Unit::GetStaticName()) const noexcept
        {
            auto It = _NameToUnitMap.find(Name);
            if (It == _NameToUnitMap.end())
            {
                return {};
            }
            TW<__F_Unit> Unit;
            It->second.TryDynamicCast<__F_Unit>(Unit);
            return Unit;
        }
        template<typename __F_Unit = F_ProgramUnit>
        void MarkUnitAsPersistent(const F_Name& Name = __F_Unit::GetStaticName())
        {
            auto Unit = GetUnit<__F_Unit>(Name);
            Unit->_IsPersistent = true;
        }
        template<typename __F_Unit = F_ProgramUnit>
        TW_Valid<__F_Unit> EnsureUnit(const F_Name& Name = __F_Unit::GetStaticName(), F_ProgramUnit::F_UniqueObjectCreator&& UnitCreator = {}, B8 IsPersistent = true)
        {
            ABYTEK_ENGINE_CORE_ASSERT(Name) << "Requires valid name";
            if (HasUnit(Name))
            {
                if (IsPersistent)
                {
                    MarkUnitAsPersistent<__F_Unit>(Name);
                }
                return GetUnit<__F_Unit>(Name);
            }

            if (!UnitCreator)
            {
                UnitCreator = F_ProgramUnit::F_UniqueObjectCreator::Make<__F_Unit>();
            }

            F_ProgramUnitBuildParams BuildParams;
            BuildParams.Container = ABYTEK_WTHIS();
            BuildParams.Name = Name;
            BuildParams.IsPersistent = IsPersistent;
            TU<F_ProgramUnit> OwnedUnit = UnitCreator(BuildParams);
            
            U64 UnitPriority = _NextUnitPriority;
            OwnedUnit->_Priority = UnitPriority;
            ++_NextUnitPriority;

            auto Unit = OwnedUnit.Weak();
            _Units.Add(ABYTEK_MOVE(OwnedUnit));
            _NameToUnitMap.insert({ Name, Unit });
            
            Queues.NewlyAddedUnits.Push(Unit);
            return Unit.FastCast<__F_Unit>();
        }
        template<typename __F_Unit = F_ProgramUnit>
        void EnqueueRemoveUnit(const F_Name& Name = __F_Unit::GetStaticName())
        {
            Queues.UnitsToRemove.Push(GetUnit<__F_Unit>(Name));
        }
        
    public:
        template<typename __F_Unit = F_ProgramUnit>
        B8 HasUnit(const TF_ReflectionTypeHandle<__F_Unit>& UnitType) const noexcept
        {
            F_Name Name = AnyCast<F_Name>(
                UnitType->GetMetadata().Get(F_ProgramUnit::GetMetadataElementName_StaticName())
            );
            return HasUnit(Name);
        }
        template<typename __F_Unit = F_ProgramUnit>
        TW_Valid<__F_Unit> GetUnit(const TF_ReflectionTypeHandle<__F_Unit>& UnitType) const noexcept
        {
            F_Name Name = AnyCast<F_Name>(
                UnitType->GetMetadata().Get(F_ProgramUnit::GetMetadataElementName_StaticName())
            );
            return GetUnit(Name);
        }
        template<typename __F_Unit = F_ProgramUnit>
        TW<__F_Unit> FindUnit(const TF_ReflectionTypeHandle<__F_Unit>& UnitType) const noexcept
        {
            F_Name Name = AnyCast<F_Name>(
                UnitType->GetMetadata().Get(F_ProgramUnit::GetMetadataElementName_StaticName())
            );
            return FindUnit(Name);
        }
        template<typename __F_Unit = F_ProgramUnit>
        void MarkUnitAsPersistent(const TF_ReflectionTypeHandle<__F_Unit>& UnitType)
        {
            F_Name Name = AnyCast<F_Name>(
                UnitType->GetMetadata().Get(F_ProgramUnit::GetMetadataElementName_StaticName())
            );
            MarkUnitAsPersistent<__F_Unit>(Name);
        }
        template<typename __F_Unit = F_ProgramUnit>
        TW_Valid<__F_Unit> EnsureUnit(const TF_ReflectionTypeHandle<__F_Unit>& UnitType, B8 IsPersistent = true)
        {
            F_Name Name = AnyCast<F_Name>(
                UnitType->GetMetadata().Get(F_ProgramUnit::GetMetadataElementName_StaticName())
            );
            auto UnitCreator = AnyCast<F_ProgramUnit::F_UniqueObjectCreator>(
                UnitType->GetMetadata().Get(F_ProgramUnit::GetMetadataElementName_CreateUniqueObject())
            );
            return EnsureUnit<__F_Unit>(Name, ABYTEK_MOVE(UnitCreator), IsPersistent);
        }
        template<typename __F_Unit = F_ProgramUnit>
        void EnqueueRemoveUnit(const TF_ReflectionTypeHandle<__F_Unit>& UnitType)
        {
            F_Name Name = AnyCast<F_Name>(
                UnitType->GetMetadata().Get(F_ProgramUnit::GetMetadataElementName_StaticName())
            );
            EnqueueRemoveUnit<__F_Unit>(Name);
        }
        
    protected:
        virtual void SetupUnits(const TF_Vector<TW<F_ProgramUnit>>& Units);
        virtual void CleanUpUnits(const TF_Vector<TW<F_ProgramUnit>>& Units);
        virtual F_FeedbackStatus ValidateUnit(const TW_Valid<F_ProgramUnit>& Unit);
        
    public:
        void EnqueueRemoveAllUnits();
        struct F_UpdateParams
        {
            TF_Function<void()> OnReflect;
            TF_Function<void()> OnPostReflect;
            TF_Function<void()> OnConfig;
            TF_Function<void()> OnPostConfig;
            TF_Function<void()> OnPreCleanUpReflectionSession;
        };
        F_FeedbackStatus Update(const F_UpdateParams& Params = {});
        
    public:
        template<typename __F_Callback>
        void ForEachUnit(__F_Callback&& Callback, B8 Sorted = true, B8 Reversed = false)
        {
            if (Sorted)
            {
                TF_Vector<TW<F_ProgramUnit>> Units;
                for (const auto& Unit : _Units)
                {
                    Units.push_back(Unit.Weak());
                }
                boost::sort(
                    Units,
                    [Reversed](const TW<F_ProgramUnit>& A, const TW<F_ProgramUnit>& B)
                    {
                        if (Reversed)
                        {
                            return (A->GetPriority() > B->GetPriority());
                        }
                        return (A->GetPriority() < B->GetPriority());
                    }
                );
                for (const auto& Unit : Units)
                {
                    if (!Callback(Unit))
                    {
                        return;
                    }
                }
                return;
            }
            for (const auto& Unit : _Units)
            {
                if (!Callback(Unit.Weak()))
                {
                    return;
                }
            }
        }
        
    public:
        virtual TF_Vector<TW<F_ReflectionSession>> GatherReflectionSessions() const;
    };
    
    namespace Internal::ProgramUnit
    {
        template<typename __F_Unit>
        TW<__F_Unit> TH_UnitUtils<__F_Unit>::FindUnit(const TW_Valid<F_ProgramUnitContainer>& UnitContainer, const F_Name& Name)
        {
            return UnitContainer->FindUnit<__F_Unit>(Name);
        };
        template<typename __F_Unit>
        void TH_UnitUtils<__F_Unit>::MarkUnitAsPersistent(const TW_Valid<F_ProgramUnitContainer>& UnitContainer, const F_Name& Name)
        {
            UnitContainer->MarkUnitAsPersistent<__F_Unit>(Name);
        };
        template<typename __F_Unit>
        TW_Valid<__F_Unit> TH_UnitUtils<__F_Unit>::EnsureUnit(const TW_Valid<F_ProgramUnitContainer>& UnitContainer, const F_Name& Name, F_UniqueObjectCreator&& UnitCreator, B8 IsPersistent)
        {
            return UnitContainer->EnsureUnit<__F_Unit>(Name, ABYTEK_MOVE(UnitCreator), IsPersistent);
        };
    }
}
