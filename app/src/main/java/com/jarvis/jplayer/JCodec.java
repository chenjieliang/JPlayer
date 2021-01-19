package com.jarvis.jplayer;

/**
 * @author chenjieliang
 */
public class JCodec {

    static {
        System.loadLibrary("jplayer-lib");
    }

    public native static void init(String key);
}
