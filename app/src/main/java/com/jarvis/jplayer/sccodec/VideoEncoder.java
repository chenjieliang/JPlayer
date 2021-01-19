package com.jarvis.jplayer.sccodec;

import com.jarvis.jplayer.OnStreamCallback;

public final class VideoEncoder
{

	public static final int FILTER_FORMAT_ROTATE_0_CROP_LT = 0;
	/**
	 * 旋转90度剪裁左上
	 */
	public static final int FILTER_FORMAT_ROTATE_90_CROP_LT = 1;
	/**
	 * 暂时没处理
	 */
	public static final int FILTER_FORMAT_ROTATE_180 = 2;
	/**
	 * 旋转270(-90)裁剪左上，左右镜像
	 */
	public static final int FILTER_FORMAT_ROTATE_270_CROP_LT_MIRROR_LR = 3;

	/** 
	 * Initialize a encoder.
	 * 
	 * @param bitrate bitrate of the video
	 * @param fps frame rate of the video
	 * @param gop Group Of Picture
	 * @param width width of the video
	 * @param height height of the video
	 * 
	 * @return return a handle to encoder
	 */
	public native static int init(int bitrate, int fps, int gop, int width, int height, int opaque, int filterFormat, OnStreamCallback onStreamCallback);

	/** 
	 * Encode one frame.
	 * 
	 * @param handle handle to the encoder
	 * @param inBuf input buffer for the sampled video
	 * @param inLen input buffer length
	 * 
	 * @return return the encoded buffer length on SUCC or return -1 on error
	 */
	public native static void encode(int handle, byte[] inBuf, int inLen, long pts,int width,int height,int filterFormat);

	/** 
	 * Release the resource the encoder holding.
	 * 
	 * @param handle handle to the encoder
	 */
	public native static void destroy(int handle);
}