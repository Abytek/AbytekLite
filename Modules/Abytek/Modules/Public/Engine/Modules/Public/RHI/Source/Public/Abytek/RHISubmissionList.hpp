#pragma once

#include "Abytek/RHISubmissionItem.hpp"
#include "Abytek/RHISubmissionItemContainerInterface.hpp"


namespace Abytek
{
    enum class E_RHISubmissionListOrder
    {
        SEQUENTIAL,
        CONCURRENT,

        DEFAULT = SEQUENTIAL
    };
    
    class ABYTEK_ENGINE_RHI_API A_RHISubmissionList : public A_RHISubmissionItem, public I_RHISubmissionItemContainer
    {
    public:
        using F_Items = TF_SmallVector<TS<A_RHISubmissionItem>, 64>;
        
    public:
        using F_Iterator = typename F_Items::iterator;
        using F_ConstIterator = typename F_Items::const_iterator;
        using F_ReverseIterator = TF_ReverseIterator<F_Iterator>;
        using F_ConstReverseIterator = TF_ReverseIterator<F_ConstIterator>;
        using F_ItemRef = TS<A_RHISubmissionItem>&;
        using F_ConstItemRef = const TS<A_RHISubmissionItem>&;
        
    private:
        TW<A_RHIProcess> _Process;
        E_RHISubmissionListOrder _Order = E_RHISubmissionListOrder::DEFAULT;
        F_Items _Items;

    public:
        TW_Valid<A_RHIProcess> GetProcess() const override
        {
            return _Process;
        }
        ABYTEK_FORCE_INLINE auto GetOrder() const noexcept
        {
            return _Order;
        }
        ABYTEK_FORCE_INLINE auto& InjectOrder() noexcept
        {
            return _Order;
        }
        ABYTEK_FORCE_INLINE const auto& GetItems() const noexcept
        {
            return _Items;
        }
        ABYTEK_FORCE_INLINE auto& InjectItems() noexcept
        {
            return _Items;
        }
        ABYTEK_FORCE_INLINE Sz GetSize() const noexcept
        {
            return _Items.size();
        }
        ABYTEK_FORCE_INLINE Sz GetCapacity() const noexcept
        {
            return _Items.capacity();
        }
        
    public:
        ABYTEK_RA_DECLARE_OBJECT_CREATABLE(A_RHISubmissionList);
        virtual void Build(const TW_Valid<A_RHIProcess>& Process, E_RHISubmissionListOrder Order = E_RHISubmissionListOrder::DEFAULT);
        virtual void Release() override;
        
    public:
        TS<A_RHISubmissionList> ForwardOrAddNew(E_RHISubmissionListOrder TargetOrder = E_RHISubmissionListOrder::DEFAULT);
        
    protected:
        void OnAddFrontSubmissionList(const TS_Valid<A_RHISubmissionList>& SubmissionList) override
        {
            _Items.insert(
                _Items.begin(),
                SubmissionList
            );
        }
        void OnAddBackSubmissionItem(const TS_Valid<A_RHISubmissionItem>& SubmissionItem) override
        {
            _Items.push_back({ SubmissionItem });
        }

    public:
        ABYTEK_FORCE_INLINE const auto& operator [] (Sz Index) const noexcept
        {
            return _Items[Index];
        }
        ABYTEK_FORCE_INLINE auto& operator [] (Sz Index) noexcept
        {
            return _Items[Index];
        }
        ABYTEK_FORCE_INLINE void Add(const TS_Valid<A_RHISubmissionItem>& Item) noexcept
        {
            AddSubmissionItem(Item);
        }
        template<typename __F_RHISubmissionItem, typename... __F_Args>
        ABYTEK_FORCE_INLINE void AddNew(__F_Args&&... Args) noexcept
        {
            auto Item = RHIPopSubmissionItem<__F_RHISubmissionItem>(ABYTEK_FORWARD(Args)...);
            Add(Item);
        }
        ABYTEK_FORCE_INLINE void Clear() noexcept
        {
            _Items.clear();
        }
        ABYTEK_FORCE_INLINE void Reset() noexcept
        {
            _Items = {};
        }

    public:
        ABYTEK_FORCE_INLINE F_Iterator GetBegin() noexcept
        {
            return _Items.begin();
        }
        ABYTEK_FORCE_INLINE F_ConstIterator GetBegin() const noexcept
        {
            return _Items.begin();
        }
        ABYTEK_FORCE_INLINE F_Iterator GetEnd() noexcept
        {
            return _Items.end();
        }
        ABYTEK_FORCE_INLINE F_ConstIterator GetEnd() const noexcept
        {
            return _Items.end();
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
