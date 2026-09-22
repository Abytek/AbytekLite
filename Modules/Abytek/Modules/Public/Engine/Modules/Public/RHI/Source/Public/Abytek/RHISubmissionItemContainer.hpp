#pragma once

#include "Abytek/RHISubmissionItem.hpp"
#include "Abytek/RHISubmissionItemContainerInterface.hpp"


namespace Abytek
{
    class ABYTEK_ENGINE_RHI_API A_RHISubmissionItemContainer : public A_RHISubmissionItem, public I_RHISubmissionItemContainer
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
        F_Items _Items;
#ifdef ABYTEK_ENGINE_RHI_ENABLE_CAPTURE
        TF_Vector<F_RHICaptureEventState> _AdditionalStackCaptureEventStates;
#endif

    public:
        TW_Valid<A_RHIProcess> GetProcess() const override
        {
            return _Process;
        }
        ABYTEK_FORCE_INLINE const auto& GetItems() const noexcept
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
#ifdef ABYTEK_ENGINE_RHI_ENABLE_CAPTURE
        ABYTEK_FORCE_INLINE const auto& GetAdditionalStackCaptureEventStates() const noexcept
        {
            return _AdditionalStackCaptureEventStates;
        }
        template<typename... __F_Args>
        ABYTEK_FORCE_INLINE void PushAdditionalStackCaptureEventStates(__F_Args&&... Args) noexcept
        {
            _AdditionalStackCaptureEventStates.push_back(
                F_RHICaptureEventState::Make(ABYTEK_FORWARD(Args)...)    
            );
        }
        ABYTEK_FORCE_INLINE void PopAdditionalStackCaptureEventStates() noexcept
        {
            _AdditionalStackCaptureEventStates.pop_back();
        }
#endif
        
    protected:
        void BuildSubmissionItemContainer(const TW_Valid<A_RHIProcess>& Process);
        
    public:
        ABYTEK_RA_DECLARE_OBJECT(A_RHISubmissionItemContainer);
        void Release() override;
        
    protected:
        void OnAddFrontSubmissionList(const TS<A_RHISubmissionList>& SubmissionList) override;
        void OnAddBackSubmissionItem(const TS<A_RHISubmissionItem>& SubmissionItem) override;
        
    protected:
        TS<A_RHISubmissionList> OnAddChild(E_RHISubmissionListOrder Order, const F_DebugName& DebugName) override;

    public:
        ABYTEK_FORCE_INLINE const auto& operator [] (Sz Index) const noexcept
        {
            return _Items[Index];
        }
        ABYTEK_FORCE_INLINE auto& operator [] (Sz Index) noexcept
        {
            return _Items[Index];
        }
        ABYTEK_FORCE_INLINE void Add(const TS<A_RHISubmissionItem>& Item) noexcept
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
    
    struct ABYTEK_ENGINE_RHI_API F_RHICaptureEventScope
    {
        TS<A_RHISubmissionItemContainer> SubmissionItemContainer;
        template<typename... __F_Args>
        F_RHICaptureEventScope(const TS<A_RHISubmissionItemContainer>& InSubmissionItemContainer, __F_Args&&... InArgs) :
            SubmissionItemContainer(InSubmissionItemContainer)
        {
            SubmissionItemContainer->PushAdditionalStackCaptureEventStates(ABYTEK_FORWARD(InArgs)...);
        }
        ~F_RHICaptureEventScope()
        {
            SubmissionItemContainer->PopAdditionalStackCaptureEventStates();
        }
    };
}

#ifdef ABYTEK_ENGINE_RHI_ENABLE_CAPTURE
#define ABYTEK_RHI_CAPTURE_EVENT_SCOPE(...) \
            Abytek::F_RHICaptureEventScope ABYTEK_GLUE(___Abytek_RHICaptureEventScope_, ABYTEK_LINE)(__VA_ARGS__)
#else
#define ABYTEK_RHI_CAPTURE_EVENT_SCOPE(...)
#endif