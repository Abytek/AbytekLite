#pragma once

#include "Abytek/ReflectionCommon.hpp"
#include "Abytek/ReflectionData.hpp"
#include "Abytek/ReflectionSession.hpp"
#include "Abytek/ReflectionType.hpp"


namespace Abytek
{
    namespace Internal
    {
        template<typename __F>
        struct TH_MakeSureCorrectIfMember
        {
            using F = __F;
        };
        template<typename __F_Return, typename... __F_Args>
        struct TH_MakeSureCorrectIfMember<__F_Return(__F_Args...)>
        {
            using F = __F_Return(__F_Args...);
        };
        template<typename __F_Return, typename... __F_Args>
        struct TH_MakeSureCorrectIfMember<__F_Return(__F_Args...) const>
        {
            using F = __F_Return(__F_Args...);
        };
        template<typename __F_Return, typename... __F_Args>
        struct TH_MakeSureCorrectIfMember<__F_Return(__F_Args...) noexcept>
        {
            using F = __F_Return(__F_Args...);
        };
        template<typename __F_Return, typename... __F_Args>
        struct TH_MakeSureCorrectIfMember<__F_Return(__F_Args...) const noexcept>
        {
            using F = __F_Return(__F_Args...);
        };
        
        template<typename __F_MemberPType>
        struct TH_MemberType;
        template<class __F_UserClass, typename __F>
        struct TH_MemberType<__F (__F_UserClass::*)>
        {
            using F = __F;
        };
        
        template<class __F_UserClass, class __F_Reflector, typename __F_Functor>
        struct TH_ReflectMethod;        
        template<class __F_UserClass, class __F_Reflector, typename __F_Return, typename... __F_Args>
        struct TH_ReflectMethod<__F_UserClass, __F_Reflector, __F_Return(__F_Args...)>
        {
            using F_Functor = __F_Return(__F_Args...);

            template<__F_Return(__F_UserClass::*__MemberP)(__F_Args...)>
            struct TH_AddToType
            {
                using F_CallerFunctor = __F_Return(__F_UserClass*, __F_Args...);

                static auto Call(__F_UserClass* ObjectRawP, __F_Args... Args)
                {
                    return (ObjectRawP->*__MemberP)(ABYTEK_FORWARD(Args)...);
                }
            };

            template<__F_Return(__F_UserClass::*__MemberP)(__F_Args...)>
            static F_ReflectionMethod& AddToType(
                const TW_Valid<F_ReflectionSession>& ReflectionSession,
                const TW_Valid<F_ReflectionType>& ReflectionType,
                const F_Char* NameCStr,
                const TF_Function<void(F_ReflectionMethod& ReflectionMethod)>& UserSetup = {}
            )
            {
                using H = TH_AddToType<__MemberP>;
                
                auto& AdditionalInfo = ReflectionType->AdditionalInfo;
                                
                ReflectionType->ReflectReferenced<F_Functor>();
                
                F_ReflectionMethod Method;
                Method.Name = ToText(NameCStr);
                Method.TypeHashCode = Abytek::H_GeneralTypeHashCode::StaticValue<F_Functor>;
                Method.CallerTypeHashCode = Abytek::H_GeneralTypeHashCode::StaticValue<__F_Return(__F_UserClass*, __F_Args...)>;
                Method.Address = (PDiff)(&H::Call);
                Method.Signature.IsStatic = false;

                auto MakeReflectionNamedVariable = [](F_GeneralTypeHashCode TypeHashCode)->F_ReflectionNamedVariable
                {
                    F_ReflectionNamedVariable Result;
                    Result.TypeHashCode = TypeHashCode;
                    
                    return boost::move(Result);
                };
                Method.Signature.NamedInputs = {
                    MakeReflectionNamedVariable(Abytek::H_GeneralTypeHashCode::StaticValue<__F_Args>)...
                };

                if constexpr (!std::is_same_v<__F_Return, void>)
                {
                    F_ReflectionVariable Output;
                    Output.TypeHashCode = Abytek::H_GeneralTypeHashCode::StaticValue<__F_Return>;
                    
                    Method.Signature.Output = Output;
                }
                
                AdditionalInfo.Methods.push_back(boost::move(Method));
                
                if (UserSetup)
                {
                    UserSetup(AdditionalInfo.Methods.back());
                }
                __F_Reflector::template ReflectGen_Method_Private<__F_UserClass, __F_Reflector, F_Functor, __MemberP>(ReflectionSession, ReflectionType, AdditionalInfo.Methods.back());
                
                return AdditionalInfo.Methods.back();
            }
        };
        template<class __F_UserClass, class __F_Reflector, typename __F_Return, typename... __F_Args>
        struct TH_ReflectMethod<__F_UserClass, __F_Reflector, __F_Return(__F_Args...) noexcept>
        {
            using F_Functor = __F_Return(__F_Args...);

