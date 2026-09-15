#pragma once

#include "Abytek/Base.Core.prerequisites.pch.hpp"
#include "Abytek/ReverseIterator.hpp"


namespace Abytek::ContainerTemplates
{
    /**
     * @brief A flat indirect mapping container that provides stable handles to elements
     *
     * TF_FlatIndirectMap implements a sparse-to-dense mapping with generational indices
     * for safe handle management. It allows O(1) lookups and maintains stable indices 
     * even after removals.
     *
     * @tparam __F_Allocator The allocator type used for memory management
     * @tparam __F_PayloadIndex The index type used for payload indices, defaults to U32
     */
    template<class __F_Allocator, typename __F_PayloadIndex = U32>
    struct TF_FlatIndirectMap
    {
    public:
        using F_Allocator = __F_Allocator;
        using F_PayloadIndex = __F_PayloadIndex;
        static constexpr F_PayloadIndex INVALID_PAYLOAD_INDEX = ~F_PayloadIndex(0);
        
    public:
        using F_Generation = U32;
        using F_SparseIndex = U32;
        static constexpr F_SparseIndex INVALID_SPARSE_INDEX = ~F_SparseIndex(0);
        
        /**
         * @brief Metadata element storing generation and free list information
         */
        struct F_MetadataElement
        {
            F_Generation Generation = 0;
            F_SparseIndex NextFreeSparseIndex = INVALID_SPARSE_INDEX;
        };
        
        /**
         * @brief Stable handle to an element in the container
         * 
         * Combines a generation counter with a sparse index for safe referencing
         */
        struct F_Id
        {
            F_Generation Generation = 0;
            F_SparseIndex SparseIndex = INVALID_SPARSE_INDEX;
        };

    public:
        using F_Iterator = typename TF_Vector<F_MetadataElement, TF_RebindAllocator<F_MetadataElement, F_Allocator>>::iterator;
        using F_ConstIterator = typename TF_Vector<F_MetadataElement, TF_RebindAllocator<F_MetadataElement, F_Allocator>>::const_iterator;
        using F_ReverseIterator = TF_ReverseIterator<F_Iterator>;
        using F_ConstReverseIterator = TF_ReverseIterator<F_ConstIterator>;
        
    private:
        F_SparseIndex _SparseSize = 0;
        F_SparseIndex _HeadFreeSparseIndex = ABYTEK_U32_MAX;
        F_SparseIndex _FreeSparseCount = 0;
        TF_Vector<F_PayloadIndex, TF_RebindAllocator<F_PayloadIndex, F_Allocator>> _PayloadIndices;
        TF_Vector<F_MetadataElement, TF_RebindAllocator<F_MetadataElement, F_Allocator>> _Metadata;

    public:
        /**
         * @brief Gets the current size of the sparse array
         * @return The sparse array size
         */
        ABYTEK_FORCE_INLINE F_SparseIndex GetSparseSize() const noexcept
        {
            return _SparseSize;
        }
        
        /**
         * @brief Gets the index of the first free element in the sparse array
         * @return The head free sparse index
         */
        ABYTEK_FORCE_INLINE F_SparseIndex GetHeadFreeSparseIndex() const noexcept
        {
            return _HeadFreeSparseIndex;
        }
        
        /**
         * @brief Gets the count of free elements in the sparse array
         * @return The free sparse count
         */
        ABYTEK_FORCE_INLINE F_SparseIndex GetFreeSparseCount() const noexcept
        {
            return _FreeSparseCount;
        }
        
        /**
         * @brief Gets the payload indices vector
         * @return Reference to the payload indices vector
         */
        ABYTEK_FORCE_INLINE auto& GetPayloadIndices() noexcept
        {
            return _PayloadIndices;
        }
        
        /**
         * @brief Gets the payload indices vector (const version)
         * @return Const reference to the payload indices vector
         */
        ABYTEK_FORCE_INLINE const auto& GetPayloadIndices() const noexcept
        {
            return _PayloadIndices;
        }
        
