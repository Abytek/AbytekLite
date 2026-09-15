#pragma once

#include "Abytek/Base.PureDOD.prerequisites.pch.hpp"

/**
 * @file PayloadDataTable.hpp
 * 
 * @brief Provides a data-oriented tabular container with compile-time defined types.
 * 
 * PayloadDataTable is a central component of the PureDOD container system, implementing
 * a table-like structure where each column has a specific type known at compile time.
 * This container allows for efficient cache-coherent operations while providing a type-safe
 * interface for working with heterogeneous data.
 * 
 * The implementation uses tuple-based storage for different column types, providing
 * both random access and sequential iteration capabilities.
 */

namespace Abytek
{
    template<B8 __IsConst, typename... __F_Values>
    struct TF_PayloadDataTableRowRef;
    template<B8 __IsConst, typename... __F_Values>
    struct TF_PayloadDataTableIterator;
    
    /**
     * @brief Reference to a row in a PayloadDataTable.
     * 
     * Provides access to a single row of data across all columns in a PayloadDataTable.
     * Supports both mutable and const access based on the IsConst template parameter.
     * 
     * @tparam __IsConst Whether this reference allows modification of the underlying data
     * @tparam __F_Values The types of each column in the row
     */
    template<B8 __IsConst, typename... __F_Values>
    struct TF_PayloadDataTableRowRef
    {
        static constexpr B8 IsConst = __IsConst;
        using F_DataPtrTuple = std::tuple<
            std::conditional_t<
                __IsConst,
                const __F_Values*,
                __F_Values*
            >...
        >;
        static constexpr I32 ColumnCount = sizeof...(__F_Values);

        using F_Iterator = TF_PayloadDataTableIterator<IsConst, __F_Values...>;

        F_DataPtrTuple DataPtrTuple;
        
        /**
         * @brief Gets the value at a specific column
         * @tparam __ColumnIndex The index of the column to access
         * @return Reference to the value at the specified column
         */
        template<I32 __ColumnIndex>
        ABYTEK_FORCE_INLINE std::conditional_t<
            IsConst,
            const TF_NthTemplateTArg<__ColumnIndex, __F_Values...>&,
            TF_NthTemplateTArg<__ColumnIndex, __F_Values...>&
        > Get() const noexcept
        {
            return *std::get<__ColumnIndex>(DataPtrTuple);
        }

        template<I32 __ColumnIndex>
        void BindColumn(const F_Iterator& Iterator);
        /**
         * @brief Binds this row reference to data pointed to by an iterator
         * @param Iterator The iterator pointing to the row
         */
        void Bind(const F_Iterator& Iterator);

        template<I32 __ColumnIndex>
        void SwapColumn(const TF_PayloadDataTableRowRef& X) noexcept
        {
            static constexpr I32 ColumnIndex = (__ColumnIndex < 0) ? 0 : __ColumnIndex;
            std::swap(
                *std::get<ColumnIndex>(DataPtrTuple),
                *std::get<ColumnIndex>(X.DataPtrTuple)
            );
            if constexpr (__ColumnIndex > 0)
            {
                SwapColumn<ColumnIndex - 1>(X);
            }
        }
        /**
         * @brief Swaps the contents of this row with another row
         * @param X The row to swap with
         */
        void Swap(const TF_PayloadDataTableRowRef& X) noexcept
        {
            SwapColumn<ColumnCount - 1>(X);
        }

        template<I32 __ColumnIndex>
        void MoveToColumn(const TF_PayloadDataTableRowRef& X) noexcept
        {
            static constexpr I32 ColumnIndex = (__ColumnIndex < 0) ? 0 : __ColumnIndex;
            *std::get<ColumnIndex>(X.DataPtrTuple) = boost::move(
                *std::get<ColumnIndex>(DataPtrTuple)
            );
            if constexpr (__ColumnIndex > 0)
            {
                MoveToColumn<ColumnIndex - 1>(X);
            }
        }
        /**
         * @brief Moves the contents of this row to another row
         * @param X The destination row
         */
        void MoveTo(const TF_PayloadDataTableRowRef& X) noexcept
        {
            MoveToColumn<ColumnCount - 1>(X);
        }

        template<I32 __ColumnIndex>
        void CopyToColumn(const TF_PayloadDataTableRowRef& X) noexcept
        {
            static constexpr I32 ColumnIndex = (__ColumnIndex < 0) ? 0 : __ColumnIndex;
            *std::get<ColumnIndex>(X.DataPtrTuple) = *std::get<ColumnIndex>(DataPtrTuple);
            if constexpr (__ColumnIndex > 0)
            {
                CopyToColumn<ColumnIndex - 1>(X);
            }
        }
        /**
         * @brief Copies the contents of this row to another row
         * @param X The destination row
         */
        void CopyTo(const TF_PayloadDataTableRowRef& X) noexcept
        {
            CopyToColumn<ColumnCount - 1>(X);
        }
    };