            template<__F_Return(__F_UserClass::*__MemberP)(__F_Args...) noexcept>
            struct TH_AddToType
            {
                using F_CallerFunctor = __F_Return(__F_UserClass*, __F_Args...);

                static auto Call(__F_UserClass* ObjectRawP, __F_Args... Args)
                {
                    return (ObjectRawP->*__MemberP)(ABYTEK_FORWARD(Args)...);
                }
            };

            template<__F_Return(__F_UserClass::*__MemberP)(__F_Args...) noexcept>
            static F_ReflectionMethod& AddToType(
                const TW_Valid<F_ReflectionSession>& ReflectionSession,
                const TW_Valid<F_ReflectionType>& ReflectionType,
                const F_Char* NameCStr,
                const TF_Function<void(F_ReflectionMethod& ReflectionMethod)>& UserSetup = {}
            )
            {
                using H = TH_AddToType<__MemberP>;
                
                auto& AdditionalInfo = ReflectionType->AdditionalInfo;
                                
                ReflectionType->ReflectReferenced<F_Functor>();
                
                F_ReflectionMethod Method;
                Method.Name = ToText(NameCStr);
                Method.TypeHashCode = Abytek::H_GeneralTypeHashCode::StaticValue<F_Functor>;
                Method.CallerTypeHashCode = Abytek::H_GeneralTypeHashCode::StaticValue<__F_Return(__F_UserClass*, __F_Args...)>;
                Method.Address = (PDiff)(&H::Call);
                Method.Signature.IsStatic = false;
                Method.Flags = FlagCombine(Method.Flags, E_ReflectionFunctionFlag::NOEXCEPT);

                auto MakeReflectionNamedVariable = [](F_GeneralTypeHashCode TypeHashCode)->F_ReflectionNamedVariable
                {
                    F_ReflectionNamedVariable Result;
                    Result.TypeHashCode = TypeHashCode;
                    
                    return boost::move(Result);
                };
                Method.Signature.NamedInputs = {
                    MakeReflectionNamedVariable(Abytek::H_GeneralTypeHashCode::StaticValue<__F_Args>)...
                };

                if constexpr (!std::is_same_v<__F_Return, void>)
                {
                    F_ReflectionVariable Output;
                    Output.TypeHashCode = Abytek::H_GeneralTypeHashCode::StaticValue<__F_Return>;
                    
                    Method.Signature.Output = Output;
                }
                
                AdditionalInfo.Methods.push_back(boost::move(Method));
                
                if (UserSetup)
                {
                    UserSetup(AdditionalInfo.Methods.back());
                }
                __F_Reflector::template ReflectGen_Method_Private<__F_UserClass, __F_Reflector, F_Functor, __MemberP>(ReflectionSession, ReflectionType, AdditionalInfo.Methods.back());
                
                return AdditionalInfo.Methods.back();
            }
        }; 
        template<class __F_UserClass, class __F_Reflector, typename __F_Return, typename... __F_Args>
        struct TH_ReflectMethod<__F_UserClass, __F_Reflector, __F_Return(__F_Args...) const>
        {
            using F_Functor = __F_Return(__F_Args...);

            template<__F_Return(__F_UserClass::*__MemberP)(__F_Args...) const>
            struct TH_AddToType
            {
                using F_CallerFunctor = __F_Return(__F_UserClass*, __F_Args...);

                static auto Call(const __F_UserClass* ObjectRawP, __F_Args... Args)
                {
                    return (ObjectRawP->*__MemberP)(ABYTEK_FORWARD(Args)...);
                }
            };

