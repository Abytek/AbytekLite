package com.abytek;

import android.content.Context;

public class Bridge {

    public static void init() {

        com.abytek.base.Bridge.init();
        com.abytek.engine.Bridge.init();
    }
    public static void deinit() {

        com.abytek.engine.Bridge.deinit();
        com.abytek.base.Bridge.deinit();
    }
}