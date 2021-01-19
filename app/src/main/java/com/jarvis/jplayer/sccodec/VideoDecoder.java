package com.jarvis.jplayer.sccodec;

public final class VideoDecoder
{
	public static final int DecoderTypeOpenCoreH264 = 1;
	public static final int DecoderTypeH264 = 2;
	public static final int DecoderTypeMPEG4 = 3;
	public static final int DecoderTypeMJPEG = 4;
	public static final int DecoderTypeMJPEGB = 5;

	public static final int OutputFormatYUV420P = 1;
	public static final int OutputFormatARGB32 = 2;
	public static final int OutputFormatRGB24 = 3;
	public static final int OutputFormatRGB565 = 4;

	public int init(int decoderType, int outputFormat, int frameWidth, int frameHeight) {
		_handle = doInit(decoderType, outputFormat, frameWidth, frameHeight, 0);
		if (-1 == _handle) {
			return -1;
		}

		return 0;
	}

	public void destroy() {
		doDestroy(_handle);
		_handle = -1;
	}

	public int decode(byte[] bitstream, int bitstreamLength,long pts, byte[] frameBuffer, int bufferSize) {
		return doDecode(_handle, bitstream, bitstreamLength,pts, frameBuffer, bufferSize);
	}

	private native int doInit(int decoderType, int outputFormat, int frameWidth, int frameHeight, int opaque);
	private native int doDecode(int handle, byte[] bitstream, int bitstreamLength, long pts, byte[] frameBuffer, int bufferSize);
	private native void doDestroy(int handle);

	private int _handle = -1;
}
