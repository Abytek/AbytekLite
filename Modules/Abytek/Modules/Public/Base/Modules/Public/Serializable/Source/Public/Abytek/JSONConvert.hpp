#pragma once

#include "Abytek/Base.Serializable.prerequisites.pch.hpp"
#include "Abytek/JSONOptions.hpp"
#include "Abytek/JSONContext.hpp"
#include "Abytek/JSONSerializable.hpp"
#include "Abytek/JSONConvertChecks.hpp"
#include "Abytek/JSONConvertDeclaration.hpp"


namespace Abytek
{
    struct I_JSONSerializable;
    
    namespace Internal
    {
        template<B8 __IsJSONSerializable, typename __F_Value>
        struct TH_JSONConvert_Reflected;
        template<typename __F_Value>
        struct TH_JSONConvert_Reflected<false, __F_Value>
        {
        };
        template<typename __F_Value>
        struct TH_JSONConvert_Reflected<true, __F_Value>
        {
            using F_NonConstValue = std::remove_const_t<__F_Value>;
            
            static F_FeedbackStatus ValueToBoostJSONValue(const __F_Value& Value, boost::json::value& JSONValue, F_JSONContext& JSONContext)
            {
                if (!JSONContext.Options->ReflectionContext)
                {
                    return F_FeedbackStatus::MakeFailed(ABYTEK_TEXT("Requires reflected context for types using JSON serialization through reflection"));
                }
                
                // Get type
                if (!JSONContext.Options->ReflectionContext->HasType<__F_Value>())
                {
                    return F_FeedbackStatus::MakeFailed(ToText("Type was not reflected: ") + ToText(TypeFullName<__F_Value>()));
                }
                auto ReflectionType = JSONContext.Options->ReflectionContext->GetType<__F_Value>();
                
                // Get metadata
                auto& Metadata = ReflectionType->GetMetadata();
                auto MetadataElementName_ValueToBoostJSONValue = I_JSONSerializable::GetMetadataElementName_ValueToBoostJSONValue();
                if (!Metadata.HasElement(MetadataElementName_ValueToBoostJSONValue))
                {
                    return F_FeedbackStatus::MakeFailed(ToText("Type not support JSON serialization: ") + ToText(TypeFullName<__F_Value>()));
                }
                const auto& MetadataElement_ValueToBoostJSONValue = Metadata.Get(MetadataElementName_ValueToBoostJSONValue);
                const auto& Metadata_ValueToBoostJSONValue = AnyCast<I_JSONSerializable::F_Metadata_ValueToBoostJSONValue>(
                    MetadataElement_ValueToBoostJSONValue
                );
                
                //
                ABYTEK_FEEDBACK_STATUS_CHECK(Metadata_ValueToBoostJSONValue(&Value, JSONValue, JSONContext));
                return F_FeedbackStatus::MakeSucceeded();
            }
            static F_FeedbackStatus BoostJSONValueToValue(__F_Value& Value, const boost::json::value& JSONValue, F_JSONContext& JSONContext)
            {
                if (!JSONContext.Options->ReflectionContext)
                {
                    return F_FeedbackStatus::MakeFailed(ABYTEK_TEXT("Requires reflected context for types using JSON deserialization through reflection"));
                }
                
                // Get type
                if (!JSONContext.Options->ReflectionContext->HasType<__F_Value>())
                {
                    return F_FeedbackStatus::MakeFailed(ToText("Type was not reflected: ") + ToText(TypeFullName<__F_Value>()));
                }
                auto ReflectionType = JSONContext.Options->ReflectionContext->GetType<__F_Value>();
                
                // Get metadata
                auto& Metadata = ReflectionType->GetMetadata();
                auto MetadataElementName_BoostJSONValueToValue = I_JSONSerializable::GetMetadataElementName_BoostJSONValueToValue();
                if (!Metadata.HasElement(MetadataElementName_BoostJSONValueToValue))
                {
                    return F_FeedbackStatus::MakeFailed(ToText("Type not support JSON deserialization: ") + ToText(TypeFullName<__F_Value>()));
                }
                const auto& MetadataElement_BoostJSONValueToValue = Metadata.Get(MetadataElementName_BoostJSONValueToValue);
                const auto& Metadata_BoostJSONValueToValue = AnyCast<I_JSONSerializable::F_Metadata_BoostJSONValueToValue>(
                    MetadataElement_BoostJSONValueToValue
                );
                
                //
                ABYTEK_FEEDBACK_STATUS_CHECK(Metadata_BoostJSONValueToValue(&Value, JSONValue, JSONContext));
                return F_FeedbackStatus::MakeSucceeded();
            }
        };
    }
    
