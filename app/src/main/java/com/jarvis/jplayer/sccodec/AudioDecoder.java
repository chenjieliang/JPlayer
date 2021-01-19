package com.jarvis.jplayer.sccodec;

public final class AudioDecoder
{
	public static final int DecoderTypeG711a = 1;
	public static final int DecoderTypeG711u = 2;
	public static final int DecoderTypeG726 = 3;

	private int _handle = -1;

	public int init(int decoderType, int bitRate, int sampleRate, int channels, int sampleSize) {
		_handle = doInit(decoderType, bitRate, sampleRate, channels, sampleSize, 0);
		if (-1 == _handle) {
			return -1;
		}

		return 0;
	}

	public void destroy() {
		doDestroy(_handle);
		_handle = -1;
	}

	public int decode(byte[] bitstream, int bitstreamLength, long pts,  short[] samplesBuffer, int bufferSize) {
		return doDecode(_handle, bitstream, bitstreamLength, pts, samplesBuffer, bufferSize);
	}

	private native int doInit(int decoderType, int bitRate, int sampleRate, int channels, int sampleSize, int opeque);
	private native int doDecode(int handle, byte[] bitstream, int bitstreamLength,long pts,  short[] samplesBuffer, int bufferSize);
	private native void doDestroy(int handle);
}
