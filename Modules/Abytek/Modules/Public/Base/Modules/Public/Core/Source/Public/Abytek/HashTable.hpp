#pragma once

#include "Abytek/Base.Core.prerequisites.pch.hpp"
#include "Abytek/BoostContainerTemplates.hpp"
#include "Abytek/BasicMath.hpp"
#include "Abytek/Assert.hpp"
#include "Abytek/Allocator.hpp"


namespace Abytek::ContainerTemplates
{
    /**
     * @brief A hash table implementation for faster lookups
     * 
     * Uses open addressing with chaining to resolve collisions.
     * 
     * @tparam __F_HashCode The hash code type
     * @tparam __F_Index The index type
     * @tparam __F_Allocator The allocator type
     */
    template<typename __F_HashCode, typename __F_Index, class __F_Allocator>
    class TF_HashTable
    {
    public:
        using F_HashCode = __F_HashCode;
        using F_Index = __F_Index;
        using F_Allocator = __F_Allocator;

        using F_Vector = TF_Vector<F_Index, TF_RebindAllocator<F_Index, F_Allocator>>;
    
    private:
        F_Index _HashSize = 2;
        F_Index _IndexSize = 0;
        
        F_HashCode _HashMask = 0;

        F_Vector _HashVector;
        F_Vector _IndexVector;
        
    public:
        /** @brief Gets the size of the hash table */
        ABYTEK_FORCE_INLINE F_Index GetHashSize() const { return _HashSize; }
        
        /** @brief Gets the size of the index array */
        ABYTEK_FORCE_INLINE F_Index GetIndexSize() const { return _IndexSize; }
        
        /** @brief Gets the hash mask used for index calculations */
        ABYTEK_FORCE_INLINE F_HashCode GetHashMask() const { return _HashMask; }
        
        /** @brief Gets the hash vector containing bucket heads */
        ABYTEK_FORCE_INLINE const F_Vector& GetHashVector() const { return _HashVector; }
        
        /** @brief Gets the index vector containing the linked indices */
        ABYTEK_FORCE_INLINE const F_Vector& GetIndexVector() const { return _IndexVector; }
    
    public:
        /**
         * @brief Constructor 
         * @param HashSize The initial size of the hash table (must be power of two)
         * @param IndexSize The initial size of the index array
         * @param Allocator The allocator to use
         */
        TF_HashTable(F_Index HashSize = 2, F_Index IndexSize = 0, const F_Allocator& Allocator = {}) :
            _HashSize(HashSize),
            _IndexSize(IndexSize),

			_HashMask(HashSize - 1),

            _HashVector(Allocator),
            _IndexVector(Allocator)
        {
			ABYTEK_BASE_CORE_ASSERT(_HashSize > 0) << "hash size must be greater than zero";
			ABYTEK_BASE_CORE_ASSERT(IsPowerOfTwo((U32)_HashSize)) << "hash size must be power of two";
            
            if(HashSize > 0)
            {
                _HashVector.resize(_HashSize, ~F_Index(0));
            }
            if(IndexSize > 0)
            {
                _IndexVector.resize(_IndexSize, ~F_Index(0));
            }
        }
        
        /**
         * @brief Copy constructor
         * @param x The hash table to copy from
         */
        TF_HashTable(const TF_HashTable& x) :
            _HashSize(x._HashSize),
            _IndexSize(x._IndexSize),

            _HashMask(x._HashMask),

            _HashVector(x._HashVector),
            _IndexVector(x._IndexVector)
        {
        }
        
        /**
         * @brief Copy assignment operator
         * @param x The hash table to copy from
         * @return Reference to this hash table
         */
        TF_HashTable& operator = (const TF_HashTable& x)
        {
            _HashSize = x._HashSize;
            _IndexSize = x._IndexSize;

            _HashMask = x._HashMask;

            _HashVector = x._HashVector;
            _IndexVector = x._IndexVector;
            return *this;
        }
        
        /**
         * @brief Move constructor
         * @param x The hash table to move from
         */
        TF_HashTable(TF_HashTable&& x) :
            _HashSize(x._HashSize),
            _IndexSize(x._IndexSize),

            _HashMask(x._HashMask),

            _HashVector(std::move(x._HashVector)),
            _IndexVector(std::move(x._IndexVector))
        {
			x.Reset();
        }
        
        /**
         * @brief Move assignment operator
         * @param x The hash table to move from
         * @return Reference to this hash table
         */
        TF_HashTable& operator = (TF_HashTable&& x)
        {
            _HashSize = x._HashSize;
            _IndexSize = x._IndexSize;

            _HashMask = x._HashMask;

            _HashVector = std::move(x._HashVector);
            _IndexVector = std::move(x._IndexVector);
			x.Reset();
            return *this;
        }
        
        /**
         * @brief Destructor
         */
        ~TF_HashTable()
        {
            Reset();
        }        
        