    template<typename __F_Value, typename>
    struct TH_JSONConvert :
        std::conditional_t<
            IsJSONSerializable_ByReflection<__F_Value>(),
            Internal::TH_JSONConvert_Reflected<IsJSONSerializable_ByReflection<__F_Value>(), __F_Value>,
            TH_JSONConvertInvalid<__F_Value>
        >
    {
    };
    
    template<typename __F_Value>
    struct TH_JSONConvert<__F_Value, std::enable_if_t<std::is_enum_v<__F_Value>>>
    {
        static F_FeedbackStatus ValueToBoostJSONValue(const __F_Value& Value, boost::json::value& JSONValue, F_JSONContext& JSONContext)
        {
            JSONValue = boost::json::value(static_cast<TF_UInt<sizeof(__F_Value)>>(Value));
            return F_FeedbackStatus::MakeSucceeded();
        }
        static F_FeedbackStatus BoostJSONValueToValue(__F_Value& Value, const boost::json::value& JSONValue, F_JSONContext& JSONContext)
        {
            if (JSONValue.is_number())
            {
                Value = static_cast<__F_Value>(JSONValue.to_number<TF_UInt<sizeof(__F_Value)>>());
                return F_FeedbackStatus::MakeSucceeded();
            }
            return F_FeedbackStatus::MakeFailed(ABYTEK_TEXT("Invalid type, requires uint64"));
        }
    };
    
    template<>
    struct TH_JSONConvert<B8>
    {
        static F_FeedbackStatus ValueToBoostJSONValue(const B8& Value, boost::json::value& JSONValue, F_JSONContext& JSONContext)
        {
            JSONValue = boost::json::value(Value);
            return F_FeedbackStatus::MakeSucceeded();
        }
        static F_FeedbackStatus BoostJSONValueToValue(B8& Value, const boost::json::value& JSONValue, F_JSONContext& JSONContext)
        {
            if (JSONValue.is_bool())
            {
                Value = JSONValue.as_bool();
                return F_FeedbackStatus::MakeSucceeded();
            }
            return F_FeedbackStatus::MakeFailed(ABYTEK_TEXT("Invalid type, requires boolean"));
        }
    };
    
