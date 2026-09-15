#pragma once

#include "Abytek/Base.Serializable.prerequisites.pch.hpp"
#include "Abytek/JSONOptions.hpp"
#include "Abytek/JSONContext.hpp"
#include "Abytek/JSONConvert.hpp"


namespace Abytek
{
    struct ABYTEK_BASE_SERIALIZABLE_API H_JSON
    {
        template<typename __F_Value>
        static F_FeedbackStatus ValueToBoostJSONValue(const __F_Value& Value, boost::json::value& JSONValue, F_JSONContext& JSONContext)
        {
            return TH_JSONConvert<__F_Value>::ValueToBoostJSONValue(Value, JSONValue, JSONContext);
        }
        template<typename __F_Value>
        static F_FeedbackStatus BoostJSONValueToValue(__F_Value& Value, const boost::json::value& JSONValue, F_JSONContext& JSONContext)
        {
            return TH_JSONConvert<__F_Value>::BoostJSONValueToValue(Value, JSONValue, JSONContext);
        }
        static F_FeedbackStatus BoostJSONValueToText(const boost::json::value& JSONValue, F_Text& OutText)
        {
            try
            {
                OutText = ToText(
                    boost::json::serialize(JSONValue).c_str()
                );
                return F_FeedbackStatus::MakeSucceeded();
            }
            catch (const boost::system::system_error& Error)
            {
                return F_FeedbackStatus::MakeFailed(ToText(Error.what()));
            }
            catch (...)
            {
                return F_FeedbackStatus::MakeFailed(ABYTEK_TEXT("Unknown error"));
            }
        }
        static F_FeedbackStatus TextToBoostJSONValue(boost::json::value& OutJSONValue, const F_Text& Text)
        {
            try
            {
                OutJSONValue = boost::json::parse(ToString(Text).c_str());
                return F_FeedbackStatus::MakeSucceeded();
            }
            catch (const boost::system::system_error& Error)
            {
                return F_FeedbackStatus::MakeFailed(ToText(Error.what()));
            }
            catch (...)
            {
                return F_FeedbackStatus::MakeFailed(ABYTEK_TEXT("Unknown error"));
            }
        }
        
        template<typename __F_Value>
        static F_FeedbackStatus Stringify(const __F_Value& Value, F_Text& Text, const F_JSONOptions& Options = {})
        {
            F_JSONContext JSONContext = F_JSONContext::Make(Options);
            try
            {
                boost::json::value JSONValue;
                if (auto Status = ValueToBoostJSONValue(Value, JSONValue, JSONContext); !Status)
                {
                    return Status;
                }
                
                Text = ToText(
                    boost::json::serialize(JSONValue).c_str()
                );
                return F_FeedbackStatus::MakeSucceeded();
            }
            catch (const boost::system::system_error& Error)
            {
                return F_FeedbackStatus::MakeFailed(ToText(Error.what()));
            }
            catch (...)
            {
                return F_FeedbackStatus::MakeFailed(ABYTEK_TEXT("Unknown error"));
            }
        }
        template<typename __F_Value>
        static F_FeedbackStatus Parse(__F_Value& Value, const F_Text& Text, const F_JSONOptions& Options = {})
        {
            F_JSONContext JSONContext = F_JSONContext::Make(Options);
            try
            {
                return BoostJSONValueToValue<__F_Value>(
                    Value,
                    boost::json::parse(ToString(Text).c_str()),
                    JSONContext
                );
            }
            catch (const boost::system::system_error& Error)
            {
                return F_FeedbackStatus::MakeFailed(ToText(Error.what()));
            }
            catch (...)
            {
                return F_FeedbackStatus::MakeFailed(ABYTEK_TEXT("Unknown error"));
            }
        }
    };
}
