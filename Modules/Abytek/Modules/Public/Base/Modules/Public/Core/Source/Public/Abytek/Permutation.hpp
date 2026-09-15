#pragma once

#include "Abytek/Base.Core.prerequisites.pch.hpp"
#include "Abytek/CrtUseAllocatorTypedefs.hpp"


namespace Abytek
{
    using F_PermutationValue = I64;
    
    namespace Internal::Permutation
    {
        template<auto... __Values>
        struct TH_TestValueType;
        
        template<auto __First>
        struct TH_TestValueType<__First>
        {
            static constexpr B8 Mismatch = false;
        };
        template<auto __First, auto __Second, auto... __Rest>
        struct TH_TestValueType<__First, __Second, __Rest...>
        {
            static constexpr B8 Mismatch = (
                TH_TestValueType<__Second, __Rest...>::Mismatch
                || !std::is_same_v<decltype(__First), decltype(__Second)>
            );
        };
        
        template<auto... __Values>
        static constexpr B8 TestValueType = !TH_TestValueType<__Values...>::Mismatch;
    }

    template<auto __Begin, auto __End, auto __DefaultValue = __Begin>
    struct TF_Permutation_Range
    {
        using F_Value = decltype(__DefaultValue);
        static constexpr F_PermutationValue Begin = static_cast<F_PermutationValue>(__Begin);
        static constexpr F_PermutationValue End = static_cast<F_PermutationValue>(__End);
        static constexpr auto DefaultValue = __DefaultValue;
        
        static_assert(Internal::Permutation::TestValueType<__Begin, __End>, "Mismatch value types"); 
        
        static TF_SmallVector<F_PermutationValue, 8> GetValues()
        {
            TF_SmallVector<F_PermutationValue, 8> Result;
            for (I32 Idx = Begin; Idx < End; ++Idx)
            {
                Result.push_back(Idx);
            }
            return ABYTEK_MOVE(Result);
        }
    };
    template<auto __DefaultValue, auto... __RestValues>
    struct TF_Permutation_Set
    {
        using F_Value = decltype(__DefaultValue);
        static constexpr auto DefaultValue = __DefaultValue;
        
        static_assert(Internal::Permutation::TestValueType<__DefaultValue, __RestValues...>, "Mismatch value types"); 
        
        static TF_SmallVector<F_PermutationValue, 8> GetValues()
        {
            F_PermutationValue Values[] = {
                static_cast<F_PermutationValue>(__DefaultValue), 
                static_cast<F_PermutationValue>(__RestValues)...
            };
            return { Values, Values + GetStaticArraySize(Values) };
        }
    };
    struct F_PermutationBase
    {
        using F_Value = F_PermutationValue;
        
        static TF_SmallVector<F_PermutationValue, 8> GetValues()
        {
            TF_SmallVector<F_PermutationValue, 8> Result;
            Result.push_back(0);
            return ABYTEK_MOVE(Result);
        }
        static constexpr F_PermutationValue DefaultValue = 0;
    };

    namespace Internal
    {
        template<auto __N>
        constexpr Sz PermutationValuesToHashCode_Static(const F_PermutationValue (&Value)[__N])
        {
            uint64_t seed = 0;
            for (size_t i = 0; i < __N; ++i) {
                seed = HashCombineU64(seed, static_cast<uint32_t>(Value[i]));
            }
            return seed;
        }
        constexpr Sz PermutationValuesToHashCode_Dynamic(const F_PermutationValue* Value, size_t N)
        {
            uint64_t seed = 0;
            for (size_t i = 0; i < N; ++i) {
                seed = HashCombineU64(seed, static_cast<uint32_t>(Value[i]));
            }
            return seed;
        }
    }

    struct F_PermutationValueSet
    {
        TF_SmallVector<F_PermutationValue, 8> Values;

        Sz GetHashCode() const
        {
            return Internal::PermutationValuesToHashCode_Dynamic(Values.data(), Values.size());
        }
    };

