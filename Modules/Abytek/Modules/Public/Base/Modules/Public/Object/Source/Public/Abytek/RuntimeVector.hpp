/**
 * @file RuntimeVector.hpp
 * @brief Implementation of runtime vector container
 * 
 * This file provides classes for handling vectors of items whose types may not be known
 * at compile time. It includes reference and iterator types for these vectors, as well
 * as the main vector implementation with runtime type support.
 */

#pragma once

#include "Abytek/Base.Object.prerequisites.pch.hpp"
#include "Abytek/RuntimeTools.hpp"


namespace Abytek
{
    template<B8 __IsConst>
    struct TF_RuntimeVectorItemRef;
    template<B8 __IsConst>
    struct TF_RuntimeVectorIterator;

    /**
     * @brief Reference to an item in a runtime vector
     * @tparam __IsConst Whether the reference is const or mutable
     */
    template<B8 __IsConst>
    struct TF_RuntimeVectorItemRef
    {
        static constexpr B8 IsConst = __IsConst;

        using F_Iterator = TF_RuntimeVectorIterator<IsConst>;
        
        /** Pointer to the referenced data */
        void* DataP = 0;
        
        /**
         * @brief Check if the reference is valid
         * @return True if the reference points to valid data
         */
        ABYTEK_FORCE_INLINE operator B8 () const noexcept
        {
            return (DataP != 0);
        }
        
        /**
         * @brief Compare with another reference for equality
         * @param X Reference to compare with
         * @return True if both references point to the same data
         */
        ABYTEK_FORCE_INLINE B8 operator == (const TF_RuntimeVectorItemRef& X) const noexcept
        {
            return (DataP == X.DataP);
        }
        
        /**
         * @brief Compare with another reference for inequality
         * @param X Reference to compare with
         * @return True if references point to different data
         */
        ABYTEK_FORCE_INLINE B8 operator != (const TF_RuntimeVectorItemRef& X) const noexcept
        {
            return (DataP != X.DataP);
        }
        
        /**
         * @brief Get the referenced data as a specific type
         * @tparam __F_Item Type to cast the data to
         * @return Reference to the data as the specified type
         */
        template<typename __F_Item>
        ABYTEK_FORCE_INLINE auto& Get() const noexcept
        {
            return *(
                (
                    std::conditional_t<
                        IsConst,
                        const __F_Item*,
                        __F_Item*
                    >
                ) DataP
            );
        }

        /**
         * @brief Convert to a non-const iterator
         * @return Iterator pointing to the same data
         */
        ABYTEK_FORCE_INLINE operator TF_RuntimeVectorIterator<!IsConst> () const noexcept
        {
            return { DataP };
        }
    };

    /**
     * @brief Iterator for runtime vectors
     * @tparam __IsConst Whether the iterator is const or mutable
     */
    template<B8 __IsConst>
    struct TF_RuntimeVectorIterator
    {
        static constexpr B8 IsConst = __IsConst;

        using F_ItemRef = TF_RuntimeVectorItemRef<IsConst>;
        
        /** Size of each item in bytes */
        U16 Size = 0;
        
        /** Pointer to the current item */
        void* DataP = 0;
        
        /**
         * @brief Check if the iterator is valid
         * @return True if the iterator points to valid data with known size
         */
        ABYTEK_FORCE_INLINE explicit operator B8 () const noexcept
        {
            return (
                (Size != 0)
                && (DataP != 0)
            );
        }
        
        /**
         * @brief Pre-increment operator
         * @return Reference to this iterator after incrementing
         */
        ABYTEK_FORCE_INLINE TF_RuntimeVectorIterator& operator ++ () noexcept
        {
            DataP = ((U8*)DataP) + (PDiff)(Size);
            return *this;
        }
        
        /**
         * @brief Pre-decrement operator
         * @return Reference to this iterator after decrementing
         */
        ABYTEK_FORCE_INLINE TF_RuntimeVectorIterator& operator -- () noexcept
        {
            DataP = ((U8*)DataP) - (PDiff)(Size);
            return *this;
        }
        
