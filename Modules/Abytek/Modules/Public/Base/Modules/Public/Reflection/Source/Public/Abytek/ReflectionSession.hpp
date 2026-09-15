#pragma once

#include "Abytek/ReflectionItem.hpp"
#include "Abytek/ReflectionQueues.hpp"
#include "Abytek/ReflectionData.hpp"
#include "Abytek/ReflectionType.hpp"
#include "Abytek/ReflectorBase.hpp"
#include "Abytek/ReflectionContext.hpp"


namespace Abytek
{
    namespace Internal::Reflection
    {
        template<E_ReflectMode __DefaultReflectMode>
        struct TH_ReflectForward;
        template<E_ReflectMode __DefaultReflectMode>
        struct TH_RegisterTypeForward;
    }
    
    struct F_ReflectionSessionCreationParams
    {
    };
    struct F_ReflectionSessionBuildParams : F_ReflectionItemBuildParams, F_ReflectionSessionCreationParams
    {
    };
    class ABYTEK_BASE_REFLECTION_API F_ReflectionSession : public A_ReflectionItem
    {
    public:
        friend class F_ReflectionType;
        template<E_ReflectMode __DefaultReflectMode>
        friend struct Internal::Reflection::TH_ReflectForward;
        template<E_ReflectMode __DefaultReflectMode>
        friend struct Internal::Reflection::TH_RegisterTypeForward;
        
    private:
        TF_Set<TW<F_ReflectionType>> _ReferencedTypes;
        TF_Map<F_Name, TW<F_ReflectionType>> _CanonicalToReferencedTypeMap;
        TF_Map<F_GeneralTypeHashCode, TW<F_ReflectionType>> _HashCodeToReferencedTypeMap;
        
        ContainerTemplates::TF_Map<F_GeneralTypeHashCode, F_LinkedReflectType> _TypeLinkedReflectMap;
        
    public:
        F_ReflectionQueues Queues;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetReferencedTypes() const noexcept
        {
            return _ReferencedTypes;
        }
        ABYTEK_FORCE_INLINE const auto& GetCanonicalToReferencedTypeMap() const noexcept
        {
            return _CanonicalToReferencedTypeMap;
        }
        ABYTEK_FORCE_INLINE const auto& GetHashCodeToReferencedTypeMap() const noexcept
        {
            return _HashCodeToReferencedTypeMap;
        }
        
        ABYTEK_FORCE_INLINE const auto& GetTypeLinkedReflectMap() const noexcept { return _TypeLinkedReflectMap; }
        
    public:
        F_ReflectionSession(const F_GBMObjectConfig& GBMConfig, const F_ReflectionSessionBuildParams& BuildParams);
        virtual ~F_ReflectionSession() override;
        
    private:
        template<typename __F_Type>
        TW<F_ReflectionType> _Reflect();
        template<typename __F_Type>
        TW<F_ReflectionType> _ReflectInline();
        template<typename __F_Type>
        TW<F_ReflectionType> _ReflectLinked();
        template<typename __F_Type>
        TW<F_ReflectionType> _ReflectForce();
        
    private:
        template<typename __F_Type, typename __F_Reflector>
        TW<F_ReflectionType> _ReflectSpecific();
        
    private:
        template<typename __F_Type>
        void _RegisterTypeLinkedReflect()
        {
            F_GeneralTypeHashCode TypeHashCode = Abytek::H_GeneralTypeHashCode::StaticValue<__F_Type>;
            F_LinkedReflectType LinkedReflectType;
            LinkedReflectType.FullName = TypeFullName<__F_Type>();
            LinkedReflectType.Callback = [](const TW_Valid<F_ReflectionSession>& ReflectionSession) -> TW<F_ReflectionType>
            {
                return ReflectionSession->_ReflectForce<__F_Type>();  
            };
            _TypeLinkedReflectMap[TypeHashCode] = LinkedReflectType;
        }
        
    private:
        template<typename __F_Type>
        void _RegisterTypeInline();
        template<typename __F_Type>
        void _RegisterTypeLinked();
    
    public:
        B8 HasReferencedType(const TW_Valid<F_ReflectionType>& Type) const;
        B8 HasReferencedType(const F_Name& Canonical) const;
        B8 HasReferencedType(F_GeneralTypeHashCode HashCode) const;
        TW_Valid<F_ReflectionType> GetReferencedType(const F_Name& Canonical) const;
        TW_Valid<F_ReflectionType> GetReferencedType(F_GeneralTypeHashCode HashCode) const;
        TW<F_ReflectionType> FindReferencedType(const F_Name& Canonical) const;
        TW<F_ReflectionType> FindReferencedType(F_GeneralTypeHashCode HashCode) const;
        TW_Valid<F_ReflectionType> ReferenceType(const TW_Valid<F_ReflectionType>& Type);
        TW<F_ReflectionType> UnreferenceType(const TW_Valid<F_ReflectionType>& Type);
        TW<F_ReflectionType> UnreferenceType(const F_Name& Canonical);
        TW<F_ReflectionType> UnreferenceType(F_GeneralTypeHashCode HashCode);
        
