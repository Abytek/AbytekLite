#pragma once

#include "Abytek/DataTraverse.hpp"


namespace Abytek
{
    template<template<typename __F_Value2> class __TH_Callback, typename __F_TraversalData, typename __F_First, typename __F_Second>
    struct TH_DataTraverse<__TH_Callback, __F_TraversalData, std::pair<__F_First, __F_Second>> :
        TH_DataTraverseBase<__TH_Callback, __F_TraversalData, std::pair<__F_First, __F_Second>>
    {
        using F_Value = std::pair<__F_First, __F_Second>;

        static B8 InvokeChilds(__F_TraversalData& TraversalData, F_Value& Value)
        {
            if (!DataTraverse<__TH_Callback>(TraversalData, Value.first))
            {
                return false;
            }
            if (!DataTraverse<__TH_Callback>(TraversalData, Value.second))
            {
                return false;
            }
            return true;
        }
    };
    
    template<template<typename __F_Value2> class __TH_Callback, typename __F_TraversalData, typename __F_Item, Sz __Size>
    struct TH_DataTraverse<__TH_Callback, __F_TraversalData, boost::array<__F_Item, __Size>> :
        TH_DataTraverseBase<__TH_Callback, __F_TraversalData, boost::array<__F_Item, __Size>>
    {
        using F_Value = boost::array<__F_Item, __Size>;

        static B8 InvokeChilds(__F_TraversalData& TraversalData, F_Value& Value)
        {
            for (auto& Item : Value)
            {
                if (!DataTraverse<__TH_Callback>(TraversalData, Item))
                {
                    return false;
                }
            }
            return true;
        }
    };
    
    template<template<typename __F_Value2> class __TH_Callback, typename __F_TraversalData, typename __F_Item>
    struct TH_DataTraverse<__TH_Callback, __F_TraversalData, boost::optional<__F_Item>> :
        TH_DataTraverseBase<__TH_Callback, __F_TraversalData, boost::optional<__F_Item>>
    {
        using F_Value = boost::optional<__F_Item>;

        static B8 InvokeSelf(__F_TraversalData& TraversalData, F_Value& Value)
        {
            if (!static_cast<B8>(Value))
            {
                return false;
            }
            if (!__TH_Callback<F_Value>::Invoke(TraversalData, Value))
            {
                return false;
            }
            return true;
        }
        static B8 InvokeChilds(__F_TraversalData& TraversalData, F_Value& Value)
        {
            if (!DataTraverse<__TH_Callback>(TraversalData, *Value))
            {
                return false;
            }
            return true;
        }
    };
    
    template<template<typename __F_Value2> class __TH_Callback, typename __F_TraversalData, typename __F_Char, class __F_Traits, class __F_Allocator>
    struct TH_DataTraverse<__TH_Callback, __F_TraversalData, boost::container::basic_string<__F_Char, __F_Traits, __F_Allocator>> :
        TH_DataTraverseBase<__TH_Callback, __F_TraversalData, boost::container::basic_string<__F_Char, __F_Traits, __F_Allocator>>
    {
        using F_Value = boost::container::basic_string<__F_Char, __F_Traits, __F_Allocator>;
    };
    
    template<template<typename __F_Value2> class __TH_Callback, typename __F_TraversalData, typename T, typename A>
    struct TH_DataTraverse<__TH_Callback, __F_TraversalData, boost::container::vector<T,A>>
    : TH_DataTraverseIterable<__TH_Callback, __F_TraversalData, boost::container::vector<T,A>>
    {};
    
    template<template<typename __F_Value2> class __TH_Callback, typename __F_TraversalData, typename T, typename A>
    struct TH_DataTraverse<__TH_Callback, __F_TraversalData, boost::container::devector<T,A>>
        : TH_DataTraverseIterable<__TH_Callback, __F_TraversalData, boost::container::devector<T,A>>
    {};
    
    template<template<typename __F_Value2> class __TH_Callback, typename __F_TraversalData, typename T, Sz N, typename A>
    struct TH_DataTraverse<__TH_Callback, __F_TraversalData, boost::container::small_vector<T,N,A>>
        : TH_DataTraverseIterable<__TH_Callback, __F_TraversalData, boost::container::small_vector<T,N,A>>
    {};
    