    public:
        /**
         * @brief Resets the hash table to initial state
         */
        void Reset()
        {
            if(_IndexSize)
            {
                _HashVector.clear();
                _IndexVector.clear();
                
                _HashSize = 2;
                _IndexSize = 2;
            }
        }
        
        /**
         * @brief Clears all elements but keeps capacity
         */
        void Clear()
        {
            if(_IndexSize)
            {
                _HashVector.resize(_HashSize, 0xFFFFFFFF);
            }
        }
        
        /**
         * @brief Clears and resizes the hash table
         * @param HashSize The new hash size (must be power of two)
         * @param IndexSize The new index size
         */
        void Clear(F_Index HashSize, F_Index IndexSize)
        {
            Reset();
            
            _HashSize = HashSize;
            _IndexSize = IndexSize;
            
            ABYTEK_BASE_CORE_ASSERT(_HashSize > 0) << "hash size must be greater than zero";
            ABYTEK_BASE_CORE_ASSERT(IsPowerOfTwo((U32)_HashSize)) << "hash size must be power of two";
            
            if( _IndexSize )
            {
                _HashMask = _HashSize - 1;
                
                _HashVector.resize(_HashSize, 0xFFFFFFFF);
                _IndexVector.resize(_IndexSize, 0xFFFFFFFF);
            }
        }
        
        /**
         * @brief Resizes the index array
         * @param IndexSize The new index size
         */
        void Resize(F_Index IndexSize)
        {
            if( _IndexSize == IndexSize )
            {
                return;
            }

            if( IndexSize == 0 )
            {
                Reset();
                return;
            }

            if( _IndexSize == 0 )
            {
                _HashMask = (_HashSize - 1);
                _HashVector.resize(_HashSize, 0xFFFFFFFF);
            }
            
            _IndexVector.resize(IndexSize);

            _IndexSize = IndexSize;
        }
        
        /**
         * @brief Returns the first index for a given hash code
         * @param Key The hash code to lookup
         * @return The first index in the chain
         */
        ABYTEK_FORCE_INLINE F_Index First(F_HashCode Key) const
        {
            return _HashVector[Key & _HashMask];
        }
        
        /**
         * @brief Returns the next index in a chain
         * @param Index The current index
         * @return The next index in the chain
         */
        ABYTEK_FORCE_INLINE F_Index Next(F_Index Index) const
        {
            ABYTEK_BASE_CORE_ASSERT(Index < _IndexSize);
            ABYTEK_BASE_CORE_ASSERT(_IndexVector[Index] != Index);
            return _IndexVector[Index];
        }
        
        /**
         * @brief Checks if an index is valid
         * @param Index The index to check
         * @return True if the index is valid, false otherwise
         */
        ABYTEK_FORCE_INLINE B8 IsValid(F_Index Index) const
        {
            return Index != ~0u;
        }
        
        /**
         * @brief Checks if a hash code and index combination exists
         * @param HashCode The hash code to check
         * @param Index The index to find
         * @return True if the combination exists, false otherwise
         */
        B8 Has(F_HashCode HashCode, F_Index Index) const
        {
            for(F_Index It = First(HashCode); IsValid(It); It = Next(It))
            {
                if(It == Index)
                {
                    return true;
                }
            }
            return false;
        }
        
        /**
         * @brief Adds a hash code and index combination
         * @param HashCode The hash code to add
         * @param Index The index to add
         */
        void Add(F_HashCode HashCode, F_Index Index)
        {
            ABYTEK_DISABLE_ALL_WARNINGS_PUSH
            if( Index >= _IndexSize )
            {
                Resize(RoundUpToPowerOfTwo(static_cast<F32>(Index + 1)));
            }
            ABYTEK_DISABLE_ALL_WARNINGS_POP

            HashCode &= _HashMask;
            _IndexVector[Index] = _HashVector[HashCode];
            _HashVector[HashCode] = Index;
        }
        
        /**
         * @brief Adds a hash code and index combination with thread safety
         * @param HashCode The hash code to add
         * @param Index The index to add
         */
        void AddConcurrent(F_HashCode HashCode, F_Index Index)
        {
            ABYTEK_BASE_CORE_ASSERT(Index < _IndexSize);

            HashCode &= _HashMask;
            _IndexVector[Index] = ((AI32*)_HashVector.data())[HashCode].exchange(Index);
        }
        
        /**
         * @brief Removes a hash code and index combination
         * @param HashCode The hash code to remove
         * @param Index The index to remove
         */
        void Remove(F_HashCode HashCode, F_Index Index)
        {
            if(Index >= _IndexSize)
            {
                return;
            }

            HashCode &= _HashMask;

            if(_HashVector[HashCode] == Index)
            {
                // Head of chain
                _HashVector[HashCode] = _IndexVector[Index];
            }
            else
            {
                for(F_Index i = _HashVector[HashCode]; IsValid(i); i = _IndexVector[i])
                {
                    if(_IndexVector[i] == Index)
                    {
                        // Next = Next->Next
                        _IndexVector[i] = _IndexVector[Index];
                        break;
                    }
                }
            }
        }
    };
}