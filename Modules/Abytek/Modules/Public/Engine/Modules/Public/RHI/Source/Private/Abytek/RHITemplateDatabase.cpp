#include "Abytek/RHITemplateDatabase.hpp"
#include "Abytek/RHITemplate.hpp"
#include "Abytek/DirectX12Shared/RHITemplateDatabase.hpp"


namespace Abytek
{
    A_RHITemplateDatabase::A_RHITemplateDatabase(E_RHIAPI API, const F_RHIFeatureSupports& FeatureSupports) :
        _API(API),
        _FeatureSupports(FeatureSupports)
    {
    }
    A_RHITemplateDatabase::~A_RHITemplateDatabase()
    {
    }
    
    void A_RHITemplateDatabase::_TrackTemplate(const TW_Valid<A_RHITemplate>& Template)
    {
        TF_ScopeLock<F_SpinLock> _(_SpinLock);
        ABYTEK_ENGINE_RHI_ASSERT(_Templates.find(Template->GetHashCode()) == _Templates.end());
        _Templates.insert({ Template->GetHashCode(), Template });
    }
    void A_RHITemplateDatabase::_UntrackTemplate(const TW_Valid<A_RHITemplate>& Template)
    {
        TF_ScopeLock<F_SpinLock> _(_SpinLock);
        ABYTEK_ENGINE_RHI_ASSERT(_Templates.find(Template->GetHashCode()) != _Templates.end());
        _Templates.erase(_Templates.find(Template->GetHashCode()));
    }

    TU<A_RHITemplateDatabase> A_RHITemplateDatabase::Create(E_RHIAPI API, const F_RHIFeatureSupports& FeatureSupports)
    {
        switch (API)
        {
        case E_RHIAPI::DIRECTX12:
            return TU<F_DirectX12SharedRHITemplateDatabase>()(FeatureSupports);
        default:
            ABYTEK_ENGINE_RHI_ASSERT(false) << "Invalid RHI API, cannot create template database";
        }
        return {};
    }

    E_RHITemplateHashCodeLockStatus A_RHITemplateDatabase::LockHashCode(
        F_RHITemplateHashCode HashCode,
        TS<A_RHITemplate>& OutTemplateIfCreated
    )
    {
        TF_ScopeLock<F_SpinLock> _(_SpinLock);
        {
            auto It = _LockedHashCodes.find(HashCode);
            if (It == _LockedHashCodes.end())
            {
                _LockedHashCodes.insert(HashCode);
                auto It2 = _Templates.find(HashCode);
                if (It2 == _Templates.end())
                {
                    return E_RHITemplateHashCodeLockStatus::NEED_TO_CREATE;
                }
                auto Template = It2->second;
                ABYTEK_ENGINE_RHI_ASSERT(Template) << "Internal RHI template database bug";
                if (
                    !FlagHas(
                        F_StandardObjectKeyPool::GetInstance().GetTrackingFlag(Template.GetObjectKey()),
                        E_StandardObjectTrackingFlag::EARLY_DESTRUCTED
                    )
                )
                {
                    auto SharedTemplate = ShareObject(Template);
                    ABYTEK_ENGINE_RHI_ASSERT(SharedTemplate) << "Internal RHI template database bug";
                    OutTemplateIfCreated = SharedTemplate;
                    return E_RHITemplateHashCodeLockStatus::CREATED;
                }
            }
        }
        return E_RHITemplateHashCodeLockStatus::FAILED;
    }
    void A_RHITemplateDatabase::UnlockHashCode(F_RHITemplateHashCode HashCode)
    {
        TF_ScopeLock<F_SpinLock> _(_SpinLock);
        auto It = _LockedHashCodes.find(HashCode);
        ABYTEK_ENGINE_RHI_ASSERT(It != _LockedHashCodes.end()) << "Hash code " << HashCode << " was not locked";
        {
            _LockedHashCodes.erase(It);
        }
    }

    TS_Valid<A_RHITemplate> A_RHITemplateDatabase::GetTemplate(F_RHITemplateHashCode HashCode)
    {
        TF_ScopeLock<F_SpinLock> _(_SpinLock);
        auto It = _Templates.find(HashCode);
        ABYTEK_ENGINE_RHI_ASSERT(It != _Templates.end()) << "Not found template of hash code " << HashCode;
        return ShareObject(It->second);
    }
    TS<A_RHITemplate> A_RHITemplateDatabase::FindTemplate(F_RHITemplateHashCode HashCode)
    {
        TF_ScopeLock<F_SpinLock> _(_SpinLock);
        auto It = _Templates.find(HashCode);
        if (It == _Templates.end())
        {
            return {};
        }
        return ShareObject(It->second);
    }

    B8 A_RHITemplateDatabase::HasTemplate(F_RHITemplateHashCode HashCode)
    {
        TF_ScopeLock<F_SpinLock> _(_SpinLock);
        auto It = _Templates.find(HashCode);
        return (It != _Templates.end());
    }

    TF_Vector<TS<A_RHITemplate>> A_RHITemplateDatabase::GatherTemplates()
    {
        TF_ScopeLock<F_SpinLock> _(_SpinLock);
        TF_Vector<TS<A_RHITemplate>> Result;
        Result.reserve(_Templates.size());
        for (const auto& Pair : _Templates)
        {
            if (auto Template = ShareObject(Pair.second))
            {
                Result.push_back(Template);
            }
        }
        return ABYTEK_MOVE(Result);
    }
    TF_Vector<F_RHITemplateHashCode> A_RHITemplateDatabase::GatherTemplateHashCodes()
    {
        TF_ScopeLock<F_SpinLock> _(_SpinLock);
        TF_Vector<F_RHITemplateHashCode> Result;
        Result.reserve(_Templates.size());
        for (const auto& Pair : _Templates)
        {
            Result.push_back(Pair.first);
        }
        return ABYTEK_MOVE(Result);
    }
}
