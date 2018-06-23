package me.fahien.sunspot;

import android.content.res.AssetManager;

// Wrapper for native library

public class SunSpotLib {
	static {
		System.loadLibrary("sunspot");
	}

	public static native void init(String external, String cache, AssetManager assets);

	public static native void resize(int width, int height);

	public static native void step();
}
