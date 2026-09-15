#pragma once

#include "Abytek/Base.PureDOD.prerequisites.pch.hpp"
#include "Abytek/RuntimeVector.hpp"

/**
 * @file RuntimePayloadDataTable.hpp
 * 
 * @brief Provides a runtime-defined, data-oriented table container.
 * 
 * RuntimePayloadDataTable is similar to PayloadDataTable but with column types and structure
 * determined at runtime rather than compile time. This allows for more flexibility in dynamic
 * scenarios where the schema isn't known until runtime.
 * 
 * Use this container when you need:
 * - A table structure with columns of different types
 * - Schema flexibility determined at runtime
 * - Data-oriented design with efficient memory access patterns
 */

namespace Abytek::ContainerTemplates
{
    /**
     * @brief A table container with column types determined at runtime.
     * 
     * TF_RuntimePayloadDataTable implements a table-like structure where column types and 
     * structure are determined at runtime. Unlike TF_PayloadDataTable, this class does not
     * provide compile-time type safety for column access, but offers more flexibility
     * for dynamic schema creation.
     * 
     * @tparam __F_Allocator The allocator type used for memory management
     */
    template<class __F_Allocator>
    struct TF_RuntimePayloadDataTable
    {
    public:
        using F_Allocator = __F_Allocator;
        
    public:
        using F_Column = TF_RuntimeVector<F_Allocator>;
        using F_Columns = ContainerTemplates::TF_Vector<F_Column, TF_RebindAllocator<F_Column, F_Allocator>>;
        
    private:
        F_Columns _Columns;

    public:
        /**
         * @brief Gets the number of rows in the table
         * @return The size of the table (number of rows)
         */
        ABYTEK_FORCE_INLINE Sz GetSize() const noexcept
        {
            return _Columns[0].GetSize();
        }
        /**
         * @brief Gets the current capacity of the table
         * @return The capacity of the table (number of rows that can be stored without reallocation)
         */
        ABYTEK_FORCE_INLINE Sz GetCapacity() const noexcept
        {
            return _Columns[0].GetCapacity();
        }
        /**
         * @brief Gets the array of columns that make up the table
         * @return Reference to the columns vector
         */
        ABYTEK_FORCE_INLINE auto& GetColumns() noexcept
        {
            return _Columns;
        }
        /**
         * @brief Gets the array of columns that make up the table (const version)
         * @return Const reference to the columns vector
         */
        ABYTEK_FORCE_INLINE const auto& GetColumns() const noexcept
        {
            return _Columns;
        }
        /**
         * @brief Gets an iterator to the beginning of a specific column
         * @param ColumnIndex The index of the column
         * @return Iterator to the first element of the column
         */
        ABYTEK_FORCE_INLINE auto GetColumnBegin(I32 ColumnIndex) noexcept
        {
            return _Columns[ColumnIndex].begin();
        }
        /**
         * @brief Gets a const iterator to the beginning of a specific column
         * @param ColumnIndex The index of the column
         * @return Const iterator to the first element of the column
         */
        ABYTEK_FORCE_INLINE auto GetColumnBegin(I32 ColumnIndex) const noexcept
        {
            return _Columns[ColumnIndex].begin();
        }
        /**
         * @brief Gets an iterator to the end of a specific column
         * @param ColumnIndex The index of the column
         * @return Iterator one past the last element of the column
         */
        ABYTEK_FORCE_INLINE auto GetColumnEnd(I32 ColumnIndex) noexcept
        {
            return _Columns[ColumnIndex].end();
        }
        /**
         * @brief Gets a const iterator to the end of a specific column
         * @param ColumnIndex The index of the column
         * @return Const iterator one past the last element of the column
         */
        ABYTEK_FORCE_INLINE auto GetColumnEnd(I32 ColumnIndex) const noexcept
        {
            return _Columns[ColumnIndex].end();
        }
        /**
         * @brief Gets a reverse iterator to the beginning of a specific column
         * @param ColumnIndex The index of the column
         * @return Reverse iterator to the first element in reverse order
         */
        ABYTEK_FORCE_INLINE auto GetColumnReverseBegin(I32 ColumnIndex) noexcept
        {
            return _Columns[ColumnIndex].rbegin();
        }
        /**
         * @brief Gets a const reverse iterator to the beginning of a specific column
         * @param ColumnIndex The index of the column
         * @return Const reverse iterator to the first element in reverse order
         */
        ABYTEK_FORCE_INLINE auto GetColumnReverseBegin(I32 ColumnIndex) const noexcept
        {
            return _Columns[ColumnIndex].rbegin();
        }
        /**
         * @brief Gets a reverse iterator to the end of a specific column
         * @param ColumnIndex The index of the column
         * @return Reverse iterator one past the last element in reverse order
         */
        ABYTEK_FORCE_INLINE auto GetColumnReverseEnd(I32 ColumnIndex) noexcept
        {
            return _Columns[ColumnIndex].rend();
        }
        /**
         * @brief Gets a const reverse iterator to the end of a specific column
         * @param ColumnIndex The index of the column
         * @return Const reverse iterator one past the last element in reverse order
         */
        ABYTEK_FORCE_INLINE auto GetColumnReverseEnd(I32 ColumnIndex) const noexcept
        {
            return _Columns[ColumnIndex].rend();
        }

