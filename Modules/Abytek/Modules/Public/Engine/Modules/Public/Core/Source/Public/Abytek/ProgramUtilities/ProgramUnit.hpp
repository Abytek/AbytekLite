#pragma once

#include "Abytek/Engine.Core.prerequisites.hpp"
#include "Abytek/Console.hpp"


#define ABYTEK_DECLARE_STATIC_PROGRAM_UNIT(...) \
    ABYTEK_PUBLIC_KEYWORD \
        static Abytek::F_Name GetStaticName(); \
        void RegisterReflectionType() override;
#define ABYTEK_DEFINE_STATIC_PROGRAM_UNIT(STATIC_NAME,...) \
    Abytek::F_Name __VA_ARGS__::GetStaticName() { return STATIC_NAME; } \
    void __VA_ARGS__::RegisterReflectionType() \
    { \
        RegisterStaticType<__VA_ARGS__>(); \
    }

namespace Abytek
{
    class F_ProgramUnitContainer;
    class F_ProgramUnit;
    
    struct F_ProgramUnitBuildParams
    {
        TW<F_ProgramUnitContainer> Container;
        F_Name Name;
        B8 IsPersistent = false;
    };
    
    namespace Internal::ProgramUnit
    {
        using F_UniqueObjectCreator = TF_UniqueObjectCreator<
            F_ProgramUnit, 
            const F_ProgramUnitBuildParams&
        >;
        
        template<typename __F_Unit>
        struct TH_UnitUtils;
    }

    class ABYTEK_ENGINE_CORE_API F_ProgramUnit : public A_Object
    {
    public:
        friend class F_ProgramUnitContainer;
        
    public:
        using F_UniqueObjectCreator = TF_UniqueObjectCreator<
            F_ProgramUnit, 
            const F_ProgramUnitBuildParams&
        >;
        
    public:
        static F_Name GetMetadataElementName_CreateUniqueObject()
        {
            return ABYTEK_NAME("Abytek::F_ProgramUnit::CreateUniqueObject");
        }
        static F_Name GetMetadataElementName_StaticName()
        {
            return ABYTEK_NAME("Abytek::F_ProgramUnit::StaticName");
        }
        
    public:
        ABYTEK_BEGIN_REFLECTOR()
            ABYTEK_REFLECT_GEN_PUBLIC()
            {
                if (!F_UniqueObjectCreator::Check<__F_SelfOrInherited>())
                {
                    return;
                }
                auto& Metadata = ReflectionType->GetMetadata();
                Metadata.Add(
                    GetMetadataElementName_StaticName(),
                    __F_SelfOrInherited::GetStaticName()
                );
                Metadata.Add(
                    GetMetadataElementName_CreateUniqueObject(),
                    F_UniqueObjectCreator::Make<__F_SelfOrInherited>()
                );
            }
        ABYTEK_END_REFLECTOR(F_ProgramUnit)
        
    public:
        static F_Name GetStaticName()
        {
            return {};
        }
        virtual void RegisterReflectionType()
        {
        }
        
    private:
        TW<F_ProgramUnitContainer> _Container;
        F_Name _Name;
        B8 _IsPersistent = false;
        
        TW<F_Console> _Console;
        
        B8 _IsOnReflect = false;
        TW<F_ReflectionSession> _ReflectionSession;
        
        B8 _IsOnConfig = false;
        TF_Vector<F_Name> _ConsoleVariableNames;
        
        TF_Vector<TW<F_ProgramUnit>> _Dependencies;
        TF_Vector<TW<F_ProgramUnit>> _Dependents;
        
        U64 _Priority = 0;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetContainer() const noexcept
        {
            return _Container;
        }
        F_Name GetFullName() const noexcept;
        ABYTEK_FORCE_INLINE const auto& GetName() const noexcept
        {
            return _Name;
        }
        ABYTEK_FORCE_INLINE auto IsPersistent() const noexcept
        {
            return _IsPersistent;
        }
        
