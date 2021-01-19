#include "FFCodec.h"
#include "SCCodec.h"
#include "../Xlog.h"


#ifndef AVCODEC_MAX_AUDIO_FRAME_SIZE
#define AVCODEC_MAX_AUDIO_FRAME_SIZE 192000
#endif

class FFCodecInitializer {
public:
	FFCodecInitializer() {

		//avcodec_init();
        av_register_all();
		//avcodec_register_all();
		XLOGI("FFCodecInitializer register ffmpeg! %u\n ", avcodec_version());
		XLOGI("FFCodecInitializer avcodec_configuration %s\n ", avcodec_configuration());

	}
};
static FFCodecInitializer ffCodecInitializer;

FFVideoDecoder *FFVideoDecoder::create(VideoDecoderCreateParam *createParam) {
	FFVideoDecoder *decoder = new FFVideoDecoder();
	if (NULL == decoder) {
		XLOGD("new FFVideoDecoder object fail.\n");
		return NULL;
	}

	if (-1 == decoder->init(createParam)) {
		XLOGD("init FFVideoDecoder object fail.\n");
		delete decoder;
		return NULL;
	}

	return decoder;
}

FFVideoDecoder::~FFVideoDecoder() {
	if (NULL != _yuv420p) {
		av_free(_yuv420p);
	}

	if (NULL != _codecCtx) {
		avcodec_close(_codecCtx);
		av_free(_codecCtx);
	}

	if (NULL != _swsCtx) {
		sws_freeContext(_swsCtx);
		avpicture_free(&_swsPict);
	}
}

int FFVideoDecoder::decode(VideoDecodeParam *decodeParam) {
	XLOGD("FFVideoDecoder decode begin");
	int got_picture = 0;
	int result = -1;
	uint32_t w, h;
	AVPacket avpkt;
	uint8_t *frameBufferPtr;

	av_init_packet(&avpkt);
	avpkt.data = decodeParam->bitstream;
	avpkt.size = decodeParam->bitstreamLength;

   /* result = avcodec_send_packet(_codecCtx, &avpkt);
    if(result != 0){
        XLOGI("avcodec_send_packet decode fail! ");
        return -1;
    }

    result = avcodec_receive_frame(_codecCtx,_yuv420p);
    if(result != 0)
    {
        XLOGI("avcodec_receive_frame decode fail! ");
        return -1;
    }

    decodeParam->frameWidth = _yuv420p->width;;
    decodeParam->frameHeight = _yuv420p->height;
    decodeParam->decodedLength = (_yuv420p->linesize[0] + _yuv420p->linesize[1] + _yuv420p->linesize[2])*_yuv420p->height;
    memcpy(decodeParam->frameBuffer, _yuv420p->data, decodeParam->decodedLength);

    XLOGI("avcodec_receive_frame decode success! ");
    return 0;*/

	result = avcodec_decode_video2(_codecCtx, _yuv420p, &got_picture, &avpkt);
	//XLOGD("avcodec_decode_video2 return valud: %d\n", result);
	XLOGD("result %d, got_picture %d",result,got_picture);
	if (-1 != result && got_picture) {
        XLOGD("avcodec_decode_video2 success");

		w = _codecCtx->width;
        h = _codecCtx->height;
        XLOGD("w %d, h %d",w,h);
		if (_frameWidth != w || _frameHeight != h) {
			if (NULL != _swsCtx) {
				sws_freeContext(_swsCtx);
				avpicture_free(&_swsPict);
			}

			_swsCtx = sws_getContext(w, h, _codecCtx->pix_fmt,
						 w, h, _outputFormat, 
						 SWS_FAST_BILINEAR, 
						 NULL, NULL, NULL);

			_outputFrameBufferSize = avpicture_fill(&_swsPict, NULL, _outputFormat, w, h);
			avpicture_alloc(&_swsPict, _outputFormat, w, h);

			_frameWidth = w;
			_frameHeight = h;
		}

		decodeParam->frameWidth = w;
		decodeParam->frameHeight = h;
        XLOGD("frameWidth %d, frameHeight %d",w,h);
		AVPicture *ip = (AVPicture *)_yuv420p;
		AVPicture *op = &_swsPict;
		sws_scale(_swsCtx, (const uint8_t **)ip->data, ip->linesize,
			  0, h, op->data, op->linesize);

 		if (_outputFrameBufferSize > decodeParam->bufferSize) {
			return -1;
		}

		frameBufferPtr = decodeParam->frameBuffer;
		memcpy(frameBufferPtr, op->data[0], op->linesize[0] * h);
		if (NULL != op->data[1]) {
			frameBufferPtr += op->linesize[0] * h;
			memcpy(frameBufferPtr, op->data[1], op->linesize[1] * h / 2);
		}
		if (NULL != op->data[2]) {
			frameBufferPtr += op->linesize[1] * h / 2;
			memcpy(frameBufferPtr, op->data[2], op->linesize[2] * h / 2);
		}

		decodeParam->decodedLength = result;

		XLOGD("FFVideoDecoder decode begin");
		return 0;
	}
	XLOGD("avcodec_decode_video2 fail");
	return -1;
}

