import java.io.File
import org.gradle.api.GradleException

val ABYTEK_ANDROID_PROJECT_DIR = gradle.extra["ABYTEK_ANDROID_PROJECT_DIR"] as File
val ABYTEK_PROJECT_DIR = gradle.extra["ABYTEK_PROJECT_DIR"] as File
val ABYTEK_EBT_ENVIRONMENT = gradle.extra["ABYTEK_EBT_ENVIRONMENT"] as String
val ABYTEK_EBT_PROJECT_SIGNATURE = gradle.extra["ABYTEK_EBT_PROJECT_SIGNATURE"] as String
println("Resolving Paths (ABYTEK_EBT_ENVIRONMENT: $ABYTEK_EBT_ENVIRONMENT, ABYTEK_ANDROID_PROJECT_DIR: $ABYTEK_ANDROID_PROJECT_DIR, ABYTEK_EBT_PROJECT_SIGNATURE: $ABYTEK_EBT_PROJECT_SIGNATURE)")

val enginePathFile = File(
    ABYTEK_PROJECT_DIR,
    "Build/AbytekEnginePath-" + ABYTEK_EBT_PROJECT_SIGNATURE + ".txt"
).canonicalFile

if (!enginePathFile.exists()) {
    throw GradleException(
        "Engine path file not found: ${enginePathFile.absolutePath}"
    )
}

val ABYTEK_ENGINE_DIR = File(enginePathFile.readText().trim())

val ABYTEK_ENGINE_ANDROID_MODULES_DIR = File("${ABYTEK_ENGINE_DIR.canonicalPath}/Platforms/Android/Modules")

println("Resolved ABYTEK_ENGINE_DIR: $ABYTEK_ENGINE_DIR")
println("Resolved ABYTEK_PROJECT_DIR: $ABYTEK_PROJECT_DIR")
println("Resolved ABYTEK_ENGINE_ANDROID_MODULES_DIR: $ABYTEK_ENGINE_ANDROID_MODULES_DIR")

gradle.extra["ABYTEK_ENGINE_DIR"] = ABYTEK_ENGINE_DIR
gradle.extra["ABYTEK_ENGINE_ANDROID_MODULES_DIR"] = ABYTEK_ENGINE_ANDROID_MODULES_DIR
gradle.extra["ABYTEK_PROJECT_DIR"] = ABYTEK_PROJECT_DIR