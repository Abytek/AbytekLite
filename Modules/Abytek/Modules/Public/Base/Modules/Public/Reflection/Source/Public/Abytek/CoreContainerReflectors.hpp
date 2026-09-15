#pragma once

/**
 * @file CoreContainerReflectors.hpp
 * @brief Provides reflection templates for standard C++ containers
 * 
 * This file contains reflection template definitions for various container types,
 * allowing them to be used within the Abytek reflection system. Each container
 * reflector handles the reflection of its contained item types.
 */

#include "Abytek/Base.Object.prerequisites.pch.hpp"
#include "Abytek/Reflection.hpp"



namespace Abytek
{
    template<typename __F_Char, class __F_Allocator>
    struct TF_StringReflector
    {
        static constexpr E_ReflectMode DefaultReflectMode = E_ReflectMode::INLINE;
        
        ABYTEK_BEGIN_REFLECTOR();
        ABYTEK_END_REFLECTOR_ADVANCED(TF_StringReflector, ContainerTemplates::TF_String<__F_Char, __F_Allocator>);
    };
    template<typename __F_Char, class __F_Allocator>
    ABYTEK_REFLECT(ABYTEK_MA(TF_StringReflector<__F_Char, __F_Allocator>))
    {
        ReflectionType->ReflectReferenced<__F_Char>();
    }
    template<typename __F_Char, class __F_Allocator>
    struct TH_ResolveReflector<ContainerTemplates::TF_String<__F_Char, __F_Allocator>>
    {
        using F = TF_StringReflector<__F_Char, __F_Allocator>;
    };

    template<typename __F_Item, Sz __Size>
    struct TF_ArrayReflector
    {
        static constexpr E_ReflectMode DefaultReflectMode = E_ReflectMode::INLINE;
        
        ABYTEK_BEGIN_REFLECTOR();
        ABYTEK_END_REFLECTOR_ADVANCED(TF_ArrayReflector, TF_Array<__F_Item, __Size>);
    };
    template<typename __F_Item, Sz __Size>
    ABYTEK_REFLECT(ABYTEK_MA(TF_ArrayReflector<__F_Item, __Size>))
    {
        ReflectionType->ReflectReferenced<__F_Item>();
    }
    template<typename __F_Item, Sz __Size>
    struct TH_ResolveReflector<TF_Array<__F_Item, __Size>>
    {
        using F = TF_ArrayReflector<__F_Item, __Size>;
    };

    template<typename __F_Item>
    struct TF_SpanReflector 
    {
        static constexpr E_ReflectMode DefaultReflectMode = E_ReflectMode::INLINE;
        
        ABYTEK_BEGIN_REFLECTOR();
        ABYTEK_END_REFLECTOR_ADVANCED(TF_SpanReflector, TF_Span<__F_Item>);
    };
    template<typename __F_Item>
    ABYTEK_REFLECT(ABYTEK_MA(TF_SpanReflector<__F_Item>))
    {
        ReflectionType->ReflectReferenced<__F_Item>();
    }
    template<typename __F_Item>
    struct TH_ResolveReflector<TF_Span<__F_Item>>
    {
        using F = TF_SpanReflector<__F_Item>;
    };

    template<typename __F_Item, class __F_Allocator>
    struct TF_VectorReflector 
    {
        static constexpr E_ReflectMode DefaultReflectMode = E_ReflectMode::INLINE;
        
        ABYTEK_BEGIN_REFLECTOR();
        ABYTEK_END_REFLECTOR_ADVANCED(TF_VectorReflector, ContainerTemplates::TF_Vector<__F_Item, __F_Allocator>);
    };
    template<typename __F_Item, class __F_Allocator>
    ABYTEK_REFLECT(ABYTEK_MA(TF_VectorReflector<__F_Item, __F_Allocator>))
    {
        ReflectionType->ReflectReferenced<__F_Item>();
    }
    template<typename __F_Item, class __F_Allocator>
    struct TH_ResolveReflector<ContainerTemplates::TF_Vector<__F_Item, __F_Allocator>>
    {
        using F = TF_VectorReflector<__F_Item, __F_Allocator>;
    };