        ABYTEK_FORCE_INLINE TF_RuntimeVectorIterator& operator += (PDiff Num) noexcept
        {
            DataP = ((U8*)DataP) + (PDiff)(Size) * Num;
            return *this;
        }
        ABYTEK_FORCE_INLINE TF_RuntimeVectorIterator& operator -= (PDiff Num) noexcept 
        {
            DataP = ((U8*)DataP) - (PDiff)(Size) * Num;
            return *this;
        }
        
        ABYTEK_FORCE_INLINE TF_RuntimeVectorIterator operator + (PDiff Num) const noexcept
        {
            return {
                Size,
                ((U8*)DataP) + (PDiff)(Size) * Num
            };
        }
        ABYTEK_FORCE_INLINE TF_RuntimeVectorIterator operator - (PDiff Num) const noexcept
        {
            return {
                Size,
                ((U8*)DataP) - (PDiff)(Size) * Num
            };
        }
        
        friend ABYTEK_FORCE_INLINE PDiff operator + (const TF_RuntimeVectorIterator& A, const TF_RuntimeVectorIterator& B) noexcept
        {
            ABYTEK_BASE_OBJECT_ASSERT(A.Size == B.Size) << "Invalid addition";
            return (PDiff)(A.DataP) + (PDiff)(B.DataP);
        }
        friend ABYTEK_FORCE_INLINE PDiff operator - (const TF_RuntimeVectorIterator& A, const TF_RuntimeVectorIterator& B) noexcept
        {
            ABYTEK_BASE_OBJECT_ASSERT(A.Size == B.Size) << "Invalid subtraction";
            return (PDiff)(A.DataP) - (PDiff)(B.DataP);
        }
        
        ABYTEK_FORCE_INLINE B8 operator == (const TF_RuntimeVectorIterator& X) const noexcept
        {
            return (Size == X.Size) && (DataP == X.DataP);
        }
        ABYTEK_FORCE_INLINE B8 operator != (const TF_RuntimeVectorIterator& X) const noexcept
        {
            return (Size != X.Size) || (DataP != X.DataP);
        }
        
        /**
         * @brief Get the referenced data as a specific type
         * @tparam __F_Item Type to cast the data to
         * @return Reference to the data as the specified type
         */
        template<typename __F_Item>
        ABYTEK_FORCE_INLINE auto& Get() const noexcept
        {
            return *(
                (
                    std::conditional_t<
                        IsConst,
                        const __F_Item*,
                        __F_Item*
                    >
                ) DataP
            );
        }

        /**
         * @brief Dereference operator
         * @return Reference to the current item
         */
        ABYTEK_FORCE_INLINE F_ItemRef operator * () const noexcept
        {
            return { DataP };
        }
        
        /**
         * @brief Array subscript operator
         * @param Idx Index relative to the current position
         * @return Reference to the item at the specified index
         */
        ABYTEK_FORCE_INLINE F_ItemRef operator [] (PDiff Idx) const noexcept
        {
            return { static_cast<U8*>(DataP) + (PDiff)(Size) * Idx };
        }

        /**
         * @brief Convert to a non-const iterator
         * @return Non-const iterator pointing to the same data
         */
        ABYTEK_FORCE_INLINE operator TF_RuntimeVectorIterator<!IsConst> () const noexcept
        {
            return { Size, DataP };
        }
    };
}

namespace Abytek::ContainerTemplates
{
    template<class __F_Allocator>
    class TA_RuntimeVectorInstance;
    template<typename __F_Item, class __F_Allocator>
    class TF_RuntimeVectorInstance;
    
    /**
     * @brief Abstract base class for runtime vector instances
     * 
     * Provides a type-erased interface for vector operations on items
     * whose type may not be known at compile time.
     * 
     * @tparam __F_Allocator Allocator type to use for memory management
     */
    template<class __F_Allocator = WithRawAllocator::TF_DefaultAllocator<U8>>
    class TA_RuntimeVectorInstance
    {
    public:
        template<typename __F_Item2, class __F_Allocator2>
        friend class TF_RuntimeVectorInstance;
        