        /**
         * @brief Gets the metadata vector
         * @return Const reference to the metadata vector
         */
        ABYTEK_FORCE_INLINE const auto& GetMetadata() const noexcept
        {
            return _Metadata;
        }

    public:
        /** @brief Default constructor */
        ABYTEK_FORCE_INLINE TF_FlatIndirectMap() noexcept = default;
        
        /** @brief Default destructor */
        ABYTEK_FORCE_INLINE ~TF_FlatIndirectMap() noexcept = default;

        /**
         * @brief Copy constructor
         * @param X The container to copy from
         */
        TF_FlatIndirectMap(const TF_FlatIndirectMap& X) :
            _SparseSize(X._SparseSize),
            _HeadFreeSparseIndex(X._HeadFreeSparseIndex),
            _FreeSparseCount(X._FreeSparseCount),
            _PayloadIndices(X._PayloadIndices),
            _Metadata(X._Metadata)
        {}
        
        /**
         * @brief Copy assignment operator
         * @param X The container to copy from
         * @return Reference to this container
         */
        TF_FlatIndirectMap& operator = (const TF_FlatIndirectMap& X)
        {
            _SparseSize = X._SparseSize;
            _HeadFreeSparseIndex = X._HeadFreeSparseIndex;
            _FreeSparseCount = X._FreeSparseCount;
            _PayloadIndices = X._PayloadIndices;
            _Metadata = X._Metadata;
            return *this;
        }

        /**
         * @brief Move constructor
         * @param X The container to move from
         */
        TF_FlatIndirectMap(TF_FlatIndirectMap&& X) noexcept :
            _SparseSize(X._SparseSize),
            _HeadFreeSparseIndex(X._HeadFreeSparseIndex),
            _FreeSparseCount(X._FreeSparseCount),
            _PayloadIndices(boost::move(X._PayloadIndices)),
            _Metadata(boost::move(X._Metadata))
        {
            X._SparseSize = 0;
            X._HeadFreeSparseIndex = INVALID_SPARSE_INDEX;
            X._FreeSparseCount = 0;
        }
        
        /**
         * @brief Move assignment operator
         * @param X The container to move from
         * @return Reference to this container
         */
        TF_FlatIndirectMap& operator = (TF_FlatIndirectMap&& X) noexcept
        {
            _SparseSize = X._SparseSize;
            _HeadFreeSparseIndex = X._HeadFreeSparseIndex;
            _FreeSparseCount = X._FreeSparseCount;
            _PayloadIndices = boost::move(X._PayloadIndices);
            _Metadata = boost::move(X._Metadata);
            
            X._SparseSize = 0;
            X._HeadFreeSparseIndex = INVALID_SPARSE_INDEX;
            X._FreeSparseCount = 0;
            return *this;
        }
        
    public:
        /**
         * @brief Checks if an ID is valid
         * @param Id The ID to check
         * @return True if the ID is valid, false otherwise
         */
        ABYTEK_FORCE_INLINE B8 IsValidId(F_Id Id) const noexcept
        {
            if(Id.SparseIndex >= GetSparseSize())
            {
                return false;
            }
            return (Id.Generation == _Metadata[Id.SparseIndex].Generation);
        }
        
        /**
         * @brief Checks if a sparse index has a valid payload
         * @param SparseIndex The sparse index to check
         * @return True if the sparse index has a valid payload, false otherwise
         */
        ABYTEK_FORCE_INLINE B8 HasPayload(F_SparseIndex SparseIndex) const noexcept
        {
            if(SparseIndex >= GetSparseSize())
            {
                return false;
            }
            return _PayloadIndices[SparseIndex] != INVALID_PAYLOAD_INDEX;
        }
        
