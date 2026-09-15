#pragma once

#include "Abytek/Base.Serializable.prerequisites.pch.hpp"


namespace Abytek
{
    class A_SerializableObject;
    class F_SerializableEnvironment;
    class F_SerializablePackage;

    struct ABYTEK_BASE_SERIALIZABLE_API F_SerializableObjectCreationParams
    {
        TF_ReflectionTypeHandle<A_SerializableObject> Type;
        F_Name Name;
        F_Name PackageName;
    };

    struct ABYTEK_BASE_SERIALIZABLE_API F_SerializableObjectPayloadRange
    {
        U64 PayloadOffsetInBytes = 0;
        U64 PayloadSizeInBytes = 0;
        
        friend F_FeedbackStatus operator << (F_ArchiveReadWriteView& View, const F_SerializableObjectPayloadRange& Value)
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.PayloadOffsetInBytes);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.PayloadSizeInBytes);
            return F_FeedbackStatus::MakeSucceeded();
        }
        friend F_FeedbackStatus operator >> (F_ArchiveReadOnlyView& View, F_SerializableObjectPayloadRange& Value)
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.PayloadOffsetInBytes);
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.PayloadSizeInBytes);
            return F_FeedbackStatus::MakeSucceeded();
        }
    };
    
    struct ABYTEK_BASE_SERIALIZABLE_API F_SerializableObjectHeader : F_SerializableObjectPayloadRange
    {
        TF_ReflectionTypeHandle<A_SerializableObject> Type;
        F_Name Path;
        TF_Vector<F_Name> ReferencePaths;
        
        TS<A_SerializableObject> CreateObject(
            const TW_Valid<F_SerializableEnvironment>& Environment,
            const TS<F_SerializablePackage>& Package
        ) const;
        
        friend F_FeedbackStatus operator << (F_ArchiveReadWriteView& View, const F_SerializableObjectHeader& Value)
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View << static_cast<const F_SerializableObjectPayloadRange&>(Value));
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.Type);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.Path);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.ReferencePaths);
            return F_FeedbackStatus::MakeSucceeded();
        }
        friend F_FeedbackStatus operator >> (F_ArchiveReadOnlyView& View, F_SerializableObjectHeader& Value)
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> static_cast<F_SerializableObjectPayloadRange&>(Value));
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.Type);
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.Path);
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.ReferencePaths);
            return F_FeedbackStatus::MakeSucceeded();
        }
        
        friend B8 operator < (const F_SerializableObjectHeader& A, const F_SerializableObjectHeader& B)
        {
            return A.Path < B.Path;
        }
        friend B8 operator > (const F_SerializableObjectHeader& A, const F_SerializableObjectHeader& B)
        {
            return A.Path > B.Path;
        }
        friend B8 operator == (const F_SerializableObjectHeader& A, const F_SerializableObjectHeader& B)
        {
            return A.Path == B.Path;
        }
        friend B8 operator != (const F_SerializableObjectHeader& A, const F_SerializableObjectHeader& B)
        {
            return A.Path != B.Path;
        }
    };

    struct ABYTEK_BASE_SERIALIZABLE_API F_SerializableObjectPayloadData
    {
        TF_Vector<U8> Bytes;
    };
}

namespace boost
{
    template<>
    struct hash<Abytek::F_SerializableObjectHeader>
    {
        size_t operator()(const Abytek::F_SerializableObjectHeader& Value) const
        {
            return boost::hash<Abytek::F_Name>()(Value.Path);
        }
    };
}