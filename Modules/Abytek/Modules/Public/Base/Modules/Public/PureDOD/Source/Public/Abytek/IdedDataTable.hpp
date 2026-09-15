#pragma once

#include "Abytek/Base.PureDOD.prerequisites.pch.hpp"
#include "Abytek/PayloadDataTable.hpp"
#include "Abytek/FlatIndirectMap.hpp"

/**
 * @file IdedDataTable.hpp
 * 
 * @brief Provides a compile-time defined, data-oriented container that associates unique IDs with data rows.
 * 
 * IdedDataTable combines a PayloadDataTable with a FlatIndirectMap to create a table structure where
 * each row has a stable ID that remains valid even when rows are removed or reordered. This allows
 * for external references to data rows that remain valid throughout modifications to the table.
 * 
 * Use this container when you need:
 * - Stable references to data elements (via IDs)
 * - Fast iteration through all elements
 * - Efficient addition and removal of elements
 * - A fixed schema known at compile-time
 */

namespace Abytek
{
    template<B8 __IsConst, typename... __F_Values>
    using TF_IdedDataTableRowRef = TF_PayloadDataTableRowRef<__IsConst, __F_Values...>;
    template<B8 __IsConst, typename... __F_Values>
    using TF_IdedDataTableIterator = TF_PayloadDataTableIterator<__IsConst, __F_Values...>;
}

namespace Abytek::ContainerTemplates
{
    /**
     * @brief A table container that associates unique IDs with rows of heterogeneous data.
     * 
     * TF_IdedDataTable maintains a mapping between unique IDs and rows of data. The IDs remain
     * stable even when elements are removed or reordered, allowing for external systems to
     * maintain references to specific rows without having to worry about invalidation.
     * 
     * This class uses compile-time defined types for its columns, making it efficient for
     * scenarios where the schema is fixed and known at compile time.
     * 
     * @tparam __F_Allocator The allocator type used for memory management
     * @tparam __F_PayloadIndex The index type used for internal indexing
     * @tparam __F_Values The types stored in each column of the table
     */
    template<class __F_Allocator, typename __F_PayloadIndex, typename... __F_Values>
    struct TF_IdedDataTable
    {
    public:
        using F_Allocator = __F_Allocator;
        using F_PayloadIndex = __F_PayloadIndex;
        static constexpr F_PayloadIndex INVALID_PAYLOAD_INDEX = ~F_PayloadIndex(0);
        static constexpr I32 ColumnCount = sizeof...(__F_Values);

    public:
        using F_PayloadDataTable = TF_PayloadDataTable<
            F_Allocator,
            __F_Values...
        >;
        using F_FlatIndirectMap = TF_FlatIndirectMap<F_Allocator, F_PayloadIndex>;

    public:
        template<I32 __ColumnIndex>
        using TF_ValueAt = TF_NthTemplateTArg<__ColumnIndex, __F_Values...>;

    public:
        using F_Id = typename F_FlatIndirectMap::F_Id;
        using F_Generation = typename F_FlatIndirectMap::F_Generation;
        using F_SparseIndex = typename F_FlatIndirectMap::F_SparseIndex;
        static constexpr F_SparseIndex INVALID_SPARSE_INDEX = ~F_SparseIndex(0);
        using F_MetadataElement = typename F_FlatIndirectMap::F_MetadataElement;

    public:
        using F_Iterator = typename F_PayloadDataTable::F_Iterator;
        using F_ConstIterator = typename F_PayloadDataTable::F_ConstIterator;
        using F_ReverseIterator = typename F_PayloadDataTable::F_ReverseIterator;
        using F_ConstReverseIterator = typename F_PayloadDataTable::F_ConstReverseIterator;
        using F_RowRef = typename F_PayloadDataTable::F_RowRef;
        using F_ConstRowRef = typename F_PayloadDataTable::F_ConstRowRef;

