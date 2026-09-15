/**
 * @file StringId.hpp
 * @brief Provides string ID support with efficient string pooling and lookup capabilities.
 * 
 * This file implements a string ID system with a central string database for efficient
 * string storage, comparison, and lookup. It helps reduce memory overhead and
 * improves performance for string-based operations by using hash codes.
 */
#pragma once

#include "Abytek/Base.Core.prerequisites.pch.hpp"
#include "Abytek/Assert.hpp"
#include "Abytek/Allocator.hpp"
#include "Abytek/BoostContainerTemplates.hpp"
#include "Abytek/HashTable.hpp"
#include "Abytek/HashBase.hpp"
#include "Abytek/DefaultAllocators.hpp"
#include "Abytek/ScopeLock.hpp"
#include "Abytek/SpinLock.hpp"


namespace Abytek::ContainerTemplates
{
	struct F_StringIdOption
	{
		static constexpr B8 EnableCounter = true;
	};
	
	/**
	 * @brief Forward declaration of string database template class
	 */
	template<class __F_StringDatabase, typename __F_Char, typename __F_Option, class __F_Allocator = WithRawAllocator::TF_DefaultAllocator<U8>>
	class TA_StringDatabase;
}

namespace Abytek
{
	/**
	 * @brief String identifier class that provides efficient string reference and comparison
	 * 
	 * TF_StringId provides a lightweight way to reference strings stored in a central database.
	 * It uses hash codes for fast comparison and memory-efficient storage.
	 * 
	 * @tparam __F_StringDatabase The database class that stores the actual string data
	 */
	template<class __F_StringDatabase>
	class TF_StringId
	{
	public:
		using F_StringDatabase = __F_StringDatabase;
		
    public:
		using F_Char = typename F_StringDatabase::F_Char;
		using F_Option = typename F_StringDatabase::F_Option;
		using F_Allocator = typename F_StringDatabase::F_Allocator;
		using F_String = typename F_StringDatabase::F_String;
		using F_StringView = typename F_StringDatabase::F_StringView;

	public:
		using F_StringHashCode = U64;
		static constexpr F_StringHashCode INVALID_STRING_HASH_CODE = ~F_StringHashCode(0);
		using F_StringIndex = U32;
		static constexpr F_StringIndex INVALID_STRING_INDEX = ~F_StringIndex(0);
		
	private:
		F_StringHashCode _HashCode = INVALID_STRING_HASH_CODE;
		F_StringIndex _Index = INVALID_STRING_INDEX;
#ifdef ABYTEK_BASE_CORE_ENABLE_STRING_ID_DEBUGGER
		F_String _DebugValue;
#endif

	public:
		/**
		 * @brief Get the hash code of the string
		 * @return The string's hash code
		 */
		ABYTEK_FORCE_INLINE F_StringHashCode GetHashCode() const noexcept
		{
			return _HashCode;
		}
		
		/**
		 * @brief Get the index of the string in the database
		 * @return The string's index in the database
		 */
		ABYTEK_FORCE_INLINE F_StringIndex GetIndex() const noexcept
		{
			return _Index;
		}

	public:
		/**
		 * @brief Default constructor, creates an invalid string ID
		 */
		ABYTEK_FORCE_INLINE TF_StringId() noexcept = default;
		ABYTEK_FORCE_INLINE ~TF_StringId() noexcept
		{
			Reset();
		}
		
		/**
		 * @brief Constructor from string view and pre-computed hash code
		 * @param StringView The string view to register
		 * @param HashCode Pre-computed hash code for the string
		 */
		ABYTEK_FORCE_INLINE TF_StringId(F_StringView StringView, F_StringHashCode HashCode) noexcept :
			_HashCode(HashCode),
			_Index(F_StringDatabase::GetInstance().Register(StringView, HashCode))
		{
#ifdef ABYTEK_BASE_CORE_ENABLE_STRING_ID_DEBUGGER
			_DebugValue = StringView;
#endif
		}
		
