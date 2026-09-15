#pragma once

#include "Abytek/Base.Serializable.prerequisites.pch.hpp"


namespace Abytek
{
    template<typename __F_Value, typename>
    struct TH_JSONConvert;
    
    template<typename __F_Value>
    struct TH_JSONConvertInvalid
    {
        static F_FeedbackStatus ValueToBoostJSONValue(const __F_Value& Value, boost::json::value& JSONValue, F_JSONContext& JSONContext)
        {
            return F_FeedbackStatus::MakeFailed(ToText("Type not support JSON serialization: ") + ToText(TypeFullName<__F_Value>()));
        }
        static F_FeedbackStatus BoostJSONValueToValue(__F_Value& Value, const boost::json::value& JSONValue, F_JSONContext& JSONContext)
        {
            return F_FeedbackStatus::MakeFailed(ToText("Type not support JSON deserialization: ") + ToText(TypeFullName<__F_Value>()));
        }
    };
    template<typename __F_Value>
    static constexpr B8 IsJSONSerializable()
    {
        return !std::is_base_of_v<TH_JSONConvertInvalid<__F_Value>, std::remove_const_t<__F_Value>>;
    }
}