            template<__F_Return(__F_UserClass::*__MemberP)(__F_Args...) const>
            static F_ReflectionMethod& AddToType(
                const TW_Valid<F_ReflectionSession>& ReflectionSession,
                const TW_Valid<F_ReflectionType>& ReflectionType,
                const F_Char* NameCStr,
                const TF_Function<void(F_ReflectionMethod& ReflectionMethod)>& UserSetup = {}
            )
            {
                using H = TH_AddToType<__MemberP>;
                
                auto& AdditionalInfo = ReflectionType->AdditionalInfo;
                                
                ReflectionType->ReflectReferenced<F_Functor>();
                
                F_ReflectionMethod Method;
                Method.Name = ToText(NameCStr);
                Method.TypeHashCode = Abytek::H_GeneralTypeHashCode::StaticValue<F_Functor>;
                Method.CallerTypeHashCode = Abytek::H_GeneralTypeHashCode::StaticValue<__F_Return(const __F_UserClass*, __F_Args...)>;
                Method.Address = (PDiff)(&H::Call);
                Method.Signature.IsStatic = false;
                Method.Flags = FlagCombine(Method.Flags, E_ReflectionFunctionFlag::CONSTANT);

                auto MakeReflectionNamedVariable = [](F_GeneralTypeHashCode TypeHashCode)->F_ReflectionNamedVariable
                {
                    F_ReflectionNamedVariable Result;
                    Result.TypeHashCode = TypeHashCode;
                    
                    return boost::move(Result);
                };
                Method.Signature.NamedInputs = {
                    MakeReflectionNamedVariable(Abytek::H_GeneralTypeHashCode::StaticValue<__F_Args>)...
                };

                if constexpr (!std::is_same_v<__F_Return, void>)
                {
                    F_ReflectionVariable Output;
                    Output.TypeHashCode = Abytek::H_GeneralTypeHashCode::StaticValue<__F_Return>;
                    
                    Method.Signature.Output = Output;
                }
                
                AdditionalInfo.Methods.push_back(boost::move(Method));
                
                if (UserSetup)
                {
                    UserSetup(AdditionalInfo.Methods.back());
                }
                __F_Reflector::template ReflectGen_Method_Private<__F_UserClass, __F_Reflector, F_Functor, __MemberP>(ReflectionSession, ReflectionType, AdditionalInfo.Methods.back());
                
                return AdditionalInfo.Methods.back();
            }
        };
        template<class __F_UserClass, class __F_Reflector, typename __F_Return, typename... __F_Args>
        struct TH_ReflectMethod<__F_UserClass, __F_Reflector, __F_Return(__F_Args...) const noexcept>
        {
            using F_Functor = __F_Return(__F_Args...);

            template<__F_Return(__F_UserClass::*__MemberP)(__F_Args...) const noexcept>
            struct TH_AddToType
            {
                using F_CallerFunctor = __F_Return(__F_UserClass*, __F_Args...);

                static auto Call(const __F_UserClass* ObjectRawP, __F_Args... Args)
                {
                    return (ObjectRawP->*__MemberP)(ABYTEK_FORWARD(Args)...);
                }
            };