    public:
        using F_Allocator = __F_Allocator;

    public:
        using F_Iterator = TF_RuntimeVectorIterator<false>;
        using F_ConstIterator = TF_RuntimeVectorIterator<true>;
        using F_ReverseIterator = TF_ReverseIterator<F_Iterator>;
        using F_ConstReverseIterator = TF_ReverseIterator<F_ConstIterator>;
        using F_ItemRef = TF_RuntimeVectorItemRef<false>;
        using F_ConstItemRef = TF_RuntimeVectorItemRef<true>;

    protected:
        /** Size of each item in bytes */
        U16 _ItemSize = 0;
        
        /** Alignment requirement of each item */
        U16 _ItemAlignment = 0;
        
        /** Number of items in the vector */
        Sz _Size = 0;
        
        /** Current capacity of the vector */
        Sz _Capacity = 0;
        
        /** Iterator to the first item */
        F_Iterator _Begin;
        
        /** Iterator past the last item */
        F_Iterator _End;

    public:
        /**
         * @brief Get the size of each item in bytes
         * @return Size of each item in bytes
         */
        ABYTEK_FORCE_INLINE U16 GetItemSize() const noexcept
        {
            return _ItemSize;
        }
        
        /**
         * @brief Get the alignment requirement of each item
         * @return Alignment requirement in bytes
         */
        ABYTEK_FORCE_INLINE U16 GetItemAlignment() const noexcept
        {
            return _ItemAlignment;
        }
        
        /**
         * @brief Get the number of items in the vector
         * @return Number of items
         */
        ABYTEK_FORCE_INLINE Sz GetSize() const noexcept
        {
            return _Size;
        }
        
        /**
         * @brief Get the current capacity of the vector
         * @return Capacity in number of items
         */
        ABYTEK_FORCE_INLINE Sz GetCapacity() const noexcept
        {
            return _Capacity;
        }
        
    protected:
        /**
         * @brief Constructor
         * @param ItemSize Size of each item in bytes
         * @param ItemAlignment Alignment requirement of each item in bytes
         */
        TA_RuntimeVectorInstance(U16 ItemSize, U16 ItemAlignment) :
            _ItemSize(ItemSize),
            _ItemAlignment(ItemAlignment)
        {}

    public:
        virtual ~TA_RuntimeVectorInstance() = default;

    public:
        /**
         * @brief Create a clone of this vector
         * @param Allocator Allocator to use for the new vector
         * @return Pointer to the cloned vector
         */
        virtual TA_RuntimeVectorInstance* Clone(F_Allocator& Allocator) = 0;

    public:
        /**
         * @brief Resize the vector
         * @param Size New size in number of items
         */
        virtual void Resize(Sz Size) = 0;
        
        /**
         * @brief Reserve capacity for future items
         * @param Capacity New capacity in number of items
         */
        virtual void Reserve(Sz Capacity) = 0;
        
        /**
         * @brief Reduce capacity to match size
         */
        virtual void ShrinkToFit() = 0;
        
    public:
        /**
         * @brief Add multiple items to the end of the vector
         * @param Num Number of items to add
         * @return Iterator to the first added item
         */
        virtual F_Iterator AddRange(Sz Num) = 0;
        
        /**
         * @brief Add one item to the end of the vector
         * @return Reference to the added item
         */
        virtual F_ItemRef Add() = 0;
        
    public:
        /**
         * @brief Insert items from a range
         * @param Location Position to insert at
         * @param BeginSrc Start of the range to insert
         * @param EndSrc End of the range to insert
         * @return Iterator to the first inserted item
         */
        virtual F_Iterator Insert(const F_Iterator& Location, const F_Iterator& BeginSrc, const F_Iterator& EndSrc) = 0;
        
        /**
         * @brief Insert a copy of an item
         * @param Location Position to insert at
         * @param SrcRef Reference to the item to copy
         * @return Iterator to the inserted item
         */
        virtual F_Iterator Insert(const F_Iterator& Location, const F_ConstItemRef& SrcRef) = 0;
        