    template<typename __F_Item, class __F_Allocator>
    struct TF_DeVectorReflector 
    {
        static constexpr E_ReflectMode DefaultReflectMode = E_ReflectMode::INLINE;
        
        ABYTEK_BEGIN_REFLECTOR();
        ABYTEK_END_REFLECTOR_ADVANCED(TF_DeVectorReflector, ContainerTemplates::TF_DeVector<__F_Item, __F_Allocator>);
    };
    template<typename __F_Item, class __F_Allocator>
    ABYTEK_REFLECT(ABYTEK_MA(TF_DeVectorReflector<__F_Item, __F_Allocator>))
    {
        ReflectionType->ReflectReferenced<__F_Item>();
    }
    template<typename __F_Item, class __F_Allocator>
    struct TH_ResolveReflector<ContainerTemplates::TF_DeVector<__F_Item, __F_Allocator>>
    {
        using F = TF_DeVectorReflector<__F_Item, __F_Allocator>;
    };

    template<typename __F_Item, Sz __StaticSize, class __F_Allocator>
    struct TF_SmallVectorReflector 
    {
        static constexpr E_ReflectMode DefaultReflectMode = E_ReflectMode::INLINE;
        
        ABYTEK_BEGIN_REFLECTOR();
        ABYTEK_END_REFLECTOR_ADVANCED(TF_SmallVectorReflector, ContainerTemplates::TF_SmallVector<__F_Item, __StaticSize, __F_Allocator>);
    };
    template<typename __F_Item, Sz __StaticSize, class __F_Allocator>
    ABYTEK_REFLECT(ABYTEK_MA(TF_SmallVectorReflector<__F_Item, __StaticSize, __F_Allocator>))
    {
        ReflectionType->ReflectReferenced<__F_Item>();
    }
    template<typename __F_Item, Sz __StaticSize, class __F_Allocator>
    struct TH_ResolveReflector<ContainerTemplates::TF_SmallVector<__F_Item, __StaticSize, __F_Allocator>>
    {
        using F = TF_SmallVectorReflector<__F_Item, __StaticSize, __F_Allocator>;
    };

    template<typename __F_Item, class __F_Allocator>
    struct TF_StableVectorReflector 
    {
        static constexpr E_ReflectMode DefaultReflectMode = E_ReflectMode::INLINE;
        
        ABYTEK_BEGIN_REFLECTOR();
        ABYTEK_END_REFLECTOR_ADVANCED(TF_StableVectorReflector, ContainerTemplates::TF_StableVector<__F_Item, __F_Allocator>);
    };
    template<typename __F_Item, class __F_Allocator>
    ABYTEK_REFLECT(ABYTEK_MA(TF_StableVectorReflector<__F_Item, __F_Allocator>))
    {
        ReflectionType->ReflectReferenced<__F_Item>();
    }
    template<typename __F_Item, class __F_Allocator>
    struct TH_ResolveReflector<ContainerTemplates::TF_StableVector<__F_Item, __F_Allocator>>
    {
        using F = TF_StableVectorReflector<__F_Item, __F_Allocator>;
    };

    template<typename __F_Item, Sz __StaticSize>
    struct TF_StaticVectorReflector 
    {
        static constexpr E_ReflectMode DefaultReflectMode = E_ReflectMode::INLINE;
        
        ABYTEK_BEGIN_REFLECTOR();
        ABYTEK_END_REFLECTOR_ADVANCED(TF_StaticVectorReflector, ContainerTemplates::TF_StaticVector<__F_Item, __StaticSize>);
    };
    template<typename __F_Item, Sz __StaticSize>
    ABYTEK_REFLECT(ABYTEK_MA(TF_StaticVectorReflector<__F_Item, __StaticSize>))
    {
        ReflectionType->ReflectReferenced<__F_Item>();
    }
    template<typename __F_Item, Sz __StaticSize>
    struct TH_ResolveReflector<ContainerTemplates::TF_StaticVector<__F_Item, __StaticSize>>
    {
        using F = TF_StaticVectorReflector<__F_Item, __StaticSize>;
    };