            template<__F_Return(__F_UserClass::*__MemberP)(__F_Args...) const noexcept>
            static F_ReflectionMethod& AddToType(
                const TW_Valid<F_ReflectionSession>& ReflectionSession,
                const TW_Valid<F_ReflectionType>& ReflectionType,
                const F_Char* NameCStr,
                const TF_Function<void(F_ReflectionMethod& ReflectionMethod)>& UserSetup = {}
            )
            {
                using H = TH_AddToType<__MemberP>;
                
                auto& AdditionalInfo = ReflectionType->AdditionalInfo;
                                
                ReflectionType->ReflectReferenced<__F_Return(__F_Args...)>();
                
                F_ReflectionMethod Method;
                Method.Name = ToText(NameCStr);
                Method.TypeHashCode = Abytek::H_GeneralTypeHashCode::StaticValue<__F_Return(__F_Args...)>;
                Method.CallerTypeHashCode = Abytek::H_GeneralTypeHashCode::StaticValue<__F_Return(const __F_UserClass*, __F_Args...)>;
                Method.Address = (PDiff)(&H::Call);
                Method.Signature.IsStatic = false;
                Method.Flags = FlagCombine(Method.Flags, E_ReflectionFunctionFlag::CONSTANT, E_ReflectionFunctionFlag::NOEXCEPT);

                auto MakeReflectionNamedVariable = [](F_GeneralTypeHashCode TypeHashCode)->F_ReflectionNamedVariable
                {
                    F_ReflectionNamedVariable Result;
                    Result.TypeHashCode = TypeHashCode;
                    
                    return boost::move(Result);
                };
                Method.Signature.NamedInputs = {
                    MakeReflectionNamedVariable(Abytek::H_GeneralTypeHashCode::StaticValue<__F_Args>)...
                };

                if constexpr (!std::is_same_v<__F_Return, void>)
                {
                    F_ReflectionVariable Output;
                    Output.TypeHashCode = Abytek::H_GeneralTypeHashCode::StaticValue<__F_Return>;
                    
                    Method.Signature.Output = Output;
                }
                
                AdditionalInfo.Methods.push_back(boost::move(Method));
                
                if (UserSetup)
                {
                    UserSetup(AdditionalInfo.Methods.back());
                }
                __F_Reflector::template ReflectGen_Method_Private<__F_UserClass, __F_Reflector, F_Functor, __MemberP>(ReflectionSession, ReflectionType, AdditionalInfo.Methods.back());
                
                return AdditionalInfo.Methods.back();
            }
        };
        
        template<class __F_UserClass, class __F_Reflector, typename __F_Functor>
        struct TH_ReflectMethodStatic;        
        template<class __F_UserClass, class __F_Reflector, typename __F_Return, typename... __F_Args>
        struct TH_ReflectMethodStatic<__F_UserClass, __F_Reflector, __F_Return(__F_Args...)>
        {
            using F_Functor = __F_Return(__F_Args...);

            static F_ReflectionMethod& AddToType(
                const TW_Valid<F_ReflectionSession>& ReflectionSession,
                const TW_Valid<F_ReflectionType>& ReflectionType,
                const F_Char* NameCStr,
                F_Functor* FunctorP,
                const TF_Function<void(F_ReflectionMethod& ReflectionMethod)>& UserSetup = {}
            )
            {
                auto& AdditionalInfo = ReflectionType->AdditionalInfo;
                
                ReflectionType->ReflectReferenced<F_Functor>();
                
                F_ReflectionMethod Method;
                Method.Name = ToText(NameCStr);
                Method.TypeHashCode = Abytek::H_GeneralTypeHashCode::StaticValue<F_Functor>;
                Method.CallerTypeHashCode = Method.TypeHashCode;
                Method.Address = (PDiff)FunctorP;
                Method.Signature.IsStatic = true;

                auto MakeReflectionNamedVariable = [](F_GeneralTypeHashCode TypeHashCode)->F_ReflectionNamedVariable
                {
                    F_ReflectionNamedVariable Result;
                    Result.TypeHashCode = TypeHashCode;
                    
                    return boost::move(Result);
                };
                Method.Signature.NamedInputs = {
                    MakeReflectionNamedVariable(Abytek::H_GeneralTypeHashCode::StaticValue<__F_Args>)...
                };

                if constexpr (!std::is_same_v<__F_Return, void>)
                {
                    F_ReflectionVariable Output;
                    Output.TypeHashCode = Abytek::H_GeneralTypeHashCode::StaticValue<__F_Return>;
                    
                    Method.Signature.Output = Output;
                }
                
                AdditionalInfo.Methods.push_back(boost::move(Method));
                
                if (UserSetup)
                {
                    UserSetup(AdditionalInfo.Methods.back());
                }
                __F_Reflector::template ReflectGen_MethodStatic_Private<__F_UserClass, __F_Reflector, F_Functor>(ReflectionSession, ReflectionType, AdditionalInfo.Methods.back());
                
                return AdditionalInfo.Methods.back();
            }
        };   
        template<class __F_UserClass, class __F_Reflector, typename __F_Return, typename... __F_Args>
        struct TH_ReflectMethodStatic<__F_UserClass, __F_Reflector, __F_Return(__F_Args...) noexcept>
        {
            using F_Functor = __F_Return(__F_Args...);

