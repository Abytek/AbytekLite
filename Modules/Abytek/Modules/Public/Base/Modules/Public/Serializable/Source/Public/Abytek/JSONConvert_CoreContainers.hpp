#pragma once

#include "Abytek/Base.Serializable.prerequisites.pch.hpp"
#include "Abytek/JSONConvert.hpp"


namespace Abytek
{
    template<class __F_StringIdDatabase>
    struct TH_JSONConvert<TF_StringId<__F_StringIdDatabase>>
    {
        static F_FeedbackStatus ValueToBoostJSONValue(const TF_StringId<__F_StringIdDatabase>& Value, boost::json::value& JSONValue, F_JSONContext& JSONContext)
        {
            JSONValue = boost::json::value(ToString(*Value).c_str());
            return F_FeedbackStatus::MakeSucceeded();
        }
        static F_FeedbackStatus BoostJSONValueToValue(TF_StringId<__F_StringIdDatabase>& Value, const boost::json::value& JSONValue, F_JSONContext& JSONContext)
        {
            if (JSONValue.is_string())
            {
                Value = TF_StringId<__F_StringIdDatabase>(ToString<F_TextChar, const F_Char*>(JSONValue.as_string().c_str()));
                return F_FeedbackStatus::MakeSucceeded();
            }
            return F_FeedbackStatus::MakeFailed(ABYTEK_TEXT("Invalid type, requires string"));
        }
    };
    template<typename __F_Type>
    struct TH_JSONConvert<TF_ReflectionTypeHandle<__F_Type>>
    {
        static F_FeedbackStatus ValueToBoostJSONValue(const TF_ReflectionTypeHandle<__F_Type>& Value, boost::json::value& JSONValue, F_JSONContext& JSONContext)
        {
            F_Name Name;
            if (Value)
            {
                Name = Value->GetCanonical();
            }
            JSONValue = boost::json::value(ToString(*Name).c_str());
            return F_FeedbackStatus::MakeSucceeded();
        }
        static F_FeedbackStatus BoostJSONValueToValue(TF_ReflectionTypeHandle<__F_Type>& Value, const boost::json::value& JSONValue, F_JSONContext& JSONContext)
        {
            if (JSONValue.is_string())
            {
                if (auto ItemSearchKey = F_Name(ToString<F_TextChar, const F_Char*>(JSONValue.as_string().c_str())))
                {
                    auto ReflectionType = GlobalSearchReflectionItem<F_ReflectionType>(ItemSearchKey);
                    if (!ReflectionType)
                    {
                        return F_FeedbackStatus::MakeFailed(ABYTEK_TEXT("Invalid reflection type: ") + *ItemSearchKey);
                    }
                    Value = ReflectionType;
                }
                else
                {
                    Value = {};
                }
                return F_FeedbackStatus::MakeSucceeded();
            }
            return F_FeedbackStatus::MakeFailed(ABYTEK_TEXT("Invalid type, requires string"));
        }
    };
}