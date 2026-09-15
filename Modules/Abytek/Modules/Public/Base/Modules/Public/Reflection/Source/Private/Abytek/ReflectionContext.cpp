#include "Abytek/ReflectionContext.hpp"


namespace Abytek
{
    TW<F_ReflectionContext> F_ReflectionContext::_Global;
    
    F_ReflectionContext::F_ReflectionContext()
    {
        _ItemContainer = TU<F_GBMObjectContainer>()();
        
        {
            _CoreSession = CreateSession({});
            _CoreSession->Begin();
            
            _CoreSession->RegisterType<U8>();
            _CoreSession->RegisterType<U16>();
            _CoreSession->RegisterType<U32>();
            _CoreSession->RegisterType<U64>();
        
            _CoreSession->RegisterType<I8>();
            _CoreSession->RegisterType<I16>();
            _CoreSession->RegisterType<I32>();
            _CoreSession->RegisterType<I64>();
            
            _CoreSession->RegisterType<F32>();
            _CoreSession->RegisterType<F64>();
        
            _CoreSession->RegisterType<B8>();
        
            _CoreSession->RegisterType<F_Char>();
#ifdef ABYTEK_ENABLE_WIDE_CHAR_TEXT
            _CoreSession->RegisterType<F_TextChar>();
#endif
            
            if constexpr (!std::is_same_v<TF_UInt<sizeof(Sz)>, Sz>)
                _CoreSession->RegisterType<Sz>();
            if constexpr (!std::is_same_v<TF_Int<sizeof(PDiff)>, PDiff>)
                _CoreSession->RegisterType<PDiff>();
        
            _CoreSession->RegisterType<A_ReflectionItem>();
            _CoreSession->RegisterType<F_ReflectionType>();
            _CoreSession->RegisterType<F_ReflectionAlias>();
            
            _CoreSession->End();
            
            Flush();
        }
    }
    F_ReflectionContext::~F_ReflectionContext()
    {
        _ItemContainer = {};
    }

    void F_ReflectionContext::_RegisterCanonical(const F_Name& Canonical, const TW_Valid<A_ReflectionItem>& Item)
    {
        ABYTEK_BASE_REFLECTION_ASSERT(!HasCanonical(Canonical)) << "type canonical was already registered: " << Canonical;
        Item->_Canonical = Canonical;
        _CanonicalMap[Canonical] = Item;
    }
    void F_ReflectionContext::_UnregisterCanonical(const F_Name& Canonical)
    {
        ABYTEK_BASE_REFLECTION_ASSERT(HasCanonical(Canonical)) << "type canonical was not registered: " << Canonical;
        _CanonicalMap.erase(_CanonicalMap.find(Canonical));
    }

    void F_ReflectionContext::_RegisterSession(const TW_Valid<F_ReflectionSession>& Session)
    {
        _Sessions.insert(Session);
    }
    void F_ReflectionContext::_UnregisterSession(const TW_Valid<F_ReflectionSession>& Session)
    {
        _Sessions.erase(_Sessions.find(Session));
    }
    void F_ReflectionContext::_SetCurrentSession(const TW<F_ReflectionSession>& Session)
    {
        _CurrentSession = Session;
    }

    void F_ReflectionContext::_RegisterType(const TW_Valid<F_ReflectionType>& Type)
    {
        ABYTEK_BASE_REFLECTION_ASSERT(!HasType(Type->Desc.HashCode)) << "type info was already registered: " << Type->Desc.HashCode;
        _TypeMap.insert({ Type->Desc.HashCode, Type });
    }
    void F_ReflectionContext::_UnregisterType(const TW_Valid<F_ReflectionType>& Type)
    {
        ABYTEK_BASE_REFLECTION_ASSERT(HasType(Type->Desc.HashCode)) << "type info was not registered: " << Type->Desc.HashCode;
        _TypeMap.erase(_TypeMap.find(Type->Desc.HashCode));
    }