    public:
        template<typename __F_Type>
        void RegisterType();
        
    public:
        void Begin();
        void End();
        
    public:
        template<typename __F_Callback>
        void ForEachType(__F_Callback&& Callback)
        {
            for (const auto& Type : _ReferencedTypes)
            {
                if (!Callback(Type))
                {
                    break;
                }
            }
        }
        template<typename __F_Callback>
        void ForEachTypeDerivedFrom(const TW_Valid<F_ReflectionType>& BaseType, __F_Callback&& Callback)
        {
            ForEachType(
                [&](const TW_Valid<F_ReflectionType>& Type)
                {
                    if (Type->HasBase(BaseType))
                    {
                        return Callback(Type);
                    }
                    return true;
                }
            );
        }
    };
    
    namespace Internal::Reflection
    {
        template<>
        struct TH_ReflectForward<E_ReflectMode::INLINE>
        {
            template <typename __F_Type>
            static ABYTEK_FORCE_INLINE TW<F_ReflectionType> Invoke(const TW_Valid<F_ReflectionSession>& ReflectionSession)
            {
                return ReflectionSession->_ReflectInline<__F_Type>();
            }
        };
        template<>
        struct TH_ReflectForward<E_ReflectMode::LINKED>
        {
            template <typename __F_Type>
            static ABYTEK_FORCE_INLINE TW<F_ReflectionType> Invoke(const TW_Valid<F_ReflectionSession>& ReflectionSession)
            {
                return ReflectionSession->_ReflectLinked<__F_Type>();
            }
        };
    }
    template<typename __F_Type>
    TW<F_ReflectionType> F_ReflectionSession::_Reflect()
    {
        static constexpr E_ReflectMode DefaultReflectMode = ReflectorDefaultReflectMode<__F_Type>;
#ifdef ABYTEK_ENABLE_REFLECTION_LOG
        ABYTEK_LOG_INFO() 
            << "Reflecting:" << F_NewLine {}
            << "    Type: \"" << TypeFullName<__F_Type>() << "\"" << F_NewLine {}
            << "    Reflector: \"" << TypeFullName<TF_ResolveReflector<__F_Type>>() << "\"" << F_NewLine {}
            << "    DefaultReflectMode: \"" << (U8)DefaultReflectMode << "\"";
#endif
        return Internal::Reflection::TH_ReflectForward<DefaultReflectMode>::template Invoke<__F_Type>(ABYTEK_WTHIS());
    }
    template<typename __F_Type>
    TW<F_ReflectionType> F_ReflectionSession::_ReflectInline()
    {
#ifdef ABYTEK_ENABLE_REFLECTION_LOG
        ABYTEK_LOG_INFO()
            << "Inline reflecting:" << F_NewLine {}
            << "    Type: \"" << TypeFullName<__F_Type>() << "\"" << F_NewLine {}
            << "    Reflector: \"" << TypeFullName<TF_ResolveReflector<__F_Type>>() << "\"";
#endif
        
        static constexpr F_GeneralTypeHashCode TypeHashCode = Abytek::H_GeneralTypeHashCode::StaticValue<__F_Type>;
        
        auto& TypeMap = GetContext()->GetTypeMap();
        auto TypeIt = TypeMap.find(TypeHashCode);
        if(TypeIt != TypeMap.end())
        {
            return TypeIt->second.Weak();
        }
        
        return _ReflectForce<__F_Type>();
    }
    template<typename __F_Type>
    TW<F_ReflectionType> F_ReflectionSession::_ReflectLinked()
    {
#ifdef ABYTEK_ENABLE_REFLECTION_LOG
        ABYTEK_LOG_INFO()
            << "Linked reflecting:" << F_NewLine {}
            << "    Type: \"" << TypeFullName<__F_Type>() << "\"" << F_NewLine {}
            << "    Reflector: \"" << TypeFullName<TF_ResolveReflector<__F_Type>>() << "\"";
#endif
        
        static constexpr F_GeneralTypeHashCode TypeHashCode = Abytek::H_GeneralTypeHashCode::StaticValue<__F_Type>;
        
        auto& TypeMap = GetContext()->GetTypeMap();
        auto TypeIt = TypeMap.find(TypeHashCode);
        if(TypeIt != TypeMap.end())
        {
            return TypeIt->second.Weak();
        }

        static F_Text (*GetInfoTypeLinkedReflectMap)(const ContainerTemplates::TF_Map<F_GeneralTypeHashCode, F_LinkedReflectType>&) = [](
            const ContainerTemplates::TF_Map<F_GeneralTypeHashCode, F_LinkedReflectType>& TypeLinkedReflectMap 
        )
        {
            F_Text Result;
            for (const auto& Pair : TypeLinkedReflectMap)
            {
                Result += ABYTEK_TEXT("    Linked reflect type: \"");
                Result += ToText(Pair.second.FullName);
                Result += ABYTEK_TEXT("\"");
                Result += ToText(F_NewLine{});
            }
            return ABYTEK_MOVE(Result);
        };
        
        auto It = _TypeLinkedReflectMap.find(TypeHashCode);
        ABYTEK_BASE_REFLECTION_ASSERT(It != _TypeLinkedReflectMap.end()) 
            << "the linked reflect function of \"" 
            << TypeFullName<__F_Type>() 
            << "\" was not registered" 
            << F_NewLine {} 
            << GetInfoTypeLinkedReflectMap(_TypeLinkedReflectMap);
        return It->second(ABYTEK_WTHIS());
    }
    template<typename __F_Type>
    TW<F_ReflectionType> F_ReflectionSession::_ReflectForce()
    {
        return _ReflectSpecific<__F_Type, TF_ResolveReflector<__F_Type>>();
    }
    template<typename __F_Type, typename __F_Reflector>
    TW<F_ReflectionType> F_ReflectionSession::_ReflectSpecific()
    {
#ifdef ABYTEK_ENABLE_REFLECTION_LOG
        ABYTEK_LOG_INFO()
            << "Specific reflecting:" << F_NewLine {}
            << "    Type: \"" << TypeFullName<__F_Type>() << "\"" << F_NewLine {}
            << "    Reflector: \"" << TypeFullName<__F_Reflector>() << "\"";
#endif
        
        ABYTEK_BASE_REFLECTION_ASSERT(!GetContext()->HasType<__F_Type>()) << "\"" << TypeFullName<__F_Type>() << "\" was already reflected";

        auto Result = GetContext()->CreateItem<F_ReflectionType>(
            MakeReflectionTypeBuildParams<__F_Type, __F_Reflector>(GetContext())    
        );
        
        using F_NoConst = std::remove_const_t<__F_Type>;
        
        if constexpr (std::is_const_v<__F_Type>)
        {
            using F_Payload = F_NoConst;
            Result->template ReflectReferenced<F_Payload>();
            Result->InsideHandle = GetContext()->GetType<F_Payload>();
        }
        if constexpr (std::is_pointer_v<__F_Type>)
        {
            using F_Payload = F_NoConst;
            Result->template ReflectReferenced<F_Payload>();
            Result->InsideHandle = GetContext()->GetType<F_Payload>();
        }
        if constexpr (std::is_reference_v<__F_Type>)
        {
            using F_Payload = F_NoConst;
            Result->template ReflectReferenced<F_Payload>();
            Result->InsideHandle = GetContext()->GetType<F_Payload>();
        }
        
        __F_Reflector::ReflectType(ABYTEK_WTHIS(), Result.Weak());
        return Result.Weak();
    }