        /**
         * @brief Insert multiple default-constructed items
         * @param Location Position to insert at
         * @param Count Number of items to insert
         * @return Iterator to the first inserted item
         */
        virtual F_Iterator Insert(const F_Iterator& Location, Sz Count) = 0;
        
        /**
         * @brief Insert multiple copies of an item
         * @param Location Position to insert at
         * @param Count Number of copies to insert
         * @param SrcRef Reference to the item to copy
         * @return Iterator to the first inserted item
         */
        virtual F_Iterator Insert(const F_Iterator& Location, Sz Count, const F_ConstItemRef& SrcRef) = 0;
        
        /**
         * @brief Insert a default-constructed item
         * @param Location Position to insert at
         * @return Iterator to the inserted item
         */
        virtual F_Iterator Insert(const F_Iterator& Location) = 0;
        
        /**
         * @brief Erase an item
         * @param Location Position of the item to erase
         */
        virtual void Erase(const F_Iterator& Location) = 0;
        
    public:
        /**
         * @brief Swap two items
         * @param A First item
         * @param B Second item
         */
        virtual void Swap(const F_Iterator& A, const F_Iterator& B) = 0;
        
        /**
         * @brief Move one item to another position
         * @param Dst Destination position
         * @param Src Source position
         */
        virtual void Move(const F_Iterator& Dst, const F_Iterator& Src) = 0;

    public:
        /**
         * @brief Get an iterator to the first item
         * @return Iterator to the first item
         */
        ABYTEK_FORCE_INLINE F_Iterator GetBegin() noexcept
        {
            return _Begin;
        }
        
        /**
         * @brief Get a const iterator to the first item
         * @return Const iterator to the first item
         */
        ABYTEK_FORCE_INLINE F_ConstIterator GetBegin() const noexcept
        {
            return _Begin;
        }
        
        /**
         * @brief Get an iterator past the last item
         * @return Iterator past the last item
         */
        ABYTEK_FORCE_INLINE F_Iterator GetEnd() noexcept
        {
            return _End;
        }
        
        /**
         * @brief Get a const iterator past the last item
         * @return Const iterator past the last item
         */
        ABYTEK_FORCE_INLINE F_ConstIterator GetEnd() const noexcept
        {
            return _End;
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
        
    public:
        ABYTEK_FORCE_INLINE F_ItemRef operator [] (PDiff Idx) noexcept
        {
            return GetBegin()[Idx];
        }
        ABYTEK_FORCE_INLINE F_ConstItemRef operator [] (PDiff Idx) const noexcept
        {
            return GetBegin()[Idx];
        }
    };
    
    /**
     * @brief Concrete implementation of runtime vector instance for a specific type
     * 
     * Wraps a standard vector of known type to provide the runtime vector interface.
     * 
     * @tparam __F_Item Type of items stored in the vector
     * @tparam __F_Allocator Allocator type to use for memory management
     */
    template<typename __F_Item, class __F_Allocator = WithRawAllocator::TF_DefaultAllocator<U8>>
    class TF_RuntimeVectorInstance : public TA_RuntimeVectorInstance<__F_Allocator>
    {
    public:
        using F_Allocator = __F_Allocator;
        using F_Item = __F_Item;

    private:
        using F_Base = TA_RuntimeVectorInstance<F_Allocator>;
        
    public:
        using F_Iterator = typename TA_RuntimeVectorInstance<F_Allocator>::F_Iterator;
        using F_ConstIterator = typename TA_RuntimeVectorInstance<F_Allocator>::F_ConstIterator;
        using F_ReverseIterator = typename TA_RuntimeVectorInstance<F_Allocator>::F_ReverseIterator;
        using F_ConstReverseIterator = typename TA_RuntimeVectorInstance<F_Allocator>::F_ConstReverseIterator;
        using F_ItemRef = typename TA_RuntimeVectorInstance<F_Allocator>::F_ItemRef;
        using F_ConstItemRef = typename TA_RuntimeVectorInstance<F_Allocator>::F_ConstItemRef;

