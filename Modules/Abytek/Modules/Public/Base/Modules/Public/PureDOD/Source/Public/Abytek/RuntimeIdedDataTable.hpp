#pragma once

#include "Abytek/Base.PureDOD.prerequisites.pch.hpp"
#include "Abytek/RuntimePayloadDataTable.hpp"
#include "Abytek/FlatIndirectMap.hpp"

/**
 * @file RuntimeIdedDataTable.hpp
 * 
 * @brief Provides a runtime-defined, data-oriented container that associates unique IDs with data rows.
 * 
 * RuntimeIdedDataTable combines a RuntimePayloadDataTable with a FlatIndirectMap to create a table 
 * structure where each row has a stable ID that remains valid even when rows are removed or reordered.
 * Unlike IdedDataTable, the column types and structure are determined at runtime rather than compile time.
 * 
 * Use this container when you need:
 * - Stable references to data elements (via IDs)
 * - Fast iteration through all elements
 * - Efficient addition and removal of elements
 * - A schema that is determined at runtime
 */

namespace Abytek::ContainerTemplates
{
    /**
     * @brief A table container that associates unique IDs with rows of dynamically structured data.
     * 
     * TF_RuntimeIdedDataTable maintains a mapping between unique IDs and rows of data, similar to
     * TF_IdedDataTable, but with column types and structure determined at runtime rather than compile time.
     * The IDs remain stable even when elements are removed or reordered, allowing for external systems
     * to maintain references to specific rows without having to worry about invalidation.
     * 
     * @tparam __F_Allocator The allocator type used for memory management
     * @tparam __F_PayloadIndex The index type used for internal indexing
     */
    template<class __F_Allocator, typename __F_PayloadIndex>
    struct TF_RuntimeIdedDataTable
    {
    public:
        using F_Allocator = __F_Allocator;
        using F_PayloadIndex = __F_PayloadIndex;
        static constexpr F_PayloadIndex INVALID_PAYLOAD_INDEX = ~F_PayloadIndex(0);

    public:
        using F_PayloadDataTable = TF_RuntimePayloadDataTable<F_Allocator>;
        using F_FlatIndirectMap = TF_FlatIndirectMap<F_Allocator, F_PayloadIndex>;

    public:
        using F_Id = typename F_FlatIndirectMap::F_Id;
        using F_Generation = typename F_FlatIndirectMap::F_Generation;
        using F_SparseIndex = typename F_FlatIndirectMap::F_SparseIndex;
        static constexpr F_SparseIndex INVALID_SPARSE_INDEX = ~F_SparseIndex(0);
        using F_MetadataElement = typename F_FlatIndirectMap::F_MetadataElement;
        
    private:
        F_PayloadDataTable _PayloadDataTable;
        F_FlatIndirectMap _FlatIndirectMap;

    public:
        /**
         * @brief Gets the underlying RuntimePayloadDataTable
         * @return Reference to the RuntimePayloadDataTable
         */
        ABYTEK_FORCE_INLINE auto& GetPayloadDataTable() noexcept
        {
            return _PayloadDataTable;
        }
        /**
         * @brief Gets the underlying RuntimePayloadDataTable (const version)
         * @return Const reference to the RuntimePayloadDataTable
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
        ABYTEK_FORCE_INLINE TF_RuntimeIdedDataTable() noexcept = default;
        ABYTEK_FORCE_INLINE ~TF_RuntimeIdedDataTable() noexcept = default;

        /**
         * @brief Constructs a table with predefined columns
         * @param Columns The columns to initialize the table with
         */
        TF_RuntimeIdedDataTable(typename F_PayloadDataTable::F_Columns&& Columns) :
            _PayloadDataTable(boost::move(Columns))
        {
        }

        TF_RuntimeIdedDataTable(const TF_RuntimeIdedDataTable& X) :
            _PayloadDataTable(X._PayloadDataTable),
            _FlatIndirectMap(X._FlatIndirectMap)
        {}
        TF_RuntimeIdedDataTable& operator = (const TF_RuntimeIdedDataTable& X)
        {
            _PayloadDataTable = X._PayloadDataTable;
            _FlatIndirectMap = X._FlatIndirectMap;
            return *this;
        }

        TF_RuntimeIdedDataTable(TF_RuntimeIdedDataTable&& X) noexcept :
            _PayloadDataTable(boost::move(X._PayloadDataTable)),
            _FlatIndirectMap(boost::move(X._FlatIndirectMap))
        {}
        TF_RuntimeIdedDataTable& operator = (TF_RuntimeIdedDataTable&& X) noexcept
        {
            _PayloadDataTable = boost::move(X._PayloadDataTable);
            _FlatIndirectMap = boost::move(X._FlatIndirectMap);
            return *this;
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
         * @return The ID associated with the new row
         */
        F_Id Add()
        {
            F_PayloadIndex PayloadIndex = static_cast<F_PayloadIndex>(_PayloadDataTable.GetSize());
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
    };
}