    template<template<typename __F_Value2> class __TH_Callback, typename __F_TraversalData, typename T, typename A>
    struct TH_DataTraverse<__TH_Callback, __F_TraversalData, boost::container::stable_vector<T,A>>
        : TH_DataTraverseIterable<__TH_Callback, __F_TraversalData, boost::container::stable_vector<T,A>>
    {};
    
    template<template<typename __F_Value2> class __TH_Callback, typename __F_TraversalData, typename T, Sz N, typename O>
    struct TH_DataTraverse<__TH_Callback, __F_TraversalData, boost::container::static_vector<T,N,O>>
        : TH_DataTraverseIterable<__TH_Callback, __F_TraversalData, boost::container::static_vector<T,N,O>>
    {};
    
    template<template<typename __F_Value2> class __TH_Callback, typename __F_TraversalData, typename T, typename A>
    struct TH_DataTraverse<__TH_Callback, __F_TraversalData, boost::container::deque<T,A,void>>
        : TH_DataTraverseIterable<__TH_Callback, __F_TraversalData, boost::container::deque<T,A,void>>
    {};
    
    template<template<typename __F_Value2> class __TH_Callback, typename __F_TraversalData, typename T, typename A>
    struct TH_DataTraverse<__TH_Callback, __F_TraversalData, boost::container::list<T,A>>
        : TH_DataTraverseIterable<__TH_Callback, __F_TraversalData, boost::container::list<T,A>>
    {};
    
    template<template<typename __F_Value2> class __TH_Callback, typename __F_TraversalData, typename T, typename A>
    struct TH_DataTraverse<__TH_Callback, __F_TraversalData, boost::container::slist<T,A>>
        : TH_DataTraverseIterable<__TH_Callback, __F_TraversalData, boost::container::slist<T,A>>
    {};
    
    template<template<typename __F_Value2> class __TH_Callback, typename __F_TraversalData, typename T, typename C, typename A>
    struct TH_DataTraverse<__TH_Callback, __F_TraversalData, boost::container::set<T,C,A>>
        : TH_DataTraverseIterable<__TH_Callback, __F_TraversalData, boost::container::set<T,C,A>>
    {};
    
    template<template<typename __F_Value2> class __TH_Callback, typename __F_TraversalData, typename T, typename H, typename P, typename A>
    struct TH_DataTraverse<__TH_Callback, __F_TraversalData, boost::unordered_set<T,H,P,A>>
        : TH_DataTraverseIterable<__TH_Callback, __F_TraversalData, boost::unordered_set<T,H,P,A>>
    {};
    
    template<template<typename __F_Value2> class __TH_Callback, typename __F_TraversalData, typename T, typename C, typename A>
    struct TH_DataTraverse<__TH_Callback, __F_TraversalData, boost::container::flat_set<T,C,A>>
        : TH_DataTraverseIterable<__TH_Callback, __F_TraversalData, boost::container::flat_set<T,C,A>>
    {};
    
    template<template<typename __F_Value2> class __TH_Callback, typename __F_TraversalData, typename K, typename V, typename C, typename A>
    struct TH_DataTraverse<__TH_Callback, __F_TraversalData, boost::container::map<K,V,C,A>>
    : TH_DataTraverseIterable<__TH_Callback, __F_TraversalData, boost::container::map<K,V,C,A>>
    {};
    
    template<template<typename __F_Value2> class __TH_Callback, typename __F_TraversalData, typename K, typename V, typename H, typename P, typename A>
    struct TH_DataTraverse<__TH_Callback, __F_TraversalData, boost::unordered_map<K,V,H,P,A>>
        : TH_DataTraverseIterable<__TH_Callback, __F_TraversalData, boost::unordered_map<K,V,H,P,A>>
    {};
    
    template<template<typename __F_Value2> class __TH_Callback, typename __F_TraversalData, typename K, typename V, typename C, typename A>
    struct TH_DataTraverse<__TH_Callback, __F_TraversalData, boost::container::flat_map<K,V,C,A>>
        : TH_DataTraverseIterable<__TH_Callback, __F_TraversalData, boost::container::flat_map<K,V,C,A>>
    {};
}