            static F_ReflectionMethod& AddToType(
                const TW_Valid<F_ReflectionSession>& ReflectionSession,
                const TW_Valid<F_ReflectionType>& ReflectionType,
                const F_Char* NameCStr,
                F_Functor* FunctorP,
                const TF_Function<void(F_ReflectionMethod& ReflectionMethod)>& UserSetup = {}
            )
            {
                auto& AdditionalInfo = ReflectionType->AdditionalInfo;
                
                ReflectionType->ReflectReferenced<F_Functor>();
                
                F_ReflectionMethod Method;
                Method.Name = ToText(NameCStr);
                Method.TypeHashCode = Abytek::H_GeneralTypeHashCode::StaticValue<F_Functor>;
                Method.CallerTypeHashCode = Method.TypeHashCode;
                Method.Address = (PDiff)FunctorP;
                Method.Signature.IsStatic = true;
                Method.Flags = FlagCombine(Method.Flags, E_ReflectionFunctionFlag::NOEXCEPT);

                auto MakeReflectionNamedVariable = [](F_GeneralTypeHashCode TypeHashCode)->F_ReflectionNamedVariable
                {
                    F_ReflectionNamedVariable Result;
                    Result.TypeHashCode = TypeHashCode;
                    
                    return boost::move(Result);
                };
                Method.Signature.NamedInputs = {
                    MakeReflectionNamedVariable(Abytek::H_GeneralTypeHashCode::StaticValue<__F_Args>)...
                };

                if constexpr (!std::is_same_v<__F_Return, void>)
                {
                    F_ReflectionVariable Output;
                    Output.TypeHashCode = Abytek::H_GeneralTypeHashCode::StaticValue<__F_Return>;
                    
                    Method.Signature.Output = Output;
                }
                
                AdditionalInfo.Methods.push_back(boost::move(Method));
                
                if (UserSetup)
                {
                    UserSetup(AdditionalInfo.Methods.back());
                }
                __F_Reflector::template ReflectGen_MethodStatic_Private<__F_UserClass, __F_Reflector, F_Functor>(ReflectionSession, ReflectionType, AdditionalInfo.Methods.back());
                
                return AdditionalInfo.Methods.back();
            }
        };
        
        template<class __F_UserClass, class __F_Reflector, typename __F_Variable>
        struct TH_ReflectProperty
        {
            using F_Variable = __F_Variable;

            template<Sz __MemberOffset>
            struct TH_AddToType
            {
                using F_NonReferenceVariable = std::remove_reference_t<__F_Variable>;
                
                using F_ReferenceAccessFunctor = F_NonReferenceVariable& (__F_UserClass* ObjectRawP);
                using F_ConstructDefaultFunctor = void(__F_UserClass* ObjectRawP);
                using F_DestructFunctor = void(__F_UserClass* ObjectRawP);
                using F_MoveConstructFunctor = void(__F_UserClass* ObjectRawP, F_NonReferenceVariable&& X);
                using F_CopyConstructFunctor = void(__F_UserClass* ObjectRawP, const F_NonReferenceVariable& X);
                using F_MoveAssignFunctor = void(__F_UserClass* ObjectRawP, F_NonReferenceVariable&& X);
                using F_CopyAssignFunctor = void(__F_UserClass* ObjectRawP, const F_NonReferenceVariable& X);

