package com.abytek.base.platform;

import android.content.Context;
import com.abytek.BridgeContext;

public class PathManager_Bridge {

    public static void init() {

        onInitialized();
    }
    public static void deinit() {

        onDeinitialized();
    }

    public static native void onInitialized();
    public static native void onDeinitialized();

    public static String getUserDataDir() {
        return BridgeContext.getAppContext().getFilesDir().getAbsolutePath();
    }
    public static String getCacheDir() {
        return BridgeContext.getAppContext().getCacheDir().getAbsolutePath();
    }
    public static String getExternalUserDataDir() {
        return BridgeContext.getAppContext().getExternalFilesDir(null).getAbsolutePath();
    }
    public static String getExternalCacheDir() {
        return BridgeContext.getAppContext().getExternalCacheDir().getAbsolutePath();
    }
}