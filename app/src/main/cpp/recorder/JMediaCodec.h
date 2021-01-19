//
// Created by chenjieliang on 2020/3/5.
//

#ifndef JPLAYER_ENCODECODEC_H
#define JPLAYER_ENCODECODEC_H


#include "../sdk.h"
#include "../sccodec/SCCodecParam.h"

class VideoEncoder
{
public:
    virtual ~VideoEncoder() {}
    virtual int encode(VideoEncodeParam *encodeParam) = 0;
};

class AudioEncoder
{
public:
    virtual ~AudioEncoder() {};
    virtual bool open(AudioEncoderCreateParam *createParam, stream_audio_encode_callback callback, void *userdata) = 0;
    virtual int encode(AudioEncodeParam *encodeParam) = 0;
};

class AudioDecoder
{
public:
    virtual ~AudioDecoder() {}
    virtual int decode(AudioDecodeParam *decodeParam) = 0;
};

class EncodeCodecFactory
{
public:
    static EncodeCodecFactory *factory();
    VideoEncoder *createVideoEncoder(VideoEncoderCreateParam *createParam, stream_video_encode_callback callback, void *userdata);
    AudioEncoder *createAudioEncoder(AudioEncoderCreateParam *createParam, stream_audio_encode_callback callback, void *userdata);

private:
    EncodeCodecFactory() {}
    ~EncodeCodecFactory() {}
    static EncodeCodecFactory *_factory;
};

#endif //JPLAYER_ENCODECODEC_H
