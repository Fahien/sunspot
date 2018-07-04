package me.fahien.sunspot;

import android.app.NativeActivity;
import android.os.Bundle;

public class SunspotNativeActivity extends NativeActivity
{
	static
	{
		System.loadLibrary("sunspot");
	}
}