		/**
		 * @brief Constructor from C-style string
		 * @param StringArray Null-terminated string array
		 */
		ABYTEK_FORCE_INLINE TF_StringId(const F_Char* StringArray) noexcept
		{
			if (StringArray == nullptr)
			{
				return;
			}
			if (*StringArray == F_Char(0))
			{
				return;
			}
			_HashCode = HashStringView(StringArray);
			_Index = F_StringDatabase::GetInstance().Register(StringArray, _HashCode);
#ifdef ABYTEK_BASE_CORE_ENABLE_STRING_ID_DEBUGGER
			_DebugValue = StringArray;
#endif
		}

        template<auto __N>
		ABYTEK_FORCE_INLINE TF_StringId(const F_Char (&StringArray)[__N]) noexcept
		{
			if constexpr (__N == 0)
			{
				return;
			}
			_HashCode = HashStringView(StringArray);
			_Index = F_StringDatabase::GetInstance().Register(StringArray, _HashCode);
#ifdef ABYTEK_BASE_CORE_ENABLE_STRING_ID_DEBUGGER
			_DebugValue = StringArray;
#endif
		}
		
		/**
		 * @brief Constructor from string view
		 * @param StringView The string view to register
		 */
		ABYTEK_FORCE_INLINE explicit TF_StringId(F_StringView StringView) noexcept
		{
			if (StringView.size() == 0)
			{
				return;
			}
			_HashCode = HashStringView(StringView);
			_Index = F_StringDatabase::GetInstance().Register(StringView, _HashCode);
#ifdef ABYTEK_BASE_CORE_ENABLE_STRING_ID_DEBUGGER
			_DebugValue = StringView;
#endif
		}
		
		/**
		 * @brief Constructor from string
		 * @param String The string to register
		 */
		ABYTEK_FORCE_INLINE TF_StringId(const F_String& String) noexcept
		{
			if (String.size() == 0)
			{
				return;
			}
			_HashCode = HashStringView(String.c_str());
			_Index = F_StringDatabase::GetInstance().Register(String.c_str(), _HashCode);
#ifdef ABYTEK_BASE_CORE_ENABLE_STRING_ID_DEBUGGER
			_DebugValue = String;
#endif
		}
		
		/**
		 * @brief Copy constructor
		 * @param X The string ID to copy
		 */
		ABYTEK_FORCE_INLINE TF_StringId(const TF_StringId& X) :
			_HashCode(X._HashCode),
			_Index(X._Index)
		{
#ifdef ABYTEK_BASE_CORE_ENABLE_STRING_ID_DEBUGGER
			_DebugValue = X._DebugValue;
#endif
			if constexpr (F_Option::EnableCounter)
			{
				if (*this)
				{
					F_StringDatabase::GetInstance().IncreaseCounter(_Index);
				}
			}
		}
		
		/**
		 * @brief Copy assignment operator
		 * @param X The string ID to copy
		 * @return Reference to this object
		 */
		ABYTEK_FORCE_INLINE TF_StringId& operator = (const TF_StringId& X)
		{
			_HashCode = X._HashCode;
			_Index = X._Index;
#ifdef ABYTEK_BASE_CORE_ENABLE_STRING_ID_DEBUGGER
			_DebugValue = X._DebugValue;
#endif
			if constexpr (F_Option::EnableCounter)
			{
				if (*this)
				{
					F_StringDatabase::GetInstance().IncreaseCounter(_Index);
				}
			}
            return *this;
		}
		
		/**
		 * @brief Move constructor
		 * @param X The string ID to move from
		 */
		ABYTEK_FORCE_INLINE TF_StringId(TF_StringId&& X) noexcept :
			_HashCode(X._HashCode),
			_Index(X._Index)
		{
#ifdef ABYTEK_BASE_CORE_ENABLE_STRING_ID_DEBUGGER
			_DebugValue = X._DebugValue;
#endif
			X._HashCode = INVALID_STRING_HASH_CODE;
			X._Index = INVALID_STRING_INDEX;
			
			X.Reset();
		}
		
