#pragma once

#include "Abytek/Base.Object.prerequisites.pch.hpp"
#include "Abytek/ObjectSmartPointerTemplates.hpp"


namespace Abytek::ObjectUtilityTemplates
{
    template<class __F_PassedObject, class __F_Allocator, class __F_ObjectConfig>
    class TF_UniqueObjectSet
    {
    public:
        using F_PassedObject = __F_PassedObject;
        using F_Allocator = __F_Allocator;
        using F_ObjectConfig = __F_ObjectConfig;

    public:
        using U_Object = ObjectSmartPointerTemplates::TU<F_PassedObject, F_Allocator, F_ObjectConfig>;
        using W_Object = ObjectSmartPointerTemplates::TW<F_PassedObject, F_ObjectConfig>;
        using W_ValidObject = ObjectSmartPointerTemplates::TW_Valid<F_PassedObject, F_ObjectConfig>;
        using F_ObjectSet = ContainerTemplates::TF_Set<U_Object>;
        
        static_assert(sizeof(U_Object) == sizeof(W_Object));
        
    public:
        using F_Iterator = typename F_ObjectSet::iterator;
        using F_ConstIterator = typename F_ObjectSet::const_iterator;
        using F_ReverseIterator = TF_ReverseIterator<F_Iterator>;
        using F_ConstReverseIterator = TF_ReverseIterator<F_ConstIterator>;

    private:
        F_ObjectSet _ObjectSet;

    public:
        ABYTEK_FORCE_INLINE const auto& GetObjectSet() const noexcept { return _ObjectSet; }
        ABYTEK_FORCE_INLINE auto& InjectObjectSet() noexcept { return _ObjectSet; }
        ABYTEK_FORCE_INLINE auto GetSize() const noexcept { return _ObjectSet.size(); }

    public:
        TF_UniqueObjectSet() = default;
        ~TF_UniqueObjectSet() = default;
        
        TF_UniqueObjectSet(const TF_UniqueObjectSet& X) = default;
        TF_UniqueObjectSet& operator = (const TF_UniqueObjectSet& X) = default;
        TF_UniqueObjectSet(TF_UniqueObjectSet&& X) = default;
        TF_UniqueObjectSet& operator = (TF_UniqueObjectSet&& X) = default;
        
    public:
        void Reset()
        {
            _ObjectSet.clear();
        }
        auto Add(U_Object&& X)
        {
            _ObjectSet.insert(boost::move(X));
        }
        auto Find(const W_Object& X) const
        {
            return _ObjectSet.find(*((const U_Object*)&X));
        }
        auto Find(const W_Object& X)
        {
            return _ObjectSet.find(*((const U_Object*)&X));
        }
        void Remove(const F_ConstIterator& Iterator)
        {
            _ObjectSet.erase(Iterator);
        }
        void Remove(const F_Iterator& Iterator)
        {
            _ObjectSet.erase(Iterator);
        }

    public:
        ABYTEK_FORCE_INLINE F_Iterator GetBegin() noexcept
        {
            return _ObjectSet.begin();
        }
        ABYTEK_FORCE_INLINE F_ConstIterator GetBegin() const noexcept
        {
            return _ObjectSet.begin();
        }
        ABYTEK_FORCE_INLINE F_Iterator GetEnd() noexcept
        {
            return _ObjectSet.end();
        }
        ABYTEK_FORCE_INLINE F_ConstIterator GetEnd() const noexcept
        {
            return _ObjectSet.end();
        }

    public:
        ABYTEK_FORCE_INLINE F_Iterator begin() noexcept
        {
            return GetBegin();
        }
        ABYTEK_FORCE_INLINE F_ConstIterator begin() const noexcept
        {
            return GetBegin();
        }
        ABYTEK_FORCE_INLINE F_ConstIterator cbegin() const noexcept
        {
            return GetBegin();
        }
        ABYTEK_FORCE_INLINE F_Iterator end() noexcept
        {
            return GetEnd();
        }
        ABYTEK_FORCE_INLINE F_ConstIterator end() const noexcept
        {
            return GetEnd();
        }
        ABYTEK_FORCE_INLINE F_ConstIterator cend() const noexcept
        {
            return GetEnd();
        }

    public:
        ABYTEK_FORCE_INLINE F_ReverseIterator GetReverseBegin() noexcept
        {
            return MakeReverseIterator(GetEnd());
        }
        ABYTEK_FORCE_INLINE F_ConstReverseIterator GetReverseBegin() const noexcept
        {
            return MakeReverseIterator(GetEnd());
        }
        ABYTEK_FORCE_INLINE F_ReverseIterator GetReverseEnd() noexcept
        {
            return MakeReverseIterator(GetBegin());
        }
        ABYTEK_FORCE_INLINE F_ConstReverseIterator GetReverseEnd() const noexcept
        {
            return MakeReverseIterator(GetBegin());
        }

    public:
        ABYTEK_FORCE_INLINE F_ReverseIterator rbegin() noexcept
        {
            return GetReverseBegin();
        }
        ABYTEK_FORCE_INLINE F_ConstReverseIterator rbegin() const noexcept
        {
            return GetReverseBegin();
        }
        ABYTEK_FORCE_INLINE F_ConstReverseIterator crbegin() const noexcept
        {
            return GetReverseBegin();
        }
        ABYTEK_FORCE_INLINE F_ReverseIterator rend() noexcept
        {
            return GetReverseEnd();
        }
        ABYTEK_FORCE_INLINE F_ConstReverseIterator rend() const noexcept
        {
            return GetReverseEnd();
        }
        ABYTEK_FORCE_INLINE F_ConstReverseIterator crend() const noexcept
        {
            return GetReverseEnd();
        }
    };
}