        /**
         * @brief Converts an ID to its payload index
         * @param Id The ID to convert
         * @return The payload index
         */
        ABYTEK_FORCE_INLINE F_PayloadIndex IdToPayloadIndex(F_Id Id) const noexcept
        {
            ABYTEK_BASE_CORE_ASSERT(IsValidId(Id)) << "invalid id";
            return SparseIndexToPayloadIndex(Id.SparseIndex);
        }
        
        /**
         * @brief Converts a sparse index to its payload index
         * @param SparseIndex The sparse index to convert
         * @return The payload index
         */
        ABYTEK_FORCE_INLINE F_PayloadIndex SparseIndexToPayloadIndex(F_SparseIndex SparseIndex) const noexcept
        {
            ABYTEK_BASE_CORE_ASSERT(HasPayload(SparseIndex)) << "invalid sparse index";
            return _PayloadIndices[SparseIndex];
        }
        
        /**
         * @brief Adds a new payload to the container
         * @param PayloadIndex The payload index to add
         * @return A new ID that references the added payload
         */
        F_Id Add(F_PayloadIndex PayloadIndex)
        {
            ABYTEK_BASE_CORE_ASSERT(PayloadIndex != INVALID_PAYLOAD_INDEX) << "invalid payload index";
            
            F_Id Id;
            
            F_SparseIndex SparseIndex;
            
            if(_HeadFreeSparseIndex == INVALID_SPARSE_INDEX)
            {
                SparseIndex = _SparseSize;

                _PayloadIndices.resize(SparseIndex + 1);
                _Metadata.resize(SparseIndex + 1);
                ++_SparseSize;

                Id.Generation = 0;
            }
            else
            {
                SparseIndex = _HeadFreeSparseIndex;

                auto& MetadataElement = _Metadata[SparseIndex];
                
                Id.Generation = MetadataElement.Generation;
                
                _HeadFreeSparseIndex = MetadataElement.NextFreeSparseIndex;

                --_FreeSparseCount;
            }

            Id.SparseIndex = SparseIndex;
            _PayloadIndices[SparseIndex] = PayloadIndex;

            return Id;
        }
        
        /**
         * @brief Tries to remove an element by ID
         * @param Id The ID to remove
         * @return True if the element was removed, false if the ID was invalid
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
         * @brief Removes an element by ID
         * @param Id The ID to remove
         */
        void Remove(F_Id Id)
        {
            ABYTEK_BASE_CORE_ASSERT(IsValidId(Id)) << "invalid Id";
            RemoveUnsafe(Id.SparseIndex);
        }
        
        /**
         * @brief Removes an element by sparse index without validation
         * @param SparseIndex The sparse index to remove
         */
        void RemoveUnsafe(F_SparseIndex SparseIndex)
        {
            ABYTEK_BASE_CORE_ASSERT(SparseIndex < _SparseSize) << "invalid SparseIndex";

            auto& MetadataElement = _Metadata[SparseIndex];

            MetadataElement.NextFreeSparseIndex = _HeadFreeSparseIndex;
            _HeadFreeSparseIndex = SparseIndex;
            
            ++MetadataElement.Generation;

            _PayloadIndices[SparseIndex] = INVALID_PAYLOAD_INDEX;

            ++_FreeSparseCount;
        }
        
