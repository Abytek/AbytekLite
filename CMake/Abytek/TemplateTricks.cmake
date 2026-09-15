
message(STATUS "<Abytek::TemplateTricks> Start generating template tricks")



#####################################################################################
#   Includes
#####################################################################################
include(Abytek/Utilities/SetGlobal)
include(Abytek/Utilities/ApplyTempGeneratedFile)



#####################################################################################
#   Setup global variables
#####################################################################################
set(ABYTEK_TEMPLATE_TRICKS_MAX_ARG_COUNT 128 CACHE STRING "Max argument count of template tricks")



#####################################################################################
#   Setup the generated file
#####################################################################################
file(WRITE ${ABYTEK_TEMPLATE_TRICKS_TEMP_FILE} "\n
    #pragma once
    #include <Abytek/Base.Minimal.prerequisites.pch.hpp>\n
    namespace Abytek {
")



#####################################################################################
#   N-th template type argument
#####################################################################################
function(Abytek_TemplateTricks_CreateTemplate_NthTemplateTArg MaxArgCount)

    set(fileContent " namespace Internal { \n")

    set(fileContent "${fileContent} template<Sz index__, typename... F_args__> struct TF_NthTemplateTArgGetterHelper; \n")

    MATH(EXPR maxIndex "${MaxArgCount} - 1")

    set(template_targs "typename arg_0__")
    set(pass_template_targs "arg_0__")

    foreach(index RANGE 0 ${maxIndex})

        if(NOT index EQUAL 0)
            set(template_targs "${template_targs}, typename arg_${index}__")
            set(pass_template_targs "${pass_template_targs}, arg_${index}__")
        endif()

        set(fileContent "${fileContent} template<${template_targs}, typename... F_rest_args__>
            struct TF_NthTemplateTArgGetterHelper<${index}, ${pass_template_targs}, F_rest_args__...> { using F = arg_${index}__; };
        ")

    endforeach()

    set(fileContent "${fileContent} template<Sz index__, typename... F_args__>
        struct TF_NthTemplateTArgHelper {
            static_assert((index__ < sizeof...(F_args__)), \"out of bound\");
            using F = typename TF_NthTemplateTArgGetterHelper<index__, F_args__...>::F;
         };
    ")

    set(fileContent "${fileContent} } \n")

    set(fileContent "${fileContent}
        template<Sz index__, typename... F_args__>
        using TF_NthTemplateTArg = typename Internal::TF_NthTemplateTArgHelper<index__, F_args__...>::F;
         \n
    ")

    file(APPEND ${ABYTEK_TEMPLATE_TRICKS_TEMP_FILE} "${fileContent}")

endfunction()

Abytek_TemplateTricks_CreateTemplate_NthTemplateTArg(${ABYTEK_TEMPLATE_TRICKS_MAX_ARG_COUNT})



#####################################################################################
#   N-th template value argument
#####################################################################################
function(Abytek_TemplateTricks_CreateTemplate_NthTemplateVArg MaxArgCount)

    set(fileContent " namespace Internal { \n")

    set(fileContent "${fileContent} template<Sz index__, auto... args__> struct TF_NthTemplateVArgGetterHelper; \n")

    MATH(EXPR maxIndex "${MaxArgCount} - 1")

    set(template_vargs "auto arg_0__")
    set(pass_template_vargs "arg_0__")

    foreach(index RANGE 0 ${maxIndex})

        if(NOT index EQUAL 0)
            set(template_vargs "${template_vargs}, auto arg_${index}__")
            set(pass_template_vargs "${pass_template_vargs}, arg_${index}__")
        endif()

        set(fileContent "${fileContent} template<${template_vargs}, auto... rest_args__>
            struct TF_NthTemplateVArgGetterHelper<${index}, ${pass_template_vargs}, rest_args__...> { static constexpr auto value = arg_${index}__; };
        ")

    endforeach()

    set(fileContent "${fileContent} template<Sz index__, auto... args__>
        struct TF_NthTemplateVArgHelper {
            static_assert((index__ < sizeof...(args__)), \"out of bound\");
            static constexpr auto value = TF_NthTemplateVArgGetterHelper<index__, args__...>::value;
         };
    ")

    set(fileContent "${fileContent} } \n")

    set(fileContent "${fileContent}
        template<Sz index__, auto... args__>
        static constexpr auto T_NthTemplateVArg = Internal::TF_NthTemplateVArgHelper<index__, args__...>::value;
         \n
    ")

    file(APPEND ${ABYTEK_TEMPLATE_TRICKS_TEMP_FILE} "${fileContent}")

endfunction()

Abytek_TemplateTricks_CreateTemplate_NthTemplateVArg(${ABYTEK_TEMPLATE_TRICKS_MAX_ARG_COUNT})



#####################################################################################
#   Finalize the generated file
#####################################################################################
file(APPEND ${ABYTEK_TEMPLATE_TRICKS_TEMP_FILE} "\n
    }
")

Abytek_ApplyTempGeneratedFile(
    "${ABYTEK_TEMPLATE_TRICKS_TEMP_FILE}"
    "${ABYTEK_TEMPLATE_TRICKS_FILE}"
)



message(STATUS "<Abytek::TemplateTricks> Generate template tricks done")