    /**
     * @brief Iterator for a PayloadDataTable.
     * 
     * Provides iteration capabilities over rows in a PayloadDataTable,
     * with support for both mutable and const access.
     * 
     * @tparam __IsConst Whether this iterator allows modification of the underlying data
     * @tparam __F_Values The types of each column in the table
     */
    template<B8 __IsConst, typename... __F_Values>
    struct TF_PayloadDataTableIterator
    {
        template<B8 __IsConst2, typename... __F_Values2>
        friend struct TF_PayloadDataTableIterator;
        
        static constexpr B8 IsConst = __IsConst;
        using F_ColumnIteratorTuple = std::tuple<
            boost::container::vec_iterator<__F_Values*, IsConst>...
        >;
        static constexpr I32 ColumnCount = sizeof...(__F_Values);

        template<class __F_Allocator>
        using TF_VectorTuple = std::tuple<
            ContainerTemplates::TF_Vector<__F_Values, TF_RebindAllocator<__F_Values, __F_Allocator>>...
        >;

        template<class __F_Allocator>
        using TF_VectorTupleRef = std::conditional_t<
            IsConst,
            const TF_VectorTuple<__F_Allocator>&,
            TF_VectorTuple<__F_Allocator>&
        >;

        using F_RowRef = TF_PayloadDataTableRowRef<IsConst, __F_Values...>;

        F_ColumnIteratorTuple Value;

        ABYTEK_FORCE_INLINE TF_PayloadDataTableIterator() noexcept
        {
        }
        
        ABYTEK_FORCE_INLINE TF_PayloadDataTableIterator(const F_ColumnIteratorTuple& InValue) noexcept :
            Value(InValue)
        {
        }

        ABYTEK_FORCE_INLINE TF_PayloadDataTableIterator(const TF_PayloadDataTableIterator& X) noexcept :
            Value(X.Value)
        {
        }
        ABYTEK_FORCE_INLINE TF_PayloadDataTableIterator& operator = (const TF_PayloadDataTableIterator& X) noexcept
        {
            Value = X.Value;
            return *this;
        }

        ABYTEK_FORCE_INLINE TF_PayloadDataTableIterator(TF_PayloadDataTableIterator&& X) noexcept :
            Value(boost::move(X.Value))
        {
        }
        ABYTEK_FORCE_INLINE TF_PayloadDataTableIterator& operator = (TF_PayloadDataTableIterator&& X) noexcept
        {
            Value = boost::move(X.Value);
            return *this;
        }

        ABYTEK_FORCE_INLINE TF_PayloadDataTableIterator(const TF_PayloadDataTableIterator<!IsConst, __F_Values...>& X) noexcept :
            Value(X.Value)
        {
        }
        ABYTEK_FORCE_INLINE TF_PayloadDataTableIterator& operator = (const TF_PayloadDataTableIterator<!IsConst, __F_Values...>& X) noexcept
        {
            Value = X.Value;
            return *this;
        }

        ABYTEK_FORCE_INLINE TF_PayloadDataTableIterator(TF_PayloadDataTableIterator<!IsConst, __F_Values...>&& X) noexcept :
            Value(boost::move(X.Value))
        {
        }
        ABYTEK_FORCE_INLINE TF_PayloadDataTableIterator& operator = (TF_PayloadDataTableIterator<!IsConst, __F_Values...>&& X) noexcept
        {
            Value = boost::move(X.Value);
            return *this;
        }

        template<I32 __ColumnIndex, class __F_Allocator>
        std::tuple<__F_Values*...> GetRawColumn(std::tuple<__F_Values*...>& Result) const noexcept
        {
            static constexpr I32 ColumnIndex = (__ColumnIndex < 0) ? 0 : __ColumnIndex;
            std::get<ColumnIndex>(Result) = &*std::get<ColumnIndex>(Value);
            if constexpr (__ColumnIndex > 0)
            {
                return GetRawColumn<ColumnIndex - 1, __F_Allocator>(Result);
            }
        }
        /**
         * @brief Gets the raw pointers to all column values at current iterator position
         * @return Tuple of pointers to all column values
         */
        template<class __F_Allocator>
        std::tuple<__F_Values*...> GetRaw() const noexcept
        {
            std::tuple<__F_Values*...> Result;
            GetRawColumn<ColumnCount - 1, __F_Allocator>(Result);
            return Result;
        }

        template<I32 __ColumnIndex>
        void IncreaseColumn(PDiff Num = 1) noexcept
        {
            static constexpr I32 ColumnIndex = (__ColumnIndex < 0) ? 0 : __ColumnIndex;
            std::get<ColumnIndex>(Value) += Num;
            if constexpr (__ColumnIndex > 0)
            {
                IncreaseColumn<ColumnIndex - 1>(Num);
            }
        }
        /**
         * @brief Advances the iterator by a specified amount
         * @param Num The number of positions to advance by (default: 1)
         */
        void Increase(PDiff Num = 1) noexcept
        {
            IncreaseColumn<ColumnCount - 1>(Num);
        }