		/**
		 * @brief Move assignment operator
		 * @param X The string ID to move from
		 * @return Reference to this object
		 */
		ABYTEK_FORCE_INLINE TF_StringId& operator = (TF_StringId&& X) noexcept
		{
			_HashCode = X._HashCode;
			_Index = X._Index;
#ifdef ABYTEK_BASE_CORE_ENABLE_STRING_ID_DEBUGGER
			_DebugValue = X._DebugValue;
#endif

			X._HashCode = INVALID_STRING_HASH_CODE;
			X._Index = INVALID_STRING_INDEX;
			
			X.Reset();
            return *this;
		}

	public:
		/**
		 * @brief Quick check if the string ID is valid
		 * @return true if both hash code and index are valid
		 */
		ABYTEK_FORCE_INLINE B8 Q_IsValid() const noexcept
		{
			return (_HashCode != INVALID_STRING_HASH_CODE) && (_Index != INVALID_STRING_INDEX);
		}
		
		/**
		 * @brief Quick check if the string ID is null
		 * @return true if either hash code or index is invalid
		 */
		ABYTEK_FORCE_INLINE B8 Q_IsNull() const noexcept
		{
			return (_HashCode == INVALID_STRING_HASH_CODE) || (_Index == INVALID_STRING_INDEX);
		}
		
		/**
		 * @brief Non-quick check if the string ID is valid (verifies with the database)
		 * @return true if the ID exists in the database
		 */
		ABYTEK_FORCE_INLINE B8 NQ_IsValid() const noexcept
		{
			if(Q_IsNull())
			{
				return false;
			}
			return F_StringDatabase::GetInstance().CheckId(*this);
		}
		
		/**
		 * @brief Non-quick check if the string ID is null (verifies with the database)
		 * @return true if the ID doesn't exist in the database
		 */
		ABYTEK_FORCE_INLINE B8 NQ_IsNull() const noexcept
		{
			if(Q_IsNull())
			{
				return true;
			}
			return !(F_StringDatabase::GetInstance().CheckId(*this));
		}
		
		/**
		 * @brief Boolean conversion operator, returns true if the ID is valid
		 * @return true if the ID is valid in the database
		 */
		ABYTEK_FORCE_INLINE explicit operator B8 () const noexcept
		{
			return NQ_IsValid();
		}
		
		/**
		 * @brief Reset the string ID to an invalid state
		 */
		void Reset() noexcept
		{
			if constexpr (F_Option::EnableCounter)
			{
				if (*this)
				{
					F_StringDatabase::GetInstance().DecreaseCounter(_Index, _HashCode);
				}
			}
			_HashCode = INVALID_STRING_HASH_CODE;
			_Index = INVALID_STRING_INDEX;
#ifdef ABYTEK_BASE_CORE_ENABLE_STRING_ID_DEBUGGER
			_DebugValue = {};
#endif
		}
		
		/**
		 * @brief Dereference operator to access the string
		 * @return Reference to the string in the database
		 */
		ABYTEK_FORCE_INLINE const F_String& operator * () const noexcept
		{
			if (Q_IsNull())
			{
				static F_String EmptyString;
				return EmptyString;
			}
			return F_StringDatabase::GetInstance().Access(_Index);
		}
		
		/**
		 * @brief Arrow operator to access the string's methods
		 * @return Pointer to the string in the database
		 */
		ABYTEK_FORCE_INLINE const F_String* operator -> () const noexcept
		{
			if (Q_IsNull())
			{
				static F_String EmptyString;
				return &EmptyString;
			}
			return &(F_StringDatabase::GetInstance().Access(_Index));
		}
		
		/**
		 * @brief Equality comparison operator
		 * @param X The string ID to compare with
		 * @return true if the hash codes are equal
		 */
		ABYTEK_FORCE_INLINE B8 operator == (const TF_StringId& X) const noexcept
		{
			return (_HashCode == X._HashCode);
		}
		
		/**
		 * @brief Inequality comparison operator
		 * @param X The string ID to compare with
		 * @return true if the hash codes are not equal
		 */
		ABYTEK_FORCE_INLINE B8 operator != (const TF_StringId& X) const noexcept
		{
			return (_HashCode != X._HashCode);
		}
		