int FFVideoDecoder::frameWidth()
{
	return _frameWidth;
}

int FFVideoDecoder::frameHeight()
{
	return _frameHeight;
}

FFVideoDecoder::FFVideoDecoder() {
	_codecCtx = NULL;
	_yuv420p = NULL;
	_swsCtx = NULL;
	_frameWidth = 0;
	_frameHeight = 0;
	_outputFrameBufferSize = 0;
}

int FFVideoDecoder::init(VideoDecoderCreateParam *createParam) {
	enum AVCodecID decoderTypeID;

	switch (createParam->bitstreamType) {
	case VideoDecoderBitstreamTypeH264:
		decoderTypeID = AV_CODEC_ID_H264;		
		break;
	case VideoDecoderBitstreamTypeMPEG4:
		decoderTypeID = AV_CODEC_ID_MPEG4;
		break;
	case VideoDecoderBitstreamTypeMJPEG:
		decoderTypeID = AV_CODEC_ID_MJPEG;
		break;
	case VideoDecoderBitstreamTypeMJPEGB:
		decoderTypeID = AV_CODEC_ID_MJPEGB;
		break;
	default:
		return -1;
	}

	AVCodec *codec = avcodec_find_decoder(decoderTypeID);
	if (NULL == codec) {
		return -1;
	}

	//_codecCtx = avcodec_alloc_context();
	_codecCtx = avcodec_alloc_context3(codec);
	if (NULL == _codecCtx) {
        XLOGD("111\n");
		return -1;
	}

	/* For some codecs, such as msmpeg4 and mpeg4, width and height
	   MUST be initialized there because this information is not
	   available in the bitstream. */
	_codecCtx->width = createParam->frameWidth;
	_codecCtx->height = createParam->frameHeight;
	_codecCtx->thread_count = 4;
    XLOGD("frame width:  %d\n", createParam->frameWidth);
    XLOGD("frame height: %d\n", createParam->frameHeight);

	//if (0 > avcodec_open(_codecCtx, codec)) {
	if (0 > avcodec_open2(_codecCtx, codec, NULL)) {
        XLOGD("avcodec_open2 fail");
		goto FAIL0;
	}

	_yuv420p = av_frame_alloc();
	if (NULL == _yuv420p) {
        XLOGD("333\n");
		goto FAIL1;
	}

	switch (createParam->outputFormatType) {
	case VideoDecoderOutputFormatTypeRGB565:
		_outputFormat = AV_PIX_FMT_RGB565LE;
		//_outputFormat = PIX_FMT_RGB565BE;
		break;
	case VideoDecoderOutputFormatTypeBGR565:
		_outputFormat = AV_PIX_FMT_BGR565LE;
		break;
	case VideoDecoderOutputFormatTypeRGB24:
		_outputFormat = AV_PIX_FMT_RGB24;
		break;
	case VideoDecoderOutputFormatTypeBGR24:
		_outputFormat = AV_PIX_FMT_BGR24;
		break;
	case VideoDecoderOutputFormatTypeARGB:
		_outputFormat = AV_PIX_FMT_ARGB;
		break;
	case VideoDecoderOutputFormatTypeRGBA:
		_outputFormat = AV_PIX_FMT_RGBA;
		break;
	case VideoDecoderOutputFormatTypeABGR:
		_outputFormat = AV_PIX_FMT_ABGR;
		break;
	case VideoDecoderOutputFormatTypeBGRA:
		_outputFormat = AV_PIX_FMT_BGRA;
		break;
	case VideoDecoderOutputFormatTypeYUYV422:
		_outputFormat = AV_PIX_FMT_YUYV422;
		break;
	case VideoDecoderOutputFormatTypeYUV420P:
	default:
		_outputFormat = AV_PIX_FMT_YUV420P;
		break;
	}

	return 0;
FAIL1:
	avcodec_close(_codecCtx);
FAIL0:
	av_free(_codecCtx);
	_codecCtx = NULL;

	return -1;
}