    public:
        ABYTEK_FORCE_INLINE TF_RuntimePayloadDataTable() noexcept = default;
        ABYTEK_FORCE_INLINE ~TF_RuntimePayloadDataTable() noexcept = default;

        /**
         * @brief Constructs a table with predefined columns
         * @param Columns The columns to initialize the table with
         */
        TF_RuntimePayloadDataTable(F_Columns&& Columns) :
            _Columns(boost::move(Columns))
        {
        }

        TF_RuntimePayloadDataTable(const TF_RuntimePayloadDataTable& X) :
            _Columns(X._Columns)
        {}
        TF_RuntimePayloadDataTable& operator = (const TF_RuntimePayloadDataTable& X)
        {
            _Columns = X._Columns;
            return *this;
        }

        TF_RuntimePayloadDataTable(TF_RuntimePayloadDataTable&& X) noexcept :
            _Columns(boost::move(X._Columns))
        {}
        TF_RuntimePayloadDataTable& operator = (TF_RuntimePayloadDataTable&& X) noexcept
        {
            _Columns = boost::move(X._Columns);
            return *this;
        }

        /**
         * @brief Constructs a table with a specified initial size
         * @param Size The initial number of rows in the table
         */
        TF_RuntimePayloadDataTable(Sz Size)
        {
            Resize(Size);
        }
        
    public:
        /**
         * @brief Resizes the table to the specified number of rows
         * @param Size The new size of the table
         */
        ABYTEK_FORCE_INLINE void Resize(Sz Size)
        {
            for(auto& Column : _Columns)
            {
                Column.Resize(Size);
            }
        }
        /**
         * @brief Reserves capacity for a specified number of rows
         * @param Capacity The number of rows to reserve capacity for
         */
        ABYTEK_FORCE_INLINE void Reserve(Sz Capacity)
        {
            for(auto& Column : _Columns) 
            {
                Column.Reserve(Capacity);
            }
        }
        /**
         * @brief Reduces the capacity to match the size, minimizing memory usage
         */
        ABYTEK_FORCE_INLINE void ShrinkToFit()
        {
            for(auto& Column : _Columns)
            {
                Column.ShrinkToFit();
            }
        }
        
    public:
        /**
         * @brief Adds multiple rows to the table and returns the index of the first new row
         * @param Num The number of rows to add
         * @return Index of the first of the newly added rows
         */
        ABYTEK_FORCE_INLINE PDiff AddRange(Sz Num)
        {
            Resize(GetSize() + Num);
            return GetSize() - Num;
        }
        /**
         * @brief Adds a single row to the table and returns its index
         * @return Index of the newly added row
         */
        ABYTEK_FORCE_INLINE PDiff Add()
        {
            Resize(GetSize() + 1);
            return GetSize() - 1;
        }
        
    public:
        /**
         * @brief Inserts a new row at a specified position
         * @param LocationIdx The index where the new row should be inserted
         */
        ABYTEK_FORCE_INLINE void Insert(PDiff LocationIdx)
        {
            for(auto& Column : _Columns)
            {
                Column.Insert(Column.GetBegin() + LocationIdx);
            }
        }
        /**
         * @brief Inserts multiple new rows at a specified position
         * @param LocationIdx The index where the new rows should be inserted
         * @param Count The number of rows to insert
         */
        ABYTEK_FORCE_INLINE void Insert(PDiff LocationIdx, Sz Count)
        {
            for(auto& Column : _Columns)
            {
                Column.Insert(Column.GetBegin() + LocationIdx, Count);
            }
        }
        /**
         * @brief Erases a row at a specified position
         * @param LocationIdx The index of the row to erase
         */
        ABYTEK_FORCE_INLINE void Erase(PDiff LocationIdx)
        {
            for(auto& Column : _Columns)
            {
                Column.Erase(Column.GetBegin() + LocationIdx);
            }
        }
        
    public:
        /**
         * @brief Swaps two rows in the table
         * @param A The index of the first row
         * @param B The index of the second row
         */
        ABYTEK_FORCE_INLINE void Swap(PDiff A, PDiff B)
        {
            for(auto& Column : _Columns)
            {
                Column.Swap(Column.GetBegin() + A, Column.GetBegin() + B);
            }
        }
        /**
         * @brief Moves the contents of one row to another row
         * @param DstIdx The index of the destination row
         * @param SrcIdx The index of the source row
         */
        ABYTEK_FORCE_INLINE void Move(PDiff DstIdx, PDiff SrcIdx)
        {
            for(auto& Column : _Columns)
            {
                Column.Move(Column.GetBegin() + DstIdx, Column.GetBegin() + SrcIdx);
            }
        }
    };
}