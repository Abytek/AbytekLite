#include "Abytek/ReflectionSession.hpp"
#include "Abytek/ReflectionContext.hpp"


namespace Abytek
{
    F_ReflectionSession::F_ReflectionSession(const F_GBMObjectConfig& GBMConfig, const F_ReflectionSessionBuildParams& BuildParams) :
        A_ReflectionItem(GBMConfig, BuildParams)
    {
        GBMMarkAsRoot();
        GetContext()->_RegisterSession(ABYTEK_WTHIS());
    }
    F_ReflectionSession::~F_ReflectionSession()
    {
        GetContext()->_UnregisterSession(ABYTEK_WTHIS());
    }

    B8 F_ReflectionSession::HasReferencedType(const TW_Valid<F_ReflectionType>& Type) const
    {
        return _ReferencedTypes.find(Type) != _ReferencedTypes.end();
    }
    B8 F_ReflectionSession::HasReferencedType(const F_Name& Canonical) const
    {
        return _CanonicalToReferencedTypeMap.find(Canonical) != _CanonicalToReferencedTypeMap.end();
    }
    B8 F_ReflectionSession::HasReferencedType(F_GeneralTypeHashCode HashCode) const
    {
        return _HashCodeToReferencedTypeMap.find(HashCode) != _HashCodeToReferencedTypeMap.end();
    }
    TW_Valid<F_ReflectionType> F_ReflectionSession::GetReferencedType(const F_Name& Canonical) const
    {
        ABYTEK_BASE_REFLECTION_ASSERT(HasReferencedType(Canonical)) << "Not found referenced type with canonical: " << Canonical;
        return _CanonicalToReferencedTypeMap.find(Canonical)->second;
    }
    TW_Valid<F_ReflectionType> F_ReflectionSession::GetReferencedType(F_GeneralTypeHashCode HashCode) const
    {
        ABYTEK_BASE_REFLECTION_ASSERT(HasReferencedType(HashCode)) << "Not found referenced type with hash code: " << HashCode;
        return _HashCodeToReferencedTypeMap.find(HashCode)->second;
    }
    TW<F_ReflectionType> F_ReflectionSession::FindReferencedType(const F_Name& Canonical) const
    {
        auto It = _CanonicalToReferencedTypeMap.find(Canonical);
        if (It == _CanonicalToReferencedTypeMap.end())
        {
            return {};
        }
        return It->second;
    }
    TW<F_ReflectionType> F_ReflectionSession::FindReferencedType(F_GeneralTypeHashCode HashCode) const
    {
        auto It = _HashCodeToReferencedTypeMap.find(HashCode);
        if (It == _HashCodeToReferencedTypeMap.end())
        {
            return {};
        }
        return It->second;
    }
    TW_Valid<F_ReflectionType> F_ReflectionSession::ReferenceType(const TW_Valid<F_ReflectionType>& Type)
    {
        if (HasReferencedType(Type))
        {
            return Type;
        }
        _ReferencedTypes.insert(Type);
        if (auto Canonical = Type->GetCanonical())
        {
            _CanonicalToReferencedTypeMap.insert({Canonical, Type}); 
        }
        _HashCodeToReferencedTypeMap.insert({Type->GetHashCode(), Type});
        GBMEnsureReference(Type);
        return Type;
    }
    TW<F_ReflectionType> F_ReflectionSession::UnreferenceType(const TW_Valid<F_ReflectionType>& Type)
    {
        _ReferencedTypes.erase(_ReferencedTypes.find(Type));
        if (auto Canonical = Type->GetCanonical())
        {
            _CanonicalToReferencedTypeMap.erase(_CanonicalToReferencedTypeMap.find(Canonical));
        }
        _HashCodeToReferencedTypeMap.erase(_HashCodeToReferencedTypeMap.find(Type->GetHashCode()));
        GBMRemoveReference(Type);
        return Type;
    }
    TW<F_ReflectionType> F_ReflectionSession::UnreferenceType(const F_Name& Canonical)
    {
        ABYTEK_BASE_REFLECTION_ASSERT(HasReferencedType(Canonical));
        auto Type = GetReferencedType(Canonical);
        _ReferencedTypes.erase(_ReferencedTypes.find(Type));
        _CanonicalToReferencedTypeMap.erase(_CanonicalToReferencedTypeMap.find(Canonical));
        _HashCodeToReferencedTypeMap.erase(_HashCodeToReferencedTypeMap.find(Type->GetHashCode()));
        GBMRemoveReference(Type);
        return Type;
    }
    TW<F_ReflectionType> F_ReflectionSession::UnreferenceType(F_GeneralTypeHashCode HashCode)
    {
        ABYTEK_BASE_REFLECTION_ASSERT(HasReferencedType(HashCode));
        auto Type = GetReferencedType(HashCode);
        _ReferencedTypes.erase(_ReferencedTypes.find(Type));
        if (auto Canonical = Type->GetCanonical())
        {
            _CanonicalToReferencedTypeMap.erase(_CanonicalToReferencedTypeMap.find(Canonical));
        }
        _HashCodeToReferencedTypeMap.erase(_HashCodeToReferencedTypeMap.find(HashCode));
        GBMRemoveReference(Type);
        return Type;
    }

    void F_ReflectionSession::Begin()
    {
        ABYTEK_BASE_REFLECTION_ASSERT(!GetContext()->GetCurrentSession()) << "Cannot execute multiple session simultaneously";
        GetContext()->_SetCurrentSession(ABYTEK_WTHIS());
    }
    void F_ReflectionSession::End()
    {
        ABYTEK_BASE_REFLECTION_ASSERT(GetContext()->GetCurrentSession() == ABYTEK_WTHIS()) << "Sessions mismatch";
        {
            ReflectionCommands::F_OnRegisterLinkedTypes Command;
            while (Queues.OnRegisterLinkedTypes.TryPop(Command))
            {
                Command();
            }
        }
        {
            ReflectionCommands::F_OnPrepare Command;
            while (Queues.OnPrepare.TryPop(Command))
            {
                Command();
            }
        }
        {
            ReflectionCommands::F_OnReflectCanonicals Command;
            while (Queues.OnReflectCanonicals.TryPop(Command))
            {
                Command();
            }
        }
        {
            ReflectionCommands::F_OnReflectMembers Command;
            while (Queues.OnReflectMembers.TryPop(Command))
            {
                Command();
            }
        }
        {
            ReflectionCommands::F_OnReflectTypes Command;
            while (Queues.OnReflectTypes.TryPop(Command))
            {
                Command();
            }
        }
        GetContext()->_SetCurrentSession({});
        GetContext()->Flush();
    }
}