        template<I32 __ColumnIndex>
        void DecreaseColumn(PDiff Num = 1) noexcept
        {
            static constexpr I32 ColumnIndex = (__ColumnIndex < 0) ? 0 : __ColumnIndex;
            std::get<ColumnIndex>(Value) -= Num;
            if constexpr (__ColumnIndex > 0)
            {
                DecreaseColumn<ColumnIndex - 1>(Num);
            }
        }
        /**
         * @brief Moves the iterator backward by a specified amount
         * @param Num The number of positions to move backward by (default: 1)
         */
        void Decrease(PDiff Num = 1) noexcept
        {
            DecreaseColumn<ColumnCount - 1>(Num);
        }

        template<I32 __ColumnIndex>
        B8 EqualColumn(const TF_PayloadDataTableIterator& X) const noexcept
        {
            static constexpr I32 ColumnIndex = (__ColumnIndex < 0) ? 0 : __ColumnIndex;
            B8 IsColumnEqual = (std::get<ColumnIndex>(Value) == std::get<ColumnIndex>(X.Value));
            if constexpr (__ColumnIndex > 0)
            {
                return (
                    IsColumnEqual
                    && EqualColumn<ColumnIndex - 1>(X)
                );
            }
            return IsColumnEqual;
        }
        /**
         * @brief Checks if this iterator equals another iterator
         * @param X The iterator to compare with
         * @return True if the iterators are equal
         */
        B8 Equal(const TF_PayloadDataTableIterator& X) const noexcept
        {
            return EqualColumn<ColumnCount - 1>(X);
        }

        template<I32 __ColumnIndex, class __F_Allocator>
        void GetBeginColumn(TF_VectorTupleRef<__F_Allocator> VectorTuple) noexcept
        {
            static constexpr I32 ColumnIndex = (__ColumnIndex < 0) ? 0 : __ColumnIndex;
            std::get<ColumnIndex>(Value) = std::get<ColumnIndex>(VectorTuple).begin();
            if constexpr (__ColumnIndex > 0)
            {
                GetBeginColumn<ColumnIndex - 1, __F_Allocator>(VectorTuple);
            }
        }
        /**
         * @brief Positions the iterator at the beginning of the table
         * @param VectorTuple The tuple of vectors representing the table's columns
         */
        template<class __F_Allocator>
        void GetBegin(TF_VectorTupleRef<__F_Allocator> VectorTuple) noexcept
        {
            GetBeginColumn<ColumnCount - 1, __F_Allocator>(VectorTuple);
        }

        template<I32 __ColumnIndex, class __F_Allocator>
        void GetEndColumn(TF_VectorTupleRef<__F_Allocator> VectorTuple) noexcept
        {
            static constexpr I32 ColumnIndex = (__ColumnIndex < 0) ? 0 : __ColumnIndex;
            std::get<ColumnIndex>(Value) = std::get<ColumnIndex>(VectorTuple).end();
            if constexpr (__ColumnIndex > 0)
            {
                GetEndColumn<ColumnIndex - 1, __F_Allocator>(VectorTuple);
            }
        }
        /**
         * @brief Positions the iterator at the end of the table
         * @param VectorTuple The tuple of vectors representing the table's columns
         */
        template<class __F_Allocator>
        void GetEnd(TF_VectorTupleRef<__F_Allocator> VectorTuple) noexcept
        {
            GetEndColumn<ColumnCount - 1, __F_Allocator>(VectorTuple);
        }
        
        /**
         * @brief Pre-increment operator (++it)
         * @return Reference to this iterator after incrementing
         */
        ABYTEK_FORCE_INLINE TF_PayloadDataTableIterator& operator ++ () noexcept
        {
            Increase();
            return *this;
        }
        /**
         * @brief Pre-decrement operator (--it)
         * @return Reference to this iterator after decrementing
         */
        ABYTEK_FORCE_INLINE TF_PayloadDataTableIterator& operator -- () noexcept
        {
            Decrease();
            return *this;
        }
        
        /**
         * @brief Addition assignment operator (it += n)
         * @param Num The number of positions to advance
         * @return Reference to this iterator after advancing
         */
        ABYTEK_FORCE_INLINE TF_PayloadDataTableIterator& operator += (PDiff Num) noexcept
        {
            Increase(Num);
            return *this;
        }
        /**
         * @brief Subtraction assignment operator (it -= n)
         * @param Num The number of positions to move backward
         * @return Reference to this iterator after moving backward
         */
        ABYTEK_FORCE_INLINE TF_PayloadDataTableIterator& operator -= (PDiff Num) noexcept 
        {
            Decrease(Num);
            return *this;
        }
        