    template<>
    struct TH_JSONConvert<U8>
    {
        static F_FeedbackStatus ValueToBoostJSONValue(const U8& Value, boost::json::value& JSONValue, F_JSONContext& JSONContext)
        {
            JSONValue = boost::json::value(Value);
            return F_FeedbackStatus::MakeSucceeded();
        }
        static F_FeedbackStatus BoostJSONValueToValue(U8& Value, const boost::json::value& JSONValue, F_JSONContext& JSONContext)
        {
            if (JSONValue.is_number())
            {
                Value = JSONValue.to_number<U8>();
                return F_FeedbackStatus::MakeSucceeded();
            }
            return F_FeedbackStatus::MakeFailed(ABYTEK_TEXT("Invalid type, requires number"));
        }
    };
    template<>
    struct TH_JSONConvert<U16>
    {
        static F_FeedbackStatus ValueToBoostJSONValue(const U16& Value, boost::json::value& JSONValue, F_JSONContext& JSONContext)
        {
            JSONValue = boost::json::value(Value);
            return F_FeedbackStatus::MakeSucceeded();
        }
        static F_FeedbackStatus BoostJSONValueToValue(U16& Value, const boost::json::value& JSONValue, F_JSONContext& JSONContext)
        {
            if (JSONValue.is_number())
            {
                Value = JSONValue.to_number<U16>();
                return F_FeedbackStatus::MakeSucceeded();
            }
            return F_FeedbackStatus::MakeFailed(ABYTEK_TEXT("Invalid type, requires number"));
        }
    };
    template<>
    struct TH_JSONConvert<U32>
    {
        static F_FeedbackStatus ValueToBoostJSONValue(const U32& Value, boost::json::value& JSONValue, F_JSONContext& JSONContext)
        {
            JSONValue = boost::json::value(Value);
            return F_FeedbackStatus::MakeSucceeded();
        }
        static F_FeedbackStatus BoostJSONValueToValue(U32& Value, const boost::json::value& JSONValue, F_JSONContext& JSONContext)
        {
            if (JSONValue.is_number())
            {
                Value = JSONValue.to_number<U32>();
                return F_FeedbackStatus::MakeSucceeded();
            }
            return F_FeedbackStatus::MakeFailed(ABYTEK_TEXT("Invalid type, requires number"));
        }
    };
    template<>
    struct TH_JSONConvert<U64>
    {
        static F_FeedbackStatus ValueToBoostJSONValue(const U64& Value, boost::json::value& JSONValue, F_JSONContext& JSONContext)
        {
            JSONValue = boost::json::value(Value);
            return F_FeedbackStatus::MakeSucceeded();
        }
        static F_FeedbackStatus BoostJSONValueToValue(U64& Value, const boost::json::value& JSONValue, F_JSONContext& JSONContext)
        {
            if (JSONValue.is_number())
            {
                Value = JSONValue.to_number<U64>();
                return F_FeedbackStatus::MakeSucceeded();
            }
            return F_FeedbackStatus::MakeFailed(ABYTEK_TEXT("Invalid type, requires number"));
        }
    };
    
    template<>
    struct TH_JSONConvert<I8>
    {
        static F_FeedbackStatus ValueToBoostJSONValue(const I8& Value, boost::json::value& JSONValue, F_JSONContext& JSONContext)
        {
            JSONValue = boost::json::value(Value);
            return F_FeedbackStatus::MakeSucceeded();
        }
        static F_FeedbackStatus BoostJSONValueToValue(I8& Value, const boost::json::value& JSONValue, F_JSONContext& JSONContext)
        {
            if (JSONValue.is_number())
            {
                Value = JSONValue.to_number<I8>();
                return F_FeedbackStatus::MakeSucceeded();
            }
            return F_FeedbackStatus::MakeFailed(ABYTEK_TEXT("Invalid type, requires number"));
        }
    };
    template<>
    struct TH_JSONConvert<I16>
    {
        static F_FeedbackStatus ValueToBoostJSONValue(const I16& Value, boost::json::value& JSONValue, F_JSONContext& JSONContext)
        {
            JSONValue = boost::json::value(Value);
            return F_FeedbackStatus::MakeSucceeded();
        }
        static F_FeedbackStatus BoostJSONValueToValue(I16& Value, const boost::json::value& JSONValue, F_JSONContext& JSONContext)
        {
            if (JSONValue.is_number())
            {
                Value = JSONValue.to_number<I16>();
                return F_FeedbackStatus::MakeSucceeded();
            }
            return F_FeedbackStatus::MakeFailed(ABYTEK_TEXT("Invalid type, requires number"));
        }
    };
    template<>
    struct TH_JSONConvert<I32>
    {
        static F_FeedbackStatus ValueToBoostJSONValue(const I32& Value, boost::json::value& JSONValue, F_JSONContext& JSONContext)
        {
            JSONValue = boost::json::value(Value);
            return F_FeedbackStatus::MakeSucceeded();
        }
        static F_FeedbackStatus BoostJSONValueToValue(I32& Value, const boost::json::value& JSONValue, F_JSONContext& JSONContext)
        {
            if (JSONValue.is_number())
            {
                Value = JSONValue.to_number<I32>();
                return F_FeedbackStatus::MakeSucceeded();
            }
            return F_FeedbackStatus::MakeFailed(ABYTEK_TEXT("Invalid type, requires number"));
        }
    };
    template<>
    struct TH_JSONConvert<I64>
    {
        static F_FeedbackStatus ValueToBoostJSONValue(const I64& Value, boost::json::value& JSONValue, F_JSONContext& JSONContext)
        {
            JSONValue = boost::json::value(Value);
            return F_FeedbackStatus::MakeSucceeded();
        }
        static F_FeedbackStatus BoostJSONValueToValue(I64& Value, const boost::json::value& JSONValue, F_JSONContext& JSONContext)
        {
            if (JSONValue.is_number())
            {
                Value = JSONValue.to_number<I64>();
                return F_FeedbackStatus::MakeSucceeded();
            }
            return F_FeedbackStatus::MakeFailed(ABYTEK_TEXT("Invalid type, requires number"));
        }
    };
    
