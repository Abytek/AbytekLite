pluginManagement {
    repositories {
        google {
            content {
                includeGroupByRegex("com\\.android.*")
                includeGroupByRegex("com\\.google.*")
                includeGroupByRegex("androidx.*")
            }
        }
        mavenCentral()
        gradlePluginPortal()
    }
} 
plugins {
    id("org.gradle.toolchains.foojay-resolver-convention") version "1.0.0"
}
dependencyResolutionManagement {
    repositoriesMode.set(RepositoriesMode.FAIL_ON_PROJECT_REPOS)
    repositories {
        google()
        mavenCentral()
    }
}

val ABYTEK_ANDROID_PROJECT_DIR = settingsDir

val ABYTEK_SCOPE_DIR = ABYTEK_ANDROID_PROJECT_DIR
    .resolve("../../../..")
    .canonicalFile
val tempProjectDir = ABYTEK_SCOPE_DIR.resolve("Build/TempProject")
val ABYTEK_PROJECT_DIR_DEFAULT =
    if (tempProjectDir.exists() && tempProjectDir.isDirectory) 
    {
        tempProjectDir
    } else 
    {
        ABYTEK_SCOPE_DIR
    }

val ABYTEK_PROJECT_DIR = File(
    providers
    .gradleProperty("ABYTEK_PROJECT_DIR")
    .orElse(ABYTEK_PROJECT_DIR_DEFAULT.absolutePath)
    .get()
)
val ABYTEK_EBT_ENVIRONMENT = providers
    .gradleProperty("ABYTEK_EBT_ENVIRONMENT")
    .orElse("Cooked")
    .get()
val ABYTEK_EBT_PROCESSOR_ARCHS = providers
    .gradleProperty("ABYTEK_EBT_PROCESSOR_ARCHS")
    .map { it.split(";").map(String::trim).filter { it.isNotEmpty() } }
    .orElse(listOf("ARM_64"))
    .get()
    .sorted()
val ABYTEK_EBT_PROCESSOR_ARCHS_CMAKE_TEXT = ABYTEK_EBT_PROCESSOR_ARCHS.joinToString(";")
val ABYTEK_EBT_ANDROID_ABIS = ABYTEK_EBT_PROCESSOR_ARCHS.mapNotNull { 
    arch ->
    when (arch.uppercase()) {
        "ARM_64" -> "arm64-v8a"
        "ARM_32" -> "armeabi-v7a"
        "X86_64" -> "x86_64"
        "X86" -> "x86"
        else -> error("Unknown processor arch: $arch") // hoặc throw nếu bạn muốn strict
    }
}
val ABYTEK_EBT_CMAKE_CONFIG = providers
    .gradleProperty("ABYTEK_EBT_CMAKE_CONFIG")
    .orElse("Debug")
    .get()
val ABYTEK_EBT_PROJECT_SIGNATURE = "${ABYTEK_EBT_ENVIRONMENT}-Android-Ninja-${ABYTEK_EBT_CMAKE_CONFIG}-${ABYTEK_EBT_PROCESSOR_ARCHS.joinToString(".")}"

gradle.extra["ABYTEK_ANDROID_PROJECT_DIR"] = ABYTEK_ANDROID_PROJECT_DIR
gradle.extra["ABYTEK_PROJECT_DIR"] = ABYTEK_PROJECT_DIR
gradle.extra["ABYTEK_EBT_ENVIRONMENT"] = ABYTEK_EBT_ENVIRONMENT
gradle.extra["ABYTEK_EBT_PROCESSOR_ARCHS"] = ABYTEK_EBT_PROCESSOR_ARCHS
gradle.extra["ABYTEK_EBT_PROCESSOR_ARCHS_CMAKE_TEXT"] = ABYTEK_EBT_PROCESSOR_ARCHS_CMAKE_TEXT
gradle.extra["ABYTEK_EBT_ANDROID_ABIS"] = ABYTEK_EBT_ANDROID_ABIS
gradle.extra["ABYTEK_EBT_CMAKE_CONFIG"] = ABYTEK_EBT_CMAKE_CONFIG
gradle.extra["ABYTEK_EBT_PROJECT_SIGNATURE"] = ABYTEK_EBT_PROJECT_SIGNATURE

apply(from = file("gradle/find-paths.gradle.kts"))

val ABYTEK_ENGINE_ANDROID_MODULES_DIR = gradle.extra["ABYTEK_ENGINE_ANDROID_MODULES_DIR"] as File

rootProject.name = "Application"
include(":app")

include(":abytek")
project(":abytek").projectDir = file("${ABYTEK_ENGINE_ANDROID_MODULES_DIR.absolutePath}/Abytek");