package com.jarvis.jplayer;

/**
 * @author chenjieliang
 */
public class JPlayer {

    private int _handle = -1;


    public void setPlayView(Object surface){
        if (_handle == -1) {
            _handle = init();
        }
        doSetPlayView(_handle,surface);
    }

    public void play(int width, int height) {
        if (_handle == -1) {
            _handle = init();
        }
        doPlay(_handle,width,height);
    }

    public void pause(boolean isPause) {
        doPause(_handle,isPause);
    }

    public void setSpeedRate(float rate){
        doSetSpeedRate(_handle,rate);
    }

    public void close(){
        doClose(_handle);
    }

    public void sendVideoFrame(
            int type,              //I、P、A
            int camType,
            int width,
            int height,
            int fps,              //帧率
            long pts,             //时间戳
            byte[] dataArray,     //unint_8 、unsigned char
            long size) {
        doSendVideoFrame(_handle,type,camType,width,height,fps,pts,dataArray,size);
    }

    public void sendAudioFrame(int type, int channels, int sps, int bps, long pts, byte[] dataArray, long size) {
        doSendAudioFrame(_handle,type,channels,sps,bps,pts,dataArray,size);
    }

    public void capture(String path, int type) {
        doCapture(_handle,path,type);
    }

    public void setSkipFrame(int skipNum) {
        doSetSkipFrame(_handle,skipNum);
    }

    public long getPlayTime() {
        return doGetPlayTime(_handle);
    }

    public void openVolume(boolean isOpen) {
        doOpenVolume(_handle,isOpen);
    }

    private native int init();

    private native void doSetPlayView(int handle,Object surface);

    private native void doPlay(int handle,int width, int height);

    private native void doPause(int handle,boolean isPause);

    private native void doSetSpeedRate(int handle, float rate);

    private native void doClose(int handle);

    private native void doSendVideoFrame(
            int handle,
            int type,              //I、P、A
            int camType,
            int width,
            int height,
            int fps,              //帧率
            long pts,             //时间戳
            byte[] dataArray,     //unint_8 、unsigned char
            long size);

    private native void doSendAudioFrame(int handle,int type, int channels, int sps, int bps, long pts, byte[] dataArray, long size);

    private native void doCapture(int handle, String path, int type);

    private native long doGetPlayTime(int handle);

    private native void doSetSkipFrame(int handle, int skipNum);

    private native void doOpenVolume(int handle, boolean isOpen);
}