        /**
         * @brief Addition operator (it + n)
         * @param Num The number of positions to advance
         * @return New iterator positioned at the advanced location
         */
        ABYTEK_FORCE_INLINE TF_PayloadDataTableIterator operator + (PDiff Num) const noexcept
        {
            TF_PayloadDataTableIterator Result = *this;
            Result.Increase(Num);
            return Result;
        }
        /**
         * @brief Subtraction operator (it - n)
         * @param Num The number of positions to move backward
         * @return New iterator positioned at the moved location
         */
        ABYTEK_FORCE_INLINE TF_PayloadDataTableIterator operator - (PDiff Num) const noexcept
        {
            TF_PayloadDataTableIterator Result = *this;
            Result.Decrease(Num);
            return Result;
        }
        
        friend ABYTEK_FORCE_INLINE PDiff operator + (const TF_PayloadDataTableIterator& A, const TF_PayloadDataTableIterator& B) noexcept
        {
            return (std::get<0>(A.Value) + std::get<0>(B.Value));
        }
        friend ABYTEK_FORCE_INLINE PDiff operator - (const TF_PayloadDataTableIterator& A, const TF_PayloadDataTableIterator& B) noexcept
        {
            return (std::get<0>(A.Value) - std::get<0>(B.Value));
        }
        
        /**
         * @brief Equality comparison operator
         * @param X The iterator to compare with
         * @return True if the iterators are equal
         */
        ABYTEK_FORCE_INLINE B8 operator == (const TF_PayloadDataTableIterator& X) const noexcept
        {
            return Equal(X);
        }
        /**
         * @brief Inequality comparison operator
         * @param X The iterator to compare with
         * @return True if the iterators are not equal
         */
        ABYTEK_FORCE_INLINE B8 operator != (const TF_PayloadDataTableIterator& X) const noexcept
        {
            return !Equal(X);
        }
        
        /**
         * @brief Accesses a specific column value at the current iterator position
         * @tparam __ColumnIndex The index of the column to access
         * @return Reference to the column value
         */
        template<I32 __ColumnIndex>
        ABYTEK_FORCE_INLINE std::conditional_t<
            IsConst,
            const TF_NthTemplateTArg<__ColumnIndex, __F_Values...>&,
            TF_NthTemplateTArg<__ColumnIndex, __F_Values...>&
        > Get() const noexcept
        {
            return *std::get<__ColumnIndex>(Value);
        }

        /**
         * @brief Dereference operator, creates a row reference at the current position
         * @return RowRef for the current row
         */
        ABYTEK_FORCE_INLINE F_RowRef operator * () const noexcept
        {
            F_RowRef Result;
            Result.Bind(*this);
            return boost::move(Result);
        }
        /**
         * @brief Random access operator, creates a row reference at position +Idx
         * @param Idx The offset from the current position
         * @return RowRef for the row at the offset position
         */
        ABYTEK_FORCE_INLINE F_RowRef operator [] (PDiff Idx) const noexcept
        {
            F_RowRef Result;
            Result.Bind((*this) + Idx);
            return boost::move(Result);
        }
    };

    template <B8 __IsConst, typename ... __F_Values>
    template <I32 __ColumnIndex>
    void TF_PayloadDataTableRowRef<__IsConst, __F_Values...>::BindColumn(const F_Iterator& Iterator)
    {
        static constexpr I32 ColumnIndex = (__ColumnIndex < 0) ? 0 : __ColumnIndex;
        std::get<ColumnIndex>(DataPtrTuple) = &*(std::get<ColumnIndex>(Iterator.Value));
        if constexpr (__ColumnIndex > 0)
        {
            BindColumn<ColumnIndex - 1>(Iterator);
        }
    }
    template <B8 __IsConst, typename ... __F_Values>
    void TF_PayloadDataTableRowRef<__IsConst, __F_Values...>::Bind(const F_Iterator& Iterator)
    {
        BindColumn<sizeof...(__F_Values) - 1>(Iterator);
    }
}

namespace Abytek::ContainerTemplates
{
    /**
     * @brief A data-oriented table container with fixed column types.
     * 
     * TF_PayloadDataTable implements a table-like structure where each column has a specific type
     * defined at compile time. The table stores its data in a tuple of columns, with each column
     * being a vector of a specific type. This design provides efficient memory access patterns
     * when iterating over specific columns, as the data for each column is stored contiguously.
     * 
     * This container is suitable for systems that require:
     * - Cache-coherent iteration through homogeneous data
     * - Type safety with compile-time validation
     * - Efficient access to specific columns
     * - Random access to rows
     * 
     * @tparam __F_Allocator The allocator type to use for memory management
     * @tparam __F_Values The types of each column in the table
     */
    template<class __F_Allocator, typename... __F_Values>
    struct TF_PayloadDataTable
    {
    public:
        using F_Allocator = __F_Allocator;
        using F_VectorTuple = std::tuple<
            TF_Vector<__F_Values, TF_RebindAllocator<__F_Values, F_Allocator>>...
        >;
        static constexpr I32 ColumnCount = sizeof...(__F_Values);

    public:
        template<I32 __ColumnIndex>
        using TF_ValueAt = TF_NthTemplateTArg<__ColumnIndex, __F_Values...>;

