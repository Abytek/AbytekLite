#pragma once

#include "Abytek/Archive.hpp"
#include "Abytek/PrimitiveArchiveReader.hpp"
#include "Abytek/PrimitiveArchiveWriter.hpp"
#include "Abytek/ContainerArchiveReader.hpp"
#include "Abytek/ContainerArchiveWriter.hpp"
#include "Abytek/BoostContainerArchiveReader.hpp"
#include "Abytek/BoostContainerArchiveWriter.hpp"


namespace Abytek
{
    struct F_ArchiveBulkHeader
    {
        U64 PayloadRelativeOffsetInBytes = 0;
        U64 PayloadSizeInBytes = 0;
        
        friend F_FeedbackStatus operator << (F_ArchiveReadWriteView& View, const F_ArchiveBulkHeader& Value)
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.PayloadRelativeOffsetInBytes);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.PayloadSizeInBytes);
            return F_FeedbackStatus::MakeSucceeded();
        }
        friend F_FeedbackStatus operator >> (F_ArchiveReadOnlyView& View, F_ArchiveBulkHeader& Value)
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.PayloadRelativeOffsetInBytes);
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.PayloadSizeInBytes);
            return F_FeedbackStatus::MakeSucceeded();
        }
    };
    
    struct H_ArchiveBulk
    {
        static F_FeedbackStatus Write(F_ArchiveReadWriteView& View, F_ArchiveBulkHeader& OutBulkHeader, const TF_Span<const U8>& Bytes)
        {
            U64 ViewOffset_Start = View.Offset;
            
            U64 ViewOffset_PayloadRelativeOffsetInBytes = View.Offset;
            ABYTEK_FEEDBACK_STATUS_CHECK(View << (U64)0);
            U64 ViewPostOffset_PayloadRelativeOffsetInBytes = View.Offset;
            
            U64 ViewOffset_PayloadSizeInBytes = View.Offset;
            ABYTEK_FEEDBACK_STATUS_CHECK(View << (U64)0);
            U64 ViewPostOffset_PayloadSizeInBytes = View.Offset;
            
            View.Shift<F_ArchiveData>(0);
            U64 ViewOffset_Payload = View.Offset;
            
            View.Offset = ViewOffset_PayloadRelativeOffsetInBytes;
            U64 PayloadRelativeOffsetInBytes = ViewOffset_Payload - ViewOffset_Start;
            ABYTEK_FEEDBACK_STATUS_CHECK(View << PayloadRelativeOffsetInBytes);
            
            View.Offset = ViewOffset_PayloadSizeInBytes;
            U64 PayloadSizeInBytes = Bytes.size();
            ABYTEK_FEEDBACK_STATUS_CHECK(View << PayloadSizeInBytes);
            
            View.Offset = ViewOffset_Payload;
            auto DstBytes = View.AccessSpan<U8>(Bytes.size());
            memcpy(
                DstBytes.data(),
                Bytes.data(),
                Bytes.size()
            );
            
            ABYTEK_BASE_CORE_ASSERT(View.Offset == (ViewOffset_Payload + Bytes.size())) << "Internal bug";
            
            OutBulkHeader.PayloadRelativeOffsetInBytes = PayloadRelativeOffsetInBytes;
            OutBulkHeader.PayloadSizeInBytes = PayloadSizeInBytes;
            return F_FeedbackStatus::MakeSucceeded();
        }
        static F_FeedbackStatus Skip(F_ArchiveReadOnlyView& View, F_ArchiveBulkHeader& OutBulkHeader)
        {
            U64 ViewOffset_Start = View.Offset;
            
            U64 PayloadRelativeOffsetInBytes = 0;
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> PayloadRelativeOffsetInBytes);
            U64 PayloadSizeInBytes = 0;
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> PayloadSizeInBytes);
            
            U64 ViewOffset_EndPayload = ViewOffset_Start + PayloadRelativeOffsetInBytes + PayloadSizeInBytes;
            
            if (!View.CheckSize(ViewOffset_EndPayload))
            {
                return F_FeedbackStatus::MakeFailed(
                    ABYTEK_TEXT("Invalid bulk, cannot skip")    
                );
            }
            
            View.Offset = ViewOffset_EndPayload;
            
            OutBulkHeader.PayloadRelativeOffsetInBytes = PayloadRelativeOffsetInBytes;
            OutBulkHeader.PayloadSizeInBytes = PayloadSizeInBytes;
            return F_FeedbackStatus::MakeSucceeded();
        }
    };
}