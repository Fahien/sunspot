package me.fahien.sunspot;

import android.content.Context;
import android.content.res.AssetManager;
import android.opengl.GLSurfaceView;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

class SunSpotView extends GLSurfaceView {
	private static final String  TAG   = "SunSpot";
	private static final boolean DEBUG = true;

	public SunSpotView(Context context) {
		super(context);
		// Pick an EGLConfig with RGB8 color, 16-bit depth, no stencil,
		// supporting OpenGL ES 2.0 or later backwards-compatible versions.
		setEGLConfigChooser(8, 8, 8, 0, 16, 0);
		setEGLContextClientVersion(3);

		setRenderer(
			new Renderer(
				context.getFilesDir().getAbsolutePath(),
				context.getCacheDir().getAbsolutePath(),
				context.getAssets()));
	}



	private static class Renderer implements GLSurfaceView.Renderer {
		private String external;
		private String cache;
		private AssetManager assets;

		Renderer(String external, String cache, AssetManager assets) {
			this.external = external;
			this.cache    = cache;
			this.assets   = assets;
		}

		public void onDrawFrame(GL10 gl) {
			SunSpotLib.step();
		}

		public void onSurfaceChanged(GL10 gl, int width, int height) {
			SunSpotLib.resize(width, height);
		}

		public void onSurfaceCreated(GL10 gl, EGLConfig config) {
			SunSpotLib.init(external, cache, assets);
		}
	}
}