    template<typename __F_Item, class __F_Allocator>
    struct TF_DequeReflector 
    {
        static constexpr E_ReflectMode DefaultReflectMode = E_ReflectMode::INLINE;
        
        ABYTEK_BEGIN_REFLECTOR();
        ABYTEK_END_REFLECTOR_ADVANCED(TF_DequeReflector, ContainerTemplates::TF_Deque<__F_Item, __F_Allocator>);
    };
    template<typename __F_Item, class __F_Allocator>
    ABYTEK_REFLECT(ABYTEK_MA(TF_DequeReflector<__F_Item, __F_Allocator>))
    {
        ReflectionType->ReflectReferenced<__F_Item>();
    }
    template<typename __F_Item, class __F_Allocator>
    struct TH_ResolveReflector<ContainerTemplates::TF_Deque<__F_Item, __F_Allocator>>
    {
        using F = TF_DequeReflector<__F_Item, __F_Allocator>;
    };

    template<typename __F_Item, class __F_Allocator>
    struct TF_ListReflector 
    {
        static constexpr E_ReflectMode DefaultReflectMode = E_ReflectMode::INLINE;
        
        ABYTEK_BEGIN_REFLECTOR();
        ABYTEK_END_REFLECTOR_ADVANCED(TF_ListReflector, ContainerTemplates::TF_List<__F_Item, __F_Allocator>);
    };
    template<typename __F_Item, class __F_Allocator>
    ABYTEK_REFLECT(ABYTEK_MA(TF_ListReflector<__F_Item, __F_Allocator>))
    {
        ReflectionType->ReflectReferenced<__F_Item>();
    }
    template<typename __F_Item, class __F_Allocator>
    struct TH_ResolveReflector<ContainerTemplates::TF_List<__F_Item, __F_Allocator>>
    {
        using F = TF_ListReflector<__F_Item, __F_Allocator>;
    };

    template<typename __F_Item, class __F_Allocator>
    struct TF_SinglyListReflector 
    {
        static constexpr E_ReflectMode DefaultReflectMode = E_ReflectMode::INLINE;
        
        ABYTEK_BEGIN_REFLECTOR();
        ABYTEK_END_REFLECTOR_ADVANCED(TF_SinglyListReflector, ContainerTemplates::TF_SinglyList<__F_Item, __F_Allocator>);
    };
    template<typename __F_Item, class __F_Allocator>
    ABYTEK_REFLECT(ABYTEK_MA(TF_SinglyListReflector<__F_Item, __F_Allocator>))
    {
        ReflectionType->ReflectReferenced<__F_Item>();
    }
    template<typename __F_Item, class __F_Allocator>
    struct TH_ResolveReflector<ContainerTemplates::TF_SinglyList<__F_Item, __F_Allocator>>
    {
        using F = TF_SinglyListReflector<__F_Item, __F_Allocator>;
    };

    template<
        typename __F_Item,
        typename __F_Compare,
        typename __F_Allocator
    >
    struct TF_SetReflector 
    {
        static constexpr E_ReflectMode DefaultReflectMode = E_ReflectMode::INLINE;
        
        ABYTEK_BEGIN_REFLECTOR();
        ABYTEK_END_REFLECTOR_ADVANCED(TF_SetReflector, ContainerTemplates::TF_Set<__F_Item, __F_Compare, __F_Allocator>);
    };
    template<
        typename __F_Item,
        typename __F_Compare,
        typename __F_Allocator
    >
    ABYTEK_REFLECT(ABYTEK_MA(TF_SetReflector<__F_Item, __F_Compare, __F_Allocator>))
    {
        ReflectionType->ReflectReferenced<__F_Item>();
    }
    template<
        typename __F_Item,
        typename __F_Compare,
        typename __F_Allocator
    >
    struct TH_ResolveReflector<ContainerTemplates::TF_Set<__F_Item, __F_Compare, __F_Allocator>>
    {
        using F = TF_SetReflector<__F_Item, __F_Compare, __F_Allocator>;
    };

