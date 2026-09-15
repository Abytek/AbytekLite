import java.io.File
import org.gradle.api.GradleException

plugins {
    alias(libs.plugins.android.application)
} 

val ABYTEK_ANDROID_PROJECT_DIR = gradle.extra["ABYTEK_ANDROID_PROJECT_DIR"] as File
val ABYTEK_ENGINE_DIR = gradle.extra["ABYTEK_ENGINE_DIR"] as File
val ABYTEK_PROJECT_DIR = gradle.extra["ABYTEK_PROJECT_DIR"] as File
val ABYTEK_EBT_ENVIRONMENT = gradle.extra["ABYTEK_EBT_ENVIRONMENT"] as String
val ABYTEK_EBT_PROCESSOR_ARCHS = gradle.extra["ABYTEK_EBT_PROCESSOR_ARCHS"] as Collection<String>
val ABYTEK_EBT_PROCESSOR_ARCHS_CMAKE_TEXT = gradle.extra["ABYTEK_EBT_PROCESSOR_ARCHS_CMAKE_TEXT"] as String
val ABYTEK_EBT_ANDROID_ABIS = gradle.extra["ABYTEK_EBT_ANDROID_ABIS"] as Collection<String>
val ABYTEK_EBT_CMAKE_CONFIG = gradle.extra["ABYTEK_EBT_CMAKE_CONFIG"] as String
val ABYTEK_EBT_PROJECT_SIGNATURE = gradle.extra["ABYTEK_EBT_PROJECT_SIGNATURE"] as String


val applicationLibFile = file("application-lib.txt")
if (!applicationLibFile.exists()) {
    throw GradleException(
        "Application lib file not found: ${applicationLibFile.absolutePath}"
    )
}
val applicationLib = applicationLibFile.readText().trim()

val applicationPackageFile = file("application-package.txt")
if (!applicationPackageFile.exists()) {
    throw GradleException(
        "Application package file not found: ${applicationPackageFile.absolutePath}"
    )
}
val applicationPackage = applicationPackageFile.readText().trim()

abstract class GenerateAbytekConfigTask : DefaultTask() {

    @get:OutputDirectory
    abstract val outputDir: DirectoryProperty

    @get:Input
    abstract val appLib: Property<String>

    @get:Input
    abstract val appPackage: Property<String>

    @TaskAction
    fun generate() {
        val file = outputDir.file("abytek/Config.java").get().asFile
        file.parentFile.mkdirs()

        file.writeText("""
            package com.abytek;

            public final class Config {
                public static final String APPLICATION_LIB = "${appLib.get()}";
                public static final String APPLICATION_PACKAGE = "${appPackage.get()}";
            }
        """.trimIndent())
    }
}
val generateAbytekConfig = tasks.register<GenerateAbytekConfigTask>("generateAbytekConfig") {
    outputDir.set(layout.buildDirectory.dir("generated/source/abytekConfig"))
    appLib.set(applicationLib)
    appPackage.set(applicationPackage)
}

androidComponents {
    beforeVariants { v ->
        if (
            (v.buildType == "debug")
            && (ABYTEK_EBT_CMAKE_CONFIG == "Debug")
        ) 
        {
            v.enable = true
        }
        if (
            (v.buildType == "release")
            && (ABYTEK_EBT_CMAKE_CONFIG == "Release")
        ) 
        {
            v.enable = true
        }
    }
    onVariants { variant ->
        variant.sources.java?.addGeneratedSourceDirectory(
            generateAbytekConfig,
            GenerateAbytekConfigTask::outputDir
        )
    }
}

android {
    namespace = applicationPackage
    compileSdk {
        version = release(36) {
            minorApiLevel = 1
        }
    }

    defaultConfig {
        applicationId = applicationPackage
        minSdk = 30
        targetSdk = 36
        versionCode = 1
        versionName = "1.0"

        testInstrumentationRunner = "androidx.test.runner.AndroidJUnitRunner"
        externalNativeBuild {
            cmake {
                // cppFlags += "-std=c++17"
                arguments += listOf("-DABYTEK_ENGINE_DIR=${ABYTEK_ENGINE_DIR.absolutePath.replace("\\", "/")}")
                arguments += listOf("-DABYTEK_PROJECT_DIR=${ABYTEK_PROJECT_DIR.absolutePath.replace("\\", "/")}")
                arguments += listOf("-DABYTEK_EBT_ENVIRONMENT=${ABYTEK_EBT_ENVIRONMENT}")
                arguments += listOf("-DABYTEK_EBT_PLATFORM=Android")
                arguments += listOf("-DABYTEK_EBT_CMAKE_GENERATOR=Ninja")
                arguments += listOf("-DABYTEK_EBT_CMAKE_CONFIG=${ABYTEK_EBT_CMAKE_CONFIG}")
                arguments += listOf("-DABYTEK_EBT_PROCESSOR_ARCHS=${ABYTEK_EBT_PROCESSOR_ARCHS_CMAKE_TEXT}")
                arguments += listOf("-DABYTEK_EBT_PROJECT_SIGNATURE=${ABYTEK_EBT_PROJECT_SIGNATURE}")
            }
        }

        manifestPlaceholders["ABYTEK_APPLICATION_LIB"] = applicationLib
        manifestPlaceholders["ABYTEK_APPLICATION_PACKAGE"] = applicationPackage

        ndk {
            abiFilters.addAll(ABYTEK_EBT_ANDROID_ABIS)
        }
    }

    buildTypes {
        release {
            isMinifyEnabled = false
            proguardFiles(
                getDefaultProguardFile("proguard-android-optimize.txt"),
                "proguard-rules.pro"
            )
        }
    }
    compileOptions {
        sourceCompatibility = JavaVersion.VERSION_11
        targetCompatibility = JavaVersion.VERSION_11
    }
    buildFeatures {
        prefab = true
    }
    externalNativeBuild {
        cmake {
            path = file("src/main/cpp/CMakeLists.txt")
            version = "4.1.2"
        }
    }
    ndkVersion = "26.3.11579264"
}

dependencies {
    implementation(libs.appcompat)
    implementation(libs.material)
    implementation(libs.games.activity)
    implementation(project(":abytek"))
}