		/**
		 * @brief Greater than comparison operator
		 * @param X The string ID to compare with
		 * @return true if this hash code is greater than X's hash code
		 */
		ABYTEK_FORCE_INLINE B8 operator > (const TF_StringId& X) const noexcept
		{
			return (_HashCode > X._HashCode);
		}
		
		/**
		 * @brief Less than comparison operator
		 * @param X The string ID to compare with
		 * @return true if this hash code is less than X's hash code
		 */
		ABYTEK_FORCE_INLINE B8 operator < (const TF_StringId& X) const noexcept
		{
			return (_HashCode < X._HashCode);
		}
		
		/**
		 * @brief Greater than or equal comparison operator
		 * @param X The string ID to compare with
		 * @return true if this hash code is greater than or equal to X's hash code
		 */
		ABYTEK_FORCE_INLINE B8 operator >= (const TF_StringId& X) const noexcept
		{
			return (_HashCode >= X._HashCode);
		}
		
		/**
		 * @brief Less than or equal comparison operator
		 * @param X The string ID to compare with
		 * @return true if this hash code is less than or equal to X's hash code
		 */
		ABYTEK_FORCE_INLINE B8 operator <= (const TF_StringId& X) const noexcept
		{
			return (_HashCode <= X._HashCode);
		}
		
		/**
		 * @brief Output stream operator for logging
		 * @param LogStream The log stream to write to
		 * @param Id The string ID to output
		 * @return Reference to the log stream
		 */
		friend ABYTEK_FORCE_INLINE A_LogStream& operator << (A_LogStream& LogStream, const TF_StringId& Id)
		{
			LogStream << *Id;
			return LogStream;
		}
	};
}

namespace boost
{
	template<class __F_StringDatabase>
	struct hash<Abytek::TF_StringId<__F_StringDatabase>>
	{
		size_t operator() (const Abytek::TF_StringId<__F_StringDatabase>& X) const
		{
			return X.GetHashCode();
		}
	};
}

namespace Abytek::ContainerTemplates
{
	/**
	 * @brief String database template that stores unique strings with efficient lookup
	 * 
	 * This class provides centralized storage for strings with hash-based lookup
	 * to improve memory usage and comparison performance.
	 * 
	 * @tparam __F_StringDatabase The concrete database class
	 * @tparam __F_Char The character type used for strings
	 * @tparam __F_Allocator The allocator type for memory management
	 */
	template<class __F_StringDatabase, typename __F_Char, typename __F_Option, class __F_Allocator>
	class TA_StringDatabase
	{
	public:
		using F_StringDatabase = __F_StringDatabase;
		using F_Char = __F_Char;
		using F_Option = __F_Option;
		using F_Allocator = __F_Allocator;

	public:
		using F_StringId = TF_StringId<F_StringDatabase>;
		using F_StringView = TF_StringView<F_Char>;
		using F_String = TF_String<F_Char, TF_RebindAllocator<F_Char, F_Allocator>>;

	public:
		using F_StringHashCode = U64;
		static constexpr F_StringHashCode INVALID_STRING_HASH_CODE = ~F_StringHashCode(0);
		using F_StringIndex = U32;
		static constexpr F_StringIndex INVALID_STRING_INDEX = ~F_StringIndex(0);
		
	public:
		struct F_StringTracking_NonRefCountedBase
		{
		};
		struct F_StringTracking_RefCountedBase
		{
			U32 Counter = 0;
		};
		struct F_StringTracking : std::conditional_t<
			F_Option::EnableCounter, 
			F_StringTracking_RefCountedBase, 
			F_StringTracking_NonRefCountedBase
		>
		{
			F_StringHashCode HashCode;
			F_String Value;
			
			U32 IncreaseCounter()
			{
				ABYTEK_BASE_CORE_ASSERT(F_Option::EnableCounter);
				using F_Temp = std::conditional_t<F_Option::EnableCounter, F_StringTracking_RefCountedBase, F_StringTracking>;
				auto& CastedRefCountedBase = *((F_Temp*)this);
				return ((AU32&)CastedRefCountedBase.Counter).fetch_add(1, boost::memory_order_acq_rel);
			}
			U32 DecreaseCounter()
			{
				ABYTEK_BASE_CORE_ASSERT(F_Option::EnableCounter);
				using F_Temp = std::conditional_t<F_Option::EnableCounter, F_StringTracking_RefCountedBase, F_StringTracking>;
				auto& CastedRefCountedBase = *((F_Temp*)this);
				return ((AU32&)CastedRefCountedBase.Counter).fetch_sub(1, boost::memory_order_acq_rel);
			}
		}; 