    template<>
    struct TH_JSONConvert<F_SzForSpecifications>
    {
        static F_FeedbackStatus ValueToBoostJSONValue(const F_SzForSpecifications& Value, boost::json::value& JSONValue, F_JSONContext& JSONContext)
        {
            JSONValue = boost::json::value(*(const Sz*)&Value);
            return F_FeedbackStatus::MakeSucceeded();
        }
        static F_FeedbackStatus BoostJSONValueToValue(F_SzForSpecifications& Value, const boost::json::value& JSONValue, F_JSONContext& JSONContext)
        {
            if (JSONValue.is_number())
            {
                Value = F_SzForSpecifications { JSONValue.to_number<Sz>() };
                return F_FeedbackStatus::MakeSucceeded();
            }
            return F_FeedbackStatus::MakeFailed(ABYTEK_TEXT("Invalid type, requires number"));
        }
    };
    template<>
    struct TH_JSONConvert<F_PDiffForSpecifications>
    {
        static F_FeedbackStatus ValueToBoostJSONValue(const PDiff& Value, boost::json::value& JSONValue, F_JSONContext& JSONContext)
        {
            JSONValue = boost::json::value(*(const PDiff*)&Value);
            return F_FeedbackStatus::MakeSucceeded();
        }
        static F_FeedbackStatus BoostJSONValueToValue(F_PDiffForSpecifications& Value, const boost::json::value& JSONValue, F_JSONContext& JSONContext)
        {
            if (JSONValue.is_number())
            {
                Value = F_PDiffForSpecifications { JSONValue.to_number<PDiff>() };
                return F_FeedbackStatus::MakeSucceeded();
            }
            return F_FeedbackStatus::MakeFailed(ABYTEK_TEXT("Invalid type, requires number"));
        }
    };
    
    template<>
    struct TH_JSONConvert<F32>
    {
        static F_FeedbackStatus ValueToBoostJSONValue(const F32& Value, boost::json::value& JSONValue, F_JSONContext& JSONContext)
        {
            JSONValue = boost::json::value(Value);
            return F_FeedbackStatus::MakeSucceeded();
        }
        static F_FeedbackStatus BoostJSONValueToValue(F32& Value, const boost::json::value& JSONValue, F_JSONContext& JSONContext)
        {
            if (JSONValue.is_double())
            {
                Value = static_cast<F32>(JSONValue.as_double());
                return F_FeedbackStatus::MakeSucceeded();
            }
            return F_FeedbackStatus::MakeFailed(ABYTEK_TEXT("Invalid type, requires number"));
        }
    };
    template<>
    struct TH_JSONConvert<F64>
    {
        static F_FeedbackStatus ValueToBoostJSONValue(const F64& Value, boost::json::value& JSONValue, F_JSONContext& JSONContext)
        {
            JSONValue = boost::json::value(Value);
            return F_FeedbackStatus::MakeSucceeded();
        }
        static F_FeedbackStatus BoostJSONValueToValue(F64& Value, const boost::json::value& JSONValue, F_JSONContext& JSONContext)
        {
            if (JSONValue.is_double())
            {
                Value = static_cast<F64>(JSONValue.as_double());
                return F_FeedbackStatus::MakeSucceeded();
            }
            return F_FeedbackStatus::MakeFailed(ABYTEK_TEXT("Invalid type, requires number"));
        }
    };
    
    template<typename __F_Char>
    struct TH_JSONConvert<const __F_Char*>
    {
        using F_Value = const __F_Char*;
        