    public:
        using F_Vector = ContainerTemplates::TF_Vector<__F_Item, TF_RebindAllocator<__F_Item, __F_Allocator>>;
        
    private:
        /** The actual vector storing the items */
        F_Vector _Vector;

    public:
        /**
         * @brief Get the underlying vector
         * @return Reference to the underlying vector
         */
        ABYTEK_FORCE_INLINE const F_Vector& GetVector() const noexcept
        {
            return _Vector;
        }
        
    public:
        /**
         * @brief Default constructor
         */
        TF_RuntimeVectorInstance() :
            F_Base(
                sizeof(F_Item),
                ABYTEK_ALIGNOF(F_Item)
            )
        {
        }
        
        /**
         * @brief Construct from an existing vector
         * @param Vector Vector to wrap
         */
        TF_RuntimeVectorInstance(const F_Vector& Vector) :
            F_Base(
                sizeof(F_Item),
                ABYTEK_ALIGNOF(F_Item)
            ),
            _Vector(Vector)
        {
            _Update();
        }
        
        /**
         * @brief Destructor
         */
        ~TF_RuntimeVectorInstance()
        {
        }

        /**
         * @brief Construct with a specific size
         * @param Size Initial size of the vector
         */
        TF_RuntimeVectorInstance(Sz Size) :
            TF_RuntimeVectorInstance()
        {
            _Vector = F_Vector(Size);
            _Update();
        }
        
        /**
         * @brief Construct from an initializer list
         * @param InitializerList Initializer list of items
         */
        TF_RuntimeVectorInstance(const std::initializer_list<__F_Item>& InitializerList) :
            TF_RuntimeVectorInstance()
        {
            _Vector = F_Vector(InitializerList);
            _Update();
        }
        
        /**
         * @brief Construct from a range
         * @param BeginP Pointer to the first item
         * @param EndP Pointer past the last item
         */
        TF_RuntimeVectorInstance(__F_Item* BeginP, __F_Item* EndP) :
            TF_RuntimeVectorInstance()
        {
            _Vector = F_Vector(BeginP, EndP);
            _Update();
        }

    public:
        ABYTEK_FORCE_INLINE auto& GetBase() noexcept
        {
            return *((F_Base*)this);
        }
        ABYTEK_FORCE_INLINE const auto& GetBase() const noexcept
        {
            return *((const F_Base*)this);
        }

    public:
        virtual F_Base* Clone(F_Allocator& Allocator) override
        {
            using F_CastedAllocator = TF_RebindAllocator<TF_RuntimeVectorInstance, F_Allocator>;
            F_CastedAllocator CastedAllocator = Allocator;

            TF_RuntimeVectorInstance* ResultP = CastedAllocator.allocate(1);
            new(ResultP) TF_RuntimeVectorInstance(_Vector);

            return static_cast<F_Base*>(ResultP);
        }

    public:
        virtual void Resize(Sz Size) override
        {
            _Vector.resize(Size);
            _Update();
        }
        virtual void Reserve(Sz Capacity) override
        {
            _Vector.reserve(Capacity);
            _Update();
        }
        virtual void ShrinkToFit() override
        {
            if(GetBase()._Size == GetBase()._Capacity)
            {
                return;
            }
            
            _Vector.shrink_to_fit();
            _Update();
        }
        
    public:
        virtual F_Iterator AddRange(Sz Num) override
        {
            Resize(
                GetBase()._Size
                + Num
            );
            _Update();
            return GetBase().GetEnd() - Num;
        }
        virtual F_ItemRef Add() override
        {
            Resize(
                GetBase()._Size
                + 1
            );
            _Update();
            return *(GetBase().GetEnd() - 1);
        }
        