	public:
		using F_StringTrackingVector = TF_Vector<F_StringTracking*, TF_RebindAllocator<F_StringTracking*, F_Allocator>>;
		using F_StringIndexVector = TF_Vector<F_StringIndex, TF_RebindAllocator<F_StringIndex, F_Allocator>>;
		using F_HashTable = TF_HashTable<F_StringHashCode, F_StringIndex, F_Allocator>;
		
	private:
		F_StringTrackingVector _StringTrackings;
		F_StringIndexVector _FreeStringIndices;
		F_HashTable _HashTable;
		mutable F_SpinLock _Lock;

	public:
		/**
		 * @brief Get the vector of stored string trackings
		 * @return Constant reference to string trackings
		 */
		ABYTEK_FORCE_INLINE const auto& GetStringTrackings() const noexcept
		{
			return _StringTrackings;
		}
		
		/**
		 * @brief Get the hash table for string lookups
		 * @return Constant reference to the hash table
		 */
		ABYTEK_FORCE_INLINE const auto& GetHashTable() const noexcept
		{
			return _HashTable;
		}
		
		/**
		 * @brief Get the lock for thread-safe operations
		 * @return Reference to the spin lock
		 */
		ABYTEK_FORCE_INLINE F_SpinLock& GetLock() const noexcept
		{
			return _Lock;
		}

	public:
		/**
		 * @brief Constructor with initial hash table size
		 * @param HashSize Initial size of the hash table
		 */
		TA_StringDatabase(U32 HashSize) :
			_HashTable(HashSize)
		{
		}
		
		/**
		 * @brief Destructor
		 */
		~TA_StringDatabase()
		{
		}
		
		/**
		 * @brief Move constructor
		 * @param X The database to move from
		 */
		TA_StringDatabase(TA_StringDatabase&& X) noexcept
		{
		}
		
		/**
		 * @brief Move assignment operator
		 * @param X The database to move from
		 * @return Reference to this database
		 */
		TA_StringDatabase& operator = (TA_StringDatabase&& X) noexcept
		{
			return *this;
		}

	public:
		/**
		 * @brief Check if a string ID exists in the database
		 * @param Id The string ID to check
		 * @return true if the ID exists in the database
		 */
		B8 CheckId(const F_StringId& Id) const noexcept
		{
			TF_ScopeLock<F_SpinLock> _(_Lock);
			
			F_StringIndex StringIndex = Id.GetIndex();
			if(StringIndex == INVALID_STRING_INDEX)
			{
				return false;
			}
			for(F_StringIndex It = _HashTable.First(Id.GetHashCode()); _HashTable.IsValid(It); It = _HashTable.Next(It))
			{
				if(It == StringIndex)
				{
					return true;
				}
			}
			return false;
		}
		
