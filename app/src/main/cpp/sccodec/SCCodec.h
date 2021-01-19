//
// Created by chenjieliang on 2020/3/5.
//

#ifndef JPLAYER_ENCODECODEC_H
#define JPLAYER_ENCODECODEC_H



#include "../sdk.h"
#include "SCCodecParam.h"
#include "../thread/XThread.h"
#include "../thread/IObserver.h"

class VideoEncoder: public IObserver
{
public:
    virtual ~VideoEncoder() {}
    virtual void create(VideoEncoderCreateParam *createParam, stream_video_encode_callback callback, void *userdata) = 0;
    virtual int encode(VideoEncodeParam *encodeParam) = 0;
    virtual void close() = 0;
};

class VideoDecoder
{
public:
    virtual ~VideoDecoder() {};
    virtual int decode(VideoDecodeParam *decodeParam) = 0;
};

class AudioEncoder: public IObserver
{
public:
    virtual ~AudioEncoder() {};
    virtual void create(AudioEncoderCreateParam *createParam, stream_audio_encode_callback callback, void *userdata) = 0;
    virtual int encode(AudioEncodeParam *encodeParam) = 0;
    virtual void close() = 0;
};

class AudioDecoder
{
public:
    virtual ~AudioDecoder() {}
    virtual int decode(AudioDecodeParam *decodeParam) = 0;
};

class SCCodecFactory
{
public:
    static SCCodecFactory *factory();
    VideoEncoder *createVideoEncoder(VideoEncoderCreateParam *createParam, stream_video_encode_callback callback, void *userdata);
    AudioEncoder *createAudioEncoder(AudioEncoderCreateParam *createParam, stream_audio_encode_callback callback, void *userdata);

    VideoDecoder *createVideoDecoder(VideoDecoderCreateParam *createParam);
    AudioDecoder *createAudioDecoder(AudioDecoderCreateParam *createParam);

private:
    SCCodecFactory() {}
    ~SCCodecFactory() {}
    static SCCodecFactory *_factory;
};

#endif //JPLAYER_ENCODECODEC_H