    public:
        virtual F_Iterator Insert(const F_Iterator& Location, const F_Iterator& BeginSrc, const F_Iterator& EndSrc) override
        {
            auto VIt = _Vector.insert(
                static_cast<typename F_Vector::iterator>((__F_Item*)(Location.DataP)),
                static_cast<typename F_Vector::iterator>((__F_Item*)(BeginSrc.DataP)),
                static_cast<typename F_Vector::iterator>((__F_Item*)(EndSrc.DataP))
            );
            _Update();
            return { sizeof(__F_Item), (void*)&*VIt };
        }
        virtual F_Iterator Insert(const F_Iterator& Location, const F_ConstItemRef& SrcRef) override
        {
            auto VIt = _Vector.insert(
                static_cast<typename F_Vector::iterator>((__F_Item*)(Location.DataP)),
                boost::move(*(const __F_Item*)(SrcRef.DataP))
            );
            _Update();
            return { sizeof(__F_Item), (void*)&*VIt };
        }
        virtual F_Iterator Insert(const F_Iterator& Location, Sz Count) override
        {
            __F_Item DefaultItem;
            auto VIt = _Vector.insert(
                static_cast<typename F_Vector::iterator>((__F_Item*)(Location.DataP)),
                Count,
                boost::move(DefaultItem)
            );
            _Update();
            return { sizeof(__F_Item), (void*)&*VIt };
        }
        virtual F_Iterator Insert(const F_Iterator& Location, Sz Count, const F_ConstItemRef& SrcRef) override
        {
            auto VIt = _Vector.insert(
                static_cast<typename F_Vector::iterator>((__F_Item*)(Location.DataP)),
                Count,
                *(const __F_Item*)(SrcRef.DataP)
            );
            _Update();
            return { sizeof(__F_Item), (void*)&*VIt };
        }
        virtual F_Iterator Insert(const F_Iterator& Location) override
        {
            __F_Item DefaultItem;
            auto VIt = _Vector.insert(
                static_cast<typename F_Vector::iterator>((__F_Item*)(Location.DataP)),
                boost::move(DefaultItem)
            );
            _Update();
            return { sizeof(__F_Item), (void*)&*VIt };
        }
        virtual void Erase(const F_Iterator& Location) override
        {
            _Vector.erase(
                static_cast<typename F_Vector::iterator>((__F_Item*)(Location.DataP))
            );
            _Update();
        }
        
    public:
        virtual void Swap(const F_Iterator& A, const F_Iterator& B) override
        {
            std::swap(
                *((__F_Item*)A.DataP),
                *((__F_Item*)B.DataP)
            );
        }
        virtual void Move(const F_Iterator& Dst, const F_Iterator& Src) override
        {
            *((__F_Item*)Dst.DataP) = boost::move(
                *((__F_Item*)Src.DataP)
            );
        }

    private:
        void _Update() noexcept
        {
            GetBase()._Size = _Vector.size();
            GetBase()._Capacity = _Vector.capacity();
            GetBase()._Begin = { sizeof(__F_Item), (void*)(_Vector.data()) };
            GetBase()._End = { sizeof(__F_Item), (U8*)(_Vector.data()) + sizeof(__F_Item) * _Vector.size() };
        }
    };
    
    /**
     * @brief Type-erased vector container
     * 
     * Provides a vector interface for items whose type may not be known
     * at compile time. Uses runtime type information to perform operations.
     * 
     * @tparam __F_Allocator Allocator type to use for memory management
     */
    template<class __F_Allocator = WithRawAllocator::TF_DefaultAllocator<U8>>
    class TF_RuntimeVector
    {
    public:
        using F_Allocator = __F_Allocator;

    public:
        using F_Iterator = TF_RuntimeVectorIterator<false>;
        using F_ConstIterator = TF_RuntimeVectorIterator<true>;
        using F_ReverseIterator = TF_ReverseIterator<F_Iterator>;
        using F_ConstReverseIterator = TF_ReverseIterator<F_ConstIterator>;
        using F_ItemRef = TF_RuntimeVectorItemRef<false>;
        using F_ConstItemRef = TF_RuntimeVectorItemRef<true>;

    public:
        using A_Instance = TA_RuntimeVectorInstance<F_Allocator>;
        template<typename __F_Item>
        using TF_Instance = TF_RuntimeVectorInstance<__F_Item, F_Allocator>;

    protected:
        /** Allocator for memory management */
        F_Allocator _Allocator;
        