/*
  FFAudioDecoder
 */
FFAudioDecoder::~FFAudioDecoder() {
	if (NULL != _codecCtx) {
		avcodec_close(_codecCtx);
		av_free(_codecCtx);
		_codecCtx = NULL;
	}

	if (NULL != frame) {
		av_frame_unref(frame);
		//free(frame);
		frame = NULL;
	}
}

int FFAudioDecoder::decode(AudioDecodeParam *decodeParam) {
	int frameSize = AVCODEC_MAX_AUDIO_FRAME_SIZE;
	AVPacket avpkt;

	av_init_packet(&avpkt);

	if (AudioDecoderBitstreamTypeADPCM_DVI4 == _bitstreamType) {
		// DVI -> IMA
		uint8_t *p = decodeParam->bitstream;
		for (uint32_t i=8; i<decodeParam->bitstreamLength; i++) {
			p[i] = p[i] << 4 | p[i] >> 4;
		}

		avpkt.data = decodeParam->bitstream + 4;
		avpkt.size = decodeParam->bitstreamLength - 4;
	} else {
		avpkt.data = decodeParam->bitstream;
		avpkt.size = decodeParam->bitstreamLength;
	}

	int ret = avcodec_send_packet(_codecCtx, &avpkt);
	if(ret != 0){
		return -1;
	}

	while(avcodec_receive_frame(_codecCtx,frame)>=0){
		frameSize = frame->pkt_size;
	}
	/*if (0 > avcodec_decode_audio3(_codecCtx, (int16_t *)_frameBuffer, &frameSize, &avpkt)) {
		return -1;
	}*/

	if ((uint32_t)frameSize > decodeParam->bufferSize) {
		XLOGD("Frame buffer is too samll, frameSize: %d, bufferLength: %d.\n", frameSize, decodeParam->bufferSize);
		return -1;
	}

	memcpy(decodeParam->frameBuffer, frame->data, frameSize);
	decodeParam->bufferSize = frameSize;

	return 0;
}

FFAudioDecoder *FFAudioDecoder::create(AudioDecoderCreateParam *createParam) {
	FFAudioDecoder *decoder = new FFAudioDecoder();
	if (NULL == decoder) {
		XLOGD("new FFAudioDecoder object fail.\n");
		return NULL;
	}

	if (-1 == decoder->init(createParam)) {
		XLOGD("init FFAudioDecoder object fail.\n");
		delete decoder;
		return NULL;
	}

	return decoder;
}

FFAudioDecoder::FFAudioDecoder() {
	_codecCtx = NULL;
	//_frameBuffer = NULL;
}

int FFAudioDecoder::init(AudioDecoderCreateParam *createParam) {
	enum AVCodecID decoderTypeID;

	switch (createParam->bitstreamType) {
	case AudioDecoderBitstreamTypeAAC:
		decoderTypeID = AV_CODEC_ID_AAC;
		break;
	case AudioDecoderBitstreamTypeADPCM_DVI4:
	case AudioDecoderBitstreamTypeADPCM_IMA_WAV:
		decoderTypeID = AV_CODEC_ID_ADPCM_IMA_WAV;
		break;
	case AudioDecoderBitstreamTypeADPCM_G726:
		decoderTypeID = AV_CODEC_ID_ADPCM_G726;
		break;
	default:
		return -1;
	}

	_bitstreamType = createParam->bitstreamType;

	AVCodec *codec = avcodec_find_decoder(decoderTypeID);

	if (NULL == codec) {
		return -1;
	}

	_codecCtx = avcodec_alloc_context3(codec);
	if (NULL == _codecCtx) {
		return -1;
	}

	_codecCtx->sample_rate = createParam->sampleRate;
        _codecCtx->channels = createParam->channels;
        _codecCtx->bit_rate = createParam->bitRate;
	
	if (decoderTypeID == AV_CODEC_ID_ADPCM_IMA_WAV) {
		_codecCtx->bits_per_coded_sample = 4;
	}

	if (0 > avcodec_open2(_codecCtx, codec, NULL)) {
		av_free(_codecCtx);
		_codecCtx = NULL;
		return -1;
	}

	//_frameBuffer = (uint8_t *)malloc(AVCODEC_MAX_AUDIO_FRAME_SIZE);
	frame = av_frame_alloc();
	if (NULL == frame) {
		avcodec_close(_codecCtx);
		av_free(_codecCtx);		
		_codecCtx = NULL;
		return -1;
	}

	return 0;
}

