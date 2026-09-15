#pragma once

#include "Abytek/ReflectionCommon.hpp"


namespace Abytek
{
    struct F_ReflectionSessionCreationParams;
    
    class ABYTEK_BASE_REFLECTION_API F_ReflectionContext final : public A_Object
    {
    public:
        friend class A_ReflectionItem;
        friend class F_ReflectionSession;
        friend class F_ReflectionType;
        friend class F_ReflectionAlias;
        
    public:
        static TW<F_ReflectionContext> _Global;
        static ABYTEK_FORCE_INLINE const auto& GetGlobal() noexcept { return _Global; }
        static ABYTEK_FORCE_INLINE auto SetGlobal(const TW<F_ReflectionContext>& Value) noexcept
        {
            auto Result = _Global;
            _Global = Value;
            return Result;
        }
        
    private:
        TU<F_GBMObjectContainer> _ItemContainer;
        
        ContainerTemplates::TF_Map<F_Name, TW<A_ReflectionItem>> _CanonicalMap;
        ContainerTemplates::TF_Set<TW<F_ReflectionSession>> _Sessions;
        ContainerTemplates::TF_Map<F_GeneralTypeHashCode, TW<F_ReflectionType>> _TypeMap;
        ContainerTemplates::TF_Map<F_Name, TW<F_ReflectionAlias>> _AliasMap;
        
        TW<F_ReflectionSession> _CoreSession;
        TW<F_ReflectionSession> _CurrentSession;

    public:
        ABYTEK_FORCE_INLINE auto GetItemContainer() const noexcept { return _ItemContainer.Weak(); }
        
        ABYTEK_FORCE_INLINE const auto& GetCanonicalMap() const noexcept { return _CanonicalMap; }
        ABYTEK_FORCE_INLINE const auto& GetSessions() const noexcept { return _Sessions; }
        ABYTEK_FORCE_INLINE const auto& GetTypeMap() const noexcept { return _TypeMap; }
        ABYTEK_FORCE_INLINE const auto& GetAliasMap() const noexcept { return _AliasMap; }
    
        ABYTEK_FORCE_INLINE const auto& GetCoreSession() const noexcept { return _CoreSession; }
        ABYTEK_FORCE_INLINE const auto& GetCurrentSession() const noexcept { return _CurrentSession; }
        
    public:
        F_ReflectionContext();
        ~F_ReflectionContext() override;
        
    private:
        void _RegisterCanonical(const F_Name& Canonical, const TW_Valid<A_ReflectionItem>& Item);
        void _UnregisterCanonical(const F_Name& Canonical);
        
    private:
        void _RegisterSession(const TW_Valid<F_ReflectionSession>& Session);
        void _UnregisterSession(const TW_Valid<F_ReflectionSession>& Session);
        void _SetCurrentSession(const TW<F_ReflectionSession>& Session);
        
    private:
        void _RegisterType(const TW_Valid<F_ReflectionType>& Type);
        void _UnregisterType(const TW_Valid<F_ReflectionType>& Type);
        
    private:
        void _RegisterAlias(const TW_Valid<F_ReflectionAlias>& Alias);
        void _UnregisterAlias(const TW_Valid<F_ReflectionAlias>& Alias);
        
    public:
        TW<A_ReflectionItem> SearchItem(const F_Name& Key) const;
        
    public:
        B8 HasCanonical(const F_Name& Canonical) const noexcept;
        TW_Valid<A_ReflectionItem> GetCanonical(const F_Name& Canonical) const noexcept;
        
    public:
        TW_Valid<F_ReflectionSession> CreateSession(const F_ReflectionSessionCreationParams& CreationParams);
        
    public:
        B8 HasType(F_GeneralTypeHashCode HashCode) const noexcept;
        template<typename __F>
        B8 HasType() const noexcept
        {
            return HasType(Abytek::H_GeneralTypeHashCode::StaticValue<__F>);
        }
        TW_Valid<F_ReflectionType> GetType(F_GeneralTypeHashCode HashCode) const noexcept;
        template<typename __F>
        TW_Valid<F_ReflectionType> GetType() const noexcept
        {
            return GetType(Abytek::H_GeneralTypeHashCode::StaticValue<__F>);
        }
        
    public:
        B8 HasAlias(const F_Name& Key) const noexcept;
        TW_Valid<F_ReflectionAlias> GetAlias(const F_Name& Key) const noexcept;
        
    public:
        template<typename __F_ReflectionItem, typename... __F_Args>
        TW_Valid<__F_ReflectionItem> CreateItem(__F_Args&&... Args)
        {
            auto Item = _ItemContainer->CreateObject<__F_ReflectionItem>(
                {},
                ABYTEK_FORWARD(Args)...
            );
            return Item;
        }
        
    public:
        void Flush();
        
    public:
        void ForEachItem(const TF_Function<B8(const TW_Valid<A_ReflectionItem>&)>& Callback);
        void ForEachType(const TF_Function<B8(const TW_Valid<F_ReflectionType>&)>& Callback);
        void ForEachTypeDerivedFrom(
            const TW_Valid<F_ReflectionType>& BaseType, 
            const TF_Function<B8(const TW_Valid<F_ReflectionType>&)>& Callback
        );
    };

    ABYTEK_FORCE_INLINE TW<F_ReflectionType> GlobalSearchReflectionType(U64 InTypeHashCode)
    {
        auto GlobalReflectionContext = F_ReflectionContext::GetGlobal();
        if (GlobalReflectionContext->HasType(InTypeHashCode))
        {
            return GlobalReflectionContext->GetType(InTypeHashCode);
        }
        return {};
    }
    template<typename __F>
    ABYTEK_FORCE_INLINE TW<F_ReflectionType> GlobalSearchReflectionType()
    {
        auto GlobalReflectionContext = F_ReflectionContext::GetGlobal();
        if (GlobalReflectionContext->HasType<__F>())
        {
            return GlobalReflectionContext->GetType<__F>();
        }
        return {};
    }
    ABYTEK_FORCE_INLINE TW<A_ReflectionItem> GlobalSearchReflectionCanonical(const F_Name& Canonical)
    {
        auto GlobalReflectionContext = F_ReflectionContext::GetGlobal();
        if (GlobalReflectionContext->HasCanonical(Canonical))
        {
            return GlobalReflectionContext->GetCanonical(Canonical);
        }
        return {};
    }
    template<typename __F_ReflectionItem = A_ReflectionItem>
    ABYTEK_FORCE_INLINE TW<__F_ReflectionItem> GlobalSearchReflectionItem(const F_Name& Key)
    {
        auto GlobalReflectionContext = F_ReflectionContext::GetGlobal();
        if (auto Item = GlobalReflectionContext->SearchItem(Key))
        {
            TW<__F_ReflectionItem> Result;
            if (Item.TryDynamicCast<__F_ReflectionItem>(Result))
            {
                return Result;
            }
        }
        return {};
    }
}