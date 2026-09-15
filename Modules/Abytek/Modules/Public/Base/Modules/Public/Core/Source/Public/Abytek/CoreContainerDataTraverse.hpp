#pragma once

#include "Abytek/DataTraverse.hpp"
#include "Abytek/StringId.hpp"


namespace Abytek
{
    template<template<typename __F_Value2> class __TH_Callback, typename __F_TraversalData, class __F_StringDatabase>
    struct TH_DataTraverse<__TH_Callback, __F_TraversalData, TF_StringId<__F_StringDatabase>> :
        TH_DataTraverseBase<__TH_Callback, __F_TraversalData, TF_StringId<__F_StringDatabase>>
    {
        using F_Value = TF_StringId<__F_StringDatabase>;
        static B8 InvokeChilds(__F_TraversalData& TraversalData, F_Value& Value)
        {
            if (!Value)
            {
                return false;
            }
            using F_ValueText = typename F_Value::F_Char;
            if (!DataTraverse<__TH_Callback>(TraversalData, *(F_ValueText*)&*Value))
            {
                return false;
            }
            return true;
        }
    };
}