        ABYTEK_FORCE_INLINE const auto& GetReflectionSession() const noexcept
        {
            return _ReflectionSession;
        }
        
        template<typename T = F_ProgramUnit>
        B8 HasDependency(const TW_Valid<T>& Unit)
        {
            if (!Unit.template CheckPolymorphism<T>())
            {
                return false;
            }
            return (
                std::find(
                    _Dependencies.begin(),
                    _Dependencies.end(),
                    Unit
                )    
                != _Dependencies.end()
            );
        }
        template<typename T = F_ProgramUnit>
        B8 HasDependency(const F_Name& Name = T::GetStaticName())
        {
            auto Unit = Internal::ProgramUnit::TH_UnitUtils<T>::FindUnit(_Container, Name);
            if (!Unit)
            {
                return false;
            }
            return HasDependency<T>(Unit);
        }
        template<typename T = F_ProgramUnit>
        B8 HasDependency(const TF_ReflectionTypeHandle<T>& Type)
        {
            F_Name Name = AnyCast<F_Name>(
                Type->GetMetadata().Get(GetMetadataElementName_StaticName())
            );
            return HasDependency<T>(Name);
        }
        
        ABYTEK_FORCE_INLINE auto GetPriority() const noexcept
        {
            return _Priority;
        }
        
    public:
        F_ProgramUnit(const F_ProgramUnitBuildParams& BuildParams);
        ~F_ProgramUnit() override;

    protected:
        virtual void OnReflect();
        virtual void OnPostReflect();
        virtual void OnConfig();
        virtual void OnPostConfig();
        
    protected:
        virtual void OnInit();
        virtual void OnPostInit();
        virtual void OnPreRelease();
        virtual void OnRelease();
        
    protected:
        virtual TF_Vector<TW<F_ReflectionSession>> GatherReflectionSessions() const;
        
    public:
        template<typename __F_Type>
        void RegisterStaticType()
        {
            ABYTEK_ENGINE_CORE_ASSERT(_IsOnReflect);
            _ReflectionSession->RegisterType<__F_Type>();
        }
        
        template<typename __F_Value>
        TW<TF_ConsoleVariable<__F_Value>> RegisterConsoleVariable(const F_Name& Name, const F_Text& Description, const __F_Value& DefaultValue) noexcept
        {
            ABYTEK_ENGINE_CORE_ASSERT(_IsOnConfig);
            auto ConsoleVariable = _Console->RegisterVariable(Name, Description, ABYTEK_FORWARD(DefaultValue));
            _ConsoleVariableNames.push_back(Name);
            return ConsoleVariable;
        }
        
    public:
        template<typename T = F_ProgramUnit>
        TW_Valid<T> AddDependency(
            const F_Name& Name = T::GetStaticName(),
            B8 IsPersistent = false
        )
        {
            auto Unit = Internal::ProgramUnit::TH_UnitUtils<T>::EnsureUnit(_Container, Name, {}, IsPersistent);
            return AddDependency<T>(Unit, IsPersistent);
        }
        template<typename T = F_ProgramUnit>
        TW_Valid<T> AddDependency(
            const TF_ReflectionTypeHandle<T>& Type,
            B8 IsPersistent = false
        )
        {
            F_Name Name = AnyCast<F_Name>(
                Type->GetMetadata().Get(GetMetadataElementName_StaticName())
            );
            return AddDependency<T>(Name, IsPersistent);
        }
        template<typename T = F_ProgramUnit>
        TW_Valid<T> AddDependency(
            const TW_Valid<T>& Unit,
            B8 IsPersistent = false
        )
        {
            if (IsPersistent)
            {
                Internal::ProgramUnit::TH_UnitUtils<T>::MarkUnitAsPersistent(_Container, Unit->GetName());
            }
            _Dependencies.push_back(Unit);
            Unit->_Dependents.push_back(ABYTEK_WTHIS());
            return Unit;
        }
    };
}