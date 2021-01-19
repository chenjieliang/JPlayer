package com.jarvis.jplayer;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.os.Build;
import android.util.AttributeSet;
import android.view.SurfaceHolder;
import android.view.View;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;


public class JPlayView extends GLSurfaceView implements SurfaceHolder.Callback, GLSurfaceView.Renderer{

    public SurfaceHolder.Callback onSurfaceCallback;

    public JPlayView(Context context, AttributeSet attrs) {
        super(context, attrs);

    }

    @Override
    public void surfaceCreated(SurfaceHolder holder) {
        if (this.onSurfaceCallback!=null) {
            this.onSurfaceCallback.surfaceCreated(holder);
        }
        if(Build.VERSION.SDK_INT >= Build.VERSION_CODES.O){
            setRenderer(this);
        }
    }

    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int w, int h) {
        if (this.onSurfaceCallback!=null) {
            this.onSurfaceCallback.surfaceChanged(holder,format,w,h);
        }
    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {
        if (this.onSurfaceCallback!=null) {
            this.onSurfaceCallback.surfaceDestroyed(holder);
        }
    }


    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {

    }

    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height) {

    }

    @Override
    public void onDrawFrame(GL10 gl) {

    }

    public void setSurfaceCallback(SurfaceHolder.Callback onSurfaceCallback) {
        this.onSurfaceCallback = onSurfaceCallback;
    }

}
