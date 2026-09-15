package com.abytek.base;

import android.content.Context;

public class Bridge {

    public static void init() {

        com.abytek.base.platform.Bridge.init();
    }
    public static void deinit() {

        com.abytek.base.platform.Bridge.deinit();
    }
}