    template<
        typename __F_Key,
        typename __F_Value,
        typename __F_Compare,
        typename __F_Allocator
    >
    struct TF_MapReflector 
    {
        static constexpr E_ReflectMode DefaultReflectMode = E_ReflectMode::INLINE;
        
        ABYTEK_BEGIN_REFLECTOR();
        ABYTEK_END_REFLECTOR_ADVANCED(TF_MapReflector, ContainerTemplates::TF_Map<__F_Key, __F_Value, __F_Compare, __F_Allocator>);
    };
    template<
        typename __F_Key,
        typename __F_Value,
        typename __F_Compare,
        typename __F_Allocator
    >
    ABYTEK_REFLECT(ABYTEK_MA(TF_MapReflector<__F_Key, __F_Value, __F_Compare, __F_Allocator>))
    {
        ReflectionType->ReflectReferenced<__F_Key>();
        ReflectionType->ReflectReferenced<__F_Value>();
    }
    template<
        typename __F_Key,
        typename __F_Value,
        typename __F_Compare,
        typename __F_Allocator
    >
    struct TH_ResolveReflector<ContainerTemplates::TF_Map<__F_Key, __F_Value, __F_Compare, __F_Allocator>>
    {
        using F = TF_MapReflector<__F_Key, __F_Value, __F_Compare, __F_Allocator>;
    };

    template<
        typename __F_Key,
        typename __F_Value,
        typename __F_Compare,
        typename __F_Allocator
    >
    struct TF_MultiMapReflector 
    {
        static constexpr E_ReflectMode DefaultReflectMode = E_ReflectMode::INLINE;
        
        ABYTEK_BEGIN_REFLECTOR();
        ABYTEK_END_REFLECTOR_ADVANCED(TF_MultiMapReflector, ContainerTemplates::TF_MultiMap<__F_Key, __F_Value, __F_Compare, __F_Allocator>);
    };
    template<
        typename __F_Key,
        typename __F_Value,
        typename __F_Compare,
        typename __F_Allocator
    >
    ABYTEK_REFLECT(ABYTEK_MA(TF_MultiMapReflector<__F_Key, __F_Value, __F_Compare, __F_Allocator>))
    {
        ReflectionType->ReflectReferenced<__F_Key>();
        ReflectionType->ReflectReferenced<__F_Value>();
    }
    template<
        typename __F_Key,
        typename __F_Value,
        typename __F_Compare,
        typename __F_Allocator
    >
    struct TH_ResolveReflector<ContainerTemplates::TF_MultiMap<__F_Key, __F_Value, __F_Compare, __F_Allocator>>
    {
        using F = TF_MultiMapReflector<__F_Key, __F_Value, __F_Compare, __F_Allocator>;
    };

    template<
        typename __F_Item,
        typename __F_Hash,
        typename __F_Predicate,
        typename __F_Allocator
    >
    struct TF_UnorderedSetReflector 
    {
        static constexpr E_ReflectMode DefaultReflectMode = E_ReflectMode::INLINE;
        
        ABYTEK_BEGIN_REFLECTOR();
        ABYTEK_END_REFLECTOR_ADVANCED(TF_UnorderedSetReflector, ContainerTemplates::TF_UnorderedSet<__F_Item, __F_Hash, __F_Predicate, __F_Allocator>);
    };
    template<
        typename __F_Item,
        typename __F_Hash,
        typename __F_Predicate,
        typename __F_Allocator
    >
    ABYTEK_REFLECT(ABYTEK_MA(TF_UnorderedSetReflector<__F_Item, __F_Hash, __F_Predicate, __F_Allocator>))
    {
        ReflectionType->ReflectReferenced<__F_Item>();
    }
    template<
        typename __F_Item,
        typename __F_Hash,
        typename __F_Predicate,
        typename __F_Allocator
    >
    struct TH_ResolveReflector<ContainerTemplates::TF_UnorderedSet<__F_Item, __F_Hash, __F_Predicate, __F_Allocator>>
    {
        using F = TF_UnorderedSetReflector<__F_Item, __F_Hash, __F_Predicate, __F_Allocator>;
    };