        /**
         * @brief Compacts the container to reduce memory usage
         * @param SortFreeMetadataElements Whether to sort free metadata elements
         */
        void Compact(B8 SortFreeMetadataElements = true)
        {
            if(!_FreeSparseCount)
            {
                return;
            }

            if(SortFreeMetadataElements)
            {
                TF_Vector<F_SparseIndex, TF_RebindAllocator<F_SparseIndex, F_Allocator>> InitialFreeSparseIndices(_FreeSparseCount);
                TF_Vector<F_SparseIndex, TF_RebindAllocator<F_SparseIndex, F_Allocator>> SortedFreeSparseIndices(_FreeSparseCount);
                TF_Vector<F_MetadataElement, TF_RebindAllocator<F_MetadataElement, F_Allocator>> SortedFreeMetadataElements(_FreeSparseCount);

                // Setup InitialFreeSparseIndices
                {
                    F_SparseIndex SparseIndex = _HeadFreeSparseIndex;
                    F_SparseIndex LocalIndexOfFreeSparseIndex = 0;
                    while(SparseIndex != INVALID_SPARSE_INDEX)
                    {
                        InitialFreeSparseIndices[LocalIndexOfFreeSparseIndex] = SparseIndex;
                        SparseIndex = _Metadata[SparseIndex].NextFreeSparseIndex;
                        ++LocalIndexOfFreeSparseIndex;
                    }
                }

                // Setup SortedFreeSparseIndices
                {
                    SortedFreeSparseIndices = InitialFreeSparseIndices;
                    
                    auto Compare = [this](F_SparseIndex SparseIndex1, F_SparseIndex SparseIndex2) -> B8
                    {
                        return SparseIndex1 < SparseIndex2;
                    };
                    boost::range::sort(SortedFreeSparseIndices, Compare);
                }

                // Setup SortedFreeMetadataElements
                for(F_SparseIndex LocalIndexOfFreeSparseIndex = 0; LocalIndexOfFreeSparseIndex < _FreeSparseCount; ++LocalIndexOfFreeSparseIndex)
                {
                    F_SparseIndex SortedSparseIndex = SortedFreeSparseIndices[LocalIndexOfFreeSparseIndex];
                    SortedFreeMetadataElements[LocalIndexOfFreeSparseIndex] = _Metadata[SortedSparseIndex];
                }

                // Update _Metadata
                _HeadFreeSparseIndex = INVALID_SPARSE_INDEX;
                for(F_SparseIndex LocalIndexOfFreeSparseIndex = 0; LocalIndexOfFreeSparseIndex < _FreeSparseCount; ++LocalIndexOfFreeSparseIndex)
                {
                    F_SparseIndex InitialSparseIndex = InitialFreeSparseIndices[LocalIndexOfFreeSparseIndex];
                    F_SparseIndex SortedSparseIndex = SortedFreeSparseIndices[LocalIndexOfFreeSparseIndex];

                    auto& TargetMetadataElement = _Metadata[InitialSparseIndex];
                    TargetMetadataElement = SortedFreeMetadataElements[LocalIndexOfFreeSparseIndex];
                    TargetMetadataElement.NextFreeSparseIndex = _HeadFreeSparseIndex;
                    
                    if(_HeadFreeSparseIndex == INVALID_SPARSE_INDEX)
                    {
                        _HeadFreeSparseIndex = SortedSparseIndex;
                    }
                }
            }

            // Calculate minimum sparse size
            F_SparseIndex MinSparseSize;
            {
                if(_PayloadIndices[_FreeSparseCount - 1] == INVALID_SPARSE_INDEX)
                {
                    MinSparseSize = 0;
                    
                    F_SparseIndex SparseIndex = _HeadFreeSparseIndex;
                    while(SparseIndex != INVALID_SPARSE_INDEX)
                    {
                        if(SparseIndex)
                        {
                            if(_PayloadIndices[SparseIndex - 1] != INVALID_SPARSE_INDEX)
                            {
                                MinSparseSize = (MinSparseSize > SparseIndex) ? MinSparseSize : SparseIndex;
                            }
                        }
                    
                        SparseIndex = _Metadata[SparseIndex].NextFreeSparseIndex;
                    }
                }
                else
                {
                    MinSparseSize = _FreeSparseCount;
                }
            }

            // Resize sparse
            _FreeSparseCount = MinSparseSize;
            _Metadata.resize(_FreeSparseCount);
            _PayloadIndices.resize(_FreeSparseCount);
        }

    public:
        /**
         * @brief Gets the begin iterator
         * @return The begin iterator
         */
        ABYTEK_FORCE_INLINE F_Iterator GetBegin() noexcept
        {
            return _PayloadIndices.begin();
        }
        