    public:
        template<I32... __ColumnIndices>
        using TF_Iterator = typename F_PayloadDataTable::template TF_Iterator<__ColumnIndices...>;
        template<I32... __ColumnIndices>
        using TF_ConstIterator = typename F_PayloadDataTable::template TF_ConstIterator<__ColumnIndices...>;
        template<I32... __ColumnIndices>
        using TF_ReverseIterator = typename F_PayloadDataTable::template TF_ReverseIterator<__ColumnIndices...>;
        template<I32... __ColumnIndices>
        using TF_ConstReverseIterator = typename F_PayloadDataTable::template TF_ConstReverseIterator<__ColumnIndices...>;
        template<I32... __ColumnIndices>
        using TF_RowRef = typename F_PayloadDataTable::template TF_RowRef<__ColumnIndices...>;
        template<I32... __ColumnIndices>
        using TF_ConstRowRef = typename F_PayloadDataTable::template TF_ConstRowRef<__ColumnIndices...>;
        
    private:
        F_PayloadDataTable _PayloadDataTable;
        F_FlatIndirectMap _FlatIndirectMap;

    public:
        /**
         * @brief Gets the underlying PayloadDataTable
         * @return Reference to the PayloadDataTable
         */
        ABYTEK_FORCE_INLINE auto& GetPayloadDataTable() noexcept
        {
            return _PayloadDataTable;
        }
        /**
         * @brief Gets the underlying PayloadDataTable (const version)
         * @return Const reference to the PayloadDataTable
         */
        ABYTEK_FORCE_INLINE const auto& GetPayloadDataTable() const noexcept
        {
            return _PayloadDataTable;
        }
        /**
         * @brief Gets the FlatIndirectMap used for ID mapping
         * @return Reference to the FlatIndirectMap
         */
        ABYTEK_FORCE_INLINE auto& GetFlatIndirectMap() noexcept
        {
            return _FlatIndirectMap;
        }
        /**
         * @brief Gets the FlatIndirectMap used for ID mapping (const version)
         * @return Const reference to the FlatIndirectMap
         */
        ABYTEK_FORCE_INLINE const auto& GetFlatIndirectMap() const noexcept
        {
            return _FlatIndirectMap;
        }

    public:
        ABYTEK_FORCE_INLINE TF_IdedDataTable() noexcept = default;
        ABYTEK_FORCE_INLINE ~TF_IdedDataTable() noexcept = default;

        TF_IdedDataTable(const TF_IdedDataTable& X) :
            _PayloadDataTable(X._PayloadDataTable),
            _FlatIndirectMap(X._FlatIndirectMap)
        {}
        TF_IdedDataTable& operator = (const TF_IdedDataTable& X)
        {
            _PayloadDataTable = X._PayloadDataTable;
            _FlatIndirectMap = X._FlatIndirectMap;
            return *this;
        }

        TF_IdedDataTable(TF_IdedDataTable&& X) noexcept :
            _PayloadDataTable(boost::move(X._PayloadDataTable)),
            _FlatIndirectMap(boost::move(X._FlatIndirectMap))
        {}
        TF_IdedDataTable& operator = (TF_IdedDataTable&& X) noexcept
        {
            _PayloadDataTable = boost::move(X._PayloadDataTable);
            _FlatIndirectMap = boost::move(X._FlatIndirectMap);
            return *this;
        }

