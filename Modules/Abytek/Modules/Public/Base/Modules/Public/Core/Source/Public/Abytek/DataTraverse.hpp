#pragma once

#include "Abytek/Base.Core.prerequisites.pch.hpp"


namespace Abytek
{
    template<template<typename __F_Value2> class __TH_Callback, typename __F_TraversalData, typename __F_Value>
    struct TH_DataTraverseBase
    {
        static B8 InvokeSelf(__F_TraversalData& TraversalData, __F_Value& Value)
        {
            if (!__TH_Callback<__F_Value>::Invoke(TraversalData, Value))
            {
                return false;
            }
            return true;
        }
        static B8 InvokeChilds(__F_TraversalData& TraversalData, __F_Value& Value)
        {
            return true;
        }
    };
    
    template<template<typename __F_Value2> class __TH_Callback, typename __F_TraversalData, typename __F_Value, typename = void>
    struct TH_DataTraverse : TH_DataTraverseBase<__TH_Callback, __F_TraversalData, __F_Value>
    {};
    
    template<template<typename __F_Value2> class __TH_Callback, typename __F_TraversalData, typename __F_Value>
    B8 DataTraverse(__F_TraversalData& TraversalData, __F_Value& Value)
    {
        if (!TH_DataTraverse<__TH_Callback, __F_TraversalData, std::remove_const_t<__F_Value>>::InvokeSelf(TraversalData, Value))
        {
            return false;
        }
        if (!TH_DataTraverse<__TH_Callback, __F_TraversalData, std::remove_const_t<__F_Value>>::InvokeChilds(TraversalData, Value))
        {
            return false;
        }
        return true;
    }
    
    template<template<typename __F_Value2> class __TH_Callback, typename __F_TraversalData, typename __F_Container>
    struct TH_DataTraverseIterable :
        TH_DataTraverseBase<__TH_Callback, __F_TraversalData, __F_Container>
    {
        using F_Value = __F_Container;

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
    
    template<template<typename __F_Value2> class __TH_Callback, typename __F_TraversalData, typename __F_Value>
    struct TH_DataTraverse<__TH_Callback, __F_TraversalData, __F_Value*> : TH_DataTraverseBase<__TH_Callback, __F_TraversalData, __F_Value>
    {
        static B8 InvokeChilds(__F_TraversalData& TraversalData, __F_Value*& Value)
        {
            if (!DataTraverse<__TH_Callback>(TraversalData, *Value))
            {
                return false;
            }
            return true;
        }
    };
}