    namespace Internal
    {
        inline void GeneratePermutationValueSets(
            U32 X,
            U32 NumX,
            const TF_SmallVector<F_PermutationValue, 8>* ValueMatrix,
            TF_SmallVector<F_PermutationValueSet, 8>& OutVectors
        )
        {
            if (X >= NumX)
            {
                return;
            }

            TF_SmallVector<F_PermutationValueSet, 8> Subvectors;
            GeneratePermutationValueSets(X + 1, NumX, ValueMatrix, Subvectors);

            const auto& MainRow = ValueMatrix[X];
            U32 NumY = MainRow.size();
            for (U32 Y = 0; Y < NumY; ++Y)
            {
                F_PermutationValue MainValue = MainRow[Y];
                if (Subvectors.size())
                {
                    for (const auto& Vector : Subvectors)
                    {
                        F_PermutationValueSet NewVector = Vector;
                        NewVector.Values.insert(
                            NewVector.Values.begin(),
                            MainValue
                        );
                        OutVectors.push_back(NewVector);
                    }
                }
                else
                {
                    F_PermutationValueSet NewVector;
                    NewVector.Values.insert(
                        NewVector.Values.begin(),
                        MainValue
                    );
                    OutVectors.push_back(NewVector);
                }
            }
        }
    }
    
    template<typename __F_Permutation, typename __F_Base>
    struct TF_Permutation : __F_Base
    {
        using F_Permutation = __F_Permutation;
        
        template<auto __Value>
        struct TF_Make
        {
            static constexpr auto Value = __Value;
            using F_Permutation = F_Permutation;
        };
        
        using F_Default = TF_Make<__F_Base::DefaultValue>;
    };
    struct F_DefaultPermutation : TF_Permutation<F_DefaultPermutation, F_PermutationBase>
    {
    };
    
    namespace Internal::Permutation
    {
        template<B8 __IsMatched, typename __F_TypedValue>
        struct TF_TypedStaticPermutationResolveValue;
        template<typename __F_TypedValue>
        struct TF_TypedStaticPermutationResolveValue<true, __F_TypedValue>
        {
            using F_TypedValue = __F_TypedValue;
            static constexpr auto Value = __F_TypedValue::Value;
        };
        template<typename __F_TypedValue>
        struct TF_TypedStaticPermutationResolveValue<false, __F_TypedValue>
        {
        };
        
        template<typename __F_Permutation, typename... __F_TypedValues>
        struct TF_TypedStaticPermutationResolve;
        template<typename __F_Permutation>
        struct TF_TypedStaticPermutationResolve<__F_Permutation>
        {
        };
        template<typename __F_Permutation, typename __F_FirstTypedValue, typename... __F_RestTypedValues>
        struct TF_TypedStaticPermutationResolve<__F_Permutation, __F_FirstTypedValue, __F_RestTypedValues...> : 
            TF_TypedStaticPermutationResolve<__F_Permutation, __F_RestTypedValues...>,
            TF_TypedStaticPermutationResolveValue<std::is_same_v<__F_Permutation, typename __F_FirstTypedValue::F_Permutation>, __F_FirstTypedValue>
        {
        };
        
        template<typename __F_NewTypedValue, typename __F_TypedValue>
        using TF_TypedStaticPermutationTrySet = std::conditional_t<
            std::is_same_v<typename __F_NewTypedValue::F_Permutation, typename __F_TypedValue::F_Permutation>,
            __F_NewTypedValue,
            __F_TypedValue
        >;
    }
    template<typename... __F_TypedValues>
    struct TF_StaticPermutationVector
    {
        static constexpr Sz GetHashCode()
        {
            F_PermutationValue Values[] = { static_cast<F_PermutationValue>(__F_TypedValues::Value)... };
            return Internal::PermutationValuesToHashCode_Static(Values);
        }
        
        template<typename __F_Permutation>
        static constexpr auto Get()
        {
            return Internal::Permutation::TF_TypedStaticPermutationResolve<__F_Permutation, __F_TypedValues...>::Value;
        }
        template<typename __F_Permutation>
        using TF_GetTypedValue = typename Internal::Permutation::TF_TypedStaticPermutationResolve<__F_Permutation, __F_TypedValues...>::F_TypedValue;

        template<typename __F_NewTypedValue>
        using TF_Set = TF_StaticPermutationVector<
            Internal::Permutation::TF_TypedStaticPermutationTrySet<
                __F_NewTypedValue, 
                __F_TypedValues
            >...
        >;
        
        static F_PermutationValueSet GetValueSet() noexcept
        {
            F_PermutationValueSet Result;
            Result.Values = { 
                static_cast<F_PermutationValue>(
                    __F_TypedValues::Value
                )...
            };
            return ABYTEK_MOVE(Result);
        }
    };
    