    template<typename __F_Type>
    void F_ReflectionSession::_RegisterTypeInline()
    {
        Queues.OnPrepare.Push(
            [this]()
            {
                auto Type = _Reflect<__F_Type>();
                ReferenceType(Type);
            }
        );
    }
    template<typename __F_Type>
    void F_ReflectionSession::_RegisterTypeLinked()
    {
        Queues.OnRegisterLinkedTypes.Push(
            [this]()
            {
                _RegisterTypeLinkedReflect<__F_Type>();
            }
        );
        Queues.OnPrepare.Push(
            [this]()
            {
                auto Type = _Reflect<__F_Type>();
                ReferenceType(Type);
            }
        );
    }
    
    namespace Internal::Reflection
    {
        template<>
        struct TH_RegisterTypeForward<E_ReflectMode::INLINE>
        {
            template <typename __F_Type>
            static ABYTEK_FORCE_INLINE void Invoke(const TW_Valid<F_ReflectionSession>& ReflectionSession)
            {
                ReflectionSession->_RegisterTypeInline<__F_Type>();
            }
        };
        template<>
        struct TH_RegisterTypeForward<E_ReflectMode::LINKED>
        {
            template <typename __F_Type>
            static ABYTEK_FORCE_INLINE void Invoke(const TW_Valid<F_ReflectionSession>& ReflectionSession)
            {
                ReflectionSession->_RegisterTypeLinked<__F_Type>();
            }
        };
    }
    template<typename __F_Type>
    void F_ReflectionSession::RegisterType()
    {
        static constexpr E_ReflectMode DefaultReflectMode = ReflectorDefaultReflectMode<__F_Type>;
        Internal::Reflection::TH_RegisterTypeForward<DefaultReflectMode>::template Invoke<__F_Type>(ABYTEK_WTHIS());
    }
}