    public:
        using F_Iterator = TF_PayloadDataTableIterator<false, __F_Values...>;
        using F_ConstIterator = TF_PayloadDataTableIterator<true, __F_Values...>;
        using F_ReverseIterator = TF_ReverseIterator<F_Iterator>;
        using F_ConstReverseIterator = TF_ReverseIterator<F_ConstIterator>;
        using F_RowRef = TF_PayloadDataTableRowRef<false, __F_Values...>;
        using F_ConstRowRef = TF_PayloadDataTableRowRef<true, __F_Values...>;

    public:
        template<I32... __ColumnIndices>
        using TF_Iterator = TF_PayloadDataTableIterator<false, TF_ValueAt<__ColumnIndices>...>;
        template<I32... __ColumnIndices>
        using TF_ConstIterator = TF_PayloadDataTableIterator<true, TF_ValueAt<__ColumnIndices>...>;
        template<I32... __ColumnIndices>
        using TF_ReverseIterator = Abytek::TF_ReverseIterator<TF_Iterator<__ColumnIndices...>>;
        template<I32... __ColumnIndices>
        using TF_ConstReverseIterator = Abytek::TF_ReverseIterator<TF_ConstIterator<__ColumnIndices...>>;
        template<I32... __ColumnIndices>
        using TF_RowRef = TF_PayloadDataTableRowRef<false, TF_ValueAt<__ColumnIndices>...>;
        template<I32... __ColumnIndices>
        using TF_ConstRowRef = TF_PayloadDataTableRowRef<true, TF_ValueAt<__ColumnIndices>...>;
        
    private:
        F_VectorTuple _VectorTuple;

    public:
        /**
         * @brief Gets the number of rows in the table
         * @return The size of the table (number of rows)
         */
        ABYTEK_FORCE_INLINE Sz GetSize() const noexcept
        {
            return std::get<0>(_VectorTuple).size();
        }
        /**
         * @brief Gets the current capacity of the table
         * @return The capacity of the table (number of rows that can be stored without reallocation)
         */
        ABYTEK_FORCE_INLINE Sz GetCapacity() const noexcept
        {
            return std::get<0>(_VectorTuple).capacity();
        }
        /**
         * @brief Gets the tuple of vectors that store the table data
         * @return Reference to the vector tuple
         */
        ABYTEK_FORCE_INLINE auto& GetVectorTuple() noexcept
        {
            return _VectorTuple;
        }
        /**
         * @brief Gets the tuple of vectors that store the table data (const version)
         * @return Const reference to the vector tuple
         */
        ABYTEK_FORCE_INLINE const auto& GetVectorTuple() const noexcept
        {
            return _VectorTuple;
        }
        /**
         * @brief Gets a specific column from the table
         * @tparam __ColumnIndex The index of the column to access
         * @return Reference to the column vector
         */
        template<I32 __ColumnIndex>
        ABYTEK_FORCE_INLINE auto& GetColumn() noexcept
        {
            return std::get<__ColumnIndex>(_VectorTuple);
        }
        /**
         * @brief Gets a specific column from the table (const version)
         * @tparam __ColumnIndex The index of the column to access
         * @return Const reference to the column vector
         */
        template<I32 __ColumnIndex>
        ABYTEK_FORCE_INLINE const auto& GetColumn() const noexcept
        {
            return std::get<__ColumnIndex>(_VectorTuple);
        }
        /**
         * @brief Gets an iterator to the beginning of a specific column
         * @tparam __ColumnIndex The index of the column
         * @return Iterator to the first element of the column
         */
        template<I32 __ColumnIndex>
        ABYTEK_FORCE_INLINE auto GetColumnBegin() noexcept
        {
            return GetColumn<__ColumnIndex>().begin();
        }
        /**
         * @brief Gets a const iterator to the beginning of a specific column
         * @tparam __ColumnIndex The index of the column
         * @return Const iterator to the first element of the column
         */
        template<I32 __ColumnIndex>
        ABYTEK_FORCE_INLINE auto GetColumnBegin() const noexcept
        {
            return GetColumn<__ColumnIndex>().begin();
        }
        /**
         * @brief Gets an iterator to the end of a specific column
         * @tparam __ColumnIndex The index of the column
         * @return Iterator one past the last element of the column
         */
        template<I32 __ColumnIndex>
        ABYTEK_FORCE_INLINE auto GetColumnEnd() noexcept
        {
            return GetColumn<__ColumnIndex>().end();
        }
        /**
         * @brief Gets a const iterator to the end of a specific column
         * @tparam __ColumnIndex The index of the column
         * @return Const iterator one past the last element of the column
         */
        template<I32 __ColumnIndex>
        ABYTEK_FORCE_INLINE auto GetColumnEnd() const noexcept
        {
            return GetColumn<__ColumnIndex>().end();
        }
        /**
         * @brief Gets a reverse iterator to the beginning of a specific column
         * @tparam __ColumnIndex The index of the column
         * @return Reverse iterator to the first element in reverse order
         */
        template<I32 __ColumnIndex>
        ABYTEK_FORCE_INLINE auto GetColumnReverseBegin() noexcept
        {
            return GetColumn<__ColumnIndex>().rbegin();
        }
        /**
         * @brief Gets a const reverse iterator to the beginning of a specific column
         * @tparam __ColumnIndex The index of the column
         * @return Const reverse iterator to the first element in reverse order
         */
        template<I32 __ColumnIndex>
        ABYTEK_FORCE_INLINE auto GetColumnReverseBegin() const noexcept
        {
            return GetColumn<__ColumnIndex>().rbegin();
        }
        /**
         * @brief Gets a reverse iterator to the end of a specific column
         * @tparam __ColumnIndex The index of the column
         * @return Reverse iterator one past the last element in reverse order
         */
        template<I32 __ColumnIndex>
        ABYTEK_FORCE_INLINE auto GetColumnReverseEnd() noexcept
        {
            return GetColumn<__ColumnIndex>().rend();
        }
        /**
         * @brief Gets a const reverse iterator to the end of a specific column
         * @tparam __ColumnIndex The index of the column
         * @return Const reverse iterator one past the last element in reverse order
         */
        template<I32 __ColumnIndex>
        ABYTEK_FORCE_INLINE auto GetColumnReverseEnd() const noexcept
        {
            return GetColumn<__ColumnIndex>().rend();
        }

