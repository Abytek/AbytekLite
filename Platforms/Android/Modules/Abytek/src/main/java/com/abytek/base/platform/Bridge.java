package com.abytek.base.platform;

import android.content.Context;

public class Bridge {

    public static void init() {

        PathManager_Bridge.init();
    }
    public static void deinit() {

        PathManager_Bridge.deinit();
    }
}