    namespace Internal
    {
        template<typename __F_Permutation>
        struct TF_DynamicPermutationItem
        {
            using F_Value = typename __F_Permutation::F_Value;
            F_Value Value;
        };
        template<typename __F_Permutation>
        TF_DynamicPermutationItem<__F_Permutation> DefaultDynamicPermutationItem()
        {
            return {
                __F_Permutation::DefaultValue
            };
        }
        
        template<typename __F_Vector, auto __Index, typename... __F_Permutations>
        struct TF_DynamicPermutationFromValueSet;
        template<typename __F_Vector, auto __Index>
        struct TF_DynamicPermutationFromValueSet<__F_Vector, __Index>
        {
            static void Invoke(__F_Vector& Result, const F_PermutationValueSet& ValueSet)
            {
            }
        };
        template<typename __F_Vector, auto __Index, typename __F_FirstPermutation, typename... __F_RestPermutations>
        struct TF_DynamicPermutationFromValueSet<__F_Vector, __Index, __F_FirstPermutation, __F_RestPermutations...>
        {
            static void Invoke(__F_Vector& Result, const F_PermutationValueSet& ValueSet)
            {
                Result.template Get<__F_FirstPermutation>() = { static_cast<typename __F_FirstPermutation::F_Value>(ValueSet.Values[__Index]) };
                TF_DynamicPermutationFromValueSet<__F_Vector, __Index + 1, __F_RestPermutations...>::Invoke(Result, ValueSet);
            }
        };
    }
    template<typename... __F_Permutations>
    struct TF_DynamicPermutationVector
    { 
        using F_Values = std::tuple<Internal::TF_DynamicPermutationItem<__F_Permutations>...>;
        F_Values Values = std::make_tuple(
            Internal::DefaultDynamicPermutationItem<__F_Permutations>()...
        );
        
        static TF_DynamicPermutationVector FromValueSet(const F_PermutationValueSet& ValueSet)
        {
            TF_DynamicPermutationVector Result;
            Internal::TF_DynamicPermutationFromValueSet<
                TF_DynamicPermutationVector, 
                0, 
                __F_Permutations...
            >::Invoke(Result, ValueSet);
            return Result;
        }
        F_PermutationValueSet GetValueSet() const noexcept
        {
            F_PermutationValueSet Result;
            Result.Values = {
                static_cast<F_PermutationValue>(
                    std::get<
                        Internal::TF_DynamicPermutationItem<__F_Permutations>
                    >(Values)
                    .Value
                )...
            };
            return ABYTEK_MOVE(Result);
        }

        Sz GetHashCode() const
        {
            F_PermutationValue ValueParsed[]
            {
                static_cast<F_PermutationValue>(
                    std::get<
                        Internal::TF_DynamicPermutationItem<__F_Permutations>
                    >(Values)
                    .Value
                )...
            };
            return Internal::PermutationValuesToHashCode_Dynamic(
                ValueParsed,
                GetStaticArraySize(ValueParsed)
            );
        }
        template<typename __F_Permutation>
        const auto& Get() const noexcept
        {
            return std::get<
                    Internal::TF_DynamicPermutationItem<__F_Permutation>
                >(Values)
                .Value;
        }
        template<typename __F_Permutation>
        auto& Get() noexcept
        {
            return std::get<
                    Internal::TF_DynamicPermutationItem<__F_Permutation>
                >(Values)
                .Value;
        }
    };
    
    template<typename... __F_Permutations>
    struct TF_PermutationDomain
    {
        using F_DynamicVector = TF_DynamicPermutationVector<__F_Permutations...>;
        using F_DefaultStaticVector = TF_StaticPermutationVector<typename __F_Permutations::F_Default...>;
        
        static TF_SmallVector<F_PermutationValueSet, 8> GetValueSets()
        {
            TF_SmallVector<F_PermutationValueSet, 8> Result;
            
            TF_SmallVector<F_PermutationValue, 8> ValueMatrix[] = {
                (__F_Permutations::GetValues())...
            };

            U32 NumX = sizeof...(__F_Permutations);
            Internal::GeneratePermutationValueSets(0, NumX, ValueMatrix, Result);
            
            return ABYTEK_MOVE(Result);
        }
    };
    using F_DefaultPermutationDomain = TF_PermutationDomain<F_DefaultPermutation>;
}

#define ABYTEK_DEFINE_PERMUTATION(Name, ...) struct Name : Abytek::TF_Permutation<Name, __VA_ARGS__> {};
#define ABYTEK_OVERRIDE_PERMUTATION_DOMAIN(...) using F_PermutationDomain = Abytek::TF_PermutationDomain<__VA_ARGS__>;