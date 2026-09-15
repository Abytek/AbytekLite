#pragma once

#include "Abytek/Base.Serializable.prerequisites.pch.hpp"
#include "Abytek/JSONConvert.hpp"


namespace Abytek
{
    template<typename __F_Item, Sz __Size>
    struct TH_JSONConvert<boost::array<__F_Item, __Size>>
    {
        using F_Value = boost::array<__F_Item, __Size>;

        static F_FeedbackStatus ValueToBoostJSONValue(
            const F_Value& Value,
            boost::json::value& JSONValue,
            F_JSONContext& JSONContext
        )
        {
            boost::json::array JSONArray;
            JSONArray.reserve(__Size);

            for (const auto& Item : Value)
            {
                boost::json::value ItemJSONValue;

                if (
                    auto Status =
                        TH_JSONConvert<__F_Item>::ValueToBoostJSONValue(
                            Item,
                            ItemJSONValue,
                            JSONContext
                        );
                    !Status
                )
                {
                    return Status;
                }

                JSONArray.emplace_back(std::move(ItemJSONValue));
            }

            JSONValue = boost::json::value(
                ABYTEK_MOVE(JSONArray)
            );

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
                    ABYTEK_TEXT("Invalid type, requires array")
                );
            }

            const auto& JSONArray = JSONValue.as_array();

            if (JSONArray.size() != __Size)
            {
                return F_FeedbackStatus::MakeFailed(
                    ABYTEK_TEXT("Invalid array size, requires size of ")
                    + ToText(__Size)
                );
            }

            F_Value Result;

            for (Sz Index = 0; Index < __Size; ++Index)
            {
                if (
                    auto Status =
                        TH_JSONConvert<__F_Item>::BoostJSONValueToValue(
                            Result[Index],
                            JSONArray[Index],
                            JSONContext
                        );
                    !Status
                )
                {
                    return Status;
                }
            }

            Value = std::move(Result);

