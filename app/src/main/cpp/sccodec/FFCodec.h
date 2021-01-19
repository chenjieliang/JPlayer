#ifndef _FF_CODEC_H_
#define _FF_CODEC_H_

#include <stdint.h>
#include <mutex>
#include "SCCodec.h"
extern "C" {
#include "libavcodec/avcodec.h"
#include "libswscale/swscale.h"
#include "libavutil/pixfmt.h"
#include <libavformat/avformat.h>
#include <libavutil/opt.h>
};

class FFVideoDecoder : public VideoDecoder
{
public:
	static FFVideoDecoder *create(VideoDecoderCreateParam *createParam);
	virtual ~FFVideoDecoder();
	virtual int decode(VideoDecodeParam *decodeParam);
	virtual int frameWidth();
	virtual int frameHeight();

private:
	FFVideoDecoder();
	int init(VideoDecoderCreateParam *createParam);

private:
	AVCodecContext *_codecCtx;
	struct SwsContext *_swsCtx;
	AVFrame *_yuv420p;
	AVPicture _swsPict;
	uint32_t _frameWidth;
	uint32_t _frameHeight;
	uint32_t _outputFrameBufferSize;
	enum AVPixelFormat _outputFormat;
};


class FFAudioDecoder : public AudioDecoder
{
public:
	static FFAudioDecoder *create(AudioDecoderCreateParam *createParam);
	virtual ~FFAudioDecoder();
	virtual int decode(AudioDecodeParam *decodeParam);

private:
	FFAudioDecoder();
	int init(AudioDecoderCreateParam *createParam);

private:
	AVCodecContext *_codecCtx;
	AudioDecoderBitstreamType _bitstreamType;
	//uint8_t *_frameBuffer;
    AVFrame *frame = 0;
};

#endif//_FF_CODEC_H_