        /** Function to delete the instance */
        void (*_DeleteInstance)(F_Allocator& Allocator, A_Instance* Instance) = 0;
        
        /** Pointer to the vector instance */
        A_Instance* _Instance = 0;

    public:
        ABYTEK_FORCE_INLINE auto& GetAllocator() noexcept
        {
            return _Allocator;
        }
        ABYTEK_FORCE_INLINE const auto& GetAllocator() const noexcept
        {
            return _Allocator;
        }
        ABYTEK_FORCE_INLINE auto GetDeleteInstance() const noexcept
        {
            return _DeleteInstance;
        }
        ABYTEK_FORCE_INLINE A_Instance& GetInstance() noexcept
        {
            ABYTEK_BASE_OBJECT_ASSERT(*this) << "the instance was not initialized";
            return *_Instance;
        }
        ABYTEK_FORCE_INLINE const A_Instance& GetInstance() const noexcept
        {
            ABYTEK_BASE_OBJECT_ASSERT(*this) << "the instance was not initialized";
            return *_Instance;
        }
        ABYTEK_FORCE_INLINE U16 GetItemSize() const noexcept
        {
            return GetInstance().GetItemSize();
        }
        ABYTEK_FORCE_INLINE U16 GetItemAlignment() const noexcept
        {
            return GetInstance().GetItemAlignment();
        }
        ABYTEK_FORCE_INLINE Sz GetSize() const noexcept
        {
            return GetInstance().GetSize();
        }
        ABYTEK_FORCE_INLINE Sz GetCapacity() const noexcept
        {
            return GetInstance().GetCapacity();
        }
        
    public:
        TF_RuntimeVector()
        {}
        TF_RuntimeVector(const F_Allocator& Allocator) :
            _Allocator(Allocator)
        {}
        ~TF_RuntimeVector()
        {
            Reset();
        }

        TF_RuntimeVector(TF_RuntimeVector&& X) noexcept :
            _Allocator(boost::move(X._Allocator)),
            _DeleteInstance(X._DeleteInstance),
            _Instance(X._Instance)
        {
            X._DeleteInstance = 0;
            X._Instance = 0;
        }
        TF_RuntimeVector& operator = (TF_RuntimeVector&& X) noexcept
        {
            _Allocator = boost::move(X._Allocator);
            _DeleteInstance = X._DeleteInstance;
            _Instance = X._Instance;
            
            X._DeleteInstance = 0;
            X._Instance = 0;
            return *this;
        }

        TF_RuntimeVector(const TF_RuntimeVector& X) :
            _Allocator(X._Allocator),
            _DeleteInstance(X._DeleteInstance),
            _Instance(X._Instance->Clone(_Allocator))
        {
        }
        TF_RuntimeVector& operator = (const TF_RuntimeVector& X)
        {
            _Allocator = X._Allocator;
            _DeleteInstance = X._DeleteInstance;
            _Instance = X._Instance->Clone(_Allocator);
            return *this;
        }

    public:
        /**
         * @brief Initialize the vector with a specific item type
         * @tparam __F_Item Type of items to store
         * @tparam __F_Args Types of constructor arguments
         * @param Args Constructor arguments
         */
        template<typename __F_Item, typename... __F_Args>
        void Initialize(__F_Args&&... Args)
        {
            using F_CastedAllocator = TF_RebindAllocator<TF_Instance<__F_Item>, F_Allocator>;
            F_CastedAllocator CastedAllocator = _Allocator;
            
            _DeleteInstance = [](F_Allocator& Allocator, A_Instance* Instance)
            {
                F_CastedAllocator CastedAllocator = Allocator;

                TF_Instance<__F_Item>* CastedInstance = static_cast<TF_Instance<__F_Item>*>(Instance);
                CastedAllocator.deallocate(CastedInstance, 1);
            };
            
            _Instance = static_cast<A_Instance*>(
                CastedAllocator.allocate(1)
            );
            new(_Instance) TF_Instance<__F_Item>(
                ABYTEK_FORWARD(Args)...
            );
        }
        void Reset()
        {
            if(_Instance)
            {
                _DeleteInstance(_Allocator, _Instance);
                _Instance = 0;
            }
        }
        ABYTEK_FORCE_INLINE explicit operator B8 () const noexcept
        {
            return (
                (_Instance != 0)
                && (_DeleteInstance != 0)
            );
        }

