#pragma once

#include "Abytek/Base.Serializable.prerequisites.pch.hpp"
#include "Abytek/SerializableObjectData.hpp"


namespace Abytek
{
    class F_SerializableEnvironment;
    class A_SerializableObject;
    
    struct F_SerializablePackageHeader
    {
        U64 ObjectHeaderRange_OffsetInBytes = 0;
        U64 ObjectHeaderRange_SizeInBytes = 0;
        U64 MainPayloadRange_OffsetInBytes = 0;
        U64 MainPayloadRange_SizeInBytes = 0;
        U64 BulkPayloadRange_OffsetInBytes = 0;
        U64 BulkPayloadRange_SizeInBytes = 0;
        
        friend F_FeedbackStatus operator << (F_ArchiveReadWriteView& View, const F_SerializablePackageHeader& Value)
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.ObjectHeaderRange_OffsetInBytes);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.ObjectHeaderRange_SizeInBytes);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.MainPayloadRange_OffsetInBytes);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.MainPayloadRange_SizeInBytes);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.BulkPayloadRange_OffsetInBytes);
            ABYTEK_FEEDBACK_STATUS_CHECK(View << Value.BulkPayloadRange_SizeInBytes);
            return F_FeedbackStatus::MakeSucceeded();
        }
        friend F_FeedbackStatus operator >> (F_ArchiveReadOnlyView& View, F_SerializablePackageHeader& Value)
        {
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.ObjectHeaderRange_OffsetInBytes);
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.ObjectHeaderRange_SizeInBytes);
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.MainPayloadRange_OffsetInBytes);
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.MainPayloadRange_SizeInBytes);
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.BulkPayloadRange_OffsetInBytes);
            ABYTEK_FEEDBACK_STATUS_CHECK(View >> Value.BulkPayloadRange_SizeInBytes);
            return F_FeedbackStatus::MakeSucceeded();
        }
    };
    
    class ABYTEK_BASE_SERIALIZABLE_API F_SerializablePackage final : public A_Object
    {
    public:
        ABYTEK_BEGIN_REFLECTOR()
        ABYTEK_END_REFLECTOR(F_SerializablePackage);
        
    public:
        static F_Name GetFileExtension();
        
    private:
        TW<F_SerializableEnvironment> _Environment;
        F_Name _Name;
        F_Text _AbsolutePath;
        
        F_YieldReentrantCriticalSection _CriticalSection;
        
        F_SerializablePackageHeader _LastHeader;
        TF_Set<F_SerializableObjectHeader> _LastObjectHeaders;
        
        TF_Map<F_Name, TW<A_SerializableObject>> _Objects;
        
        B8 _IsDirty = false;
        
    public:
        ABYTEK_FORCE_INLINE const auto& GetEnvironment() const noexcept
        {
            return _Environment;
        }
        ABYTEK_FORCE_INLINE const auto& GetName() const noexcept
        {
            return _Name;
        }
        ABYTEK_FORCE_INLINE const auto& GetAbsolutePath() const noexcept
        {
            return _AbsolutePath;
        }
        ABYTEK_FORCE_INLINE const auto& GetLastHeader() const noexcept
        {
            return _LastHeader;
        }
        ABYTEK_FORCE_INLINE const auto& GetLastObjectHeaders() const noexcept
        {
            return _LastObjectHeaders;
        }
        ABYTEK_FORCE_INLINE const auto& GetObjects() const noexcept
        {
            return _Objects;
        }
        ABYTEK_FORCE_INLINE B8 HasObject(const F_Name& Name) const noexcept
        {
            return _Objects.find(Name) != _Objects.end();
        }
        ABYTEK_FORCE_INLINE const auto& GetObject_(const F_Name& Name) const noexcept
        {
            ABYTEK_BASE_SERIALIZABLE_ASSERT(HasObject(Name)) << "Not found object: " << Name;
            return _Objects.find(Name)->second;
        }
        ABYTEK_FORCE_INLINE TW<A_SerializableObject> FindObject(const F_Name& Name) const noexcept
        {
            auto It = _Objects.find(Name);
            if (It == _Objects.end())
            {
                return {};
            }
            return It->second;
        }
        
        ABYTEK_FORCE_INLINE auto IsDirty() const noexcept
        {
            return _IsDirty;
        }
        
    public:
        F_SerializablePackage(const TW_Valid<F_SerializableEnvironment>& Environment, const F_Name& Name);
        ~F_SerializablePackage() override;
        
    public:
        void _RegisterObject(const TW_Valid<A_SerializableObject>& Object);
        void _UnregisterObject(const TW_Valid<A_SerializableObject>& Object);
        
    private:
        TS<A_FSFile> _OpenFile(B8 EnableWrite, const TW<F_SerializableEnvironment>& Environment = {});
        void _InitialLoad();
        
    public:
        void CriticalSection(TF_Function<void()>&& Callback);
        void LoadObjectPayload(const F_SerializableObjectHeader& ObjectHeader, TF_Vector<U8>& OutPayload);
        void LoadMainPayload(U64 OffsetInBytes, U64 SizeInBytes, TF_Vector<U8>& OutPayload);
        void LoadBulkPayload(U64 OffsetInBytes, U64 SizeInBytes, TF_Vector<U8>& OutPayload);
        void Save(const TW<F_SerializableEnvironment>& Environment = {});
        
    public:
        B8 SearchLastObjectHeader(const F_Name& ObjectPath, F_SerializableObjectHeader& OutObjectHeader) const;
        
    public:
        static F_FeedbackStatus ResolveAbsolutePath(const TW_Valid<F_SerializableEnvironment>& Environment, const F_Name& Name, F_Text& OutAbsolutePath);
    
    public:
        B8 MarkDirty();
        void SaveIfDirty();
        
    public:
        // Map: object path -> object
        void GetOrLoadAllObjects(TF_Map<F_Name, TS<A_SerializableObject>>& OutObjects);
        TF_Vector<TS<A_SerializableObject>> GetOrLoadAllObjects();
    };
}