                static F_NonReferenceVariable& ReferenceAccess(__F_UserClass* ObjectRawP)
                {
                    return (F_NonReferenceVariable&)*(
                        (F_NonReferenceVariable*)(
                            ((U8*)ObjectRawP)
                            + __MemberOffset
                        )
                    );
                }
                static void ConstructDefault(__F_UserClass* ObjectRawP)
                {
                    using F_Target = std::conditional_t<
                        std::is_default_constructible_v<F_NonReferenceVariable>,
                        F_NonReferenceVariable,
                        U64
                    >;
                    ABYTEK_BASE_REFLECTION_ASSERT(std::is_same_v<F_Target, F_NonReferenceVariable>) << "can't default construct";
                    
                    new (
                        (F_Target*)(
                            ((U8*)ObjectRawP)
                            + __MemberOffset
                        )
                    ) F_Target {};
                }
                static void Destruct(__F_UserClass* ObjectRawP)
                {
                    using F_Target = std::conditional_t<
                        std::is_destructible_v<F_NonReferenceVariable>,
                        F_NonReferenceVariable,
                        U64
                    >;
                    ABYTEK_BASE_REFLECTION_ASSERT(std::is_same_v<F_Target, F_NonReferenceVariable>) << "can't destruct";
                    
                    (
                        (F_Target*)(
                            ((U8*)ObjectRawP)
                            + __MemberOffset
                        )
                    )->~F_Target();
                }
                static void MoveConstruct(__F_UserClass* ObjectRawP, F_NonReferenceVariable&& X)
                {
                    using F_Target = std::conditional_t<
                        std::is_move_constructible_v<F_NonReferenceVariable>,
                        F_NonReferenceVariable,
                        U64
                    >;
                    ABYTEK_BASE_REFLECTION_ASSERT(std::is_same_v<F_Target, F_NonReferenceVariable>) << "can't move construct";
                    
                    new (
                        (F_Target*)(
                            ((U8*)ObjectRawP)
                            + __MemberOffset
                        )
                    ) F_Target {
                        (F_Target&&)boost::move(X)
                    };
                }
                static void CopyConstruct(__F_UserClass* ObjectRawP, const F_NonReferenceVariable& X)
                {
                    using F_Target = std::conditional_t<
                        std::is_copy_constructible_v<F_NonReferenceVariable>,
                        F_NonReferenceVariable,
                        U64
                    >;
                    ABYTEK_BASE_REFLECTION_ASSERT(std::is_same_v<F_Target, F_NonReferenceVariable>) << "can't copy construct";
                    
                    new (
                        (F_Target*)(
                            ((U8*)ObjectRawP)
                            + __MemberOffset
                        )
                    ) F_Target {
                        (const F_Target&)X
                    };
                }
                static void MoveAssign(__F_UserClass* ObjectRawP, F_NonReferenceVariable&& X)
                {
                    using F_Target = std::conditional_t<
                        std::is_move_assignable_v<F_NonReferenceVariable>,
                        F_NonReferenceVariable,
                        U64
                    >;
                    ABYTEK_BASE_REFLECTION_ASSERT(std::is_same_v<F_Target, F_NonReferenceVariable>) << "can't move assign";
                    
                    *(
                        (F_Target*)(
                            ((U8*)ObjectRawP)
                            + __MemberOffset
                        )
                    ) = (F_Target&&)boost::move(X);
                }
                static void CopyAssign(__F_UserClass* ObjectRawP, const F_NonReferenceVariable& X)
                {
                    using F_Target = std::conditional_t<
                        std::is_copy_assignable_v<F_NonReferenceVariable>,
                        F_NonReferenceVariable,
                        U64
                    >;
                    ABYTEK_BASE_REFLECTION_ASSERT(std::is_same_v<F_Target, F_NonReferenceVariable>) << "can't copy assign";
                    
                    *(
                        (F_Target*)(
                            ((U8*)ObjectRawP)
                            + __MemberOffset
                        )
                    ) = (const F_Target&)X;
                }
            };

