#include "Abytek/IHIButton.hpp"
#include "Abytek/IHIButtonCoder.hpp"


namespace Abytek
{
    ABYTEK_REFLECT(F_IHIButton)
    {
        ABYTEK_REFLECT_CANONICAL(ABYTEK_NAME("Abytek::F_IHIButton"));
        
        ABYTEK_REFLECT_PROPERTY_SERIALIZABLE(DeviceType);
        ABYTEK_REFLECT_PROPERTY_SERIALIZABLE(Code);
    }

    F_IHIButton::F_IHIButton(E_IHIPredefinedButton Predefined)
    {
        ABYTEK_FEEDBACK_STATUS_CHECK_HARD(
            A_IHIButtonCoder::GetInstance()->Encode(Predefined, *this)  
        );
    }

    F_FeedbackStatus operator << (F_ArchiveReadWriteView& View, const F_IHIButton& Value)
    {
        B8 IsValid = Value.IsValid();
        ABYTEK_FEEDBACK_STATUS_CHECK(View << IsValid);
        if (IsValid)
        {
            E_IHIPredefinedButton PredefinedButton = E_IHIPredefinedButton::NONE;
            if (
                auto Status = A_IHIButtonCoder::GetInstance()->Decode(
                    PredefinedButton,
                    Value
                );
                Status
            )
            {
                ABYTEK_FEEDBACK_STATUS_CHECK(View << true);
                ABYTEK_FEEDBACK_STATUS_CHECK(View << PredefinedButton);
            }
            else
            {
                ABYTEK_FEEDBACK_STATUS_CHECK(View << false);
                ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.DeviceType);
                ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.Code);
            }
        }
        return F_FeedbackStatus::MakeSucceeded();
    }
    F_FeedbackStatus operator >> (F_ArchiveReadOnlyView& View, F_IHIButton& Value)
    {
        B8 IsValid = false;
        ABYTEK_FEEDBACK_STATUS_CHECK(View >> IsValid);
        if (IsValid)
        {
            B8 UsePredefinedButton = false;
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> UsePredefinedButton);
            if (UsePredefinedButton)
            {
                E_IHIPredefinedButton PredefinedButton = E_IHIPredefinedButton::NONE;
                ABYTEK_FEEDBACK_STATUS_CHECK(View >> PredefinedButton);
                ABYTEK_FEEDBACK_STATUS_CHECK(A_IHIButtonCoder::GetInstance()->Encode(PredefinedButton, Value));
            }
            else
            {
                ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.DeviceType);
                ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.Code);
            }
        }
        return F_FeedbackStatus::MakeSucceeded();
    }

    F_FeedbackStatus TH_JSONConvert<F_IHIButton, void>::ValueToBoostJSONValue(const F_IHIButton& Value, boost::json::value& JSONValue, const F_JSONOptions& Options)
    {
        if (!Value.IsValid())
        {
            JSONValue = nullptr;
            return F_FeedbackStatus::MakeSucceeded();
        }

        E_IHIPredefinedButton PredefinedButton =
            E_IHIPredefinedButton::NONE;

        if (auto Status =
                A_IHIButtonCoder::GetInstance()->Decode(
                    PredefinedButton,
                    Value);
            Status)
        {
            boost::json::object Object;

            Object["UsePredefinedButton"] = true;
            Object["PredefinedButton"] =
                static_cast<F_IHIPredefinedButtonIndex>(PredefinedButton);

            JSONValue = std::move(Object);

            return F_FeedbackStatus::MakeSucceeded();
        }

        boost::json::object Object;

        Object["UsePredefinedButton"] = false;
        Object["DeviceType"] =
            static_cast<F_IHIButtonDeviceTypeIndex>(Value.DeviceType);
        Object["Code"] =
            Value.Code;

        JSONValue = std::move(Object);

        return F_FeedbackStatus::MakeSucceeded();
    }
    F_FeedbackStatus TH_JSONConvert<F_IHIButton, void>::BoostJSONValueToValue(F_IHIButton& Value, const boost::json::value& JSONValue, const F_JSONOptions& Options)
    {
        if (JSONValue.is_null())
        {
            Value = F_IHIButton{};
            return F_FeedbackStatus::MakeSucceeded();
        }

        if (!JSONValue.is_object())
        {
            return F_FeedbackStatus::MakeFailed(
                ABYTEK_TEXT("Invalid type, requires object or null")
            );
        }

        const boost::json::object& Object =
            JSONValue.as_object();

        auto UsePredefinedIt =
            Object.find("UsePredefinedButton");

        if (UsePredefinedIt == Object.end() ||
            !UsePredefinedIt->value().is_bool())
        {
            return F_FeedbackStatus::MakeFailed(
                ABYTEK_TEXT("Missing or invalid UsePredefinedButton")
            );
        }

        const B8 UsePredefinedButton =
            UsePredefinedIt->value().as_bool();

        if (UsePredefinedButton)
        {
            auto PredefinedIt =
                Object.find("PredefinedButton");

            if (PredefinedIt == Object.end() ||
                !PredefinedIt->value().is_uint64())
            {
                return F_FeedbackStatus::MakeFailed(
                    ABYTEK_TEXT("Missing or invalid PredefinedButton")
                );
            }

            const auto PredefinedIndex =
                static_cast<F_IHIPredefinedButtonIndex>(
                    PredefinedIt->value().as_uint64()
                );

            const auto PredefinedButton =
                static_cast<E_IHIPredefinedButton>(
                    PredefinedIndex
                );

            Value = F_IHIButton(PredefinedButton);

            return F_FeedbackStatus::MakeSucceeded();
        }

        auto DeviceTypeIt =
            Object.find("DeviceType");

        auto CodeIt =
            Object.find("Code");

        if (DeviceTypeIt == Object.end() ||
            !DeviceTypeIt->value().is_uint64())
        {
            return F_FeedbackStatus::MakeFailed(
                ABYTEK_TEXT("Missing or invalid DeviceType")
            );
        }

        if (CodeIt == Object.end() ||
            !CodeIt->value().is_uint64())
        {
            return F_FeedbackStatus::MakeFailed(
                ABYTEK_TEXT("Missing or invalid Code")
            );
        }

        Value.DeviceType =
            static_cast<E_IHIButtonDeviceType>(
                DeviceTypeIt->value().as_uint64()
            );

        Value.Code =
            static_cast<F_IHIButtonCode>(
                CodeIt->value().as_uint64()
            );

        return F_FeedbackStatus::MakeSucceeded();
    }
}