        /**
         * @brief Gets the begin iterator (const version)
         * @return The begin iterator
         */
        ABYTEK_FORCE_INLINE F_ConstIterator GetBegin() const noexcept
        {
            return _PayloadIndices.begin();
        }
        
        /**
         * @brief Gets the end iterator
         * @return The end iterator
         */
        ABYTEK_FORCE_INLINE F_Iterator GetEnd() noexcept
        {
            return _PayloadIndices.end();
        }
        
        /**
         * @brief Gets the end iterator (const version)
         * @return The end iterator
         */
        ABYTEK_FORCE_INLINE F_ConstIterator GetEnd() const noexcept
        {
            return _PayloadIndices.end();
        }

    public:
        /** @brief Standard begin iterator */
        ABYTEK_FORCE_INLINE F_Iterator begin() noexcept
        {
            return GetBegin();
        }
        
        /** @brief Standard begin const iterator */
        ABYTEK_FORCE_INLINE F_ConstIterator begin() const noexcept
        {
            return GetBegin();
        }
        
        /** @brief Standard const begin iterator */
        ABYTEK_FORCE_INLINE F_ConstIterator cbegin() const noexcept
        {
            return GetBegin();
        }
        
        /** @brief Standard end iterator */
        ABYTEK_FORCE_INLINE F_Iterator end() noexcept
        {
            return GetEnd();
        }
        
        /** @brief Standard end const iterator */
        ABYTEK_FORCE_INLINE F_ConstIterator end() const noexcept
        {
            return GetEnd();
        }
        
        /** @brief Standard const end iterator */
        ABYTEK_FORCE_INLINE F_ConstIterator cend() const noexcept
        {
            return GetEnd();
        }

    public:
        /**
         * @brief Gets the reverse begin iterator
         * @return The reverse begin iterator
         */
        ABYTEK_FORCE_INLINE F_ReverseIterator GetReverseBegin() noexcept
        {
            return MakeReverseIterator(GetEnd());
        }
        
        /**
         * @brief Gets the reverse begin iterator (const version)
         * @return The reverse begin iterator
         */
        ABYTEK_FORCE_INLINE F_ConstReverseIterator GetReverseBegin() const noexcept
        {
            return MakeReverseIterator(GetEnd());
        }
        
        /**
         * @brief Gets the reverse end iterator
         * @return The reverse end iterator
         */
        ABYTEK_FORCE_INLINE F_ReverseIterator GetReverseEnd() noexcept
        {
            return MakeReverseIterator(GetBegin());
        }
        
        /**
         * @brief Gets the reverse end iterator (const version)
         * @return The reverse end iterator
         */
        ABYTEK_FORCE_INLINE F_ConstReverseIterator GetReverseEnd() const noexcept
        {
            return MakeReverseIterator(GetBegin());
        }

    public:
        /** @brief Standard reverse begin iterator */
        ABYTEK_FORCE_INLINE F_ReverseIterator rbegin() noexcept
        {
            return GetReverseBegin();
        }
        
        /** @brief Standard reverse begin const iterator */
        ABYTEK_FORCE_INLINE F_ConstReverseIterator rbegin() const noexcept
        {
            return GetReverseBegin();
        }
        
        /** @brief Standard const reverse begin iterator */
        ABYTEK_FORCE_INLINE F_ConstReverseIterator crbegin() const noexcept
        {
            return GetReverseBegin();
        }
        
        /** @brief Standard reverse end iterator */
        ABYTEK_FORCE_INLINE F_ReverseIterator rend() noexcept
        {
            return GetReverseEnd();
        }
        
        /** @brief Standard reverse end const iterator */
        ABYTEK_FORCE_INLINE F_ConstReverseIterator rend() const noexcept
        {
            return GetReverseEnd();
        }
        
        /** @brief Standard const reverse end iterator */
        ABYTEK_FORCE_INLINE F_ConstReverseIterator crend() const noexcept
        {
            return GetReverseEnd();
        }
    };
}
