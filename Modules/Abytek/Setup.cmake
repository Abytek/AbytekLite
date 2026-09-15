
if(ABYTEK_MODULE_PHASE_INIT)
    set(Abytek.Type CUSTOM)

    option(ABYTEK_ENABLE_WIDE_CHAR_TEXT "" ON)

    option(ABYTEK_ENABLE_DOCUMENTATION "If enabled, creating a target for generating documentation" OFF)
    Abytek_SetGlobal(
        ABYTEK_DOCUMENTATION_PREDEFINED
        ""
    )
    if(ABYTEK_ENABLE_DOCUMENTATION)
        option(ABYTEK_ENABLE_INTERNAL_DOCUMENTATION "Generate docs for internal code" ON)

        include(Abytek/Utilities/DocsHelper)

        Abytek_DocsHelper_CreateTarget(
            NAME Abytek
            BRIEF "The Next-Generation Metaverse Platform."

            TARGET_NAME Abytek.Documentation
            TARGET_FOLDER Abytek
            TARGET_LABEL Documentation

            INPUT_DIRS
                "${Abytek.Directory}"
                "${ABYTEK_DOCUMENTATION_DIR}"
                "${ABYTEK_ROOT_DIR}/README.md"
            STYLE_DIRS
                "${ABYTEK_DOCUMENTATION_DIR}/Styles"
            OUTPUT_DIR "${ABYTEK_GENERATED_DOCUMENTATION_DIR}"
            CWD "${ABYTEK_ROOT_DIR}"
            INTERNAL_DOCS ${ABYTEK_ENABLE_INTERNAL_DOCUMENTATION}

            PREDEFINED ${ABYTEK_DOCUMENTATION_PREDEFINED}

            MAIN_PAGE "${ABYTEK_ROOT_DIR}/README.md"
        )
    endif()
endif()