#pragma once

#include "ReflectionCommon.hpp"

#define ABYTEK_REFLECT_COMMAND(Name, ...) ReflectionSession->Queues.Name.Push(__VA_ARGS__);

#define ABYTEK_REFLECT_ABSTRACT() ABYTEK_REFLECT_COMMAND( \
                OnReflectTypes, \
                [=]() { \
                    ReflectionType->AdditionalInfo.IsAbstract = true; \
                } \
            )
#define ABYTEK_REFLECT_CANONICAL(...) ABYTEK_REFLECT_COMMAND( \
                OnReflectCanonicals, \
                [=]() { \
                    ReflectionType->SetCanonical(__VA_ARGS__); \
                } \
            )
#define ABYTEK_REFLECT_ALIAS(...) ABYTEK_REFLECT_COMMAND( \
                OnReflectCanonicals, \
                [=]() { \
                    if (const auto& Canonical = ReflectionType->GetCanonical()) \
                    { \
                        auto ReflectionContext = ReflectionSession->GetContext(); \
                        ReflectionContext->CreateItem<Abytek::F_ReflectionAlias>( \
                            Abytek::F_ReflectionAliasBuildParams::Make(ReflectionContext, __VA_ARGS__, Canonical) \
                        ); \
                    } \
                    else \
                    { \
                        ReflectionType->SetCanonical(__VA_ARGS__); \
                    } \
                } \
            )

#define ABYTEK_REFLECT_PROPERTY_ADVANCED(Name, MemberType, MemberOffset) ABYTEK_REFLECT_COMMAND( \
                OnReflectMembers, \
                [=]() { \
                    Abytek::Internal::TH_ReflectProperty< \
                        F_Reflected, \
                        F_Reflector, \
                        MemberType \
                    >::template AddToType<MemberOffset>( \
                        ReflectionSession, \
                        ReflectionType, \
                        #Name \
                    ); \
                } \
            )
#define ABYTEK_REFLECT_PROPERTY(Name) ABYTEK_REFLECT_PROPERTY_ADVANCED( \
                Name, \
                typename Abytek::Internal::TH_MemberType<decltype(&F_Reflected::Name)>::F, \
                ABYTEK_OFFSETOF(F_Reflected, Name) \
            )
#define ABYTEK_REFLECT_PROPERTY_STATIC(Name) ABYTEK_REFLECT_COMMAND( \
                OnReflectMembers, \
                [=]() { \
                    Abytek::Internal::TH_ReflectPropertyStatic< \
                        F_Reflected, \
                        F_Reflector, \
                        decltype(F_Reflected::Name) \
                    >::AddToType( \
                        ReflectionSession, \
                        ReflectionType, \
                        #Name, \
                        F_Reflected::Name \
                    ); \
                } \
            )

#define ABYTEK_REFLECT_METHOD(Name) ABYTEK_REFLECT_COMMAND( \
                OnReflectMembers, \
                [=]() { \
                    Abytek::Internal::TH_ReflectMethod< \
                        F_Reflected, \
                        F_Reflector, \
                        typename Abytek::Internal::TH_MemberType<decltype(&F_Reflected::Name)>::F \
                    >::template AddToType<&F_Reflected::Name>( \
                        ReflectionSession, \
                        ReflectionType, \
                        #Name \
                    ); \
                } \
            )
#define ABYTEK_REFLECT_METHOD_STATIC(Name) ABYTEK_REFLECT_COMMAND( \
                OnReflectMembers, \
                [=]() { \
                    Abytek::Internal::TH_ReflectMethodStatic< \
                        F_Reflected, \
                        F_Reflector, \
                        decltype(F_Reflected::Name) \
                    >::AddToType( \
                        ReflectionSession, \
                        ReflectionType, \
                        #Name, \
                        &F_Reflected::Name \
                    ); \
                } \
            )

#define ABYTEK_REFLECT_ENUM_VALUE(Name) ABYTEK_REFLECT_COMMAND( \
                OnReflectMembers, \
                [=]() { \
                    Abytek::Internal::TH_ReflectEnumValue< \
                        F_Reflected, \
                        F_Reflector \
                    >::AddToType( \
                        ReflectionSession, \
                        ReflectionType, \
                        #Name, \
                        F_Reflected::Name \
                    ); \
                } \
            )