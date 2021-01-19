//
// Created by chenjieliang on 2020/3/5.
//

#include <cstddef>
#include "SCCodec.h"
#include "G711Codec.h"
#include "../Xlog.h"
#include "SCCodecParam.h"
#include "ff/FFAudioEncoder.h"
#include "ff/FFVideoEncoder.h"
#include "FFCodec.h"

class SCCodecFactorySingletonThreadSafeHelper
{
public:
    SCCodecFactorySingletonThreadSafeHelper() {
        SCCodecFactory::factory();
    }
};

static SCCodecFactorySingletonThreadSafeHelper threadSafeHelper;

SCCodecFactory *SCCodecFactory::_factory = NULL;

SCCodecFactory *SCCodecFactory::factory()
{
    if (NULL == _factory) {
        _factory = new SCCodecFactory;
    }

    return _factory;
}

VideoEncoder *SCCodecFactory::createVideoEncoder(VideoEncoderCreateParam *createParam, stream_video_encode_callback callback, void *userdata)
{
    VideoEncoder *encoder = new FFVideoEncoder;
    encoder->create(createParam,callback,userdata);
    return encoder;
}

AudioEncoder *SCCodecFactory::createAudioEncoder(AudioEncoderCreateParam *createParam, stream_audio_encode_callback callback, void *userdata)
{
    AudioEncoder *encoder = NULL;

    switch (createParam->bitstreamType) {
        case AudioEncoderBitstreamTypeG711a:
            encoder = new G711aEncoder;
            XLOGD("createAudioEncoder G711aEncoder");
            break;
        case AudioEncoderBitstreamTypeG711u:
            encoder = new G711uEncoder;
            XLOGD("createAudioEncoder G711uEncoder");
            break;
        default:
            encoder = new FFAudioEncoder;
            break;
    }
    encoder->create(createParam,callback,userdata);
    return encoder;
}

VideoDecoder *SCCodecFactory::createVideoDecoder(VideoDecoderCreateParam *createParam)
{
    VideoDecoder *decoder = NULL;

    switch (createParam->bitstreamType) {
        case VideoDecoderBitstreamTypeOpenCoreH264:
            //decoder = OCH264Decoder::create();
            //break;
        case VideoDecoderBitstreamTypeH264:
        case VideoDecoderBitstreamTypeMPEG4:
        case VideoDecoderBitstreamTypeMJPEG:
        case VideoDecoderBitstreamTypeMJPEGB:
        default:
            decoder = FFVideoDecoder::create(createParam);
            break;
    }

    return decoder;
}

AudioDecoder *SCCodecFactory::createAudioDecoder(AudioDecoderCreateParam *createParam)
{
    AudioDecoder *decoder = NULL;

    switch (createParam->bitstreamType) {
        case AudioDecoderBitstreamTypeG711a:
            decoder = new G711aDecoder;
            break;
        case AudioDecoderBitstreamTypeG711u:
            decoder = new G711uDecoder;
            break;
        default:
            decoder = FFAudioDecoder::create(createParam);
            break;
    }

    return decoder;
}