            template<Sz __MemberOffset>
            static F_ReflectionProperty& AddToType(
                const TW_Valid<F_ReflectionSession>& ReflectionSession,
                const TW_Valid<F_ReflectionType>& ReflectionType,
                const F_Char* NameCStr,
                const TF_Function<void(F_ReflectionProperty& ReflectionProperty)>& UserSetup = {}
            )
            {
                using H = TH_AddToType<__MemberOffset>;
                
                auto& AdditionalInfo = ReflectionType->AdditionalInfo;

                ReflectionType->ReflectReferenced<__F_Variable>();
                                
                F_ReflectionProperty Property;
                Property.Name = ToText(NameCStr);
                Property.TypeHashCode = Abytek::H_GeneralTypeHashCode::StaticValue<__F_Variable>;
                Property.ObjectTypeHashCode = Abytek::H_GeneralTypeHashCode::StaticValue<__F_UserClass>;
                Property.ReferenceAccessFunctorAddress = (PDiff)(&H::ReferenceAccess);
                Property.ConstructDefaultFunctorAddress = (PDiff)(&H::ConstructDefault);
                Property.DestructFunctorAddress = (PDiff)(&H::Destruct);
                Property.MoveConstructFunctorAddress = (PDiff)(&H::MoveConstruct);
                Property.CopyConstructFunctorAddress = (PDiff)(&H::CopyConstruct);
                Property.MoveAssignFunctorAddress = (PDiff)(&H::MoveAssign);
                Property.CopyAssignFunctorAddress = (PDiff)(&H::CopyAssign);
                
                AdditionalInfo.Properties.push_back(boost::move(Property));
                
                if (UserSetup)
                {
                    UserSetup(AdditionalInfo.Properties.back());
                }
                __F_Reflector::template ReflectGen_Property_Private<__F_UserClass, __F_Reflector, __F_Variable, __MemberOffset>(ReflectionSession, ReflectionType, AdditionalInfo.Properties.back());
                
                return AdditionalInfo.Properties.back();
            }
        };
        
        template<class __F_UserClass, class __F_Reflector, typename __F_Variable>
        struct TH_ReflectPropertyStatic
        {
            static_assert(std::is_same_v<__F_Variable, std::remove_reference_t<__F_Variable>>, "reference static property is not supported");
            
            using F_Variable = __F_Variable;

            struct H_AddToType
            {
                using F_NonReferenceVariable = std::remove_reference_t<__F_Variable>;
                
                using F_ReferenceAccessFunctor = F_NonReferenceVariable& (PDiff StaticAddress);
                using F_ConstructDefaultFunctor = void(PDiff StaticAddress);
                using F_DestructFunctor = void(PDiff StaticAddress);
                using F_MoveConstructFunctor = void(PDiff StaticAddress, __F_Variable&& X);
                using F_CopyConstructFunctor = void(PDiff StaticAddress, const __F_Variable& X);
                using F_MoveAssignFunctor = void(PDiff StaticAddress, __F_Variable&& X);
                using F_CopyAssignFunctor = void(PDiff StaticAddress, const __F_Variable& X);

                static F_NonReferenceVariable& ReferenceAccess(PDiff StaticAddress)
                {
                    return *((__F_Variable*)StaticAddress);
                }
                static void ConstructDefault(PDiff StaticAddress)
                {
                    using F_Target = std::conditional_t<
                        std::is_default_constructible_v<__F_Variable>,
                        __F_Variable,
                        U64
                    >;
                    ABYTEK_BASE_REFLECTION_ASSERT(std::is_same_v<F_Target, __F_Variable>) << "can't default construct";
                    
                    new ((F_Target*)StaticAddress) F_Target {};
                }
                static void Destruct(PDiff StaticAddress)
                {
                    using F_Target = std::conditional_t<
                        std::is_destructible_v<__F_Variable>,
                        __F_Variable,
                        U64
                    >;
                    ABYTEK_BASE_REFLECTION_ASSERT(std::is_same_v<F_Target, __F_Variable>) << "can't destruct";
                    
                    ((F_Target*)StaticAddress)->~F_Target();
                }
                static void MoveConstruct(PDiff StaticAddress, __F_Variable&& X)
                {
                    using F_Target = std::conditional_t<
                        std::is_move_constructible_v<__F_Variable>,
                        __F_Variable,
                        U64
                    >;
                    ABYTEK_BASE_REFLECTION_ASSERT(std::is_same_v<F_Target, __F_Variable>) << "can't move construct";
                    
                    new ((F_Target*)StaticAddress) F_Target {
                        (F_Target&&)boost::move(X)
                    };
                }
                static void CopyConstruct(PDiff StaticAddress, const __F_Variable& X)
                {
                    using F_Target = std::conditional_t<
                        std::is_copy_constructible_v<__F_Variable>,
                        __F_Variable,
                        U64
                    >;
                    ABYTEK_BASE_REFLECTION_ASSERT(std::is_same_v<F_Target, __F_Variable>) << "can't copy construct";
                    
                    new ((F_Target*)StaticAddress) F_Target {
                        (const F_Target&)X
                    };
                }
                static void MoveAssign(PDiff StaticAddress, __F_Variable&& X)
                {
                    using F_Target = std::conditional_t<
                        std::is_move_assignable_v<__F_Variable>,
                        __F_Variable,
                        U64
                    >;
                    ABYTEK_BASE_REFLECTION_ASSERT(std::is_same_v<F_Target, __F_Variable>) << "can't move assign";
                    
                    *((F_Target*)StaticAddress) = (F_Target&&)boost::move(X);
                }
                static void CopyAssign(PDiff StaticAddress, const __F_Variable& X)
                {
                    using F_Target = std::conditional_t<
                        std::is_copy_assignable_v<__F_Variable>,
                        __F_Variable,
                        U64
                    >;
                    ABYTEK_BASE_REFLECTION_ASSERT(std::is_same_v<F_Target, __F_Variable>) << "can't copy assign";
                    
                    *((F_Target*)StaticAddress) = (const F_Target&)X;
                }
            };