            return F_FeedbackStatus::MakeSucceeded();
        }
    };
    
    template<typename __F_Item>
    struct TH_JSONConvert<boost::optional<__F_Item>>
    {
        using F_Value = boost::optional<__F_Item>;

        static F_FeedbackStatus ValueToBoostJSONValue(
            const F_Value& Value,
            boost::json::value& JSONValue,
            F_JSONContext& JSONContext
        )
        {
            if (!Value)
            {
                JSONValue = boost::json::value(nullptr);
                return F_FeedbackStatus::MakeSucceeded();
            }

            return TH_JSONConvert<__F_Item>::ValueToBoostJSONValue(
                *Value,
                JSONValue,
                JSONContext
            );
        }

        static F_FeedbackStatus BoostJSONValueToValue(
            F_Value& Value,
            const boost::json::value& JSONValue,
            F_JSONContext& JSONContext
        )
        {
            if (JSONValue.is_null())
            {
                Value = F_Value{};
                return F_FeedbackStatus::MakeSucceeded();
            }

            __F_Item Result;

            if (
                auto Status =
                    TH_JSONConvert<__F_Item>::BoostJSONValueToValue(
                        Result,
                        JSONValue,
                        JSONContext
                    );
                !Status
            )
            {
                return Status;
            }

            Value = std::move(Result);

            return F_FeedbackStatus::MakeSucceeded();
        }
    };
    
    template<typename __F_Char, class __F_Traits, class __F_Allocator>
    struct TH_JSONConvert<boost::container::basic_string<__F_Char, __F_Traits, __F_Allocator>>
    {
        using F_Value = boost::container::basic_string<__F_Char, __F_Traits, __F_Allocator>;
        
        static F_FeedbackStatus ValueToBoostJSONValue(const F_Value& Value, boost::json::value& JSONValue, F_JSONContext& JSONContext)
        {
            JSONValue = boost::json::value(ToString(Value).c_str());
            return F_FeedbackStatus::MakeSucceeded();
        }
        static F_FeedbackStatus BoostJSONValueToValue(F_Value& Value, const boost::json::value& JSONValue, F_JSONContext& JSONContext)
        {
            if (JSONValue.is_string())
            {
                Value = ToString<__F_Char, const F_Char*>(JSONValue.as_string().c_str());
                return F_FeedbackStatus::MakeSucceeded();
            }
            return F_FeedbackStatus::MakeFailed(ABYTEK_TEXT("Invalid type, requires string"));
        }
    };
    
    template<typename __F_Container>
    struct TH_JSONConvertSequence
    {
        using F_Value = __F_Container;

        static F_FeedbackStatus ValueToBoostJSONValue(
            const F_Value& Value,
            boost::json::value& JSONValue,
            F_JSONContext& JSONContext
        )
        {
            boost::json::array Array;
            Array.reserve(Value.size());

            for (const auto& Item : Value)
            {
                boost::json::value ItemJSONValue;

                if (
                    auto Status =
                        TH_JSONConvert<
                            std::remove_const_t<
                                std::remove_reference_t<decltype(Item)>
                            >
                        >::ValueToBoostJSONValue(
                            Item,
                            ItemJSONValue,
                            JSONContext
                        );
                    !Status
                )
                {
                    return Status;
                }

                Array.emplace_back(std::move(ItemJSONValue));
            }

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
                    ABYTEK_TEXT("Invalid type, requires array")
                );
            }

            F_Value Result;

            for (const auto& JSONItem : JSONValue.as_array())
            {
                using F_Item = typename F_Value::value_type;

                F_Item Item;

                if (
                    auto Status =
                        TH_JSONConvert<F_Item>
                        ::BoostJSONValueToValue(
                            Item,
                            JSONItem,
                            JSONContext
                        );
                    !Status
                )
                {
                    return Status;
                }

                Result.emplace_back(std::move(Item));
            }

            Value = std::move(Result);

            return F_FeedbackStatus::MakeSucceeded();
        }
    };
    
    template<typename T, typename A>
    struct TH_JSONConvert<boost::container::vector<T,A>>
    : TH_JSONConvertSequence<boost::container::vector<T,A>>
    {};
    
    template<typename T, typename A>
    struct TH_JSONConvert<boost::container::devector<T,A>>
        : TH_JSONConvertSequence<boost::container::devector<T,A>>
    {};
    
    template<typename T, Sz N, typename A>
    struct TH_JSONConvert<boost::container::small_vector<T,N,A>>
        : TH_JSONConvertSequence<boost::container::small_vector<T,N,A>>
    {};
    
    template<typename T, typename A>
    struct TH_JSONConvert<boost::container::stable_vector<T,A>>
        : TH_JSONConvertSequence<boost::container::stable_vector<T,A>>
    {};
    
    template<typename T, Sz N, typename O>
    struct TH_JSONConvert<boost::container::static_vector<T,N,O>>
        : TH_JSONConvertSequence<boost::container::static_vector<T,N,O>>
    {};
    
    template<typename T, typename A>
    struct TH_JSONConvert<boost::container::deque<T,A,void>>
        : TH_JSONConvertSequence<boost::container::deque<T,A,void>>
    {};
    
    template<typename T, typename A>
    struct TH_JSONConvert<boost::container::list<T,A>>
        : TH_JSONConvertSequence<boost::container::list<T,A>>
    {};
    
    template<typename T, typename A>
    struct TH_JSONConvert<boost::container::slist<T,A>>
        : TH_JSONConvertSequence<boost::container::slist<T,A>>
    {};
    
    template<typename __F_Set>
    struct TH_JSONConvertSet
    {
        using F_Value = __F_Set;

        static F_FeedbackStatus ValueToBoostJSONValue(
            const F_Value& Value,
            boost::json::value& JSONValue,
            F_JSONContext& JSONContext
        )
        {
            boost::json::array Array;
            Array.reserve(Value.size());

            for (const auto& Item : Value)
            {
                boost::json::value ItemJSONValue;

                if (
                    auto Status =
                        TH_JSONConvert<typename F_Value::value_type>
                        ::ValueToBoostJSONValue(
                            Item,
                            ItemJSONValue,
                            JSONContext
                        );
                    !Status
                )
                {
                    return Status;
                }

                Array.emplace_back(std::move(ItemJSONValue));
            }

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
                    ABYTEK_TEXT("Invalid type, requires array")
                );
            }

            F_Value Result;

            for (const auto& JSONItem : JSONValue.as_array())
            {
                typename F_Value::value_type Item;

                if (
                    auto Status =
                        TH_JSONConvert<typename F_Value::value_type>
                        ::BoostJSONValueToValue(
                            Item,
                            JSONItem,
                            JSONContext
                        );
                    !Status
                )
                {
                    return Status;
                }

                Result.insert(std::move(Item));
            }

            Value = std::move(Result);

            return F_FeedbackStatus::MakeSucceeded();
        }
    };
    
    template<typename T, typename C, typename A>
    struct TH_JSONConvert<boost::container::set<T,C,A>>
    : TH_JSONConvertSet<boost::container::set<T,C,A>>
    {};
    
    template<typename T, typename H, typename P, typename A>
    struct TH_JSONConvert<boost::unordered_set<T,H,P,A>>
        : TH_JSONConvertSet<boost::unordered_set<T,H,P,A>>
    {};
    
    template<typename T, typename C, typename A>
    struct TH_JSONConvert<boost::container::flat_set<T,C,A>>
        : TH_JSONConvertSet<boost::container::flat_set<T,C,A>>
    {};
    
    template<typename __F_Map>
    struct TH_JSONConvertMapObject
    {
        using F_Value  = __F_Map;
        using F_Key    = typename F_Value::key_type;
        using F_Mapped = typename F_Value::mapped_type;

        static F_FeedbackStatus ValueToBoostJSONValue(
            const F_Value& Value,
            boost::json::value& JSONValue,
            F_JSONContext& JSONContext
        )
        {
            boost::json::object Object;

            for (const auto& [Key, Val] : Value)
            {
                boost::json::value ValueJSONValue;

                if (
                    auto Status =
                        TH_JSONConvert<F_Mapped>::ValueToBoostJSONValue(
                            Val,
                            ValueJSONValue,
                            JSONContext
                        );
                    !Status
                )
                {
                    return Status;
                }

                Object[boost::json::string(Key)] =
                    std::move(ValueJSONValue);
            }

            JSONValue = boost::json::value(std::move(Object));

            return F_FeedbackStatus::MakeSucceeded();
        }

        static F_FeedbackStatus BoostJSONValueToValue(
            F_Value& Value,
            const boost::json::value& JSONValue,
            F_JSONContext& JSONContext
        )
        {
            if (!JSONValue.is_object())
            {
                return F_FeedbackStatus::MakeFailed(
                    ABYTEK_TEXT("Invalid type, requires object")
                );
            }

            F_Value Result;

            for (const auto& [Key, JSONItem] :
                 JSONValue.as_object())
            {
                F_Mapped Mapped;

                if (
                    auto Status =
                        TH_JSONConvert<F_Mapped>::BoostJSONValueToValue(
                            Mapped,
                            JSONItem,
                            JSONContext
                        );
                    !Status
                )
                {
                    return Status;
                }

                Result.emplace(
                    F_Key(Key),
                    std::move(Mapped)
                );
            }

            Value = std::move(Result);

            return F_FeedbackStatus::MakeSucceeded();
        }
    };
    
    template<typename __F_Map>
    struct TH_JSONConvertMapSequence
    {
        using F_Value  = __F_Map;
        using F_Key    = typename F_Value::key_type;
        using F_Mapped = typename F_Value::mapped_type;

        static F_FeedbackStatus ValueToBoostJSONValue(
            const F_Value& Value,
            boost::json::value& JSONValue,
            F_JSONContext& JSONContext
        )
        {
            boost::json::array Array;

            for (const auto& [Key, Val] : Value)
            {
                boost::json::object Item;

                boost::json::value KeyJSONValue;
                if (
                    auto Status =
                        TH_JSONConvert<F_Key>::ValueToBoostJSONValue(
                            Key,
                            KeyJSONValue,
                            JSONContext
                        );
                    !Status
                )
                {
                    return Status;
                }

                boost::json::value ValueJSONValue;
                if (
                    auto Status =
                        TH_JSONConvert<F_Mapped>::ValueToBoostJSONValue(
                            Val,
                            ValueJSONValue,
                            JSONContext
                        );
                    !Status
                )
                {
                    return Status;
                }

                Item["key"]   = std::move(KeyJSONValue);
                Item["value"] = std::move(ValueJSONValue);

                Array.emplace_back(std::move(Item));
            }

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
                    ABYTEK_TEXT("Invalid type, requires array")
                );
            }

            F_Value Result;

            for (const auto& JSONItem : JSONValue.as_array())
            {
                if (!JSONItem.is_object())
                {
                    return F_FeedbackStatus::MakeFailed(
                        ABYTEK_TEXT("Invalid array item type, requires object")
                    );
                }

                const auto& Object = JSONItem.as_object();

                auto KeyHandle = Object.find("key");
                if (KeyHandle == Object.end())
                {
                    return F_FeedbackStatus::MakeFailed(
                        ABYTEK_TEXT("Missing required field: key")
                    );
                }

                auto ValueHandle = Object.find("value");
                if (ValueHandle == Object.end())
                {
                    return F_FeedbackStatus::MakeFailed(
                        ABYTEK_TEXT("Missing required field: value")
                    );
                }

                F_Key Key;
                if (
                    auto Status =
                        TH_JSONConvert<F_Key>::BoostJSONValueToValue(
                            Key,
                            KeyHandle->value(),
                            JSONContext
                        );
                    !Status
                )
                {
                    return Status;
                }

                F_Mapped Mapped;
                if (
                    auto Status =
                        TH_JSONConvert<F_Mapped>::BoostJSONValueToValue(
                            Mapped,
                            ValueHandle->value(),
                            JSONContext
                        );
                    !Status
                )
                {
                    return Status;
                }

                Result.emplace(
                    std::move(Key),
                    std::move(Mapped)
                );
            }

            Value = std::move(Result);

            return F_FeedbackStatus::MakeSucceeded();
        }
    };
    
    template<typename __F_Key, typename __F_Map>
    struct TH_JSONConvertMap : TH_JSONConvertMapSequence<__F_Map>
    {
    };
    template<typename __F_Char, class __F_Traits, class __F_Allocator, typename __F_Map>
    struct TH_JSONConvertMap<boost::container::basic_string<__F_Char, __F_Traits, __F_Allocator>, __F_Map>
    {
    };
    
    template<typename K, typename V, typename C, typename A>
    struct TH_JSONConvert<boost::container::map<K,V,C,A>>
    : TH_JSONConvertMap<K, boost::container::map<K,V,C,A>>
    {};
    
    template<typename K, typename V, typename H, typename P, typename A>
    struct TH_JSONConvert<boost::unordered_map<K,V,H,P,A>>
        : TH_JSONConvertMap<K, boost::unordered_map<K,V,H,P,A>>
    {};
    
    template<typename K, typename V, typename C, typename A>
    struct TH_JSONConvert<boost::container::flat_map<K,V,C,A>>
        : TH_JSONConvertMap<K, boost::container::flat_map<K,V,C,A>>
    {};
}