    public:
        ABYTEK_FORCE_INLINE TF_PayloadDataTable() noexcept = default;
        ABYTEK_FORCE_INLINE ~TF_PayloadDataTable() noexcept = default;

        TF_PayloadDataTable(const TF_PayloadDataTable& X) :
            _VectorTuple(X._VectorTuple)
        {}
        TF_PayloadDataTable& operator = (const TF_PayloadDataTable& X)
        {
            _VectorTuple = X._VectorTuple;
            return *this;
        }

        TF_PayloadDataTable(TF_PayloadDataTable&& X) noexcept :
            _VectorTuple(boost::move(X._VectorTuple))
        {}
        TF_PayloadDataTable& operator = (TF_PayloadDataTable&& X) noexcept
        {
            _VectorTuple = boost::move(X._VectorTuple);
            return *this;
        }

        /**
         * @brief Constructs a table with a specified initial size
         * @param Size The initial number of rows in the table
         */
        TF_PayloadDataTable(Sz Size)
        {
            Resize(Size);
        }

    private:
        template<I32 __ColumnIndex>
        void _ResizeColumn(Sz Size)
        {
            constexpr I32 ColumnIndex = (__ColumnIndex < 0) ? 0 : __ColumnIndex;
            std::get<ColumnIndex>(_VectorTuple).resize(Size);
            if constexpr (__ColumnIndex > 0)
            {
                _ResizeColumn<ColumnIndex - 1>(Size);
            }
        }
        template<I32 __ColumnIndex>
        void _ReserveColumn(Sz Size)
        {
            constexpr I32 ColumnIndex = (__ColumnIndex < 0) ? 0 : __ColumnIndex;
            std::get<ColumnIndex>(_VectorTuple).reserve(Size);
            if constexpr (__ColumnIndex > 0)
            {
                _ReserveColumn<ColumnIndex - 1>(Size);
            }
        }
        template<I32 __ColumnIndex>
        void _ShrinkToFitColumn()
        {
            constexpr I32 ColumnIndex = (__ColumnIndex < 0) ? 0 : __ColumnIndex;
            std::get<ColumnIndex>(_VectorTuple).shrink_to_fit();
            if constexpr (__ColumnIndex > 0)
            {
                _ShrinkToFitColumn<ColumnIndex - 1>();
            }
        }
        
    public:
        /**
         * @brief Resizes the table to the specified number of rows
         * @param Size The new size of the table
         */
        ABYTEK_FORCE_INLINE void Resize(Sz Size)
        {
            _ResizeColumn<ColumnCount - 1>(Size);
        }
        /**
         * @brief Reserves capacity for a specified number of rows
         * @param Capacity The number of rows to reserve capacity for
         */
        ABYTEK_FORCE_INLINE void Reserve(Sz Capacity)
        {
            _ReserveColumn<ColumnCount - 1>(Capacity);
        }
        /**
         * @brief Reduces the capacity to match the size, minimizing memory usage
         */
        ABYTEK_FORCE_INLINE void ShrinkToFit()
        {
            _ShrinkToFitColumn<ColumnCount - 1>();
        }
        
    public:
        /**
         * @brief Adds multiple rows to the table and returns an iterator to the first new row
         * @param Num The number of rows to add
         * @return Iterator to the first of the newly added rows
         */
        ABYTEK_FORCE_INLINE F_Iterator AddRange(Sz Num)
        {
            Resize(GetSize() + Num);
            return GetEnd() - Num;
        }
        /**
         * @brief Adds a single row to the table and returns a reference to it
         * @return Reference to the newly added row
         */
        ABYTEK_FORCE_INLINE F_RowRef Add()
        {
            F_Iterator Result = GetEnd();
            Resize(GetSize() + 1);
            return *(GetEnd() - 1);
        }