		/**
		 * @brief Register a string in the database
		 * @param StringView The string view to register
		 * @param StringHashCode The hash code of the string
		 * @return The index of the string in the database
		 */
		F_StringIndex Register(F_StringView StringView, F_StringHashCode StringHashCode)
		{
			TF_ScopeLock<F_SpinLock> _(_Lock);
			
			ABYTEK_BASE_CORE_ASSERT(StringHashCode != INVALID_STRING_HASH_CODE) << "invalid string hash code";

			//
			F_StringIndex StringIndex;
			
			// Early return if already registered
			for(StringIndex = _HashTable.First(StringHashCode); _HashTable.IsValid(StringIndex); StringIndex = _HashTable.Next(StringIndex))
			{
				auto StringTracking = _StringTrackings[StringIndex];  
				ABYTEK_BASE_CORE_ASSERT(StringTracking->HashCode == HashStringView(StringTracking->Value.c_str())) << "Hash mismatch";
				if(StringTracking->HashCode == StringHashCode)
				{
					ABYTEK_BASE_CORE_ASSERT(StringTracking->Value == StringView) << "Hash collision";
					if constexpr (F_Option::EnableCounter)
					{
						IncreaseCounter_Unsafe(StringIndex);
					}
					return StringIndex;
				}
			}
			
			// Acquire string index
			auto NumFreeStringIndices = _FreeStringIndices.size();
			if (NumFreeStringIndices > 0)
			{
				StringIndex = _FreeStringIndices.back();
				_FreeStringIndices.resize(NumFreeStringIndices - 1);
			}
			else
			{
				StringIndex = _StringTrackings.size();
				_StringTrackings.resize(StringIndex + 1);
			}
			
			// Apply string tracking
			F_StringTracking*& StringTracking = _StringTrackings[StringIndex];
			StringTracking = new F_StringTracking {};
			StringTracking->Value = StringView.data();
			StringTracking->HashCode = StringHashCode;

			// Add to hash table
			_HashTable.Add(StringHashCode, StringIndex);
			
			// Setup initial counter
			if constexpr (F_Option::EnableCounter)
			{
				IncreaseCounter_Unsafe(StringIndex);
			} 
			return StringIndex;
		}
		void IncreaseCounter(F_StringIndex StringIndex)
		{
			if constexpr (!F_Option::EnableCounter)
			{
				return;
			}
			TF_ScopeLock<F_SpinLock> _(_Lock);
			IncreaseCounter_Unsafe(StringIndex);
		}
		void DecreaseCounter(F_StringIndex StringIndex, F_StringHashCode StringHashCode)
		{
			if constexpr (!F_Option::EnableCounter)
			{
				return;
			}
			TF_ScopeLock<F_SpinLock> _(_Lock);
			DecreaseCounter_Unsafe(StringIndex, StringHashCode);
		}
		void IncreaseCounter_Unsafe(F_StringIndex StringIndex)
		{
			if constexpr (!F_Option::EnableCounter)
			{
				return;
			}
			_StringTrackings[StringIndex]->IncreaseCounter();
		}
		void DecreaseCounter_Unsafe(F_StringIndex StringIndex, F_StringHashCode StringHashCode)
		{
			if constexpr (!F_Option::EnableCounter)
			{ 
				return;
			}
			auto& StringTracking = _StringTrackings[StringIndex];
			U32 LastCounter = StringTracking->DecreaseCounter();
			if (LastCounter == 1)
			{
				_HashTable.Remove(StringHashCode, StringIndex);
				_FreeStringIndices.push_back(StringIndex);
				delete StringTracking;
				StringTracking = nullptr;
			}
		}
		
		/**
		 * @brief Access a string by its index
		 * @param StringIndex The index of the string
		 * @return Constant reference to the string
		 */
		const F_String& Access(F_StringIndex StringIndex) const noexcept
		{
			TF_ScopeLock<F_SpinLock> _(_Lock);
			return _StringTrackings[StringIndex]->Value;
		}
	};
}


/**
 * @brief Macro to declare a string database with instance getter
 * @param ... The string database class name
 */
#define ABYTEK_DECLARE_STRING_DATABASE(...) \
	ABYTEK_PROTECTED_KEYWORD \
		static __VA_ARGS__* _Instance; \
		\
	ABYTEK_PUBLIC_KEYWORD \
		static ABYTEK_FORCE_INLINE __VA_ARGS__& GetInstance() noexcept \
		{ \
			ABYTEK_BASE_CORE_ASSERT(_Instance) << "database instance was not created"; \
			return *_Instance; \
		}

/**
 * @brief Macro to define a string database instance pointer
 * @param ... The string database class name
 */
#define ABYTEK_DEFINE_STRING_DATABASE(...) __VA_ARGS__* __VA_ARGS__::_Instance = 0;

/**
 * @brief Macro to bind the current instance as the singleton instance
 */
#define ABYTEK_BIND_STRING_DATABASE() _Instance = this;

/**
 * @brief Macro to unbind the singleton instance
 */
#define ABYTEK_UNBIND_STRING_DATABASE() _Instance = 0;