    template<
        typename __F_Key,
        typename __F_Value,
        typename __F_Hash,
        typename __F_Predicate,
        typename __F_Allocator
    >
    struct TF_UnorderedMapReflector 
    {
        static constexpr E_ReflectMode DefaultReflectMode = E_ReflectMode::INLINE;
        
        ABYTEK_BEGIN_REFLECTOR();
        ABYTEK_END_REFLECTOR_ADVANCED(TF_UnorderedMapReflector, ContainerTemplates::TF_UnorderedMap<__F_Key, __F_Value, __F_Hash, __F_Predicate, __F_Allocator>);
    };
    template<
        typename __F_Key,
        typename __F_Value,
        typename __F_Hash,
        typename __F_Predicate,
        typename __F_Allocator
    >
    ABYTEK_REFLECT(ABYTEK_MA(TF_UnorderedMapReflector<__F_Key, __F_Value, __F_Hash, __F_Predicate, __F_Allocator>))
    {
        ReflectionType->ReflectReferenced<__F_Key>();
        ReflectionType->ReflectReferenced<__F_Value>();
    }
    template<
        typename __F_Key,
        typename __F_Value,
        typename __F_Hash,
        typename __F_Predicate,
        typename __F_Allocator
    >
    struct TH_ResolveReflector<ContainerTemplates::TF_UnorderedMap<__F_Key, __F_Value, __F_Hash, __F_Predicate, __F_Allocator>>
    {
        using F = TF_UnorderedMapReflector<__F_Key, __F_Value, __F_Hash, __F_Predicate, __F_Allocator>;
    };

    template<
        typename __F_Key,
        typename __F_Value,
        typename __F_Hash,
        typename __F_Predicate,
        typename __F_Allocator
    >
    struct TF_UnorderedMultiMapReflector 
    {
        static constexpr E_ReflectMode DefaultReflectMode = E_ReflectMode::INLINE;
        
        ABYTEK_BEGIN_REFLECTOR();
        ABYTEK_END_REFLECTOR_ADVANCED(TF_UnorderedMultiMapReflector, ContainerTemplates::TF_UnorderedMultiMap<__F_Key, __F_Value, __F_Hash, __F_Predicate, __F_Allocator>);
    };
    template<
        typename __F_Key,
        typename __F_Value,
        typename __F_Hash,
        typename __F_Predicate,
        typename __F_Allocator
    >
    ABYTEK_REFLECT(ABYTEK_MA(TF_UnorderedMultiMapReflector<__F_Key, __F_Value, __F_Hash, __F_Predicate, __F_Allocator>))
    {
        ReflectionType->ReflectReferenced<__F_Key>();
        ReflectionType->ReflectReferenced<__F_Value>();
    }
    template<
        typename __F_Key,
        typename __F_Value,
        typename __F_Hash,
        typename __F_Predicate,
        typename __F_Allocator
    >
    struct TH_ResolveReflector<ContainerTemplates::TF_UnorderedMultiMap<__F_Key, __F_Value, __F_Hash, __F_Predicate, __F_Allocator>>
    {
        using F = TF_UnorderedMultiMapReflector<__F_Key, __F_Value, __F_Hash, __F_Predicate, __F_Allocator>;
    };

    template<
        typename __F_Item,
        typename __F_Compare,
        typename __F_Allocator
    >
    struct TF_FlatSetReflector 
    {
        static constexpr E_ReflectMode DefaultReflectMode = E_ReflectMode::INLINE;
        
        ABYTEK_BEGIN_REFLECTOR();
        ABYTEK_END_REFLECTOR_ADVANCED(TF_FlatSetReflector, ContainerTemplates::TF_FlatSet<__F_Item, __F_Compare, __F_Allocator>);
    };
    template<
        typename __F_Item,
        typename __F_Compare,
        typename __F_Allocator
    >
    ABYTEK_REFLECT(ABYTEK_MA(TF_FlatSetReflector<__F_Item, __F_Compare, __F_Allocator>))
    {
        ReflectionType->ReflectReferenced<__F_Item>();
    }
    template<
        typename __F_Item,
        typename __F_Compare,
        typename __F_Allocator
    >
    struct TH_ResolveReflector<ContainerTemplates::TF_FlatSet<__F_Item, __F_Compare, __F_Allocator>>
    {
        using F = TF_FlatSetReflector<__F_Item, __F_Compare, __F_Allocator>;
    };