    private:
        template<I32 __ColumnIndex>
        void _InsertColumn(const F_Iterator& Location, const F_Iterator& BeginSrc, const F_Iterator& EndSrc)
        {
            constexpr I32 ColumnIndex = (__ColumnIndex < 0) ? 0 : __ColumnIndex;
            std::get<ColumnIndex>(_VectorTuple).insert(
                std::get<ColumnIndex>(Location.Value),
                std::get<ColumnIndex>(BeginSrc.Value),
                std::get<ColumnIndex>(EndSrc.Value)
            );
            if constexpr (__ColumnIndex > 0)
            {
                _InsertColumn<ColumnIndex - 1>(Location, BeginSrc, EndSrc);
            }
        }
        template<I32 __ColumnIndex>
        void _InsertColumn(const F_Iterator& Location, const F_ConstRowRef& SrcRef)
        {
            constexpr I32 ColumnIndex = (__ColumnIndex < 0) ? 0 : __ColumnIndex;
            std::get<ColumnIndex>(_VectorTuple).insert(
                std::get<ColumnIndex>(Location.Value),
                SrcRef.template Get<ColumnIndex>()
            );
            if constexpr (__ColumnIndex > 0)
            {
                _InsertColumn<ColumnIndex - 1>(Location, SrcRef);
            }
        }
        template<I32 __ColumnIndex>
        void _EraseColumn(const F_Iterator& Location)
        {
            constexpr I32 ColumnIndex = (__ColumnIndex < 0) ? 0 : __ColumnIndex;
            std::get<ColumnIndex>(_VectorTuple).erase(
                std::get<ColumnIndex>(Location.Value)
            );
            if constexpr (__ColumnIndex > 0)
            {
                _EraseColumn<ColumnIndex - 1>(Location);
            }
        }
        
    public:
        /**
         * @brief Inserts a range of rows from another table at a specified position
         * @param Location Iterator pointing to the insertion position
         * @param BeginSrc Iterator to the first row to insert
         * @param EndSrc Iterator one past the last row to insert
         */
        ABYTEK_FORCE_INLINE void Insert(const F_Iterator& Location, const F_Iterator& BeginSrc, const F_Iterator& EndSrc)
        {
            _InsertColumn<ColumnCount - 1>(Location, BeginSrc, EndSrc);
        }
        /**
         * @brief Inserts a single row at a specified position
         * @param Location Iterator pointing to the insertion position
         * @param SrcRef Reference to the row to insert
         */
        ABYTEK_FORCE_INLINE void Insert(const F_Iterator& Location, const F_ConstRowRef& SrcRef)
        {
            _InsertColumn<ColumnCount - 1>(Location, SrcRef);
        }
        /**
         * @brief Erases a row at a specified position
         * @param Location Iterator pointing to the row to erase
         */
        ABYTEK_FORCE_INLINE void Erase(const F_Iterator& Location)
        {
            _EraseColumn<ColumnCount - 1>(Location);
        }