    public:
        /**
         * @brief Gets an iterator to the beginning of the table
         * @return Iterator pointing to the first element
         */
        ABYTEK_FORCE_INLINE F_Iterator GetBegin() noexcept
        {
            return _PayloadDataTable.GetBegin();
        }
        /**
         * @brief Gets a const iterator to the beginning of the table
         * @return Const iterator pointing to the first element
         */
        ABYTEK_FORCE_INLINE F_ConstIterator GetBegin() const noexcept
        {
            return _PayloadDataTable.GetBegin();
        }
        /**
         * @brief Gets an iterator to the end of the table
         * @return Iterator pointing one past the last element
         */
        ABYTEK_FORCE_INLINE F_Iterator GetEnd() noexcept
        {
            return _PayloadDataTable.GetEnd();
        }
        /**
         * @brief Gets a const iterator to the end of the table
         * @return Const iterator pointing one past the last element
         */
        ABYTEK_FORCE_INLINE F_ConstIterator GetEnd() const noexcept
        {
            return _PayloadDataTable.GetEnd();
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
            return _PayloadDataTable.GetReverseBegin();
        }
        ABYTEK_FORCE_INLINE F_ConstReverseIterator GetReverseBegin() const noexcept
        {
            return _PayloadDataTable.GetReverseBegin();
        }
        ABYTEK_FORCE_INLINE F_ReverseIterator GetReverseEnd() noexcept
        {
            return _PayloadDataTable.GetReverseEnd();
        }
        ABYTEK_FORCE_INLINE F_ConstReverseIterator GetReverseEnd() const noexcept
        {
            return _PayloadDataTable.GetReverseEnd();
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
        template<I32... __ColumnIndices>
        ABYTEK_FORCE_INLINE TF_Iterator<__ColumnIndices...> GetBegin() noexcept
        {
            return _PayloadDataTable.template GetBegin<__ColumnIndices...>();
        }
        template<I32... __ColumnIndices>
        ABYTEK_FORCE_INLINE TF_ConstIterator<__ColumnIndices...> GetBegin() const noexcept
        {
            return _PayloadDataTable.template GetBegin<__ColumnIndices...>();
        }
        template<I32... __ColumnIndices>
        ABYTEK_FORCE_INLINE TF_Iterator<__ColumnIndices...> GetEnd() noexcept
        {
            return _PayloadDataTable.template GetEnd<__ColumnIndices...>();
        }
        template<I32... __ColumnIndices>
        ABYTEK_FORCE_INLINE TF_ConstIterator<__ColumnIndices...> GetEnd() const noexcept
        {
            return _PayloadDataTable.template GetEnd<__ColumnIndices...>();
        }

    public:
        template<I32... __ColumnIndices>
        ABYTEK_FORCE_INLINE auto GetReverseBegin() noexcept
        {
            return _PayloadDataTable.template GetReverseBegin<__ColumnIndices...>();
        }
        template<I32... __ColumnIndices>
        ABYTEK_FORCE_INLINE auto GetReverseBegin() const noexcept
        {
            return _PayloadDataTable.template GetReverseBegin<__ColumnIndices...>();
        }
        template<I32... __ColumnIndices>
        ABYTEK_FORCE_INLINE auto GetReverseEnd() noexcept
        {
            return _PayloadDataTable.template GetReverseEnd<__ColumnIndices...>();
        }
        template<I32... __ColumnIndices>
        ABYTEK_FORCE_INLINE auto GetReverseEnd() const noexcept
        {
            return _PayloadDataTable.template GetReverseEnd<__ColumnIndices...>();
        }
        
    public:
        /**
         * @brief Checks if an ID is valid
         * @param Id The ID to check
         * @return True if the ID is valid and refers to an existing element
         */
        ABYTEK_FORCE_INLINE B8 IsValidId(F_Id Id) const noexcept
        {
            return _FlatIndirectMap.IsValidId(Id);
        }
        /**
         * @brief Checks if a sparse index has associated payload data
         * @param SparseIndex The sparse index to check
         * @return True if the sparse index has payload data
         */
        ABYTEK_FORCE_INLINE B8 HasPayload(F_SparseIndex SparseIndex) const noexcept
        {
            return _FlatIndirectMap.HasPayload(SparseIndex);
        }
        /**
         * @brief Converts an ID to its corresponding payload index
         * @param Id The ID to convert
         * @return The payload index associated with the ID
         */
        ABYTEK_FORCE_INLINE F_PayloadIndex IdToPayloadIndex(F_Id Id) const noexcept
        {
            return _FlatIndirectMap.IdToPayloadIndex(Id);
        }
        /**
         * @brief Converts a sparse index to its corresponding payload index
         * @param SparseIndex The sparse index to convert
         * @return The payload index associated with the sparse index
         */
        ABYTEK_FORCE_INLINE F_PayloadIndex SparseIndexToPayloadIndex(F_SparseIndex SparseIndex) const noexcept
        {
            return _FlatIndirectMap.SparseIndexToPayloadIndex(SparseIndex);
        }
        /**
         * @brief Adds a new row to the table and returns its ID
         * @param OutRowRef Reference to store the newly created row
         * @return The ID associated with the new row
         */
        F_Id Add(F_RowRef& OutRowRef)
        {
            F_PayloadIndex PayloadIndex = _PayloadDataTable.GetSize();
            _PayloadDataTable.Resize(PayloadIndex + 1);

            OutRowRef = _PayloadDataTable[PayloadIndex];
            
            F_Id Id = _FlatIndirectMap.Add(PayloadIndex);
            return Id;
        }
        /**
         * @brief Adds a new row to the table and returns its ID
         * @return The ID associated with the new row
         */
        F_Id Add()
        {
            F_PayloadIndex PayloadIndex = _PayloadDataTable.GetSize();
            _PayloadDataTable.Resize(PayloadIndex + 1);

            F_Id Id = _FlatIndirectMap.Add(PayloadIndex);
            return Id;
        }
        /**
         * @brief Attempts to remove a row by its ID
         * @param Id The ID of the row to remove
         * @return True if the row was found and removed, false otherwise
         */
        B8 TryRemove(F_Id Id)
        {
            if(IsValidId(Id))
            {
                RemoveUnsafe(Id.SparseIndex);
                return true;
            }
            return false;
        }
        /**
         * @brief Removes a row by its ID
         * @param Id The ID of the row to remove
         * @note Asserts if the ID is invalid
         */
        void Remove(F_Id Id)
        {
            ABYTEK_BASE_PURE_DOD_ASSERT(IsValidId(Id)) << "invalid Id";
            RemoveUnsafe(Id.SparseIndex);
        }
        /**
         * @brief Removes a row by its sparse index without checking validity
         * @param SparseIndex The sparse index of the row to remove
         * @note Asserts if the sparse index is invalid
         */
        void RemoveUnsafe(F_SparseIndex SparseIndex)
        {
            ABYTEK_BASE_PURE_DOD_ASSERT(HasPayload(SparseIndex)) << "invalid SparseIndex";

            F_SparseIndex LastSparseIndex = _FlatIndirectMap.GetSparseSize() - 1;

            auto& PayloadIndices = _FlatIndirectMap.GetPayloadIndices();
            
            auto& PayloadIndex = PayloadIndices[SparseIndex];
            auto& LastPayloadIndex = PayloadIndices[LastSparseIndex];

            _PayloadDataTable[SparseIndex].Swap(
                _PayloadDataTable[LastSparseIndex]
            );
            _PayloadDataTable.Resize(LastSparseIndex);
            
            PayloadIndex = LastPayloadIndex;
            LastPayloadIndex = INVALID_PAYLOAD_INDEX;

            _FlatIndirectMap.RemoveUnsafe(SparseIndex);
        }
        
    public:
        /**
         * @brief Accesses a row by its index
         * @param Idx The index of the row
         * @return Reference to the row
         */
        ABYTEK_FORCE_INLINE F_RowRef operator [] (PDiff Idx) noexcept
        {
            return _PayloadDataTable[Idx];
        }
        /**
         * @brief Accesses a row by its index (const version)
         * @param Idx The index of the row
         * @return Const reference to the row
         */
        ABYTEK_FORCE_INLINE F_ConstRowRef operator [] (PDiff Idx) const noexcept
        {
            return _PayloadDataTable[Idx];
        }
    };
}
