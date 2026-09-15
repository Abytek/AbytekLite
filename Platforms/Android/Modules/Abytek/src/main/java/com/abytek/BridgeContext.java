package com.abytek;

import android.content.Context;

public class BridgeContext {

    private static Context appContext;
    public static Context getAppContext() { return appContext; }

    private static boolean ready = false;
    public static boolean isReady() { return ready; }

    public static void init(Context context) {

        appContext = context.getApplicationContext();
        com.abytek.Bridge.init();
        ready = true;
        onInitialized();
    }
    public static void deinit() {

        onDeinitialized();
        ready = false;
        com.abytek.Bridge.deinit();
    }

    public static native void onInitialized();
    public static native void onDeinitialized();
}