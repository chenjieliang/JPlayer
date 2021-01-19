package com.jarvis.jplayer.sccodec;

import com.jarvis.jplayer.OnStreamCallback;

public final class AudioEncoder
{
	public static final int EncoderTypeG711a = 1;
	public static final int EncoderTypeG711u = 2;
	public static final int EncoderTypeG726 = 3;

	private int _handle = -1;

	public int init(int encoderType, int bitRate, int sampleRate, int channels, int sampleSize, int frameSize, OnStreamCallback onStreamCallback) {
		_handle = doInit(encoderType, bitRate, sampleRate, channels, sampleSize, frameSize, 0,onStreamCallback);
		if (-1 == _handle) {
			return -1;
		}

		return 0;
	}

	public void destroy() {
		doDestroy(_handle);
		_handle = -1;
	}

	public int encode(short[] samplesBuffer, int bufferSize, long pts, byte[] bitstream, int bitstreamLength) {
		return doEncode(_handle, samplesBuffer, bufferSize, pts, bitstream, bitstreamLength);
	}

	private native int doInit(int encoderType, int bitRate, int sampleRate, int channels, int sampleSize, int frameSize, int opaque, OnStreamCallback onStreamCallback);
	private native int doEncode(int handle,short[] samplesBuffer, int bufferSize, long pts, byte[] bitstream, int bitstreamLength);
	private native void doDestroy(int handle);
}
