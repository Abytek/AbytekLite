#pragma once

#include "Abytek/Base.Platform.prerequisites.pch.hpp"


namespace Abytek
{
    class A_FSAdapter;
    
    enum class E_CommonDirectory : U8
    {
        NONE,
        WORKING,
        USER_DATA,
        CACHE,
        EXTERNAL_USER_DATA,
        EXTERNAL_CACHE,
        BUNDLED_DATA
    };
    
    /**
     * @brief Helper class for path operations
     * 
     * Provides static utility functions for manipulating file paths
     */
    struct ABYTEK_BASE_PLATFORM_API H_Path
    {
        /**
         * @brief Resolves a path relative to a base path
         * @param path The path to resolve
         * @param base_path The base path to resolve from
         * @return The resolved absolute path
         */
        static F_Text Resolve(const F_Text& path, const F_Text& base_path);

        /**
         * @brief Finds the absolute path for a given path
         * @param path The path to find
         * @param external_base_paths Additional base paths to check
         * @return The absolute path if found, none otherwise
         */
        static TF_Optional<F_Text> FindAbsolutePath(const F_Text& Path, const TF_Vector<F_Text>& ExternalBasePaths = {}, const TW<A_FSAdapter>& Adapter = {});

        /**
         * @brief Gets the file extension from a path
         * @param path The path to get the extension from
         * @return The file extension (including the dot)
         */
        static F_Text GetExtension(const F_Text& path);

        /**
         * @brief Gets the file name from a path
         * @param path The path to get the file name from
         * @return The file name (including extension)
         */
        static F_Text GetFileName(const F_Text& path);

        /**
         * @brief Removes the extension from a path
         * @param path The path to remove the extension from
         * @return The path without the extension
         */
        static F_Text RemoveExtension(const F_Text& path);

        /**
         * @brief Gets the base name from a path (file name without extension)
         * @param path The path to get the base name from
         * @return The base name
         */
        static F_Text GetBaseName(const F_Text& path);

        /**
         * @brief Splits a path into its components
         * @param path The path to split
         * @return A vector of path components
         */
        static TF_Vector<F_Text> Split(const F_Text& path);

        /**
         * @brief Normalizes a path (resolves . and .. segments)
         * @param path The path to normalize
         * @return The normalized path
         */
        static F_Text Normalize(const F_Text& path);
    };
}