    public:
        /**
         * @brief Gets an iterator to the beginning of the table
         * @return Iterator pointing to the first row
         */
        ABYTEK_FORCE_INLINE F_Iterator GetBegin() noexcept
        {
            F_Iterator Result;
            Result.template GetBegin<F_Allocator>(_VectorTuple);
            return Result;
        }
        /**
         * @brief Gets a const iterator to the beginning of the table
         * @return Const iterator pointing to the first row
         */
        ABYTEK_FORCE_INLINE F_ConstIterator GetBegin() const noexcept
        {
            F_Iterator Result;
            Result.template GetBegin<F_Allocator>(_VectorTuple);
            return Result;
        }
        /**
         * @brief Gets an iterator to the end of the table
         * @return Iterator pointing one past the last row
         */
        ABYTEK_FORCE_INLINE F_Iterator GetEnd() noexcept
        {
            F_Iterator Result;
            Result.template GetEnd<F_Allocator>(_VectorTuple);
            return Result;
        }
        /**
         * @brief Gets a const iterator to the end of the table
         * @return Const iterator pointing one past the last row
         */
        ABYTEK_FORCE_INLINE F_ConstIterator GetEnd() const noexcept
        {
            F_Iterator Result;
            Result.template GetEnd<F_Allocator>(_VectorTuple);
            return Result;
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
        /**
         * @brief Gets a reverse iterator to the beginning of the table
         * @return Reverse iterator pointing to the first row in reverse order
         */
        ABYTEK_FORCE_INLINE F_ReverseIterator GetReverseBegin() noexcept
        {
            return MakeReverseIterator(GetEnd());
        }
        /**
         * @brief Gets a const reverse iterator to the beginning of the table
         * @return Const reverse iterator pointing to the first row in reverse order
         */
        ABYTEK_FORCE_INLINE F_ConstReverseIterator GetReverseBegin() const noexcept
        {
            return MakeReverseIterator(GetEnd());
        }
        /**
         * @brief Gets a reverse iterator to the end of the table
         * @return Reverse iterator pointing one past the last row in reverse order
         */
        ABYTEK_FORCE_INLINE F_ReverseIterator GetReverseEnd() noexcept
        {
            return MakeReverseIterator(GetBegin());
        }
        /**
         * @brief Gets a const reverse iterator to the end of the table
         * @return Const reverse iterator pointing one past the last row in reverse order
         */
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
        /**
         * @brief Gets an iterator to the beginning of the table for specific columns
         * @tparam __ColumnIndices The indices of the columns to include in the iterator
         * @return Iterator for the specified columns
         */
        template<I32... __ColumnIndices>
        ABYTEK_FORCE_INLINE TF_Iterator<__ColumnIndices...> GetBegin() noexcept
        {
            return {
                std::make_tuple(
                    std::get<__ColumnIndices>(_VectorTuple).begin()...
                )
            };
        }
        /**
         * @brief Gets a const iterator to the beginning of the table for specific columns
         * @tparam __ColumnIndices The indices of the columns to include in the iterator
         * @return Const iterator for the specified columns
         */
        template<I32... __ColumnIndices>
        ABYTEK_FORCE_INLINE TF_ConstIterator<__ColumnIndices...> GetBegin() const noexcept
        {
            return {
                std::make_tuple(
                    std::get<__ColumnIndices>(_VectorTuple).begin()...
                )
            };
        }
        /**
         * @brief Gets an iterator to the end of the table for specific columns
         * @tparam __ColumnIndices The indices of the columns to include in the iterator
         * @return Iterator for the specified columns
         */
        template<I32... __ColumnIndices>
        ABYTEK_FORCE_INLINE TF_Iterator<__ColumnIndices...> GetEnd() noexcept
        {
            return {
                std::make_tuple(
                    std::get<__ColumnIndices>(_VectorTuple).end()...
                )
            };
        }
        /**
         * @brief Gets a const iterator to the end of the table for specific columns
         * @tparam __ColumnIndices The indices of the columns to include in the iterator
         * @return Const iterator for the specified columns
         */
        template<I32... __ColumnIndices>
        ABYTEK_FORCE_INLINE TF_ConstIterator<__ColumnIndices...> GetEnd() const noexcept
        {
            return {
                std::make_tuple(
                    std::get<__ColumnIndices>(_VectorTuple).end()...
                )
            };
        }

    public:
        /**
         * @brief Gets a reverse iterator to the beginning of the table for specific columns
         * @tparam __ColumnIndices The indices of the columns to include in the iterator
         * @return Reverse iterator for the specified columns
         */
        template<I32... __ColumnIndices>
        ABYTEK_FORCE_INLINE auto GetReverseBegin() noexcept
        {
            return MakeReverseIterator(GetEnd<__ColumnIndices...>());
        }
        /**
         * @brief Gets a const reverse iterator to the beginning of the table for specific columns
         * @tparam __ColumnIndices The indices of the columns to include in the iterator
         * @return Const reverse iterator for the specified columns
         */
        template<I32... __ColumnIndices>
        ABYTEK_FORCE_INLINE auto GetReverseBegin() const noexcept
        {
            return MakeReverseIterator(GetEnd<__ColumnIndices...>());
        }
        /**
         * @brief Gets a reverse iterator to the end of the table for specific columns
         * @tparam __ColumnIndices The indices of the columns to include in the iterator
         * @return Reverse iterator for the specified columns
         */
        template<I32... __ColumnIndices>
        ABYTEK_FORCE_INLINE auto GetReverseEnd() noexcept
        {
            return MakeReverseIterator(GetBegin<__ColumnIndices...>());
        }
        /**
         * @brief Gets a const reverse iterator to the end of the table for specific columns
         * @tparam __ColumnIndices The indices of the columns to include in the iterator
         * @return Const reverse iterator for the specified columns
         */
        template<I32... __ColumnIndices>
        ABYTEK_FORCE_INLINE auto GetReverseEnd() const noexcept
        {
            return MakeReverseIterator(GetBegin<__ColumnIndices...>());
        }
        
    public:
        /**
         * @brief Accesses a row by its index
         * @param Idx The index of the row
         * @return Reference to the row
         */
        ABYTEK_FORCE_INLINE F_RowRef operator [] (PDiff Idx) noexcept
        {
            return GetBegin()[Idx];
        }
        /**
         * @brief Accesses a row by its index (const version)
         * @param Idx The index of the row
         * @return Const reference to the row
         */
        ABYTEK_FORCE_INLINE F_ConstRowRef operator [] (PDiff Idx) const noexcept
        {
            return GetBegin()[Idx];
        }
    };
}