    void F_ReflectionContext::_RegisterAlias(const TW_Valid<F_ReflectionAlias>& Alias)
    {
        ABYTEK_BASE_REFLECTION_ASSERT(!HasAlias(Alias->GetKey())) << "Alias was already registered: " << Alias->GetKey();
        _AliasMap.insert({ Alias->GetKey(), Alias });
    }
    void F_ReflectionContext::_UnregisterAlias(const TW_Valid<F_ReflectionAlias>& Alias)
    {
        ABYTEK_BASE_REFLECTION_ASSERT(HasAlias(Alias->GetKey())) << "Alias was not registered: " << Alias->GetKey();
        _AliasMap.erase(_AliasMap.find(Alias->GetKey()));
    }

    TW<A_ReflectionItem> F_ReflectionContext::SearchItem(const F_Name& Key) const
    {
        if (HasCanonical(Key))
        {
            return GetCanonical(Key);
        }
        if (HasAlias(Key))
        {
            return SearchItem(GetAlias(Key)->GetValue());
        }
        return {};
    }

    B8 F_ReflectionContext::HasCanonical(const F_Name& Canonical) const noexcept
    {
        return (_CanonicalMap.find(Canonical) != _CanonicalMap.end());
    }
    TW_Valid<A_ReflectionItem> F_ReflectionContext::GetCanonical(const F_Name& Canonical) const noexcept
    {
        ABYTEK_BASE_REFLECTION_ASSERT(HasCanonical(Canonical)) << "type info canonical was not registered";
        return _CanonicalMap.find(Canonical)->second;
    }

    TW_Valid<F_ReflectionSession> F_ReflectionContext::CreateSession(const F_ReflectionSessionCreationParams& CreationParams)
    {
        F_ReflectionSessionBuildParams BuildParams;
        BuildParams.Context = ABYTEK_WTHIS();
        static_cast<F_ReflectionSessionCreationParams&>(BuildParams) = CreationParams;
        auto Session = CreateItem<F_ReflectionSession>(BuildParams);
        return Session;
    }

    B8 F_ReflectionContext::HasType(U64 HashCode) const noexcept
    {
        return (_TypeMap.find(HashCode) != _TypeMap.end());
    }
    TW_Valid<F_ReflectionType> F_ReflectionContext::GetType(U64 HashCode) const noexcept
    {
        ABYTEK_BASE_REFLECTION_ASSERT(HasType(HashCode)) << "type info was not registered: " << HashCode;
        return _TypeMap.find(HashCode)->second.Weak();
    }

    B8 F_ReflectionContext::HasAlias(const F_Name& Key) const noexcept
    {
        return (_AliasMap.find(Key) != _AliasMap.end());
    }
    TW_Valid<F_ReflectionAlias> F_ReflectionContext::GetAlias(const F_Name& Key) const noexcept
    {
        ABYTEK_BASE_REFLECTION_ASSERT(HasAlias(Key)) << "Alias was not registered: " << Key;
        return _AliasMap.find(Key)->second.Weak();
    }

    void F_ReflectionContext::Flush()
    {
        _ItemContainer->Flush();
    }

    void F_ReflectionContext::ForEachItem(const TF_Function<B8(const TW_Valid<A_ReflectionItem>&)>& Callback)
    {
        for (const auto& Item : *_ItemContainer)
        {
            if (!Callback(Item.FastCast<A_ReflectionItem>().Weak()))
            {
                break;
            }
        }
    }
    void F_ReflectionContext::ForEachType(const TF_Function<B8(const TW_Valid<F_ReflectionType>&)>& Callback)
    {
        for (const auto& [HashCode, Type] : _TypeMap)
        {
            if (!Callback(Type))
            {
                break;
            }
        }
    }
    void F_ReflectionContext::ForEachTypeDerivedFrom(
        const TW_Valid<F_ReflectionType>& BaseType,
        const TF_Function<B8(const TW_Valid<F_ReflectionType>&)>& Callback
    )
    {
        ForEachType(
            [&](const TW_Valid<F_ReflectionType>& Type)
            {
                if (Type->HasBase(BaseType))
                {
                    return Callback(Type);
                }
                return true;
            }
        );
    }
}
