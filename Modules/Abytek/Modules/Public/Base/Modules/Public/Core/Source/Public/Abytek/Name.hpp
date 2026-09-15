#pragma once
 
#include "Abytek/Base.Core.prerequisites.pch.hpp"
#include "Abytek/StringId.hpp"


namespace Abytek
{
    class F_NameDatabase;
    
    struct F_NameOption : ContainerTemplates::F_StringIdOption
    {
    };

    /**
     * @brief String database for efficient name storage and retrieval
     * 
     * F_NameDatabase provides a centralized storage for string identifiers,
     * improving memory usage by storing each unique string only once.
     */
    class ABYTEK_BASE_CORE_API F_NameDatabase : public ContainerTemplates::TA_StringDatabase<F_NameDatabase, F_TextChar, F_NameOption, TF_CrtAllocator<U8>>
    {
        ABYTEK_DECLARE_STRING_DATABASE(F_NameDatabase);

    public:
        /**
         * @brief String identifier type used for efficient name references
         */
        using F_Name = TF_StringId<F_NameDatabase>;
        
    public:
        /**
         * @brief Constructor
         */
        F_NameDatabase();
        
        /**
         * @brief Destructor
         */
        ~F_NameDatabase();
    };

    /**
     * @brief String identifier type for efficient name references
     * 
     * F_Name provides a lightweight reference to a string stored in the name database.
     */
    using F_Name = typename F_NameDatabase::F_Name;
}


/**
 * @brief Macro to create a name from a string literal
 * 
 * Creates an F_Name object with the given string and its hash.
 * 
 * @param ... The string literal to create a name from
 * @return An F_Name object
 */
#define ABYTEK_NAME(...) Abytek::F_Name( \
                Abytek::TF_StringView<Abytek::F_TextChar>( \
                    ABYTEK_TEXT(__VA_ARGS__) \
                ), \
                Abytek::HashStringView( \
                    Abytek::TF_StringView<Abytek::F_TextChar>( \
                        ABYTEK_TEXT(__VA_ARGS__) \
                    ) \
                ) \
            )


////////////////////////////////////////////////////////////////////////////////////
//  ABYTEK_MODULE_NAME macros
////////////////////////////////////////////////////////////////////////////////////
#define ABYTEK_MODULE_NAME ABYTEK_NAME(ABYTEK_MODULE)