    template<
        typename __F_Key,
        typename __F_Value,
        typename __F_Compare,
        typename __F_Allocator
    >
    struct TF_FlatMapReflector 
    {
        static constexpr E_ReflectMode DefaultReflectMode = E_ReflectMode::INLINE;
        
        ABYTEK_BEGIN_REFLECTOR();
        ABYTEK_END_REFLECTOR_ADVANCED(TF_FlatMapReflector, ContainerTemplates::TF_FlatMap<__F_Key, __F_Value, __F_Compare, __F_Allocator>);
    };
    template<
        typename __F_Key,
        typename __F_Value,
        typename __F_Compare,
        typename __F_Allocator
    >
    ABYTEK_REFLECT(ABYTEK_MA(TF_FlatMapReflector<__F_Key, __F_Value, __F_Compare, __F_Allocator>))
    {
        ReflectionType->ReflectReferenced<__F_Key>();
        ReflectionType->ReflectReferenced<__F_Value>();
    }
    template<
        typename __F_Key,
        typename __F_Value,
        typename __F_Compare,
        typename __F_Allocator
    >
    struct TH_ResolveReflector<ContainerTemplates::TF_FlatMap<__F_Key, __F_Value, __F_Compare, __F_Allocator>>
    {
        using F = TF_FlatMapReflector<__F_Key, __F_Value, __F_Compare, __F_Allocator>;
    };

    template<
        typename __F_Key,
        typename __F_Value,
        typename __F_Compare,
        typename __F_Allocator
    >
    struct TF_FlatMultiMapReflector 
    {
        static constexpr E_ReflectMode DefaultReflectMode = E_ReflectMode::INLINE;
        
        ABYTEK_BEGIN_REFLECTOR();
        ABYTEK_END_REFLECTOR_ADVANCED(TF_FlatMultiMapReflector, ContainerTemplates::TF_FlatMultiMap<__F_Key, __F_Value, __F_Compare, __F_Allocator>);
    };
    template<
        typename __F_Key,
        typename __F_Value,
        typename __F_Compare,
        typename __F_Allocator
    >
    ABYTEK_REFLECT(ABYTEK_MA(TF_FlatMultiMapReflector<__F_Key, __F_Value, __F_Compare, __F_Allocator>))
    {
        ReflectionType->ReflectReferenced<__F_Key>();
        ReflectionType->ReflectReferenced<__F_Value>();
    }
    template<
        typename __F_Key,
        typename __F_Value,
        typename __F_Compare,
        typename __F_Allocator
    >
    struct TH_ResolveReflector<ContainerTemplates::TF_FlatMultiMap<__F_Key, __F_Value, __F_Compare, __F_Allocator>>
    {
        using F = TF_FlatMultiMapReflector<__F_Key, __F_Value, __F_Compare, __F_Allocator>;
    };

    template<typename __F_RawFunctor>
    struct TF_FunctionReflector 
    {
        static constexpr E_ReflectMode DefaultReflectMode = E_ReflectMode::INLINE;
        
        ABYTEK_BEGIN_REFLECTOR();
        ABYTEK_END_REFLECTOR_ADVANCED(TF_FunctionReflector, TF_Function<__F_RawFunctor>);
    };
    template<typename __F_RawFunctor>
    ABYTEK_REFLECT(ABYTEK_MA(TF_FunctionReflector<__F_RawFunctor>))
    {
        ReflectionType->ReflectReferenced<__F_RawFunctor>();
    }
    template<typename __F_RawFunctor>
    struct TH_ResolveReflector<TF_Function<__F_RawFunctor>>
    {
        using F = TF_FunctionReflector<__F_RawFunctor>;
    };
}