    public:
        ABYTEK_FORCE_INLINE void Resize(Sz Size)
        {
            ABYTEK_BASE_OBJECT_ASSERT(*this) << "invalid runtime vector";
            
            if(Size == GetInstance().GetSize())
            {
                return;
            }

            GetInstance().Resize(Size);
        }
        ABYTEK_FORCE_INLINE void Reserve(Sz Capacity)
        {
            ABYTEK_BASE_OBJECT_ASSERT(*this) << "invalid runtime vector";
            
            if(Capacity == GetInstance().GetCapacity())
            {
                return;
            }

            GetInstance().Reserve(Capacity);
        }
        ABYTEK_FORCE_INLINE void ShrinkToFit() 
        {
            ABYTEK_BASE_OBJECT_ASSERT(*this) << "invalid runtime vector";
            
            if(GetInstance().GetSize() == GetInstance().GetCapacity())
            {
                return;
            }

            GetInstance().ShrinkToFit();
        }
        
    public:
        ABYTEK_FORCE_INLINE F_Iterator AddRange(Sz Num)
        {
            return GetInstance().AddRange(Num);
        }
        ABYTEK_FORCE_INLINE F_ItemRef Add()
        {
            return GetInstance().Add();
        }
        
    public:
        ABYTEK_FORCE_INLINE F_Iterator Insert(const F_Iterator& Location, const F_Iterator& BeginSrc, const F_Iterator& EndSrc)
        {
            return GetInstance().Insert(Location, BeginSrc, EndSrc);
        }
        ABYTEK_FORCE_INLINE F_Iterator Insert(const F_Iterator& Location, const F_ConstItemRef& SrcRef) 
        {
            return GetInstance().Insert(Location, SrcRef);
        }
        ABYTEK_FORCE_INLINE F_Iterator Insert(const F_Iterator& Location, Sz Count) 
        {
            return GetInstance().Insert(Location, Count);
        }
        ABYTEK_FORCE_INLINE F_Iterator Insert(const F_Iterator& Location, Sz Count, const F_ConstItemRef& SrcRef) 
        {
            return GetInstance().Insert(Location, Count, SrcRef);
        }
        ABYTEK_FORCE_INLINE F_Iterator Insert(const F_Iterator& Location) 
        {
            return GetInstance().Insert(Location);
        }
        ABYTEK_FORCE_INLINE void Erase(const F_Iterator& Location) 
        {
            GetInstance().Erase(Location);
        }
        
    public:
        ABYTEK_FORCE_INLINE void Swap(const F_Iterator& A, const F_Iterator& B)
        {
            GetInstance().Swap(A, B);
        }
        ABYTEK_FORCE_INLINE void Move(const F_Iterator& Dst, const F_Iterator& Src)
        {
            GetInstance().Move(Dst, Src);
        }

    public:
        ABYTEK_FORCE_INLINE F_Iterator GetBegin() noexcept
        {
            return GetInstance().GetBegin();
        }
        ABYTEK_FORCE_INLINE F_ConstIterator GetBegin() const noexcept
        {
            return GetInstance().GetBegin();
        }
        ABYTEK_FORCE_INLINE F_Iterator GetEnd() noexcept
        {
            return GetInstance().GetEnd();
        }
        ABYTEK_FORCE_INLINE F_ConstIterator GetEnd() const noexcept
        {
            return GetInstance().GetEnd();
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
        
    public:
        ABYTEK_FORCE_INLINE F_ItemRef operator [] (PDiff Idx) noexcept
        {
            return GetInstance()[Idx];
        }
        ABYTEK_FORCE_INLINE F_ConstItemRef operator [] (PDiff Idx) const noexcept
        {
            return GetInstance()[Idx];
        }
    };
}