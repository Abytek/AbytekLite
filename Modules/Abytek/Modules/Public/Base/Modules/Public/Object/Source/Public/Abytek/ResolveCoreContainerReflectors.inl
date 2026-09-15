/**
 * @file ResolveCoreContainerReflectors.inl
 * @brief Resolver implementations for the container reflector templates
 * 
 * This file contains specializations of the TH_ResolveReflector template for various
 * container types. These specializations map container types to their corresponding
 * reflector types, allowing the reflection system to automatically select the appropriate
 * reflector for each container.
 */

namespace Abytek
{
    template<typename __F_Char, class __F_Allocator>
    struct TF_StringReflector;
    template<typename __F_Char, class __F_Allocator>
    struct TH_ResolveReflector<ContainerTemplates::TF_String<__F_Char, __F_Allocator>>
    {
        using F = TF_StringReflector<__F_Char, __F_Allocator>;
    };
    
    template<typename __F_Item, Sz __Size>
    struct TF_ArrayReflector;
    template<typename __F_Item, Sz __Size>
    struct TH_ResolveReflector<TF_Array<__F_Item, __Size>>
    {
        using F = TF_ArrayReflector<__F_Item, __Size>;
    };
    
    template<typename __F_Item>
    struct TF_SpanReflector;
    template<typename __F_Item>
    struct TH_ResolveReflector<TF_Span<__F_Item>>
    {
        using F = TF_SpanReflector<__F_Item>;
    };
    
    template<typename __F_Item, class __F_Allocator>
    struct TF_VectorReflector;
    template<typename __F_Item, class __F_Allocator>
    struct TH_ResolveReflector<ContainerTemplates::TF_Vector<__F_Item, __F_Allocator>>
    {
        using F = TF_VectorReflector<__F_Item, __F_Allocator>;
    };
    
    template<typename __F_Item, class __F_Allocator>
    struct TF_DeVectorReflector;
    template<typename __F_Item, class __F_Allocator>
    struct TH_ResolveReflector<ContainerTemplates::TF_DeVector<__F_Item, __F_Allocator>>
    {
        using F = TF_DeVectorReflector<__F_Item, __F_Allocator>;
    };
    
    template<typename __F_Item, Sz __StaticSize, class __F_Allocator>
    struct TF_SmallVectorReflector;
    template<typename __F_Item, Sz __StaticSize, class __F_Allocator>
    struct TH_ResolveReflector<ContainerTemplates::TF_SmallVector<__F_Item, __StaticSize, __F_Allocator>>
    {
        using F = TF_SmallVectorReflector<__F_Item, __StaticSize, __F_Allocator>;
    };
    
    template<typename __F_Item, class __F_Allocator>
    struct TF_StableVectorReflector;
    template<typename __F_Item, class __F_Allocator>
    struct TH_ResolveReflector<ContainerTemplates::TF_StableVector<__F_Item, __F_Allocator>>
    {
        using F = TF_StableVectorReflector<__F_Item, __F_Allocator>;
    };
    
    template<typename __F_Item, Sz __StaticSize>
    struct TF_StaticVectorReflector;
    template<typename __F_Item, Sz __StaticSize>
    struct TH_ResolveReflector<ContainerTemplates::TF_StaticVector<__F_Item, __StaticSize>>
    {
        using F = TF_StaticVectorReflector<__F_Item, __StaticSize>;
    };
    
    template<typename __F_Item, class __F_Allocator>
    struct TF_DequeReflector;
    template<typename __F_Item, class __F_Allocator>
    struct TH_ResolveReflector<ContainerTemplates::TF_Deque<__F_Item, __F_Allocator>>
    {
        using F = TF_DequeReflector<__F_Item, __F_Allocator>;
    };
    
    template<typename __F_Item, class __F_Allocator>
    struct TF_ListReflector;
    template<typename __F_Item, class __F_Allocator>
    struct TH_ResolveReflector<ContainerTemplates::TF_List<__F_Item, __F_Allocator>>
    {
        using F = TF_ListReflector<__F_Item, __F_Allocator>;
    };
    
    template<typename __F_Item, class __F_Allocator>
    struct TF_SinglyListReflector;
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
    struct TF_SetReflector;
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
    struct TF_MapReflector;
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
    struct TF_MultiMapReflector;
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
    struct TF_UnorderedSetReflector;
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
    struct TF_UnorderedMapReflector;
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
    struct TF_UnorderedMultiMapReflector;
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
    struct TF_FlatSetReflector;
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
    struct TF_FlatMapReflector;
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
    struct TF_FlatMultiMapReflector;
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
    struct TF_FunctionReflector;
    template<typename __F_RawFunctor>
    struct TH_ResolveReflector<TF_Function<__F_RawFunctor>>
    {
        using F = TF_FunctionReflector<__F_RawFunctor>;
    };
}