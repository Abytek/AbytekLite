#pragma once

#include "Abytek/Base.Math.prerequisites.pch.hpp"


namespace Abytek
{
    template<U32 __RowCount, U32 __ColumnCount, typename __F_Element, class __F_Config>
    struct TA_DefaultData
    {
    };
    
    template<
        template<U32 __RowCount, U32 __ColumnCount, typename __F_Element, class __F_Config>
        class __TA_DefaultData = TA_DefaultData
    >
    struct TA_DataConfig
    {
        template<U32 __RowCount2, U32 __ColumnCount2, typename __F_Element2, class __F_Config2>
        using TA_DefaultData = __TA_DefaultData<__RowCount2, __ColumnCount2, __F_Element2, __F_Config2>;
    };
    
    /**
     * @brief General configuration class for data structures
     * 
     * Acts as a default configuration for data templates when no specific
     * configuration is needed.
     */
    struct F_GeneralDataConfig : TA_DataConfig<TA_DefaultData>
    {};
    
    /**
     * @brief Template for data structures with configurable dimensions and element type
     * 
     * @tparam __RowCount Number of rows in the data structure
     * @tparam __ColumnCount Number of columns in the data structure
     * @tparam __F_Element The element type to store (F32, I32, U32, etc.)
     * @tparam __F_Config Configuration type for specialized behavior
     */
    template<U32 __RowCount, U32 __ColumnCount, typename __F_Element, class __F_Config>
    struct TF_Data;
    
    /**
     * @brief Alias template that represents a passed argument of TF_Data
     * 
     * Used for parameter passing to avoid unnecessary copies and maintain 
     * consistent function signatures.
     * 
     * @tparam __RowCount Number of rows in the data structure
     * @tparam __ColumnCount Number of columns in the data structure
     * @tparam __F_Element The element type to store (F32, I32, U32, etc.)
     * @tparam __F_Config Configuration type for specialized behavior
     */
    template<U32 __RowCount, U32 __ColumnCount, typename __F_Element, class __F_Config>
    using TPA_Data = typename TF_Data<__RowCount, __ColumnCount, __F_Element, __F_Config>::F_PassedArgument;
    
    /**
     * @brief Simplified alias for data structures with general configuration
     * 
     * Convenience alias for TF_Data that uses the default F_GeneralDataConfig.
     * 
     * @tparam __RowCount Number of rows in the data structure
     * @tparam __ColumnCount Number of columns in the data structure
     * @tparam __F_Element The element type to store (F32, I32, U32, etc.)
     */
    template<U32 __RowCount, U32 __ColumnCount, typename __F_Element>
    using TF_GeneralData = TF_Data<__RowCount, __ColumnCount, __F_Element, F_GeneralDataConfig>;
}