        static F_FeedbackStatus ValueToBoostJSONValue(const F_Value& Value, boost::json::value& JSONValue, F_JSONContext& JSONContext)
        {
            JSONValue = boost::json::value(ToString(Value).c_str());
            return F_FeedbackStatus::MakeSucceeded();
        }
    };
    template<typename __F_Char>
    struct TH_JSONConvert<__F_Char*>
    {
        using F_Value = __F_Char*;
        
        static F_FeedbackStatus ValueToBoostJSONValue(const F_Value& Value, boost::json::value& JSONValue, F_JSONContext& JSONContext)
        {
            JSONValue = boost::json::value(ToString(Value).c_str());
            return F_FeedbackStatus::MakeSucceeded();
        }
    };
    template<typename __F_Char, Sz __N>
    struct TH_JSONConvert<const __F_Char[__N]>
    {
        using F_Value = const __F_Char[__N];
        
        static F_FeedbackStatus ValueToBoostJSONValue(const F_Value& Value, boost::json::value& JSONValue, F_JSONContext& JSONContext)
        {
            JSONValue = boost::json::value(ToString(Value).c_str());
            return F_FeedbackStatus::MakeSucceeded();
        }
    };
    template<typename __F_Char, Sz __N>
    struct TH_JSONConvert<__F_Char[__N]>
    {
        using F_Value = __F_Char[__N];
        
        static F_FeedbackStatus ValueToBoostJSONValue(const F_Value& Value, boost::json::value& JSONValue, F_JSONContext& JSONContext)
        {
            JSONValue = boost::json::value(ToString(Value).c_str());
            return F_FeedbackStatus::MakeSucceeded();
        }
    };
    
    template<typename __F_First, typename __F_Second>
    struct TH_JSONConvert<std::pair<__F_First, __F_Second>>
    {
        using F_Value = std::pair<__F_First, __F_Second>;

        static F_FeedbackStatus ValueToBoostJSONValue(
            const F_Value& Value,
            boost::json::value& JSONValue,
            F_JSONContext& JSONContext
        )
        {
            boost::json::array Array;

            boost::json::value First;

            if (
                auto Status =
                    TH_JSONConvert<__F_First>::ValueToBoostJSONValue(
                        Value.first,
                        First,
                        JSONContext
                    );
                !Status
            )
            {
                return Status;
            }

            boost::json::value Second;

            if (
                auto Status =
                    TH_JSONConvert<__F_Second>::ValueToBoostJSONValue(
                        Value.second,
                        Second,
                        JSONContext
                    );
                !Status
            )
            {
                return Status;
            }

            Array.emplace_back(std::move(First));
            Array.emplace_back(std::move(Second));

            JSONValue = boost::json::value(std::move(Array));

            return F_FeedbackStatus::MakeSucceeded();
        }

        static F_FeedbackStatus BoostJSONValueToValue(
            F_Value& Value,
            const boost::json::value& JSONValue,
            F_JSONContext& JSONContext
        )
        {
            if (!JSONValue.is_array())
            {
                return F_FeedbackStatus::MakeFailed(
                    ABYTEK_TEXT("Requires JSON array for pair structure")
                );
            }

            const auto& Array = JSONValue.as_array();

            if (Array.size() != 2)
            {
                return F_FeedbackStatus::MakeFailed(
                    ABYTEK_TEXT("Requires JSON array length of 2 for pair structure")
                );
            }

            __F_First First;
            if (
                auto Status =
                    TH_JSONConvert<__F_First>::BoostJSONValueToValue(
                        First,
                        Array[0],
                        JSONContext
                    );
                !Status
            )
            {
                return Status;
            }

            __F_Second Second;
            if (
                auto Status =
                    TH_JSONConvert<__F_Second>::BoostJSONValueToValue(
                        Second,
                        Array[1],
                        JSONContext
                    );
                !Status
            )
            {
                return Status;
            }

            Value = F_Value(
                std::move(First),
                std::move(Second)
            );

            return F_FeedbackStatus::MakeSucceeded();
        }
    };
}