            static F_ReflectionProperty& AddToType(
                const TW_Valid<F_ReflectionSession>& ReflectionSession,
                const TW_Valid<F_ReflectionType>& ReflectionType,
                const F_Char* NameCStr,
                std::remove_reference_t<__F_Variable>& StaticVariable,
                const TF_Function<void(F_ReflectionProperty& ReflectionProperty)>& UserSetup = {}
            )
            {
                using H = H_AddToType;
                
                auto& AdditionalInfo = ReflectionType->AdditionalInfo;
                
                ReflectionType->ReflectReferenced<__F_Variable>();
                                
                F_ReflectionProperty Property;
                Property.Name = ToText(NameCStr);
                Property.TypeHashCode = Abytek::H_GeneralTypeHashCode::StaticValue<__F_Variable>;
                Property.StaticAddress = (PDiff)&StaticVariable;
                Property.ReferenceAccessFunctorAddress = (PDiff)(&H::ReferenceAccess);
                Property.ConstructDefaultFunctorAddress = (PDiff)(&H::ConstructDefault);
                Property.DestructFunctorAddress = (PDiff)(&H::Destruct);
                Property.MoveConstructFunctorAddress = (PDiff)(&H::MoveConstruct);
                Property.CopyConstructFunctorAddress = (PDiff)(&H::CopyConstruct);
                Property.MoveAssignFunctorAddress = (PDiff)(&H::MoveAssign);
                Property.CopyAssignFunctorAddress = (PDiff)(&H::CopyAssign);
                
                AdditionalInfo.Properties.push_back(boost::move(Property));
                
                if (UserSetup)
                {
                    UserSetup(AdditionalInfo.Properties.back());
                }
                __F_Reflector::template ReflectGen_PropertyStatic_Private<__F_UserClass, __F_Reflector, __F_Variable>(ReflectionSession, ReflectionType, AdditionalInfo.Properties.back());
                
                return AdditionalInfo.Properties.back();
            }
        };
        
        template<class __F_Enum, class __F_Reflector>
        struct TH_ReflectEnumValue
        {
            static F_ReflectionEnumValue& AddToType(
                const TW_Valid<F_ReflectionSession>& ReflectionSession,
                const TW_Valid<F_ReflectionType>& ReflectionType,
                const F_Char* NameCStr,
                __F_Enum Value,
                const TF_Function<void(F_ReflectionEnumValue& ReflectionEnumValue)>& UserSetup = {}
            )
            {
                auto& AdditionalInfo = ReflectionType->AdditionalInfo;
                
                F_ReflectionEnumValue EnumValue;
                EnumValue.Name = ToText(NameCStr);
                EnumValue.TypeHashCode = Abytek::H_GeneralTypeHashCode::StaticValue<__F_Enum>;
                EnumValue.Value = Value;
                
                AdditionalInfo.EnumValues.push_back(boost::move(EnumValue));
                
                if (UserSetup)
                {
                    UserSetup(AdditionalInfo.EnumValues.back());
                }
                __F_Reflector::template ReflectGen_EnumValue_Private<__F_Enum, __F_Reflector>(ReflectionSession, ReflectionType, AdditionalInfo.EnumValues.back());
                
                return AdditionalInfo.EnumValues.back();
            }
        };
    }
}